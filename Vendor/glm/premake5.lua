project "glm"
    kind "Utility"
    language "C++"
    warnings "Off"

    files {
        "include/**"
    }

    includedirs { 
        "include"
    }

    vpaths {
        ["Header Files/*"] = {
            "include/glm/**.h"
        },
        ["Source Files/*"] = {
            "include/glm/**"
        }
    }
