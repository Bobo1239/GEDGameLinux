# Visual Studio doesn't ship with a higher version
cmake_minimum_required(VERSION 3.12)

project(Vendor LANGUAGES C CXX)

# Header-only libraries

add_library(compat compat/src/compat.cpp)
target_include_directories(compat SYSTEM PUBLIC compat/include)
target_link_libraries(compat safestringlib salieri)

add_library(salieri INTERFACE)
target_include_directories(salieri SYSTEM INTERFACE salieri)

add_library(intrin INTERFACE)
target_include_directories(intrin SYSTEM INTERFACE intrin)

add_library(DirectXMath INTERFACE)
target_include_directories(DirectXMath SYSTEM INTERFACE DirectXMath/Inc)
# AVX has been available in CPUs since 2011 which should be a reasonable support target
target_compile_definitions(DirectXMath INTERFACE _XM_AVX_INTRINSICS_)
if(UNIX)
    # Causes compilation failure on Linux
    # Somehow everything still works on Linux while Windows fails to compile with this define
    target_compile_definitions(DirectXMath INTERFACE _XM_NO_XMVECTOR_OVERLOADS_)
    target_link_libraries(DirectXMath INTERFACE intrin)
endif()

# Standard libraries

add_library(safestringlib
    safestringlib/safeclib/safe_str_constraint.c
    safestringlib/safeclib/ignore_handler_s.c
    safestringlib/safeclib/wcscpy_s.c
    safestringlib/safeclib/wcsncpy_s.c
    safestringlib/safeclib/wcsnlen_s.c
    safestringlib/safeclib/wcscat_s.c
    safestringlib/safeclib/strcat_s.c
    safestringlib/safeclib/strcpy_s.c
)
target_include_directories(safestringlib PUBLIC safestringlib/include)

add_library(DXUT
    DXUT/Core/DXUT.cpp
    DXUT/Core/DXUTDevice11.cpp
    DXUT/Core/DXUTmisc.cpp
    DXUT/Core/DDSTextureLoader.cpp
    DXUT/Core/dxerr.cpp
    DXUT/Optional/DXUTres.cpp
    DXUT/Optional/DXUTcamera.cpp
    DXUT/Optional/DXUTgui.cpp
    DXUT/Optional/SDKmisc.cpp
    DXUT/Optional/DXUTsettingsdlg.cpp
    # Remaining optional stuff is missing but we don't use them anyway
)
if (MSVC)
    # Linux version has calls to functions from these files patched out
    target_sources(DXUT PRIVATE
        DXUT/Core/ScreenGrab.cpp
        DXUT/Core/WICTextureLoader.cpp
    )
endif()
target_include_directories(DXUT SYSTEM PUBLIC DXUT/Core)
target_include_directories(DXUT SYSTEM PUBLIC DXUT/Optional)
target_link_libraries(DXUT DirectXMath)
target_link_libraries(DXUT ole32 shell32 gdi32 comctl32 usp10)
target_compile_definitions(DXUT PUBLIC UNICODE)
if (UNIX)
    target_link_libraries(DXUT compat)
    # We're missing dxgidebug.h
    target_compile_definitions(DXUT PUBLIC NDEBUG)
endif()

add_library(FX11
    FX11/EffectAPI.cpp
    FX11/EffectLoad.cpp
    FX11/EffectNonRuntime.cpp
    FX11/EffectRuntime.cpp
    FX11/d3dxGlobal.cpp
    FX11/EffectReflection.cpp
)
target_include_directories(FX11 SYSTEM PUBLIC FX11/inc)
target_include_directories(FX11 SYSTEM PUBLIC FX11/Binary)
target_link_libraries(FX11 d3dcompiler uuid)
if (UNIX)
    target_link_libraries(FX11 compat)
    target_compile_options(FX11 PRIVATE -fpermissive)
endif()
