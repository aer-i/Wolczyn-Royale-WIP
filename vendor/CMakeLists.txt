add_subdirectory(ARLN)
add_subdirectory(meshoptimizer)
add_subdirectory(assimp)

include(FetchContent)

# JOLT DEFINES ----------------------------------
set(DOUBLE_PRECISION OFF)
set(GENERATE_DEBUG_SYMBOLS OFF)
set(CROSS_PLATFORM_DETERMINISTIC OFF)
set(INTERPROCEDURAL_OPTIMIZATION ON)
set(FLOATING_POINT_EXCEPTIONS_ENABLED OFF)
set(OBJECT_LAYER_BITS 16)
# -----------------------------------------------
FetchContent_Declare(
    JoltPhysics
    GIT_REPOSITORY "https://github.com/jrouwe/JoltPhysics"
    GIT_TAG "v4.0.2"
    SOURCE_SUBDIR "Build"
)
FetchContent_MakeAvailable(JoltPhysics)

SET_INTERPROCEDURAL_OPTIMIZATION()

target_include_directories(${PROJECT_NAME} PUBLIC stb fast_obj ${JoltPhysics_SOURCE_DIR}/..)
target_link_libraries(${PROJECT_NAME} PUBLIC ARLN meshoptimizer Jolt assimp)