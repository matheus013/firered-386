add_executable(hg_tests
    main.cpp
    engine_tests.cpp
    resource_tests.cpp
    graphics_tests.cpp
    compression_tests.cpp
    font_tests.cpp
    save_image_tests.cpp
    save_validation_tests.cpp
    message_data_tests.cpp
    text_commands_tests.cpp
    newgame_blocks_tests.cpp
    pokemon_crypto_tests.cpp
    random_tests.cpp
    roamer_init_tests.cpp
    game_stats_init_tests.cpp
    save_context_tests.cpp
    identity_init_tests.cpp
    crc_block_init_tests.cpp
    pc_storage_init_tests.cpp
    system_info_init_tests.cpp
    friend_group_init_tests.cpp misc_init_tests.cpp frontier_init_tests.cpp)

target_link_libraries(hg_tests PRIVATE hg_engine hg_game_data hg_warnings)

add_test(NAME engine.viewport COMMAND hg_tests "viewport:")
add_test(NAME engine.input COMMAND hg_tests "input:")
add_test(NAME engine.fixed_step COMMAND hg_tests "fixed_step:")
add_test(NAME engine.log COMMAND hg_tests "log:")
add_test(NAME engine.resource COMMAND hg_tests "resource:")
add_test(NAME engine.graphics COMMAND hg_tests "graphics:")
add_test(NAME engine.compression COMMAND hg_tests "compression:")
add_test(NAME engine.font COMMAND hg_tests "font:")
add_test(NAME game.save_image COMMAND hg_tests "save_image:")
add_test(NAME game.save_validation COMMAND hg_tests "save_validation:")
add_test(NAME game.messages COMMAND hg_tests "messages:")
add_test(NAME game.text_commands COMMAND hg_tests "text commands:")
add_test(NAME game.newgame_blocks COMMAND hg_tests "newgame_blocks:")
add_test(NAME game.pokemon_crypto COMMAND hg_tests "pokemon crypto:")
add_test(NAME game.random COMMAND hg_tests "random:")
add_test(NAME game.roamer_init COMMAND hg_tests "roamer_init:")
add_test(NAME game.game_stats_init COMMAND hg_tests "game_stats_init:")
add_test(NAME game.save_context COMMAND hg_tests "save context:")
add_test(NAME game.identity_init COMMAND hg_tests "identity_init:")
add_test(NAME game.crc_block_init COMMAND hg_tests "crc block init:")
add_test(NAME game.pc_storage_init COMMAND hg_tests "pc_storage_init:")
add_test(NAME game.system_info_init COMMAND hg_tests "system_info_init:")
add_test(NAME game.frontier_init COMMAND hg_tests "frontier_init:")
add_test(NAME game.misc_init COMMAND hg_tests "misc_init:")
add_test(NAME game.friend_group_init COMMAND hg_tests "friend_group_init:")

add_executable(hg_save_selection_tests save_selection_tests.cpp)
target_link_libraries(hg_save_selection_tests PRIVATE hg_game_data hg_warnings)
add_test(NAME game.save_selection COMMAND hg_save_selection_tests)

add_executable(hg_native_overlay_tests native_overlay_tests.c)
target_link_libraries(hg_native_overlay_tests PRIVATE hg_native_overlays hg_warnings)
set_target_properties(hg_native_overlay_tests PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED ON)
add_test(NAME game.native_overlays COMMAND hg_native_overlay_tests)

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
        add_test(NAME app.text_resources
            COMMAND Python3::Interpreter ${PROJECT_SOURCE_DIR}/tests/test_text_renderer.py $<TARGET_FILE:SuperHeartGold>)
        set_tests_properties(app.text_resources PROPERTIES TIMEOUT 90)
        if(HG_REFERENCE_TEXT_FLOW)
            add_test(NAME app.text_flow_resources
                COMMAND Python3::Interpreter ${PROJECT_SOURCE_DIR}/tests/test_text_flow_renderer.py $<TARGET_FILE:SuperHeartGold>)
            set_tests_properties(app.text_flow_resources PROPERTIES TIMEOUT 180)
        endif()
        if(HG_REFERENCE_TITLE_FLOW)
            add_test(NAME app.title_text_resources
                COMMAND Python3::Interpreter ${PROJECT_SOURCE_DIR}/tests/test_title_text_renderer.py $<TARGET_FILE:SuperHeartGold>)
            set_tests_properties(app.title_text_resources PROPERTIES TIMEOUT 180)
        endif()
    endif()
    add_test(NAME tools.resource_import
        COMMAND Python3::Interpreter ${PROJECT_SOURCE_DIR}/tests/test_resource_import.py)
    add_test(NAME tools.synthetic_fixtures
        COMMAND Python3::Interpreter ${PROJECT_SOURCE_DIR}/tests/test_tools.py)
    add_test(NAME assets.manifest
        COMMAND Python3::Interpreter ${PROJECT_SOURCE_DIR}/tools/check_assets.py ${PROJECT_SOURCE_DIR}/assets)
endif()
