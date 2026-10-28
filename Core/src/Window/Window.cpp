#include <Core/Window/Window.h>
#include <Core/Common/Log.h>
#include <Core/Common/Debug.h>
#include <Core/Common/ScopeGuard.h>

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <glad/glad.h>

#if defined(_WIN32)

#ifndef NOMINMAX
#define NOMINMAX
#endif

#define GLFW_EXPOSE_NATIVE_WIN32

#include <Windows.h>
#include <dwmapi.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#pragma comment(lib, "dwmapi.lib")

#include <timeapi.h>
#pragma comment(lib, "winmm.lib")

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

static void EnableDarkTitlebar(HWND hwnd, bool enable)
{
    BOOL dark = enable;

    DwmSetWindowAttribute(
        hwnd,
        DWMWA_USE_IMMERSIVE_DARK_MODE,
        &dark,
        sizeof(dark)
    );
}

#else

#include <GLFW/glfw3.h>

#endif

namespace Core {

    class WindowCallback {
    public:
        static void Error(int error, const char* description) {
            Log::Error("GLFW Error: {} {}", error, description);
        }

        // GLFW to Key mapping
        static Key GLFWKeyToKey(int glfwKey) {
            switch (glfwKey) {
            case GLFW_KEY_SPACE: return Key::Space;
            case GLFW_KEY_APOSTROPHE: return Key::Apostrophe;
            case GLFW_KEY_COMMA: return Key::Comma;
            case GLFW_KEY_MINUS: return Key::Minus;
            case GLFW_KEY_PERIOD: return Key::Period;
            case GLFW_KEY_SLASH: return Key::Slash;
            case GLFW_KEY_0: return Key::Num0;
            case GLFW_KEY_1: return Key::Num1;
            case GLFW_KEY_2: return Key::Num2;
            case GLFW_KEY_3: return Key::Num3;
            case GLFW_KEY_4: return Key::Num4;
            case GLFW_KEY_5: return Key::Num5;
            case GLFW_KEY_6: return Key::Num6;
            case GLFW_KEY_7: return Key::Num7;
            case GLFW_KEY_8: return Key::Num8;
            case GLFW_KEY_9: return Key::Num9;
            case GLFW_KEY_SEMICOLON: return Key::Semicolon;
            case GLFW_KEY_EQUAL: return Key::Equal;
            case GLFW_KEY_A: return Key::A;
            case GLFW_KEY_B: return Key::B;
            case GLFW_KEY_C: return Key::C;
            case GLFW_KEY_D: return Key::D;
            case GLFW_KEY_E: return Key::E;
            case GLFW_KEY_F: return Key::F;
            case GLFW_KEY_G: return Key::G;
            case GLFW_KEY_H: return Key::H;
            case GLFW_KEY_I: return Key::I;
            case GLFW_KEY_J: return Key::J;
            case GLFW_KEY_K: return Key::K;
            case GLFW_KEY_L: return Key::L;
            case GLFW_KEY_M: return Key::M;
            case GLFW_KEY_N: return Key::N;
            case GLFW_KEY_O: return Key::O;
            case GLFW_KEY_P: return Key::P;
            case GLFW_KEY_Q: return Key::Q;
            case GLFW_KEY_R: return Key::R;
            case GLFW_KEY_S: return Key::S;
            case GLFW_KEY_T: return Key::T;
            case GLFW_KEY_U: return Key::U;
            case GLFW_KEY_V: return Key::V;
            case GLFW_KEY_W: return Key::W;
            case GLFW_KEY_X: return Key::X;
            case GLFW_KEY_Y: return Key::Y;
            case GLFW_KEY_Z: return Key::Z;
            case GLFW_KEY_LEFT_BRACKET: return Key::LeftBracket;
            case GLFW_KEY_BACKSLASH: return Key::Backslash;
            case GLFW_KEY_RIGHT_BRACKET: return Key::RightBracket;
            case GLFW_KEY_GRAVE_ACCENT: return Key::GraveAccent;
            case GLFW_KEY_WORLD_1: return Key::World1;
            case GLFW_KEY_WORLD_2: return Key::World2;
            case GLFW_KEY_ESCAPE: return Key::Escape;
            case GLFW_KEY_ENTER: return Key::Enter;
            case GLFW_KEY_TAB: return Key::Tab;
            case GLFW_KEY_BACKSPACE: return Key::Backspace;
            case GLFW_KEY_INSERT: return Key::Insert;
            case GLFW_KEY_DELETE: return Key::Delete;
            case GLFW_KEY_RIGHT: return Key::Right;
            case GLFW_KEY_LEFT: return Key::Left;
            case GLFW_KEY_DOWN: return Key::Down;
            case GLFW_KEY_UP: return Key::Up;
            case GLFW_KEY_PAGE_UP: return Key::PageUp;
            case GLFW_KEY_PAGE_DOWN: return Key::PageDown;
            case GLFW_KEY_HOME: return Key::Home;
            case GLFW_KEY_END: return Key::End;
            case GLFW_KEY_CAPS_LOCK: return Key::CapsLock;
            case GLFW_KEY_SCROLL_LOCK: return Key::ScrollLock;
            case GLFW_KEY_NUM_LOCK: return Key::NumLock;
            case GLFW_KEY_PRINT_SCREEN: return Key::PrintScreen;
            case GLFW_KEY_PAUSE: return Key::Pause;
            case GLFW_KEY_F1: return Key::F1;
            case GLFW_KEY_F2: return Key::F2;
            case GLFW_KEY_F3: return Key::F3;
            case GLFW_KEY_F4: return Key::F4;
            case GLFW_KEY_F5: return Key::F5;
            case GLFW_KEY_F6: return Key::F6;
            case GLFW_KEY_F7: return Key::F7;
            case GLFW_KEY_F8: return Key::F8;
            case GLFW_KEY_F9: return Key::F9;
            case GLFW_KEY_F10: return Key::F10;
            case GLFW_KEY_F11: return Key::F11;
            case GLFW_KEY_F12: return Key::F12;
            case GLFW_KEY_F13: return Key::F13;
            case GLFW_KEY_F14: return Key::F14;
            case GLFW_KEY_F15: return Key::F15;
            case GLFW_KEY_F16: return Key::F16;
            case GLFW_KEY_F17: return Key::F17;
            case GLFW_KEY_F18: return Key::F18;
            case GLFW_KEY_F19: return Key::F19;
            case GLFW_KEY_F20: return Key::F20;
            case GLFW_KEY_F21: return Key::F21;
            case GLFW_KEY_F22: return Key::F22;
            case GLFW_KEY_F23: return Key::F23;
            case GLFW_KEY_F24: return Key::F24;
            case GLFW_KEY_KP_0: return Key::KP0;
            case GLFW_KEY_KP_1: return Key::KP1;
            case GLFW_KEY_KP_2: return Key::KP2;
            case GLFW_KEY_KP_3: return Key::KP3;
            case GLFW_KEY_KP_4: return Key::KP4;
            case GLFW_KEY_KP_5: return Key::KP5;
            case GLFW_KEY_KP_6: return Key::KP6;
            case GLFW_KEY_KP_7: return Key::KP7;
            case GLFW_KEY_KP_8: return Key::KP8;
            case GLFW_KEY_KP_9: return Key::KP9;
            case GLFW_KEY_KP_DECIMAL: return Key::KPDecimal;
            case GLFW_KEY_KP_DIVIDE: return Key::KPDivide;
            case GLFW_KEY_KP_MULTIPLY: return Key::KPMultiply;
            case GLFW_KEY_KP_SUBTRACT: return Key::KPSubtract;
            case GLFW_KEY_KP_ADD: return Key::KPAdd;
            case GLFW_KEY_KP_ENTER: return Key::KPEnter;
            case GLFW_KEY_KP_EQUAL: return Key::KPEqual;
            case GLFW_KEY_LEFT_SHIFT: return Key::LeftShift;
            case GLFW_KEY_LEFT_CONTROL: return Key::LeftControl;
            case GLFW_KEY_LEFT_ALT: return Key::LeftAlt;
            case GLFW_KEY_LEFT_SUPER: return Key::LeftSuper;
            case GLFW_KEY_RIGHT_SHIFT: return Key::RightShift;
            case GLFW_KEY_RIGHT_CONTROL: return Key::RightControl;
            case GLFW_KEY_RIGHT_ALT: return Key::RightAlt;
            case GLFW_KEY_RIGHT_SUPER: return Key::RightSuper;
            case GLFW_KEY_MENU: return Key::Menu;
            default: return Key::Unknown;
            }
        }

