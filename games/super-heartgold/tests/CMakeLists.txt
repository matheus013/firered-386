add_executable(hg_tests
    main.cpp
    engine_tests.cpp
    resource_tests.cpp
    graphics_tests.cpp
    compression_tests.cpp
    save_image_tests.cpp
    save_validation_tests.cpp)

target_link_libraries(hg_tests PRIVATE hg_engine hg_game_data hg_warnings)

add_test(NAME engine.viewport COMMAND hg_tests "viewport:")
add_test(NAME engine.input COMMAND hg_tests "input:")
add_test(NAME engine.fixed_step COMMAND hg_tests "fixed_step:")
add_test(NAME engine.log COMMAND hg_tests "log:")
add_test(NAME engine.resource COMMAND hg_tests "resource:")
add_test(NAME engine.graphics COMMAND hg_tests "graphics:")
add_test(NAME engine.compression COMMAND hg_tests "compression:")
add_test(NAME game.save_image COMMAND hg_tests "save_image:")
add_test(NAME game.save_validation COMMAND hg_tests "save_validation:")

add_executable(hg_save_selection_tests save_selection_tests.cpp)
target_link_libraries(hg_save_selection_tests PRIVATE hg_game_data hg_warnings)
add_test(NAME game.save_selection COMMAND hg_save_selection_tests)

if(HG_BUILD_APP)
    # Roda o executável real sem janela visível: driver de vídeo "dummy",
    # eventos sintéticos de teclado e gamepad, e saída após 120 quadros.
    add_test(NAME app.headless_smoke
        COMMAND SuperHeartGold --video-driver dummy --frames 120 --self-test)
    set_tests_properties(app.headless_smoke PROPERTIES
        TIMEOUT 30
        PASS_REGULAR_EXPRESSION "Autoteste OK")
endif()

find_package(Python3 COMPONENTS Interpreter)
if(Python3_FOUND)
    if(NOT CMAKE_CROSSCOMPILING)
        add_test(NAME game.save_inspector
            COMMAND Python3::Interpreter ${PROJECT_SOURCE_DIR}/tests/test_save_inspector.py $<TARGET_FILE:hg_save_inspect>)
        set_tests_properties(game.save_inspector PROPERTIES TIMEOUT 60)
    endif()
    if(HG_BUILD_APP AND NOT CMAKE_CROSSCOMPILING)
        add_test(NAME app.graphics_resources
            COMMAND Python3::Interpreter ${PROJECT_SOURCE_DIR}/tests/test_graphics_renderer.py $<TARGET_FILE:SuperHeartGold>)
        set_tests_properties(app.graphics_resources PROPERTIES TIMEOUT 90)
    endif()
    add_test(NAME tools.resource_import
        COMMAND Python3::Interpreter ${PROJECT_SOURCE_DIR}/tests/test_resource_import.py)
    add_test(NAME tools.synthetic_fixtures
        COMMAND Python3::Interpreter ${PROJECT_SOURCE_DIR}/tests/test_tools.py)
    add_test(NAME assets.manifest
        COMMAND Python3::Interpreter ${PROJECT_SOURCE_DIR}/tools/check_assets.py ${PROJECT_SOURCE_DIR}/assets)
endif()
