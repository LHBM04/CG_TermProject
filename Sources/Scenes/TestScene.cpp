#include "TestScene.h"

#include "../Core/Input.h"
#include "../Core/Time.h"

#ifndef MAX_LINE_LENGTH
    #define MAX_LINE_LENGTH 10000
#endif

TestScene::TestScene() noexcept
    : Scene("TestScene")
{
    make_shaderProgram();

    glEnable(GL_DEPTH_TEST);
}

void TestScene::make_vertexShaders()
{
    vertexSource = filetobuf("vertex.glsl");
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, NULL);
    glCompileShader(vertexShader);

    GLint  result;
    GLchar errorLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        SPDLOG_INFO("ERROR: vertex shader fail");
        return;
    }
    else
        SPDLOG_INFO("vertex shader sucess");
}

void TestScene::make_fragmentShaders()
{
    fragmentSource = filetobuf("fragment.glsl");
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, NULL);
    glCompileShader(fragmentShader);

    GLint  result;
    GLchar errorLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        SPDLOG_INFO("ERROR: frag_shader fail");
        return;
    }
    else
        SPDLOG_INFO("fragment shader sucess");
}

void TestScene::make_shaderProgram()
{
    make_vertexShaders();
    make_fragmentShaders();

    shaderProgramID = glCreateProgram();

    glAttachShader(shaderProgramID, vertexShader);
    glAttachShader(shaderProgramID, fragmentShader);
    glLinkProgram(shaderProgramID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgramID);
}

void TestScene::read_newline(char* str)
{
    char* pos;
    if ((pos = strchr(str, '\n')) != NULL)
        *pos = '\0';
}

