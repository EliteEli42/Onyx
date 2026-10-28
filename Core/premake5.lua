local name = "Core"

project(name)
    kind "StaticLib"

    files {
        "include/**",
        "src/**"
    }

    includedirs { 
        "include"
    }

    externalincludedirs {
        path.join(SolutionRoot, "Vendor/glad/include"),
        path.join(SolutionRoot, "Vendor/glfw/include"),
        path.join(SolutionRoot, "Vendor/glm/include"),
        path.join(SolutionRoot, "Vendor/stb/include")
    }
    
    links { 
        "glad",
        "glfw",
        "stb"
    } 

    vpaths {
        ["Header Files/*"] = "include/" .. name .. "/**",
        ["Source Files/*"] = "src/**"
    }