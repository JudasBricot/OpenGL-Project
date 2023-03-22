workspace "Judas-Engine"
    architecture "x64"
    startproject "Game"
    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Judas-Engine/vendor/GLFW/include"

include "Judas-Engine/vendor/GLFW"

project "Judas-Engine"
    location "Judas-Engine"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links 
	{ 
		"GLFW",
		"opengl32.lib"
	}

    filter "system:windows"
        cppdialect "C++17"
        staticruntime  "On"
        systemversion "latest"

        defines
        {
            "JE_PLATFORM_WINDOWS",
            "JE_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Game")
        }
    
    	filter "configurations:Debug"
		defines "HZ_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "HZ_DIST"
		buildoptions "/MD"
		optimize "On"

project "Game"
    location "Game"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
	"Judas-Engine/vendor/spdlog/include",
        "Judas-Engine/src"
    }

    links
    {
        "Judas-Engine"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime  "On"
        systemversion "latest"

        defines
        {
            "JE_PLATFORM_WINDOWS",
        }
    
    filter "configurations:Debug"
        defines "JE_DEBUG"
	buildoptions "/MDd"
        symbols "On"

    filter "configurations:Release"
        defines "JE_RELEASE"
	buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines "JE_DIST"
	buildoptions "/MD"
        optimize "On"