        static MouseButton GLFWMouseToMouse(int glfwButton) {
            switch (glfwButton) {
            case GLFW_MOUSE_BUTTON_LEFT: return MouseButton::Left;
            case GLFW_MOUSE_BUTTON_RIGHT: return MouseButton::Right;
            case GLFW_MOUSE_BUTTON_MIDDLE: return MouseButton::Middle;
            case GLFW_MOUSE_BUTTON_4: return MouseButton::Button4;
            case GLFW_MOUSE_BUTTON_5: return MouseButton::Button5;
            case GLFW_MOUSE_BUTTON_6: return MouseButton::Button6;
            case GLFW_MOUSE_BUTTON_7: return MouseButton::Button7;
            case GLFW_MOUSE_BUTTON_8: return MouseButton::Button8;
            default: return MouseButton::Unknown;
            }
        }

        static KeyAction GLFWActionToKeyAction(int action) {
            switch (action)
            {
            case GLFW_RELEASE: return KeyAction::Release;
            case GLFW_PRESS: return KeyAction::Press;
            case GLFW_REPEAT: return KeyAction::Repeat;
            }
            return KeyAction::Release;
        }

        static void WindowSize(GLFWwindow* window, int width, int height) {
            Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (!win) return;
            win->mSize.x = width;
            win->mSize.y = height;

            if (!win->mSizeCallback) return;
            win->mSizeCallback(*win, win->mSize);
        }
        static void WindowRefresh(GLFWwindow* window) {
            Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (!win) return;

            if (!win->mRefreshCallback) return;
            win->mRefreshCallback(*win);
        }
        static void WindowPos(GLFWwindow* window, int xpos, int ypos) {
            Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (!win) return;
            win->mPos.x = xpos;
            win->mPos.y = ypos;

            if (!win->mPositionCallback) return;
            win->mPositionCallback(*win, win->mPos);
        }

