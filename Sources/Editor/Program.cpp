#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <spdlog/spdlog.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

static constexpr int MAP_LIMIT     = 16;

static constexpr const char* WINDOW_TITLE  = "Labyrinth Maker";
static constexpr int         WINDOW_WIDTH  = 1280;
static constexpr int         WINDOW_HEIGHT = 720;

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
        tiles.resize(width * height, TileType::Empty);
    }

    int GetIndex(int x, int z)
    {
        if (x < 0 || x >= width || z < 0 || z >= height)
            return -1;

        return z * width + x;
    }
};

struct EditorState
{
    float camX      = MAP_LIMIT / 2.0f;
    float camZ      = MAP_LIMIT / 2.0f;
    float zoomLevel = 15.0f;

    bool   isPanning  = false;
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;

    int      hoverX       = -1;
    int      hoverZ       = -1;
    TileType currentBrush = TileType::Floor;
    MapData  map;
};

EditorState g_State;

void SaveMap(const char* filename, const MapData& map)
{
    json j;
    j["width"]  = map.width;
    j["height"] = map.height;

    std::vector<int> tileInts(map.tiles.size());
    for (size_t i = 0; i < map.tiles.size(); ++i)
    {
        tileInts[i] = static_cast<int>(map.tiles[i]);
    }
    j["tiles"] = tileInts;

    std::ofstream out(filename);
    if (out.is_open())
    {
        out << j.dump(4);
        out.close();
        SPDLOG_INFO("Map saved: {}", filename);
    }
}

void LoadMap(const char* filename, MapData& map)
{
    std::ifstream in(filename);
    if (!in.is_open())
    {
        SPDLOG_ERROR("File not found: {}", filename);
        return;
    }

    json j;
    in >> j;
    in.close();

    map.width  = j["width"];
    map.height = j["height"];

    std::vector<int> tileInts = j["tiles"].get<std::vector<int>>();
    map.tiles.resize(tileInts.size());

    for (size_t i = 0; i < tileInts.size(); ++i)
    {
        map.tiles[i] = static_cast<TileType>(tileInts[i]);
    }

    SPDLOG_INFO("Map Loaded: {}", filename);
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return;

    g_State.zoomLevel -= (float)yoffset * 2.0f;
    if (g_State.zoomLevel < 2.0f)
        g_State.zoomLevel = 2.0f;
    if (g_State.zoomLevel > 100.0f)
        g_State.zoomLevel = 100.0f;
}

void DrawCube(float x, float z, float r, float g, float b, float hOffset = 0.0f, float height = 1.0f)
{
    float h = height / 2.0f;
    float s = 0.45f;

    glPushMatrix();
    glTranslatef(x + 0.5f, hOffset, z + 0.5f);

    glColor3f(r, g, b);
    glBegin(GL_QUADS);

    glVertex3f(-s, h, -s);
    glVertex3f(-s, h, s);
    glVertex3f(s, h, s);
    glVertex3f(s, h, -s);

    glColor3f(r * 0.8f, g * 0.8f, b * 0.8f);
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
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
    float s = 0.5f;
    glVertex3f(-s, 0, -s);
    glVertex3f(-s, 0, s);
    glVertex3f(s, 0, s);
    glVertex3f(s, 0, -s);
    glEnd();
    glPopMatrix();
}

void DrawInfiniteGrid(float camX, float camZ, float zoom, float aspectRatio)
{
    float viewH = zoom;
    float viewW = zoom * aspectRatio;

    int minX = (int)floor(camX - viewW / 2.0f) - 2;
    int maxX = (int)ceil(camX + viewW / 2.0f) + 2;
    int minZ = (int)floor(camZ - viewH / 2.0f) - 2;
    int maxZ = (int)ceil(camZ + viewH / 2.0f) + 2;

    glColor3f(0.3f, 0.3f, 0.3f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);

    for (int x = minX; x <= maxX; ++x)
    {
        glVertex3f(x, 0, minZ);
        glVertex3f(x, 0, maxZ);
    }
    for (int z = minZ; z <= maxZ; ++z)
    {
        glVertex3f(minX, 0, z);
        glVertex3f(maxX, 0, z);
    }
    glEnd();

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

int main(int, char**)
{
    if (!glfwInit())
    {
        SPDLOG_CRITICAL("GLFW initialize failed");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (!window)
    {
        SPDLOG_CRITICAL("GLFW Window initialize failed");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        SPDLOG_CRITICAL("GLAD initialize failed");
        return -1;
    }

    glfwSetScrollCallback(window, ScrollCallback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    SPDLOG_INFO("Labyrinth Maker initialized successfully!");
    SPDLOG_INFO("ImGui Version: {}", ImGui::GetVersion());

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        float  aspectRatio = (float)display_w / (float)display_h;
        double mx, my;
        glfwGetCursorPos(window, &mx, &my);

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

                float unitsPerPixel = g_State.zoomLevel / (float)display_h;

                g_State.camX -= dx * unitsPerPixel;
                g_State.camZ -= dy * unitsPerPixel;

                g_State.lastMouseX = mx;
                g_State.lastMouseY = my;
            }
        }
        else
        {
            g_State.isPanning = false;
        }

        float unitsPerPixel = g_State.zoomLevel / (float)display_h;
        float pixelDx       = (float)mx - (display_w / 2.0f);
        float pixelDy       = (float)my - (display_h / 2.0f);

        float worldMouseX = g_State.camX + (pixelDx * unitsPerPixel);
        float worldMouseZ = g_State.camZ + (pixelDy * unitsPerPixel);

        g_State.hoverX = (int)floor(worldMouseX);
        g_State.hoverZ = (int)floor(worldMouseZ);

        if (!isQPressed && glfwGetMouseButton(window, 0) == GLFW_PRESS && !ImGui::GetIO().WantCaptureMouse)
        {
            int idx = g_State.map.GetIndex(g_State.hoverX, g_State.hoverZ);
            if (idx != -1)
            {
                g_State.map.tiles[idx] = g_State.currentBrush;
            }
        }

        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float viewW = g_State.zoomLevel * aspectRatio;
        float viewH = g_State.zoomLevel;
        glOrtho(-viewW / 2, viewW / 2, -viewH / 2, viewH / 2, -100, 100);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef(90, 1, 0, 0);

        glTranslatef(-g_State.camX, 0.0f, -g_State.camZ);

        DrawInfiniteGrid(g_State.camX, g_State.camZ, g_State.zoomLevel, aspectRatio);

        for (int z = 0; z < MAP_LIMIT; ++z)
        {
            for (int x = 0; x < MAP_LIMIT; ++x)
            {
                int      idx = g_State.map.GetIndex(x, z);
                TileType t   = g_State.map.tiles[idx];

                if (t == TileType::Empty)
                    continue;

                DrawFloor(x, z);

                if (t == TileType::Wall)
                    DrawCube(x, z, 0.8f, 0.3f, 0.3f, 0.5f);
                else if (t == TileType::PlayerSpawn)
                    DrawCube(x, z, 0.3f, 0.8f, 0.3f, 0.5f);
                else if (t == TileType::Goal)
                    DrawCube(x, z, 0.3f, 0.3f, 0.8f, 0.5f);
            }
        }

        if (!g_State.isPanning)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            bool isValid = (g_State.map.GetIndex(g_State.hoverX, g_State.hoverZ) != -1);
            if (isValid)
                glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
            else
                glColor4f(1.0f, 0.0f, 0.0f, 0.3f);

            glBegin(GL_QUADS);
            float hx = g_State.hoverX;
            float hz = g_State.hoverZ;
            float y  = 0.01f;
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

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}