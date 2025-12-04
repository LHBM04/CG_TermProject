#include "Resources.h"

#include <AL/al.h>
#include <AL/alc.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_VORBIS_HEADER_ONLY
#include <stb_vorbis.c>

#define DR_MP3_IMPLEMENTATION
#include <dr_mp3.h>

#define DR_WAV_IMPLEMENTATION
#include <dr_wav.h>

#define DR_FLAC_IMPLEMENTATION
#include <dr_flac.h>

#include "Debug.h"
#include "IO.h"

Resource::~Resource() noexcept
{
}

#pragma region Texture Implementation
Texture::Texture() noexcept
    : width(0)
    , height(0)
    , channels(0)
    , textureID(0)
{
}

Texture::~Texture() noexcept
{
    if (textureID)
    {
        glDeleteTextures(1, &textureID);
    }
}

bool Texture::Load(const std::filesystem::path& path_) noexcept
{
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(path_.string().c_str(), &width, &height, &channels, 0);
    if (!data)
    {
        Logger::Error("Failed to load texture image: {}", path_.string());
        return false;
    }

    GLenum format;
    if (channels == 1)
    {
        format = GL_RED;
    }
    else if (channels == 3)
    {
        format = GL_RGB;
    }
    else if (channels == 4)
    {
        format = GL_RGBA;
    }
    else
    {
        format = GL_RGB;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    Logger::Info("Texture loaded successfully: {} ({}x{}, {}ch)", path_.string(), width, height, channels);
    return true;
}
#pragma endregion

#pragma region Shader Implementation
Shader::Shader() noexcept
    : programID(0)
{
}

Shader::~Shader() noexcept
{
    if (programID)
    {
        glDeleteProgram(programID);
    }
}

bool Shader::Load(const std::filesystem::path& path_) noexcept
{
    std::filesystem::path vertexPath = path_;
    vertexPath += ".vert";

    std::filesystem::path fragmentPath = path_;
    fragmentPath += ".frag";

    const std::string vertexCode   = File::ReadAllText(vertexPath);
    const std::string fragmentCode = File::ReadAllText(fragmentPath);

    if (vertexCode.empty())
    {
        Logger::Error("Vertex shader source is empty or file not found: {}", vertexPath.string());
        return false;
    }
    if (fragmentCode.empty())
    {
        Logger::Error("Fragment shader source is empty or file not found: {}", fragmentPath.string());
        return false;
    }

    const unsigned int vertexShader = Shader::Compile(GL_VERTEX_SHADER, vertexCode);
    if (vertexShader == 0)
        return false;

    const unsigned int fragmentShader = Shader::Compile(GL_FRAGMENT_SHADER, fragmentCode);
    if (fragmentShader == 0)
        return false;

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    int success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        char infoLog[1024];
        glGetProgramInfoLog(programID, 1024, nullptr, infoLog);
        Logger::Error("Shader Linking Failed [{}]:\n{}", path_.string(), infoLog);
        glDeleteProgram(programID);
        programID = 0;
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    Logger::Info("Shader loaded successfully: {}", path_.string());
    return true;
}

unsigned int Shader::Compile(unsigned int type_, std::string_view source_) noexcept
{
    const unsigned int shader     = glCreateShader(type_);
    const char*        sourceCStr = source_.data();
    glShaderSource(shader, 1, &sourceCStr, nullptr);
    glCompileShader(shader);

    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        Logger::Error("Shader Compilation Failed (Type: {}):\n{}", type_, infoLog);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}
#pragma endregion

#pragma region Mesh Implementation
Mesh::Mesh() noexcept
    : vao(0)
    , vbo(0)
    , ebo(0)
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}

Mesh::~Mesh() noexcept
{
    if (vao != 0)
    {
        glDeleteVertexArrays(1, &vao);
    }
    if (vbo != 0)
    {
        glDeleteBuffers(1, &vbo);
    }
    if (ebo != 0)
    {
        glDeleteBuffers(1, &ebo);
    }
}

void Mesh::Draw() noexcept
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

