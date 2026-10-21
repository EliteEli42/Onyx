project "glfw"
    kind "StaticLib"
    language "C"

    files {
        "include/glfw/**.h"
    }

    files {
        "src/internal.h",
        "src/platform.h",
        "src/mappings.h",

        "src/context.c",
        "src/init.c",
        "src/input.c",
        "src/monitor.c",
        "src/platform.c",
        "src/vulkan.c",
        "src/window.c",
        "src/egl_context.c",
        "src/osmesa_context.c",

        "src/null_platform.h",
        "src/null_joystick.h",
        "src/null_init.c",
        "src/null_monitor.c",
        "src/null_window.c",
        "src/null_joystick.c"
    }

    includedirs {
        "include"
    }

    filter "system:windows"
        systemversion "latest"

        defines {
            "_GLFW_WIN32",
            "_GLFW_WGL"
        }

        files {
            "src/win32_platform.h",
            "src/win32_joystick.h",
            "src/win32_init.c",
            "src/win32_joystick.c",
            "src/win32_module.c",
            "src/win32_monitor.c",
            "src/win32_time.c",
            "src/win32_thread.c",
            "src/win32_window.c",
            "src/wgl_context.c"
        }

        links {
            "user32",
            "gdi32",
            "shell32"
        }

    filter "system:linux"
        defines {
            "_GLFW_X11",
            "_GLFW_GLX"
        }

        files {
            "src/x11_platform.h",
            "src/xkb_unicode.h",
            "src/x11_init.c",
            "src/x11_monitor.c",
            "src/x11_window.c",
            "src/xkb_unicode.c",
            "src/posix_module.c",
            "src/posix_poll.c",
            "src/posix_time.c",
            "src/posix_thread.c",
            "src/glx_context.c",
            "src/linux_joystick.c"
        }

        links {
            "X11",
            "Xrandr",
            "Xinerama",
            "Xcursor",
            "Xi",
            "Xxf86vm",
            "GL",
            "dl",
            "pthread",
            "m"
        }

    filter "system:macosx"
        defines {
            "_GLFW_COCOA",
            "_GLFW_NSGL"
        }

        files {
            "src/cocoa_platform.h",
            "src/cocoa_joystick.h",
            "src/cocoa_init.m",
            "src/cocoa_joystick.m",
            "src/cocoa_monitor.m",
            "src/cocoa_window.m",
            "src/cocoa_time.c",
            "src/posix_module.c",
            "src/posix_poll.c",
            "src/posix_thread.c",
            "src/nsgl_context.m"
        }

        links {
            "Cocoa.framework",
            "IOKit.framework",
            "CoreFoundation.framework",
            "QuartzCore.framework"
        }

    filter {}

    vpaths {
        ["Header Files/*"] = {
            "include/glfw/**"
        },
        ["Source Files/*"] = {
            "src/**"
        }
    }
