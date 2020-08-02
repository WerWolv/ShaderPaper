#include <windows.h>

#include <glad/gl.h>
#include <glad/wgl.h>

#include "texture.hpp"
#include "shader.hpp"

#include <chrono>
#include <string>
#include <thread>

static constexpr UINT MsgCreateWorkerWBehindDesktopIcons = 0x052C;
static constexpr bool OpenConsoleWindow = false;

static constexpr GLfloat vertexPositions[] = {
        -1.0F,  1.0F, 0.0F,
        -1.0F, -1.0F, 0.0F,
         1.0F, -1.0F, 0.0F,
         1.0F, -1.0F, 0.0F,
         1.0F,  1.0F, 0.0F,
        -1.0F,  1.0F, 0.0F
};

/**
 * Queries the desktop's wallpaper window
 * @return wallpaper window handler
 */
HWND getWallpaperWindow() {
    // Spawn a new WorkerW behind the desktop icons
    HWND hWndProgMan = FindWindow(L"ProgMan", nullptr);
    SendMessageTimeout(hWndProgMan, MsgCreateWorkerWBehindDesktopIcons, 0, 0, SMTO_NORMAL, 1000, nullptr);

    // Get the wallpaper window handle by enumerating through all window layers until
    // the window gets found which has SHELLDLL_DefView as a child. That's the wallpaper.
    HWND hWndWallpaper = nullptr;
    EnumWindows([](HWND hWnd, LPARAM lParam) -> BOOL {
        HWND hWndChild = FindWindowEx(hWnd, nullptr, L"SHELLDLL_DefView", nullptr);

        if (hWndChild != nullptr)
            *reinterpret_cast<HWND*>(lParam) = FindWindowEx(nullptr, hWnd, L"WorkerW", nullptr);

        return true;
    }, (LPARAM)&hWndWallpaper);

    return hWndWallpaper;
}

/**
 * Initialize glad's OpenGL context from a window device context
 * @param window Window to use OpenGL context from
 */
void initGLContextFromWindow(HDC hDcWindow) {
    constexpr PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,
        8,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    // Set pixel format
    SetPixelFormat(hDcWindow, ChoosePixelFormat(hDcWindow, &pfd), &pfd);
    wglMakeCurrent(hDcWindow, wglCreateContext(hDcWindow));

    // Initialize glad
    gladLoaderLoadWGL(hDcWindow);
    gladLoaderLoadGL();
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        puts("[*] Usage: shaderpaperd <shader papaer path> <target fps>\n");
        return EXIT_FAILURE;
    }

    if (!OpenConsoleWindow)
        ShowWindow(GetConsoleWindow(), SW_HIDE);

    const std::string themeFolder = argv[1];
    const double targetFps = std::strtoul(argv[2], nullptr, 10);

    if (targetFps < 1) {
        puts("[*] Invalid FPS specified. Must be >= 1");
        return EXIT_FAILURE;
    }


    const HWND hWndWallpaper = getWallpaperWindow();
    const HDC hDc = GetDC(hWndWallpaper);

    // Init OpenGL context
    initGLContextFromWindow(hDc);

    // Load shaders and textures
    wlp::gl::Shader shader(themeFolder + "\\vert.glsl", themeFolder + "\\frag.glsl");
    wlp::gl::Texture channel[4] = { themeFolder + "\\channel0.png", themeFolder + "\\channel1.png", themeFolder + "\\channel2.png", themeFolder + "\\channel3.png" };

    printf("%s\n", (themeFolder + "\\vert.glsl").c_str());

    // Get wallpaper window size
    RECT screenBounds = { 0 };
    GetWindowRect(hWndWallpaper, &screenBounds);

    // Initialize frame times for FPS limiting
    auto startTime = std::chrono::high_resolution_clock::now();
    auto frameStart = startTime;
    auto frameEnd = startTime;

    // Bind textures to texture slots
    channel[0].bind(0);
    channel[1].bind(1);
    channel[2].bind(2);
    channel[3].bind(3);

    // Creat vertex buffer and add vertex positions for a rectangle to it
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);

    GLuint vaId;
    glGenVertexArrays(1, &vaId);
    glBindVertexArray(vaId);

    struct tm *date = new struct tm();
    time_t now = { 0 };
    POINT mousePos = { 0 };

    // Main loop
    while (true) {
        // Update frame start time
        frameStart = std::chrono::high_resolution_clock::now();

        // Update current time
        {
            now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            localtime_s(date, &now);
        }

        // Update current mouse position
        GetCursorPos(&mousePos);

        // Clear screen
        glViewport(-screenBounds.right, -screenBounds.bottom, screenBounds.right * 2, screenBounds.bottom * 2);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind shader and specify uniform variables
        shader.use();
        shader.uniforms({ "iChannel0", "iChannel1", "iChannel2", "iChannel3",
                                      "iChannelResolution", "iResolution", "iTime", "iDate", "iTimeDelta", "iMouse" });

        // Initialize uniform variables
        shader.setUniform("iResolution", 1.0F, 1.0F);
        shader.setUniform("iTime", (frameStart - startTime).count() / 1'000'000'000.0F);
        shader.setUniform("iDate", 1900 + date->tm_year, date->tm_mon + 1, date->tm_mday, date->tm_sec);
        shader.setUniform("iTimeDelta", (frameStart - frameEnd).count() / 1'000'000'000.0F);
        shader.setUniform("iMouse", (GLint)mousePos.x, (GLint)mousePos.y);

        shader.setUniform("iChannel0", 0);
        shader.setUniform("iChannel1", 1);
        shader.setUniform("iChannel2", 2);
        shader.setUniform("iChannel3", 3);
        shader.setUniformIVec2Array("iChannelResolution", 4,
                channel[0].getWidth(), channel[0].getHeight(),
                channel[1].getWidth(), channel[1].getHeight(),
                channel[2].getWidth(), channel[2].getHeight(),
                channel[3].getWidth(), channel[3].getHeight());

        // Bind vertex buffer
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        // Render image calculated by shader
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(0);

        // Swap buffers to display new image as wallpaper
        wglSwapLayerBuffers(hDc, WGL_SWAP_MAIN_PLANE);

        // Limit FPS
        std::this_thread::sleep_until(frameStart + std::chrono::milliseconds((long long)(1'000 / targetFps)));
        frameEnd = std::chrono::high_resolution_clock::now();
    }

    delete date;

    return EXIT_SUCCESS;
}