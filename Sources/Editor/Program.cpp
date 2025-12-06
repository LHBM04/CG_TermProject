#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <algorithm> // for min, max
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

// ==========================================
// 1. 데이터 구조 및 상수
// ==========================================
// 맵 사이즈는 데이터 저장용 한계일 뿐, 시각적으로는 무한해보임
const int MAP_LIMIT     = 16;
const int WINDOW_WIDTH  = 1280;
const int WINDOW_HEIGHT = 720;

enum class TileType : int
{
    Empty       = 0, // 허공 (아무것도 없음)
    Floor       = 1, // 바닥
    Wall        = 2, // 벽
    PlayerSpawn = 3, // 시작점
    Goal        = 4  // 도착점
};

struct MapData
{
    int                   width  = MAP_LIMIT;
    int                   height = MAP_LIMIT;
    std::vector<TileType> tiles;

    MapData()
    {
        // 요구사항: 처음부터 바닥이 있는 게 아님 -> Empty로 초기화
        tiles.resize(width * height, TileType::Empty);
    }

    int GetIndex(int x, int z)
    {
        // 데이터 범위를 벗어나면 -1 리턴 (그리드는 그려지더라도 데이터는 저장 안 함)
        if (x < 0 || x >= width || z < 0 || z >= height)
            return -1;
        return z * width + x;
    }
};

struct EditorState
{
    // 카메라 상태
    float camX      = MAP_LIMIT / 2.0f; // 초기 위치는 맵 중앙
    float camZ      = MAP_LIMIT / 2.0f;
    float zoomLevel = 15.0f; // 화면 세로에 보여질 월드 유닛 크기 (작을수록 확대)

    // 입력 상태
    bool   isPanning  = false;
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;

    // 에디터 상태
    int      hoverX       = -1;
    int      hoverZ       = -1;
    TileType currentBrush = TileType::Floor; // 기본 브러시를 바닥으로 변경
    MapData  map;
};

// 전역 상태 (콜백 함수 접근용)
EditorState g_State;

// ==========================================
// 2. 파일 입출력 (수정됨: 엄격한 포맷)
// ==========================================
void SaveMap(const char* filename, const MapData& map)
{
    // 1. JSON 객체 생성
    json j;
    j["width"]  = map.width;
    j["height"] = map.height;

    // 2. 타일 데이터 변환 (Enum -> int 변환 필요)
    // vector<TileType>을 바로 넣을 수 없으므로 int vector로 캐스팅하거나 반복문 사용
    std::vector<int> tileInts(map.tiles.size());
    for (size_t i = 0; i < map.tiles.size(); ++i)
    {
        tileInts[i] = static_cast<int>(map.tiles[i]);
    }
    j["tiles"] = tileInts;

    // 3. 파일 쓰기
    // 확장자를 .json으로 저장하는 것을 권장 (예: level1.json)
    std::ofstream out(filename);
    if (out.is_open())
    {
        out << j.dump(4); // 4칸 들여쓰기로 예쁘게 저장
        out.close();
        std::cout << "[System] Map Saved (JSON): " << filename << std::endl;
    }
}

void LoadMap(const char* filename, MapData& map)
{
    std::ifstream in(filename);
    if (!in.is_open())
    {
        std::cout << "[Error] File not found: " << filename << std::endl;
        return;
    }

    int w, h;
    if (!(in >> w >> h))
        return; // 헤더 읽기 실패 시 중단

    // 맵 리사이즈 (현재는 고정이지만 확장성을 위해)
    map.width  = w;
    map.height = h;
    map.tiles.resize(w * h);

    int val;
    int count = 0;
    while (in >> val && count < w * h)
    {
        map.tiles[count++] = (TileType)val;
    }
    in.close();
    std::cout << "[System] Map Loaded: " << filename << " (" << w << "x" << h << ")" << std::endl;
}

// ==========================================
// 3. 입력 콜백 (줌/팬)
// ==========================================
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return;

    // 줌 레벨 조정 (최소 2 ~ 최대 50)
    g_State.zoomLevel -= (float)yoffset * 2.0f;
    if (g_State.zoomLevel < 2.0f)
        g_State.zoomLevel = 2.0f;
    if (g_State.zoomLevel > 100.0f)
        g_State.zoomLevel = 100.0f;
}

// ==========================================
// 4. 렌더링 헬퍼
// ==========================================
void DrawCube(float x, float z, float r, float g, float b, float hOffset = 0.0f, float height = 1.0f)
{
    float h = height / 2.0f;
    float s = 0.45f; // 꽉 채우지 않고 약간 여백 (0.5 -> 0.45)

    glPushMatrix();
    glTranslatef(x + 0.5f, hOffset, z + 0.5f);

    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    // Top
    glVertex3f(-s, h, -s);
    glVertex3f(-s, h, s);
    glVertex3f(s, h, s);
    glVertex3f(s, h, -s);
    // Sides
    glColor3f(r * 0.8f, g * 0.8f, b * 0.8f); // 음영
    glVertex3f(-s, -h, -s);
    glVertex3f(-s, h, -s);
    glVertex3f(s, h, -s);
    glVertex3f(s, -h, -s);
    glVertex3f(-s, -h, s);
    glVertex3f(s, -h, s);
    glVertex3f(s, h, s);
    glVertex3f(-s, h, s);
    glVertex3f(s, -h, s);
    glVertex3f(s, h, s);
    glVertex3f(s, h, -s);
    glVertex3f(s, -h, -s);
    glVertex3f(-s, -h, -s);
    glVertex3f(-s, -h, s);
    glVertex3f(-s, h, s);
    glVertex3f(-s, h, -s);
    glEnd();
    glPopMatrix();
}

