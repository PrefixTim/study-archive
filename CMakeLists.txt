cmake_minimum_required(VERSION 3.19)
project(atomic VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)

FetchContent_Declare(
    Corrosion
    GIT_REPOSITORY https://github.com/corrosion-rs/corrosion.git
    GIT_TAG v0.4 # Optionally specify a commit hash, version tag or branch here
)
FetchContent_MakeAvailable(Corrosion)
corrosion_import_crate(MANIFEST_PATH queue_lib/Cargo.toml)
# corrosion_cbindgen(
#     queue_lib
#     MANIFEST_PATH queue_lib/Cargo.toml
#     HEADER_NAME queue_lib.h
# )

add_executable(single_none ./threaded-expl/single_none.cpp)
target_link_libraries(single_none PUBLIC queue_lib)

add_executable(single_mutex ./threaded-expl/single_mutex.cpp)
target_link_libraries(single_mutex PUBLIC queue_lib)

add_executable(single_atomic ./threaded-expl/single_atomic.cpp)
target_link_libraries(single_atomic PUBLIC queue_lib)

add_executable(multi_none ./threaded-expl/multi_none.cpp)
target_link_libraries(multi_none PUBLIC queue_lib)

add_executable(multi_mutex ./threaded-expl/multi_mutex.cpp)
target_link_libraries(multi_mutex PUBLIC queue_lib)

add_executable(multi_atomic ./threaded-expl/multi_atomic.cpp)
target_link_libraries(multi_atomic PUBLIC queue_lib)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
