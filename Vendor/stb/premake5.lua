project "stb"
    kind "StaticLib"
    language "C++"
    warnings "Off"

    files {
        "include/**",
        "src/**"
    }

    includedirs {
        "include"
    }

    vpaths {
        ["Header Files/*"] = { 
            "include/stb/**",
        },
        ["Source Files/*"] = {
            "src/**"
        }
    }
