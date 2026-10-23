add_executable(hg_tests
    main.cpp
    engine_tests.cpp)

target_link_libraries(hg_tests PRIVATE hg_engine hg_warnings)

add_test(NAME engine.viewport COMMAND hg_tests "viewport:")
add_test(NAME engine.input COMMAND hg_tests "input:")
add_test(NAME engine.fixed_step COMMAND hg_tests "fixed_step:")
add_test(NAME engine.log COMMAND hg_tests "log:")

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
    add_test(NAME assets.manifest
        COMMAND Python3::Interpreter ${PROJECT_SOURCE_DIR}/tools/check_assets.py ${PROJECT_SOURCE_DIR}/assets)
endif()
