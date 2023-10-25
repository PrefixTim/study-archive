cmake_minimum_required(VERSION 3.19)
project(atomic VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)

FetchContent_Declare(
    Corrosion
    GIT_REPOSITORY https://github.com/corrosion-rs/corrosion.git
    GIT_TAG v0.4.4 # Optionally specify a commit hash, version tag or branch here
)
FetchContent_MakeAvailable(Corrosion)
corrosion_import_crate(MANIFEST_PATH stack_lib/Cargo.toml)
corrosion_experimental_cbindgen(
    TARGET stack_lib
    MANIFEST_PATH stack_lib/Cargo.toml
    HEADER_NAME stack_lib.h
)

add_executable(bench_stack ./threaded-expl/bench_stack.cpp)
target_link_libraries(bench_stack PUBLIC stack_lib)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