        static void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
            (void)scancode;
            (void)mods;
            Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (!win) return;
            if (!win->mKeyCallback) return;

            win->mKeyCallback(*win, GLFWKeyToKey(key), GLFWActionToKeyAction(action));
        }
        static void OnMouseButton(GLFWwindow* window, int button, int action, int mods) {
            (void)mods;
            Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (!win) return;
            if (!win->mMouseButtonCallback) return;

            win->mMouseButtonCallback(*win, GLFWMouseToMouse(button), GLFWActionToKeyAction(action));
        }

        static void CursorPos(GLFWwindow* window, double xpos, double ypos) {
            Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (!win) return;
            if (!win->mMouseMoveCallback) return;

            // If the window is not the main window, we need to adjust the mouse position to be relative to the main window
            if (win->mHandle != window)
            {
                int winX, winY;
                int mainX, mainY;

                glfwGetWindowPos(window, &winX, &winY);
                glfwGetWindowPos(win->mHandle, &mainX, &mainY);

                xpos = (xpos + winX) - mainX;
                ypos = (ypos + winY) - mainY;
            }

            win->mMouseMoveCallback(*win, IVec2(xpos, ypos));
        }
        static void OnScroll(GLFWwindow* window, double xoffset, double yoffset) {
            Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (!win) return;
            if (!win->mScrollCallback) return;

            win->mScrollCallback(*win, Vec2(xoffset, yoffset));
        }
        static void OnChar(GLFWwindow* window, unsigned int codepoint) {
            Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (!win) return;
            if (!win->mCharCallback) return;

            win->mCharCallback(*win, codepoint);
        }
        static void OnDrop(GLFWwindow* window, int count, const char** paths) {
            Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (!win) return;
            if (!win->mDropCallback) return;

            win->mDropCallback(*win, count, paths);
        }

