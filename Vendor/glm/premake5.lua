project "glm"
    kind "Utility"
    language "C++"

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
