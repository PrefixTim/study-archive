cmake_minimum_required(VERSION 3.19)
project(ditrib VERSION 0.1.0 LANGUAGES C CXX)

add_executable(${PROJECT_NAME} ./basic/main.cpp)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

# include(FetchContent)
# 
# FetchContent_Declare(
#     Corrosion
#     GIT_REPOSITORY https://github.com/corrosion-rs/corrosion.git
#     GIT_TAG v0.4.4 # Optionally specify a commit hash, version tag or branch here
# )
# FetchContent_MakeAvailable(Corrosion)
# corrosion_import_crate(MANIFEST_PATH stack_lib/Cargo.toml)
# corrosion_experimental_cbindgen(
#     TARGET stack_lib
#     MANIFEST_PATH stack_lib/Cargo.toml
#     HEADER_NAME stack_lib.h
# )

# target_link_libraries(${PROJECT_NAME} PUBLIC stack_lib)

# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND python3 "${CMAKE_CURRENT_LIST_DIR}/graph/plot.py"
# )


