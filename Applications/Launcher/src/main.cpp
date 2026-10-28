#include <Core/Common/Log.h>
#include <Core/Common/Debug.h>

#include <Core/Window/Window.h>

#include <glad\glad.h>

using namespace Core;

static void KeyCallBack(Window& window, Key key, KeyAction action) {
    switch (key) {
        case Key::Escape:
            if(action == KeyAction::Release) 
                window.RequestClose();
            return;
    }
}

static void SizeCallBack(Window& window, IVec2 size) {
    (void)window;
    glViewport(0, 0, size.x, size.y);
}

int main() {
    Log::SetLevel(LogLevel::Trace);

    WindowDesc wDesc;
    wDesc.centered = true;
    wDesc.darkTitleBar = true;
    wDesc.width = 1280;
    wDesc.height = 720;
    wDesc.title = "Test Window";
    wDesc.transparent = true;

    Window window;
    ASSERT(window.Create(wDesc),"Startup Failed");

    window.SetKeyCallback(KeyCallBack);
    window.SetSizeCallback(SizeCallBack);

    glViewport(0, 0, wDesc.width, wDesc.height);
    glClearColor(0.0f, 0.4f, 0.4f, 0.8f);

    while (!window.ShouldClose()) {
        window.PollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        window.SwapBuffers();
    }

    return 0;
}
