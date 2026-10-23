cmake_minimum_required(VERSION 3.24)

project(SuperHeartGold
    VERSION 0.1.0
    DESCRIPTION "Super HeartGold: jogo nativo para PC e Mac"
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Executáveis (e o .app no macOS) ficam todos em build/bin.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

option(HG_BUILD_APP "Compila o executável SDL3" ON)
option(HG_BUILD_TESTS "Compila os testes unitários" ON)
option(HG_FETCH_SDL3 "Baixa e compila o SDL3 estático em vez de usar o instalado" OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Tipo de build" FORCE)
endif()

# Avisos de compilação compartilhados por todos os alvos do projeto.
add_library(hg_warnings INTERFACE)
if(MSVC)
    target_compile_options(hg_warnings INTERFACE /W4 /permissive- /utf-8)
else()
    target_compile_options(hg_warnings INTERFACE -Wall -Wextra -Wpedantic)
endif()

# A engine não conhece SDL nem dados de região.
add_subdirectory(engine)

if(HG_BUILD_APP)
    if(HG_FETCH_SDL3)
        include(FetchContent)
        set(SDL_SHARED OFF CACHE BOOL "" FORCE)
        set(SDL_STATIC ON CACHE BOOL "" FORCE)
        set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
        FetchContent_Declare(SDL3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG release-3.4.18
            GIT_SHALLOW TRUE)
        FetchContent_MakeAvailable(SDL3)
    else()
        find_package(SDL3 REQUIRED CONFIG COMPONENTS SDL3)
    endif()
    add_subdirectory(platform/sdl3)
endif()

if(HG_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
