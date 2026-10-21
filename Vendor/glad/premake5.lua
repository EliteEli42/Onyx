project "glad"
    kind "StaticLib"
    language "C"

    files {
        "include/**",
        "src/**"
    }

    includedirs {
        "include"
    }

    vpaths {
        ["Header Files/*"] = { 
            "include/glad/**",
            "include/KHR/**" 
        },
        ["Source Files/*"] = {
            "src/**"
        }
    }