bool Mesh::Load(const std::filesystem::path& path_) noexcept
{
    tinyobj::attrib_t                attrib;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;
    std::string                      warn, err;

    std::filesystem::path baseDirPath = Path::GetDirectoryName(path_);
    std::string           baseDir     = baseDirPath.string() + "/";

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path_.string().c_str(), baseDir.c_str());

    if (!warn.empty())
        Logger::Warn("Mesh Load Warning [{}]: {}", path_.string(), warn);
    if (!err.empty())
        Logger::Error("Mesh Load Error [{}]: {}", path_.string(), err);
    if (!ret)
        return false;

    // 기존 GL 리소스 정리
    if (vao)
        glDeleteVertexArrays(1, &vao);
    if (vbo)
        glDeleteBuffers(1, &vbo);
    if (ebo)
        glDeleteBuffers(1, &ebo);
    vao = vbo = ebo = 0;

    vertices.clear();
    indices.clear();

    size_t totalFaces = 0;
    for (const auto& shape : shapes)
        totalFaces += shape.mesh.num_face_vertices.size();

    vertices.reserve(totalFaces * 3);
    indices.reserve(totalFaces * 3);

    // [수정됨] 최소/최대값 계산 로직은 유지하되, 강제 스케일링은 제거합니다.
    glm::vec3 minP(FLT_MAX);
    glm::vec3 maxP(-FLT_MAX);

    for (const auto& shape : shapes)
    {
        size_t      index_offset = 0;
        const auto& nf           = shape.mesh.num_face_vertices;
        for (size_t f = 0; f < nf.size(); ++f)
        {
            int fv = nf[f];
            if (fv < 3)
            {
                index_offset += fv;
                continue;
            }

            glm::vec3 p[3];
            glm::vec2 uv[3];

            for (int k = 0; k < 3; ++k)
            {
                const tinyobj::index_t& idx = shape.mesh.indices[index_offset + k];

                if (idx.vertex_index >= 0)
                {
                    p[k] = glm::vec3(attrib.vertices[3 * idx.vertex_index + 0],
                                     attrib.vertices[3 * idx.vertex_index + 1],
                                     attrib.vertices[3 * idx.vertex_index + 2]);

                    // min/max 갱신
                    minP = glm::min(minP, p[k]);
                    maxP = glm::max(maxP, p[k]);
                }
                else
                {
                    p[k] = glm::vec3(0.0f);
                }

                if (idx.texcoord_index >= 0)
                {
                    uv[k] = glm::vec2(attrib.texcoords[2 * idx.texcoord_index + 0],
                                      attrib.texcoords[2 * idx.texcoord_index + 1]);
                }
                else
                {
                    uv[k] = glm::vec2(0.0f, 0.0f);
                }
            }

            glm::vec3 n = glm::normalize(glm::cross(p[1] - p[0], p[2] - p[0]));
            if (!glm::all(glm::isinf(n)))
                n = glm::vec3(0.0f, 1.0f, 0.0f);

            for (int k = 0; k < 3; ++k)
            {
                Vertex vertex{};
                vertex.position  = p[k];
                vertex.normal    = n;
                vertex.texCoords = uv[k];

                vertices.push_back(vertex);
                indices.push_back(static_cast<unsigned int>(vertices.size() - 1));
            }
            index_offset += fv;
        }
    }

    // [수정됨] 중심점만 보정하고, 스케일(크기) 변경 로직은 삭제했습니다.
    // 이렇게 해야 Cube.obj가 원래 크기(2.0)를 유지하여 맵이 정상적으로 보입니다.
    glm::vec3 center = (minP + maxP) * 0.5f;
    for (auto& v : vertices)
    {
        v.position -= center;
    }

    // GL 버퍼 생성 및 데이터 전송
    if (vao == 0)
        glGenVertexArrays(1, &vao);
    if (vbo == 0)
        glGenBuffers(1, &vbo);
    if (ebo == 0)
        glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);

    return true;
}
#pragma endregion

#pragma region AudioClip Implementation
AudioClip::AudioClip() noexcept
    : bufferID(0)
{
}

AudioClip::~AudioClip() noexcept
{
    if (bufferID)
    {
        alDeleteBuffers(1, &bufferID);
        bufferID = 0;
    }
}

bool AudioClip::Load(const std::filesystem::path& path_) noexcept
{
    std::string pathStr = path_.string();
    std::string ext     = path_.extension().string();

    // 확장자 소문자 변환
    for (auto& c : ext)
        c = std::tolower(c);

    short*       pSampleData        = nullptr;
    unsigned int channels           = 0;
    unsigned int sampleRate         = 0;
    uint64_t     totalPCMFrameCount = 0;

    // 1. 확장자에 따른 디코딩
    if (ext == ".mp3")
    {
        drmp3_config config;
        pSampleData = drmp3_open_file_and_read_pcm_frames_s16(pathStr.c_str(), &config, &totalPCMFrameCount, nullptr);
    }
    else if (ext == ".wav")
    {
        pSampleData = drwav_open_file_and_read_pcm_frames_s16(
                pathStr.c_str(), &channels, &sampleRate, &totalPCMFrameCount, nullptr);
    }
    else if (ext == ".flac")
    {
        pSampleData = drflac_open_file_and_read_pcm_frames_s16(
                pathStr.c_str(), &channels, &sampleRate, &totalPCMFrameCount, nullptr);
    }
    else if (ext == ".ogg")
    {
        int ch, rate;
        int frames = stb_vorbis_decode_filename(pathStr.c_str(), &ch, &rate, &pSampleData);
        if (frames > 0)
        {
            channels           = ch;
            sampleRate         = rate;
            totalPCMFrameCount = frames;
        }
    }
    else
    {
        Logger::Error("Unsupported audio format: {}", ext);
        return false;
    }

    // 2. 데이터 로드 실패 체크
    if (!pSampleData)
    {
        Logger::Error("Failed to decode audio file: {}", pathStr);
        return false;
    }

    // 3. OpenAL 포맷 결정
    ALenum format = 0;
    if (channels == 1)
        format = AL_FORMAT_MONO16;
    else if (channels == 2)
        format = AL_FORMAT_STEREO16;

    if (format == 0)
    {
        Logger::Error("Unsupported channel count ({}) in: {}", channels, pathStr);
        free(pSampleData); // dr_libs와 stb 모두 standard free 사용 가능
        return false;
    }

    // 4. OpenAL 버퍼 생성 및 업로드
    if (bufferID == 0)
        alGenBuffers(1, &bufferID);

    // 데이터 크기(Byte) 계산: 프레임 수 * 채널 수 * 샘플 크기(2bytes)
    ALsizei dataSize = static_cast<ALsizei>(totalPCMFrameCount * channels * sizeof(short));

    alBufferData(bufferID, format, pSampleData, dataSize, sampleRate);

    // 5. 메모리 해제
    free(pSampleData);

    // 6. 에러 체크
    if (alGetError() != AL_NO_ERROR)
    {
        Logger::Error("OpenAL Error while buffering: {}", pathStr);
        return false;
    }

    Logger::Info("AudioClip Loaded: {} ({} Hz, {} ch)", pathStr, sampleRate, channels);
    return true;
}
#pragma endregion

std::unordered_map<std::filesystem::path, std::unique_ptr<Resource>> ResourceManager::resources;