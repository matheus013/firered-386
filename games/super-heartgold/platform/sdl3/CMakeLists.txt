add_executable(SuperHeartGold
    main.cpp
    title_preview.cpp
    message_preview.cpp
    preview_texture.cpp
    default_bindings.cpp)

target_link_libraries(SuperHeartGold PRIVATE hg_engine hg_game_data hg_warnings SDL3::SDL3)
if(HG_REFERENCE_TITLE_FLOW)
    target_sources(SuperHeartGold PRIVATE title_runtime.cpp)
    target_link_libraries(SuperHeartGold PRIVATE hg_title_flow hg_checksave_flow)
    target_compile_definitions(SuperHeartGold PRIVATE HG_PRIVATE_TITLE_FLOW=1)
endif()

if(APPLE)
    set_target_properties(SuperHeartGold PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/platform/macos/Info.plist.in
        MACOSX_BUNDLE_GUI_IDENTIFIER io.github.superheartgold
        MACOSX_BUNDLE_BUNDLE_NAME "Super HeartGold"
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION})
endif()

if(WIN32 AND NOT HG_FETCH_SDL3)
    # Com SDL3 dinâmico, copia SDL3.dll para junto do .exe.
    add_custom_command(TARGET SuperHeartGold POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:SuperHeartGold> $<TARGET_FILE_DIR:SuperHeartGold>
        COMMAND_EXPAND_LISTS)
endif()
