local name = "Engine"

project(name)
    kind "StaticLib"

    files {
        "include/**",
        "src/**"
    }

    includedirs { 
        "include"
    }

    vpaths {
        ["Header Files/*"] = "include/" .. name .. "/**",
        ["Source Files/*"] = "src/**"
    }