void TestScene::read_obj_file(const char* filename, Model* model)
{
    if (!model || !filename)
    {
        SPDLOG_ERROR("read_obj_file: invalid arguments.");
        return;
    }

    model->vertices       = nullptr;
    model->faces          = nullptr;
    model->texcoords      = nullptr;
    model->vertex_count   = 0;
    model->face_count     = 0;
    model->texcoord_count = 0;

    FILE* file = nullptr;
    if (fopen_s(&file, filename, "r") != 0 || !file)
    {
        SPDLOG_ERROR("read_obj_file: cannot open file {}", filename);
        return;
    }

    // 1차 패스: v 개수, 폴리곤을 삼각형으로 분할한 총 face(삼각형) 개수 계산
    size_t vertex_count = 0;
    size_t tri_count    = 0;

    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file))
    {
        if (line[0] == 'v' && line[1] == ' ')
        {
            ++vertex_count;
        }
        else if (line[0] == 'f' && line[1] == ' ')
        {
            int tokenCount = 0;
            char* cursor = line + 2;
            while (*cursor)
            {
                while (*cursor == ' ' || *cursor == '\t') ++cursor;
                if (*cursor == '\0' || *cursor == '\n') break;

                char* endToken = cursor;
                while (*endToken && *endToken != ' ' && *endToken != '\t' && *endToken != '\n')
                    ++endToken;

                if (endToken > cursor)
                    ++tokenCount;
                cursor = endToken;
            }
            if (tokenCount >= 3)
                tri_count += (size_t)(tokenCount - 2);
        }
    }

    if (vertex_count == 0 || tri_count == 0)
    {
        SPDLOG_WARN("read_obj_file: no usable vertices or faces in {}", filename);
    }

    // 메모리 할당: vt는 v 좌표 기반으로 정규화해서 생성할 것이므로 v 개수만큼 할당
    model->vertices  = (Vt*)malloc(vertex_count * sizeof(Vt));
    model->faces     = (Face*)malloc(tri_count * sizeof(Face));
    model->texcoords = (TexCoord*)malloc(vertex_count * sizeof(TexCoord));

    if ((!model->vertices && vertex_count) || (!model->faces && tri_count) || (!model->texcoords && vertex_count))
    {
        SPDLOG_ERROR("read_obj_file: allocation failed.");
        if (file) fclose(file);
        free(model->vertices);
        free(model->faces);
        free(model->texcoords);
        model->vertices       = nullptr;
        model->faces          = nullptr;
        model->texcoords      = nullptr;
        model->vertex_count   = 0;
        model->face_count     = 0;
        model->texcoord_count = 0;
        return;
    }

    model->vertex_count   = vertex_count;
    model->face_count     = tri_count;
    model->texcoord_count = vertex_count; // vt는 v 수와 동일

    // 2차 패스: 실제 데이터 파싱
    rewind(file);

    size_t v_index = 0;
    size_t f_index = 0;

    // 정규화를 위한 min/max (x,y 기준)
    float minX = FLT_MAX, maxX = -FLT_MAX;
    float minY = FLT_MAX, maxY = -FLT_MAX;

    // 먼저 v와 f를 파싱한다. v는 저장만 하고 min/max 업데이트, vt는 이후 한 번에 채움.
    std::vector<std::string> faceLines;
    faceLines.reserve(tri_count); // 대략적 예약 (라인 수와 삼각형 수가 다를 수 있으나 충분)

    while (fgets(line, MAX_LINE_LENGTH, file))
    {
        read_newline(line);
        if (line[0] == '#')
            continue;

        if (line[0] == 'v' && line[1] == ' ')
        {
            float x, y, z;
            int parsed = sscanf_s(line + 2, "%f %f %f", &x, &y, &z);
            if (parsed == 3 && v_index < vertex_count)
            {
                model->vertices[v_index].x = x;
                model->vertices[v_index].y = y;
                model->vertices[v_index].z = z;

                if (x < minX) minX = x;
                if (x > maxX) maxX = x;
                if (y < minY) minY = y;
                if (y > maxY) maxY = y;

                ++v_index;
            }
            else
            {
                SPDLOG_WARN("read_obj_file: malformed vertex line: {}", line);
            }
        }
        else if (line[0] == 'f' && line[1] == ' ')
        {
            // 나중 삼각화 처리를 위해 라인을 저장
            faceLines.emplace_back(line);
        }
    }

    // v 좌표 기반으로 vt 정규화 생성: u = (x - minX)/(maxX - minX), v = (y - minY)/(maxY - minY)
    float invDX = (maxX > minX) ? 1.0f / (maxX - minX) : 1.0f; // 분모=0 방지
    float invDY = (maxY > minY) ? 1.0f / (maxY - minY) : 1.0f;
    for (size_t i = 0; i < vertex_count; ++i)
    {
        float u = (model->vertices[i].x - minX) * invDX;
        float v = (model->vertices[i].y - minY) * invDY;
        // NaN/Inf 방지
        if (!std::isfinite(u)) u = 0.0f;
        if (!std::isfinite(v)) v = 0.0f;

        model->texcoords[i].u = u;
        model->texcoords[i].v = v;
    }

    // face 라인들을 삼각화하여 faces 채우기
    for (const std::string& fLine : faceLines)
    {
        // 토큰 분리
        std::vector<unsigned int> vIdx;
        std::vector<unsigned int> tIdx;
        vIdx.reserve(8);
        tIdx.reserve(8);

        const char* cstr = fLine.c_str();
        char* cursor = const_cast<char*>(cstr) + 2; // "f " 이후
        while (*cursor)
        {
            while (*cursor == ' ' || *cursor == '\t') ++cursor;
            if (*cursor == '\0') break;

            char* endToken = cursor;
            while (*endToken && *endToken != ' ' && *endToken != '\t')
                ++endToken;

            char token[128] = { 0 };
            size_t len = (size_t)(endToken - cursor);
            if (len >= sizeof(token)) len = sizeof(token) - 1;
            memcpy(token, cursor, len);
            token[len] = '\0';

            unsigned int v_i = 0;
            unsigned int t_i = 0;

            // 지원: v / v/vt / v//vn / v/vt/vn
            char* firstSlash = strchr(token, '/');
            if (!firstSlash)
            {
                char* endPtr = nullptr;
                unsigned long val = strtoul(token, &endPtr, 10);
                if (endPtr != token && val > 0) v_i = (unsigned int)val;
                // vt 없으면 v와 동일하게 매핑
                t_i = v_i;
            }
            else
            {
                *firstSlash = '\0';
                char* endPtr = nullptr;
                unsigned long val = strtoul(token, &endPtr, 10);
                if (endPtr != token && val > 0) v_i = (unsigned int)val;

                char* rest = firstSlash + 1;
                if (*rest != '\0')
                {
                    if (*rest == '/')
                    {
                        // v//vn → vt 없음 → v 매핑
                        t_i = v_i;
                    }
                    else
                    {
                        char* secondSlash = strchr(rest, '/');
                        if (secondSlash)
                        {
                            *secondSlash = '\0';
                            char* endPtr2 = nullptr;
                            unsigned long vtVal = strtoul(rest, &endPtr2, 10);
                            if (endPtr2 != rest && vtVal > 0) t_i = (unsigned int)vtVal;
                            else t_i = v_i;
                            // vn 무시
                        }
                        else
                        {
                            // v/vt
                            char* endPtr2 = nullptr;
                            unsigned long vtVal = strtoul(rest, &endPtr2, 10);
                            if (endPtr2 != rest && vtVal > 0) t_i = (unsigned int)vtVal;
                            else t_i = v_i;
                        }
                    }
                }
                else
                {
                    // 'v/' → vt 없음 → v 매핑
                    t_i = v_i;
                }
            }

            vIdx.push_back(v_i);
            tIdx.push_back(t_i);
            cursor = endToken;
        }

        if (vIdx.size() < 3)
        {
            SPDLOG_WARN("read_obj_file: face with <3 vertices skipped: {}", fLine);
            continue;
        }

        // 팬 삼각화
        for (size_t k = 1; k + 1 < vIdx.size(); ++k)
        {
            if (f_index >= tri_count) break;

            unsigned int v0 = vIdx[0], v1 = vIdx[k], v2 = vIdx[k + 1];
            unsigned int t0 = tIdx[0], t1 = tIdx[k], t2 = tIdx[k + 1];

            // 1-based → 0-based, 범위 체크
            if (!v0 || !v1 || !v2 || v0 > vertex_count || v1 > vertex_count || v2 > vertex_count)
            {
                SPDLOG_WARN("read_obj_file: vertex index out of range.");
                continue;
            }

            // vt는 v 개수만큼 생성했으므로 v 인덱스와 동일하게 0-based 매핑하는 것이 안전
            auto map_vt = [vertex_count](unsigned int idx, unsigned int v_as_fallback) -> unsigned int
            {
                if (idx == 0) return v_as_fallback - 1;
                if (idx <= vertex_count) return idx - 1;
                // 범위 초과 시 v 기반으로 보정
                return v_as_fallback - 1;
            };

            model->faces[f_index].v1 = v0 - 1;
            model->faces[f_index].v2 = v1 - 1;
            model->faces[f_index].v3 = v2 - 1;

            model->faces[f_index].t1 = map_vt(t0, v0);
            model->faces[f_index].t2 = map_vt(t1, v1);
            model->faces[f_index].t3 = map_vt(t2, v2);

            ++f_index;
        }
    }

    fclose(file);

    if (v_index != vertex_count || f_index != tri_count)
    {
        SPDLOG_WARN("read_obj_file: counts mismatch. expected v:{} f(tri):{}, parsed v:{} f:{}",
                    vertex_count, tri_count, v_index, f_index);
    }
}