        static void TitlebarHitTest(GLFWwindow* window, int x, int y, int* hit) {
            Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (!win) return;
            if (!win->mTitleBarHitTestCallback) return;

            *hit = win->mTitleBarHitTestCallback(*win, IVec2(x, y));
        }
    };

    class GLFWContext {
    public:
        static bool Init() {
            if (sWindowCount == 0) {

#if defined(DEBUG)
                glfwSetErrorCallback(WindowCallback::Error);
#endif // DEBUG

#if defined(_WIN32)
                timeBeginPeriod(1);
#endif
                if (!glfwInit()) {
                    return false; // Failed to initialize
                }
                Log::Info("GLFW initialized ({})", glfwGetVersionString());
                initializied = true;
            }
            sWindowCount++;
            return true;
        }
        static void Shutdown() {
            sWindowCount--;
            if (sWindowCount == 0) {

#if defined(_WIN32)
                timeEndPeriod(1);
#endif

                glfwTerminate();
                Log::Info("GLFW terminated.");
                initializied = false;
            }
        }

        static bool IsInitializied() { return initializied; }

    private:
        static inline std::atomic<int> sWindowCount = 0;
        static inline std::atomic<bool> initializied = false;
    };

    static GLFWmonitor* GetCurrentMonitor(GLFWwindow* window) {
        int nMonitors;
        GLFWmonitor** monitors = glfwGetMonitors(&nMonitors);

        int wx, wy, ww, wh;
        glfwGetWindowPos(window, &wx, &wy);
        glfwGetWindowSize(window, &ww, &wh);

        GLFWmonitor* bestMonitor = monitors[0];
        int maxArea = 0;

        for (int i = 0; i < nMonitors; ++i) {
            int mx, my;
            glfwGetMonitorPos(monitors[i], &mx, &my);
            const GLFWvidmode* mode = glfwGetVideoMode(monitors[i]);
            int mw = mode->width;
            int mh = mode->height;

            int overlapX = std::max(0, std::min(wx + ww, mx + mw) - std::max(wx, mx));
            int overlapY = std::max(0, std::min(wy + wh, my + mh) - std::max(wy, my));
            int area = overlapX * overlapY;

            if (area > maxArea) {
                maxArea = area;
                bestMonitor = monitors[i];
            }
        }

        return bestMonitor;
    }

#define REQUIRE_HANDLE(ret) \
    VERIFY_RETURN(mHandle, ret, "{}: Window has not been created.", __func__)

    Window::~Window() {
        Destroy();
    }

    bool Window::Create(const WindowDesc& desc) {

        VERIFY_RETURN(mHandle == nullptr, false, "Window already created.");

        bool firstWindow = !GLFWContext::IsInitializied();

        VERIFY_RETURN(GLFWContext::Init(), false, "Failed to initialize GLFW.");

        ScopeGuard cleanup([&] { Destroy(); });

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, desc.transparent ? GLFW_TRUE : GLFW_FALSE);

        switch (desc.mode) {
        case WindowMode::SystemDecorated:
            glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
            //glfwWindowHint(GLFW_TITLEBAR, GLFW_TRUE);
            break;
        case WindowMode::SystemNoTitleBar:
            glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
            //glfwWindowHint(GLFW_TITLEBAR, GLFW_FALSE);
            break;
        case WindowMode::FullyUndecorated:
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            //glfwWindowHint(GLFW_TITLEBAR, GLFW_FALSE);
            break;
        }

#ifndef DEBUG 
        std::thread wd(&Window::Watchdog, this);
        wd.detach();
