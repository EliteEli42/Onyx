SolutionRoot = _SCRIPT_DIR

newoption {
    trigger = "build-dir",
    value = "DIR",
    description = "Name of the build output subdirectory"
}

local BuildRoot = path.join(SolutionRoot, "Build")
local BuildDirName = _OPTIONS["build-dir"]

if BuildDirName and BuildDirName ~= "" then
    BuildRoot = path.join(BuildRoot, BuildDirName)
end

workspace "Onyx"
    configurations { "Debug", "Release", "Dist" }
    architecture "x86_64"

    location(BuildRoot)
    startproject "Launcher"

    debugdir(path.join(SolutionRoot, "DebugDir"))

    language "C++"
    cppdialect "C++20"
    warnings "Extra"

    targetdir(path.join(
        BuildRoot, "bin", "%{cfg.buildcfg}", "%{prj.name}"
    ))

    objdir(path.join(
        BuildRoot, "obj", "%{cfg.buildcfg}", "%{prj.name}"
    ))

    filter "system:windows"
        systemversion "latest"
        defines {
            "NOMINMAX",
            "WIN32_LEAN_AND_MEAN"
        }

    filter "toolset:msc*"
        conformancemode "On"
        multiprocessorcompile "On"
        buildoptions { "/utf-8","/Zc:preprocessor" }
        externalwarnings "Off"

        staticruntime "On"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
        symbols "On"

    filter "configurations:Dist"
        defines { "NDEBUG", "DIST" }
        optimize "Full"
        symbols "Off"

    filter { "configurations:Dist", "toolset:msc*" }
        linktimeoptimization "On"

    filter {}

local originalProject = project

function project(name)
    originalProject(name)

    local projectDir = path.getrelative(
        SolutionRoot,
        _SCRIPT_DIR
    )

    if projectDir == "." then
        location(BuildRoot)
    else
        location(path.join(BuildRoot, "Projects", projectDir))
    end
end


include "Vendor"
include "Core"
include "Engine"
include "Applications"
-- include "Tests"