void DrawFloor(float x, float z)
{
    glPushMatrix();
    glTranslatef(x + 0.5f, 0.0f, z + 0.5f);
    glColor3f(0.5f, 0.5f, 0.5f); // 회색 바닥
    glBegin(GL_QUADS);
    float s = 0.5f;
    glVertex3f(-s, 0, -s);
    glVertex3f(-s, 0, s);
    glVertex3f(s, 0, s);
    glVertex3f(s, 0, -s);
    glEnd();
    glPopMatrix();
}

// 무한 그리드 그리기 (카메라 시야에 들어오는 부분만 계산해서 그림)
void DrawInfiniteGrid(float camX, float camZ, float zoom, float aspectRatio)
{
    float viewH = zoom;
    float viewW = zoom * aspectRatio;

    // 현재 화면에 보이는 월드 좌표 범위 계산 (여유분 +2)
    int minX = (int)floor(camX - viewW / 2.0f) - 2;
    int maxX = (int)ceil(camX + viewW / 2.0f) + 2;
    int minZ = (int)floor(camZ - viewH / 2.0f) - 2;
    int maxZ = (int)ceil(camZ + viewH / 2.0f) + 2;

    glColor3f(0.3f, 0.3f, 0.3f); // 어두운 그리드 색
    glLineWidth(1.0f);
    glBegin(GL_LINES);

    // 세로선
    for (int x = minX; x <= maxX; ++x)
    {
        // 데이터 영역(0~MAP_LIMIT) 경계는 좀 더 밝게 표시할 수도 있음
        glVertex3f(x, 0, minZ);
        glVertex3f(x, 0, maxZ);
    }
    // 가로선
    for (int z = minZ; z <= maxZ; ++z)
    {
        glVertex3f(minX, 0, z);
        glVertex3f(maxX, 0, z);
    }
    glEnd();

    // 데이터 영역 경계 표시 (빨간색 라인)
    glColor3f(0.8f, 0.2f, 0.2f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(0, 0, 0);
    glVertex3f(MAP_LIMIT, 0, 0);
    glVertex3f(MAP_LIMIT, 0, MAP_LIMIT);
    glVertex3f(0, 0, MAP_LIMIT);
    glEnd();
    glLineWidth(1.0f);
}

// ==========================================
// 5. 메인 로직
// ==========================================
int main()
{
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Infinite Map Editor", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // VSync

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "GLAD Initialize Failed" << std::endl;
        return -1;
    }

    // 콜백 등록
    glfwSetScrollCallback(window, ScrollCallback);

    // ImGui Setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // --------------------------------------------------
        // [입력 처리] : 패닝 및 좌표 계산
        // --------------------------------------------------
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        float  aspectRatio = (float)display_w / (float)display_h;
        double mx, my;
        glfwGetCursorPos(window, &mx, &my);

        // 1. 유니티 스타일 패닝 (Q + 드래그)
        bool isQPressed = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;

        if (isQPressed && glfwGetMouseButton(window, 0) == GLFW_PRESS && !ImGui::GetIO().WantCaptureMouse)
        {
            if (!g_State.isPanning)
            {
                g_State.isPanning  = true;
                g_State.lastMouseX = mx;
                g_State.lastMouseY = my;
            }
            else
            {
                double dx = mx - g_State.lastMouseX;
                double dy = my - g_State.lastMouseY;

                // 화면 픽셀 이동을 월드 이동으로 변환
                float unitsPerPixel = g_State.zoomLevel / (float)display_h;

                // 카메라 이동 (마우스를 왼쪽으로 끌면 카메라는 오른쪽으로 가야 맵이 왼쪽으로 가는 효과)
                g_State.camX -= dx * unitsPerPixel;
                g_State.camZ -= dy * unitsPerPixel; // 2D Screen Y는 아래가 +

                g_State.lastMouseX = mx;
                g_State.lastMouseY = my;
            }
        }
        else
        {
            g_State.isPanning = false;
        }

        // 2. 마우스 피킹 (World Coordinate 계산)
        // 화면 중앙(display_w/2, display_h/2)이 camX, camZ에 해당함.
        float unitsPerPixel = g_State.zoomLevel / (float)display_h;
        float pixelDx       = (float)mx - (display_w / 2.0f);
        float pixelDy       = (float)my - (display_h / 2.0f);

        float worldMouseX = g_State.camX + (pixelDx * unitsPerPixel);
        float worldMouseZ = g_State.camZ + (pixelDy * unitsPerPixel);

        g_State.hoverX = (int)floor(worldMouseX);
        g_State.hoverZ = (int)floor(worldMouseZ);

        // 3. 그리기/지우기 (Q키 안 누르고, ImGui 아닐 때)
        if (!isQPressed && glfwGetMouseButton(window, 0) == GLFW_PRESS && !ImGui::GetIO().WantCaptureMouse)
        {
            int idx = g_State.map.GetIndex(g_State.hoverX, g_State.hoverZ);
            if (idx != -1)
            {
                g_State.map.tiles[idx] = g_State.currentBrush;
            }
        }

        // --------------------------------------------------
        // [렌더링]
        // --------------------------------------------------
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f); // 배경색 더 어둡게
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // Projection Setup
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float viewW = g_State.zoomLevel * aspectRatio;
        float viewH = g_State.zoomLevel;
        glOrtho(-viewW / 2, viewW / 2, -viewH / 2, viewH / 2, -100, 100); // 화면 중앙 기준 Ortho

        // View Setup
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef(90, 1, 0, 0); // Top-Down View

        // 카메라 위치 적용 (장면 전체를 카메라 반대 방향으로 이동)
        glTranslatef(-g_State.camX, 0.0f, -g_State.camZ);

        // 1. 그리드 그리기 (무한 효과)
        DrawInfiniteGrid(g_State.camX, g_State.camZ, g_State.zoomLevel, aspectRatio);

        // 2. 오브젝트 그리기 (화면에 보이는 범위 내의 것만 그리는 최적화 가능하지만, 여기선 전체 순회)
        // 최적화를 원하면 minX, maxX 범위 내의 타일만 순회하면 됨.
        for (int z = 0; z < MAP_LIMIT; ++z)
        {
            for (int x = 0; x < MAP_LIMIT; ++x)
            {
                int      idx = g_State.map.GetIndex(x, z);
                TileType t   = g_State.map.tiles[idx];

                if (t == TileType::Empty)
                    continue; // 빈 곳은 그리지 않음

                // 바닥이 아니면 바닥을 먼저 깔아줌 (벽도 바닥 위에 있어야 하므로)
                DrawFloor(x, z);

                if (t == TileType::Wall)
                    DrawCube(x, z, 0.8f, 0.3f, 0.3f, 0.5f); // 벽은 위로 솟음
                else if (t == TileType::PlayerSpawn)
                    DrawCube(x, z, 0.3f, 0.8f, 0.3f, 0.5f);
                else if (t == TileType::Goal)
                    DrawCube(x, z, 0.3f, 0.3f, 0.8f, 0.5f);
            }
        }

        // 3. 하이라이트 (커서)
        if (!g_State.isPanning)
        { // 패닝 중엔 하이라이트 숨김
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // 데이터 범위 밖이면 빨간색 경고, 안이면 흰색
            bool isValid = (g_State.map.GetIndex(g_State.hoverX, g_State.hoverZ) != -1);
            if (isValid)
                glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
            else
                glColor4f(1.0f, 0.0f, 0.0f, 0.3f);

            glBegin(GL_QUADS);
            // Z-fighting 방지용 높이 살짝 띄우기
            float hx = g_State.hoverX;
            float hz = g_State.hoverZ;
            float y  = 0.01f;
            // 만약 벽 위에 커서가 있다면 더 위로
            if (isValid && g_State.map.tiles[g_State.map.GetIndex(hx, hz)] != TileType::Empty &&
                g_State.map.tiles[g_State.map.GetIndex(hx, hz)] != TileType::Floor)
            {
                y = 1.01f;
            }

            glVertex3f(hx, y, hz);
            glVertex3f(hx + 1, y, hz);
            glVertex3f(hx + 1, y, hz + 1);
            glVertex3f(hx, y, hz + 1);
            glEnd();
            glDisable(GL_BLEND);
        }

        // --------------------------------------------------
        // [UI]
        // --------------------------------------------------
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
        ImGui::Begin("Tools", NULL, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Text("Pos: (%d, %d)", g_State.hoverX, g_State.hoverZ);
        ImGui::Text("Cam: (%.1f, %.1f) Zoom: %.1f", g_State.camX, g_State.camZ, g_State.zoomLevel);
        ImGui::TextDisabled("Hold 'Q' + Drag to Pan");
        ImGui::TextDisabled("Scroll to Zoom");
        ImGui::Separator();

        const char* items[]     = {"Eraser", "Floor", "Wall", "Start", "Goal"};
        int         currentItem = (int)g_State.currentBrush;
        for (int i = 0; i < 5; i++)
        {
            if (ImGui::RadioButton(items[i], currentItem == i))
                g_State.currentBrush = (TileType)i;
        }

        ImGui::Separator();
        static char buf[64] = "level_v2.txt";
        ImGui::InputText("File", buf, 64);
        if (ImGui::Button("Export"))
            SaveMap(buf, g_State.map);
        ImGui::SameLine();
        if (ImGui::Button("Import"))
            LoadMap(buf, g_State.map);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}