add_library(hg_engine STATIC
    src/fixed_step.cpp
    src/input.cpp
    src/log.cpp
    src/viewport.cpp)

target_include_directories(hg_engine PUBLIC include)
target_link_libraries(hg_engine PRIVATE hg_warnings)
