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
option(HG_REFERENCE_FILESYSTEM_PROBE "Private native resource module probe (not distributed)" OFF)
option(HG_REFERENCE_TITLE_FLOW "Private original title decision flow (2D host services only)" OFF)
option(HG_REFERENCE_TEXT_FLOW "Private original text printer decision flow (host pixel window)" OFF)
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

# Game data formats and policy are independent of SDL and the DS ABI.
add_library(hg_game_data STATIC game/core/src/save_image.cpp game/core/src/save_selection.cpp
    game/core/src/save_validation.cpp game/core/src/message_data.cpp
    game/core/src/text_commands.cpp game/core/src/newgame_blocks.cpp
    game/core/src/pokemon_crypto.cpp game/core/src/random.cpp
    game/core/src/roamer_init.cpp game/core/src/game_stats_init.cpp
    game/core/src/save_context.cpp game/core/src/identity_init.cpp
    game/core/src/crc_block_init.cpp game/core/src/pc_storage_init.cpp
    game/core/src/system_info_init.cpp game/core/src/friend_group_init.cpp game/core/src/misc_init.cpp)
target_include_directories(hg_game_data PUBLIC game/core/include)
target_link_libraries(hg_game_data PRIVATE hg_warnings)

add_executable(hg_save_inspect tools/save_inspect.cpp)
target_link_libraries(hg_save_inspect PRIVATE hg_game_data hg_warnings)
if(MINGW)
    target_link_options(hg_save_inspect PRIVATE -municode)
endif()

if(HG_REFERENCE_TITLE_FLOW)
    find_package(Python3 REQUIRED COMPONENTS Interpreter)
    set(HG_TITLE_SOURCE_DIR "${PROJECT_BINARY_DIR}/private-reference-title")
    execute_process(COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/tools/prepare_title_flow.py" "${HG_TITLE_SOURCE_DIR}"
        RESULT_VARIABLE HG_TITLE_PREPARE_RESULT)
    if(NOT HG_TITLE_PREPARE_RESULT EQUAL 0)
        message(FATAL_ERROR "Private title source preparation failed")
    endif()
    add_library(hg_title_flow STATIC game/core/src/title_session.cpp
        "${HG_TITLE_SOURCE_DIR}/overlay_manager.c" "${HG_TITLE_SOURCE_DIR}/title_decision.c")
    target_include_directories(hg_title_flow PUBLIC game/core/include PRIVATE "${HG_TITLE_SOURCE_DIR}" tools/reference_compat)
    target_link_libraries(hg_title_flow PRIVATE hg_warnings)
    set(HG_CHECK_SOURCE_DIR "${PROJECT_BINARY_DIR}/private-reference-checksave")
    execute_process(COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/tools/prepare_checksave.py" "${HG_CHECK_SOURCE_DIR}"
        RESULT_VARIABLE HG_CHECK_PREPARE_RESULT)
    if(NOT HG_CHECK_PREPARE_RESULT EQUAL 0)
        message(FATAL_ERROR "Private check-save decision preparation failed")
    endif()
    add_library(hg_checksave_flow STATIC game/core/src/checksave_session.cpp "${HG_CHECK_SOURCE_DIR}/checksave_decision.c")
    target_include_directories(hg_checksave_flow PUBLIC game/core/include PRIVATE tools/reference_compat)
    target_link_libraries(hg_checksave_flow PRIVATE hg_warnings)
endif()

if(HG_REFERENCE_TEXT_FLOW)
    find_package(Python3 REQUIRED COMPONENTS Interpreter)
    set(HG_TEXT_SOURCE_DIR "${PROJECT_BINARY_DIR}/private-reference-text")
    execute_process(COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/tools/prepare_text_flow.py" "${HG_TEXT_SOURCE_DIR}"
        RESULT_VARIABLE HG_TEXT_PREPARE_RESULT)
    if(NOT HG_TEXT_PREPARE_RESULT EQUAL 0)
        message(FATAL_ERROR "Private text printer source preparation failed")
    endif()
    add_library(hg_text_flow STATIC game/core/src/text_flow_session.cpp "${HG_TEXT_SOURCE_DIR}/text_decision.c")
    target_include_directories(hg_text_flow PUBLIC game/core/include PRIVATE tools/reference_compat)
    target_link_libraries(hg_text_flow PUBLIC hg_engine hg_game_data PRIVATE hg_warnings)
    if(NOT MSVC)
        set_source_files_properties("${HG_TEXT_SOURCE_DIR}/text_decision.c" PROPERTIES
            COMPILE_OPTIONS "-Wno-unknown-pragmas;-Wno-unused-parameter")
    endif()
endif()

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
    if(HG_REFERENCE_TEXT_FLOW)
        add_executable(hg_text_flow_tests tests/main.cpp tests/text_flow_session_tests.cpp)
        target_link_libraries(hg_text_flow_tests PRIVATE hg_text_flow hg_warnings)
        add_test(NAME reference.text_printer_native COMMAND hg_text_flow_tests)
    endif()
    if(HG_REFERENCE_TITLE_FLOW)
        add_executable(hg_title_flow_tests tests/title_session_tests.cpp)
        target_link_libraries(hg_title_flow_tests PRIVATE hg_title_flow hg_warnings)
        add_test(NAME reference.title_flow_native COMMAND hg_title_flow_tests)
        add_executable(hg_checksave_flow_tests tests/checksave_session_tests.cpp)
        target_link_libraries(hg_checksave_flow_tests PRIVATE hg_checksave_flow hg_warnings)
        add_test(NAME reference.checksave_controlled_flags COMMAND hg_checksave_flow_tests)
    endif()
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

if(HG_REFERENCE_FILESYSTEM_PROBE)
    find_package(Python3 REQUIRED COMPONENTS Interpreter)
    set(HG_FS_SOURCE_DIR "${PROJECT_BINARY_DIR}/private-reference-filesystem")
    execute_process(COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/tools/prepare_task_probe.py" "${HG_FS_SOURCE_DIR}" --filesystem
        RESULT_VARIABLE HG_FS_PREPARE_RESULT)
    if(NOT HG_FS_PREPARE_RESULT EQUAL 0)
        message(FATAL_ERROR "Private filesystem source preparation failed")
    endif()
    add_executable(hg_reference_filesystem "${HG_FS_SOURCE_DIR}/filesystem.c"
        tests/reference_filesystem.cpp tools/reference_compat/host_files.cpp)
    target_include_directories(hg_reference_filesystem PRIVATE "${HG_FS_SOURCE_DIR}" tools/reference_compat)
    target_link_libraries(hg_reference_filesystem PRIVATE hg_engine hg_warnings)
    if(HG_BUILD_TESTS)
        add_test(NAME reference.filesystem_native COMMAND hg_reference_filesystem)
    endif()
endif()
