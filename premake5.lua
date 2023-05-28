workspace "FartherThanWhere"
    configurations { "Debug", "Release", }
    location "_Premake"

project "FartherThanWhere"
    filter "configurations:Debug"
        kind "ConsoleApp"
    filter "configurations:Release"
        kind "WindowedApp"
    filter {}

    language "C++"
    architecture "x86_64"

    targetdir "_Build/Bin/%{cfg.buildcfg}"
    objdir "_Build/Objs/%{cfg.buildcfg}"

    includedirs { 
        "Codes/", 
        "_Libs/GLFW/include/", 
        "_Libs/GLAD/include/",
        "_Libs/GLM/include/",
        "_Libs/Stb_image/include/",
        "_Libs/FastNoise2/include/",
        "_Libs/FreeType/include/",
    }

    files { "Codes/Codes/**.cpp", "_Libs/Stb_image/Codes/stb_image.cpp", }

    buildoptions { "-Wall" }

    defines { "FASTNOISE_STATIC_LIB" }

    links { "gdi32", "GLFW", "GLAD", "FastNoise2", "FreeType", }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"

LIB_BINDIR = "_Libs/_Build/%{prj.name}/Bin/%{cfg.buildcfg}"
LIB_OBJDIR = "_Libs/_Build/%{prj.name}/Objs/%{cfg.buildcfg}"