#endif
        mHandle = glfwCreateWindow(desc.width, desc.height, desc.title, nullptr, desc.sharedContext ? desc.sharedContext->mHandle : nullptr);

        const char* err = nullptr;
        glfwGetError(&err);
        VERIFY_RETURN(mHandle, false, "Failed to create GLFW window: {}", err ? err : "unknown");

        Log::Info("Window '{}' created ({}x{})", desc.title, mSize.x, mSize.y);

        mCreated = true;

        mLastFrameTime = glfwGetTime();

        glfwGetWindowSize(mHandle, &mSize.x, &mSize.y);
        glfwGetWindowPos(mHandle, &mPos.x, &mPos.y);

        SetDarkTitleBar(desc.darkTitleBar);

        if (desc.centered) CenterWindow();

        glfwSetWindowSizeLimits(mHandle, desc.WindowSizeLimits.x, desc.WindowSizeLimits.y, desc.WindowSizeLimits.z, desc.WindowSizeLimits.w);

        SetActive();

        if (firstWindow) {
            VERIFY_RETURN(gladLoadGL(glfwGetProcAddress), false, "Failed to load glad");
        }

        Log::Info("OpenGL {}",(const char*)glGetString(GL_VERSION));
        Log::Info("{} / {}", (const char*)glGetString(GL_VENDOR), (const char*)glGetString(GL_RENDERER));

        glfwSetWindowAttrib(mHandle, GLFW_RESIZABLE, desc.resizeable);

        glfwSetWindowUserPointer(mHandle, this);

        glfwSetFramebufferSizeCallback(mHandle, WindowCallback::WindowSize);
        glfwSetWindowPosCallback(mHandle, WindowCallback::WindowPos);
        glfwSetWindowRefreshCallback(mHandle, WindowCallback::WindowRefresh);

        glfwSetKeyCallback(mHandle, WindowCallback::OnKey);
        glfwSetMouseButtonCallback(mHandle, WindowCallback::OnMouseButton);
        glfwSetCursorPosCallback(mHandle, WindowCallback::CursorPos);
        glfwSetScrollCallback(mHandle, WindowCallback::OnScroll);
        glfwSetCharCallback(mHandle, WindowCallback::OnChar);
        glfwSetDropCallback(mHandle, WindowCallback::OnDrop);
        //glfwSetTitlebarHitTestCallback(mHandle, WindowCallback::TitlebarHitTest);

        glfwSetWindowSize(mHandle, mSize.x, mSize.y + 1);
        glfwSetWindowSize(mHandle, mSize.x, mSize.y);

        cleanup.Dismiss();
        return true;
    }

    void Window::Destroy() {
        if (mHandle) glfwDestroyWindow(mHandle);
        mHandle = nullptr;

        mCreated = false;

        Log::Info("Window destroyed.");

        GLFWContext::Shutdown();
    }

    void Window::PollEvents()
    {
        glfwPollEvents();
    }

    void Window::SwapBuffers() {
        REQUIRE_HANDLE(void());
        if (mTargetFPS > 0.0) {
            double now = glfwGetTime();
            double elapsed = now - mLastFrameTime;
            double remaining = mTargetFrameTime - elapsed;

            if (remaining > 0.002) {
                std::this_thread::sleep_for(std::chrono::duration<double>(remaining - 0.002));
            }
            while (glfwGetTime() - mLastFrameTime < mTargetFrameTime) {}
        }

        glfwSwapBuffers(mHandle);

        double now = glfwGetTime();
        mDeltaTime = now - mLastFrameTime;
        mLastFrameTime = now;

        mCurrentFPS = (mDeltaTime > 0.0) ? (1.0 / mDeltaTime) : 0.0;
        if (mSmoothedFPS == 0) {
            mSmoothedFPS = static_cast<int>(round(mCurrentFPS));
        }
        else {
            mSmoothedFPS = static_cast<int>(round(mSmoothedFPS * 0.75 + mCurrentFPS * 0.25));
        }

        if (mRequestMinimize) {
            mRequestMinimize = false;
            Minimize();
        }
        else if (mRequestMaximize) {
            mRequestMaximize = false;
            Maximize();
        }
        else if (mRequestRestore) {
            mRequestRestore = false;
            Restore();
        }
    }

    bool Window::ShouldClose() const
    {
        REQUIRE_HANDLE(true);
        return mShouldClose || glfwWindowShouldClose(mHandle);
    }

    void Window::SetActive(bool active)
    {
        REQUIRE_HANDLE(void());
        if (active) glfwMakeContextCurrent(mHandle);
        else glfwMakeContextCurrent(nullptr);
    }

    void Window::Maximize()
    {
        REQUIRE_HANDLE(void());
        glfwMaximizeWindow(mHandle);
    }

    bool Window::IsMaximized() const
    {
        REQUIRE_HANDLE(false);
        return glfwGetWindowAttrib(mHandle, GLFW_MAXIMIZED) == GLFW_TRUE;
    }

    void Window::Minimize()
    {
        REQUIRE_HANDLE(void());
        glfwIconifyWindow(mHandle);
    }

    void Window::Restore()
    {
        REQUIRE_HANDLE(void());
        if (mIsFullscreen) {
            SetFullscreen(false);
        }

        glfwRestoreWindow(mHandle);
    }

    void Window::Hide() {
        REQUIRE_HANDLE(void());
        glfwHideWindow(mHandle);
    }

    void Window::Show() {
        REQUIRE_HANDLE(void());
        glfwShowWindow(mHandle);
    }

    bool Window::SetIcon(Image& image)
    {
        REQUIRE_HANDLE(false);
        VERIFY_RETURN(image.GetNrChannels() == 4, false, "SetIcon: icon must have 4 channels (RGBA), got {}.", image.GetNrChannels());
        VERIFY_RETURN(image.GetLayerCount() >= 1, false, "SetIcon: image has no layers.");

        GLFWimage icon{};
        icon.pixels = image.GetLayer(0);
        icon.width = image.GetWidth();
        icon.height = image.GetHeight();

        VERIFY_RETURN(icon.pixels, false, "SetIcon: image layer 0 has no pixel data.");

        glfwSetWindowIcon(mHandle, 1, &icon);
        return true;
    }

    void Window::SetFullscreen(bool enabled) {
        REQUIRE_HANDLE(void());
        if (enabled == mIsFullscreen) return;
        GLFWwindow* win = mHandle;
        GLFWmonitor* monitor = GetCurrentMonitor(win);
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        int mx, my;
        glfwGetMonitorPos(monitor, &mx, &my);

        glfwHideWindow(win);

        if (enabled) {
            glfwGetWindowPos(win, &mWindowedPos.x, &mWindowedPos.y);
            glfwGetWindowSize(win, &mWindowedSize.x, &mWindowedSize.y);

            glfwSetWindowAttrib(win, GLFW_DECORATED, GLFW_FALSE);
            glfwSetWindowAttrib(win, GLFW_RESIZABLE, GLFW_FALSE);
            glfwSetWindowPos(win, mx, my);
            glfwSetWindowSize(win, mode->width, mode->height);
        }
        else {
            glfwSetWindowAttrib(win, GLFW_DECORATED, GLFW_TRUE);
            glfwSetWindowAttrib(win, GLFW_RESIZABLE, GLFW_TRUE);
            glfwSetWindowPos(win, mWindowedPos.x, mWindowedPos.y);
            glfwSetWindowSize(win, mWindowedSize.x, mWindowedSize.y);
        }

        glfwShowWindow(win);
        mIsFullscreen = enabled;
    }

    void Window::SetPosition(IVec2 pos)
    {
        REQUIRE_HANDLE(void());
        glfwSetWindowPos(mHandle, pos.x, pos.y);
    }

    void Window::SetSize(IVec2 pos)
    {
        REQUIRE_HANDLE(void());
        glfwSetWindowSize(mHandle, pos.x, pos.y);
    }

    void Window::CenterWindow() {
        REQUIRE_HANDLE(void());
        GLFWmonitor* monitor = GetCurrentMonitor(mHandle);
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        IVec2 pos{
            (mode->width / 2) - (mSize.x / 2),
            (mode->height / 2) - (mSize.y / 2)
        };

        SetPosition(pos);
    }

    Image Window::GetScreenshot()
    {
        Image img;

        REQUIRE_HANDLE(img);

        SetActive();

        int width, height;
        glfwGetFramebufferSize(mHandle, &width, &height);

        if (width <= 0 || height <= 0)
            return img;

        img.CreateEmpty(width, height, 4);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        glPixelStorei(GL_PACK_ALIGNMENT, 1);

        glReadPixels(
            0, 0,
            width, height,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            img.GetData().data()
        );

        return img;
    }

    void Window::SetVSync(bool enabled)
    {
        REQUIRE_HANDLE(void());
        glfwSwapInterval(enabled ? 1 : 0);

        mVSync = enabled;

        if (enabled)
        {
            mTargetFPS = 0.0;
            mTargetFrameTime = 0.0;
        }
    }

    void Window::SetTitle(const char* title)
    {
        REQUIRE_HANDLE(void());
        glfwSetWindowTitle(mHandle, title);
    }

    void Window::SetDarkTitleBar(bool enabled)
    {
        REQUIRE_HANDLE(void());
#if defined(_WIN32)
        HWND hwnd = glfwGetWin32Window(mHandle);
        EnableDarkTitlebar(hwnd, enabled);
#endif
    }

    void Window::SetTargetFPS(double fps)
    {
        REQUIRE_HANDLE(void());
        mTargetFPS = fps;
        mTargetFrameTime = (fps > 0.0) ? (1.0 / fps) : 0.0;

        if (fps > 0.0)
        {
            SetVSync(false);
        }
    }

    IVec2 Window::GetMousePosition()
    {
        REQUIRE_HANDLE(IVec2(0,0));
        double x, y;
        glfwGetCursorPos(mHandle, &x, &y);
        return IVec2(x, y);
    }

    void Window::SetMousePosition(IVec2 pos)
    {
        REQUIRE_HANDLE(void());
        glfwSetCursorPos(mHandle, pos.x, pos.y);
    }

    void Window::CenterMouse()
    {
        REQUIRE_HANDLE(void());
        SetMousePosition(GetSize() / 2);
    }

    void Window::SetCursorMode(CursorMode mode)
    {
        REQUIRE_HANDLE(void());
        mCursorMode = mode;

        switch (mode)
        {
        case CursorMode::Normal:
            glfwSetInputMode(mHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;

        case CursorMode::Hidden:
            glfwSetInputMode(mHandle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            break;

        case CursorMode::Locked:
            glfwSetInputMode(mHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        }
    }

    void Window::Watchdog()
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));

        ASSERT(mCreated, "Window creation timed out! Killing process.");
    }

} // namespace Core