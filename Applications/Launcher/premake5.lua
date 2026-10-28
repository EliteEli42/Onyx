local name = "Launcher"

project(name)
    kind "ConsoleApp"
        
    filter {
        "system:windows",
        "configurations:not Debug"
    }
        kind "WindowedApp"
        entrypoint "mainCRTStartup"

    filter {}

    files {
        "include/**",
        "src/**"
    }

    includedirs {
        "include",
    }

    externalincludedirs {
        path.join(SolutionRoot, "Vendor/glad/include"),
        path.join(SolutionRoot, "Vendor/glfw/include"),
        path.join(SolutionRoot, "Vendor/glm/include"),
        path.join(SolutionRoot, "Vendor/stb/include"),

        path.join(SolutionRoot, "Core/include")
    }
    
    links { 
        "Core"
    }                       

    vpaths {
        ["Header Files/*"] = "include/" .. name .. "/**",
        ["Source Files/*"] = "src/**"
    }