project "GLFW"
    kind "StaticLib"
    language "C"
    architecture "x86_64"

    targetdir(LIB_BINDIR)
    objdir(LIB_OBJDIR)

    files {
        "_Libs/GLFW/src/glfw_config.h",
        "_Libs/GLFW/include/GLFW/glfw3.h",
        "_Libs/GLFW/include/GLFW/glfw3native.h",

        "_Libs/GLFW/src/context.c",
        "_Libs/GLFW/src/init.c",
        "_Libs/GLFW/src/input.c",
        "_Libs/GLFW/src/monitor.c",
        "_Libs/GLFW/src/vulkan.c",
        "_Libs/GLFW/src/window.c",
    }

    filter "system:windows"
        files {
            "_Libs/GLFW/src/win32_platform.h", 
            "_Libs/GLFW/src/win32_joystick.h",
            "_Libs/GLFW/src/wgl_context.h", 
            "_Libs/GLFW/src/egl_context.h", 
            "_Libs/GLFW/src/osmesa_context.h",

            "_Libs/GLFW/src/win32_init.c", 
            "_Libs/GLFW/src/win32_joystick.c",
            "_Libs/GLFW/src/win32_monitor.c", 
            "_Libs/GLFW/src/win32_time.c", 
            "_Libs/GLFW/src/win32_thread.c", 
            "_Libs/GLFW/src/win32_window.c",
            "_Libs/GLFW/src/wgl_context.c", 
            "_Libs/GLFW/src/egl_context.c", 
            "_Libs/GLFW/src/osmesa_context.c",
        }

        defines { 
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS",
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"

project "GLAD"
    kind "StaticLib"
    language "C"
    architecture "x86_64"

    targetdir(LIB_BINDIR)
    objdir(LIB_OBJDIR)

    includedirs { "_Libs/GLAD/include/" }

    files { "_Libs/GLAD/src/glad.c" }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"

project "FastNoise2"
    kind "StaticLib"
    language "C++"
    architecture "x86_64"

    targetdir(LIB_BINDIR)
    objdir(LIB_OBJDIR)

    includedirs { "_Libs/FastNoise2/include/" }

    files { 
        "_Libs/FastNoise2/src/FastSIMD/FastSIMD.cpp",
        "_Libs/FastNoise2/src/FastSIMD/FastSIMD_Level_AVX2.cpp",
        "_Libs/FastNoise2/src/FastSIMD/FastSIMD_Level_AVX512.cpp",
        "_Libs/FastNoise2/src/FastSIMD/FastSIMD_Level_Scalar.cpp",
        "_Libs/FastNoise2/src/FastSIMD/FastSIMD_Level_SSE2.cpp",
        "_Libs/FastNoise2/src/FastSIMD/FastSIMD_Level_SSE3.cpp",
        "_Libs/FastNoise2/src/FastSIMD/FastSIMD_Level_SSE41.cpp",
        "_Libs/FastNoise2/src/FastSIMD/FastSIMD_Level_SSE42.cpp",
        "_Libs/FastNoise2/src/FastSIMD/FastSIMD_Level_SSSE3.cpp ",

        "_Libs/FastNoise2/src/FastNoise/Metadata.cpp",
        "_Libs/FastNoise2/src/FastNoise/SmartNode.cpp",
        "_Libs/FastNoise2/src/FastNoise/FastNoise_C.cpp",
    }

    buildoptions { "-march=core-avx2", "-mavx512f -mavx512dq", }

    defines { "FASTNOISE_STATIC_LIB" }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"

project "FreeType"
    kind "StaticLib"

    language "C"
    architecture "x86_64"

    targetdir(LIB_BINDIR)
    objdir(LIB_OBJDIR)

    includedirs { "_Libs/FreeType/include/" }

    files { 
        "_Libs/FreeType/src/autofit/autofit.c",
        "_Libs/FreeType/src/base/ftbase.c",
        "_Libs/FreeType/src/base/ftbbox.c",
        "_Libs/FreeType/src/base/ftbdf.c",
        "_Libs/FreeType/src/base/ftbitmap.c",
        "_Libs/FreeType/src/base/ftcid.c",
        "_Libs/FreeType/src/base/ftfstype.c",
        "_Libs/FreeType/src/base/ftgasp.c",
        "_Libs/FreeType/src/base/ftglyph.c",
        "_Libs/FreeType/src/base/ftgxval.c",
        "_Libs/FreeType/src/base/ftinit.c",
        "_Libs/FreeType/src/base/ftmm.c",
        "_Libs/FreeType/src/base/ftotval.c",
        "_Libs/FreeType/src/base/ftpatent.c",
        "_Libs/FreeType/src/base/ftpfr.c",
        "_Libs/FreeType/src/base/ftstroke.c",
        "_Libs/FreeType/src/base/ftsynth.c",
        "_Libs/FreeType/src/base/fttype1.c",
        "_Libs/FreeType/src/base/ftwinfnt.c",
        "_Libs/FreeType/src/bdf/bdf.c",
        "_Libs/FreeType/src/bzip2/ftbzip2.c",
        "_Libs/FreeType/src/cache/ftcache.c",
        "_Libs/FreeType/src/cff/cff.c",
        "_Libs/FreeType/src/cid/type1cid.c",
        "_Libs/FreeType/src/gzip/ftgzip.c",
        "_Libs/FreeType/src/lzw/ftlzw.c",
        "_Libs/FreeType/src/pcf/pcf.c",
        "_Libs/FreeType/src/pfr/pfr.c",
        "_Libs/FreeType/src/psaux/psaux.c",
        "_Libs/FreeType/src/pshinter/pshinter.c",
        "_Libs/FreeType/src/psnames/psnames.c",
        "_Libs/FreeType/src/raster/raster.c",
        "_Libs/FreeType/src/sdf/sdf.c",
        "_Libs/FreeType/src/sfnt/sfnt.c",
        "_Libs/FreeType/src/smooth/smooth.c",
        "_Libs/FreeType/src/svg/svg.c",
        "_Libs/FreeType/src/truetype/truetype.c",
        "_Libs/FreeType/src/type1/type1.c",
        "_Libs/FreeType/src/type42/type42.c",
        "_Libs/FreeType/src/winfonts/winfnt.c",        
    }

    filter "system:windows"
        files {
            "_Libs/FreeType/builds/windows/ftsystem.c",

            "_Libs/FreeType/builds/windows/ftdebug.c",
            "_Libs/FreeType/src/base/ftver.rc",
        }

    filter {}

    defines { "FT2_BUILD_LIBRARY" }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