char* TestScene::filetobuf(const char* file)
{
    FILE* fptr;
    long  length;
    char* buf;
    fopen_s(&fptr, file, "rb");
    if (!fptr)
        return NULL;
    fseek(fptr, 0, SEEK_END);
    length = ftell(fptr);
    buf    = (char*)malloc(length + 1);
    fseek(fptr, 0, SEEK_SET);
    fread(buf, length, 1, fptr);
    fclose(fptr);
    buf[length] = 0;
    return buf;
}

TestScene::~TestScene() noexcept
{
    delete cam;
    delete cube;
    delete light;
    free(ballModel.vertices);
    free(ballModel.faces);
    free(ballModel.texcoords);
}

void TestScene::OnEnter() noexcept
{
    SPDLOG_INFO("Entered TestScene.");

    read_obj_file("Sources/Rendering/Ball.obj", &ballModel);
    SPDLOG_INFO("OBJ loaded: vertices={}, faces={}, texcoords={}",
                ballModel.vertex_count, ballModel.face_count, ballModel.texcoord_count);

    cam = new Camera();
    cam->rotate(45.0f);

    splines.AddPoint(glm::vec3(40.0f, 35.0f, 0.0f));
    splines.AddPoint(glm::vec3(0.0f, 30.0f, 40.0f));
    splines.AddPoint(glm::vec3(-40.0f, 25.0f, 0.0f));
    splines.AddPoint(glm::vec3(0.0f, 20.0f, -40.0f));
    splines.AddPoint(glm::vec3(40.0f, 15.0f, 0.0f));
    splines.AddPoint(glm::vec3(0.0f, 20.0f, 40.0f));
    splines.AddPoint(glm::vec3(20.0f, 20.0f, 20.0f));

    cube = new Cube();
    cube->setTexture(poket_ball);
    cube->LoadFromModel(ballModel); // 핵심: OBJ 모델을 cube 버퍼로 교체
    cube->move(Vector3(0.0f, 5.0f, 0.0f));
    cube->SetPhysicsParams(1.8f, 2.5f, 1.3f);

    light = new Light(Vector3(3.0f, 5.0f, 0.0f));
    labyrinth = new Labyrinth();
}

