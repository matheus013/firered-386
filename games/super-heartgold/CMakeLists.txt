cmake_minimum_required(VERSION 3.24)

project(SuperHeartGold
    VERSION 0.1.0
    DESCRIPTION "Super HeartGold: jogo nativo para PC e Mac"
    LANGUAGES C CXX)

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Super HeartGold requires a 64-bit host toolchain")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Executáveis (e o .app no macOS) ficam todos em build/bin.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

option(HG_BUILD_APP "Compila o executável SDL3" ON)
option(HG_BUILD_TESTS "Compila os testes unitários" ON)
option(HG_REFERENCE_TASK_PROBE "Private probe of pinned original C task code (not distributed)" OFF)
option(HG_FETCH_SDL3 "Baixa e compila o SDL3 estático em vez de usar o instalado" ON)

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
            URL https://github.com/libsdl-org/SDL/releases/download/release-3.4.18/SDL3-3.4.18.tar.gz
            URL_HASH SHA256=9c75cf16330322c217dedd2e0609f1124f1b54b8633e763467b4684d0f4334a3
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
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

if(HG_REFERENCE_TASK_PROBE)
    find_package(Python3 REQUIRED COMPONENTS Interpreter)
    set(HG_TASK_SOURCE_DIR "${PROJECT_BINARY_DIR}/private-reference-tasks")
    execute_process(COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/tools/prepare_task_probe.py" "${HG_TASK_SOURCE_DIR}"
        RESULT_VARIABLE HG_PREPARE_RESULT)
    if(NOT HG_PREPARE_RESULT EQUAL 0)
        message(FATAL_ERROR "Private reference preparation failed; nothing was copied into the product source tree")
    endif()
    add_executable(hg_reference_tasks "${HG_TASK_SOURCE_DIR}/sys_task.c" tests/reference_tasks.cpp)
    target_include_directories(hg_reference_tasks PRIVATE "${HG_TASK_SOURCE_DIR}" tools/reference_compat)
    target_link_libraries(hg_reference_tasks PRIVATE hg_warnings)
    if(HG_BUILD_TESTS)
        add_test(NAME reference.sys_task_native COMMAND hg_reference_tasks)
    endif()
endif()