void TestScene::OnUpdate() noexcept
{
    cam->setPos(splines.ApplySplines());

    if (Input::IsKeyHeld(GLFW_KEY_SPACE))
    {
        if (Input::IsKeyHeld(GLFW_KEY_W))
            cam->moveUp();
        else if (Input::IsKeyHeld(GLFW_KEY_S))
            cam->moveDown();

        if (Input::IsKeyHeld(GLFW_KEY_A))
            cam->moveLeft();
        else if (Input::IsKeyHeld(GLFW_KEY_D))
            cam->moveRight();

        if (Input::IsKeyHeld(GLFW_KEY_Q))
            cam->rotate(0.5f);
        else if (Input::IsKeyHeld(GLFW_KEY_E))
            cam->rotate(-0.5f);

        if (Input::IsMouseButtonHeld(GLFW_MOUSE_BUTTON_LEFT))
            cam->moveForward();
        else if (Input::IsMouseButtonHeld(GLFW_MOUSE_BUTTON_RIGHT))
            cam->moveBackward();
    }
    else
    {
        if (Input::IsKeyPressed(GLFW_KEY_SPACE))
            cube->teleport(Vector3(0.0f, 5.0f, 0.0f));
        else if (Input::IsKeyPressed(GLFW_KEY_Q))
            cube->rotate(10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        else if (Input::IsKeyPressed(GLFW_KEY_E))
            cube->rotate(-10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        else if (Input::IsKeyPressed(GLFW_KEY_T))
            cube->resize(glm::vec3(0.5f, 0.5f, 0.5f));
    }

    glm::vec2 mousedelta = Input::GetMousePositionDelta();
    if (mousedelta.x > 0)
        labyrinth->Xrotate(-0.2f);
    else if (mousedelta.x < 0)
        labyrinth->Xrotate(0.2f);

    if (mousedelta.y > 0)
        labyrinth->Zrotate(0.2f);
    else if (mousedelta.y < 0)
        labyrinth->Zrotate(-0.2f);

    if (Input::IsKeyHeld(GLFW_KEY_I))
        labyrinth->Zrotate(-0.2f);
    else if (Input::IsKeyHeld(GLFW_KEY_K))
        labyrinth->Zrotate(0.2f);
    else if (Input::IsKeyHeld(GLFW_KEY_J))
        labyrinth->Xrotate(0.2f);
    else if (Input::IsKeyHeld(GLFW_KEY_L))
        labyrinth->Xrotate(-0.2f);

    for (int i{}; i < labyrinth->getMap().size(); ++i)
        cube->checkCollisions(labyrinth->getMap()[i]);

    cube->UpdateWithSlope(labyrinth->getGroundNormal());
}

void TestScene::OnRender() noexcept
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgramID);

    cam->settingCamera(shaderProgramID);
    light->applyLight(shaderProgramID);

    cube->Draw(shaderProgramID);
    labyrinth->draw(shaderProgramID);
}

void TestScene::OnExit() noexcept
{
    SPDLOG_INFO("Exited TestScene.");
}