include(FetchContent)

# JOLT DEFINES ----------------------------------
set(DOUBLE_PRECISION OFF)
set(GENERATE_DEBUG_SYMBOLS OFF)
set(CROSS_PLATFORM_DETERMINISTIC OFF)
set(INTERPROCEDURAL_OPTIMIZATION ON)
set(FLOATING_POINT_EXCEPTIONS_ENABLED OFF)
set(OBJECT_LAYER_BITS 16)
# ASSIMP DEFINES --------------------------------
set(ASSIMP_NO_EXPORT ON CACHE BOOL "")
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "")
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "")
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "")
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "")
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "")
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "")
# -----------------------------------------------

FetchContent_Declare(
    JoltPhysics
    GIT_REPOSITORY "https://github.com/jrouwe/JoltPhysics"
    GIT_TAG "v4.0.2"
    SOURCE_SUBDIR "Build"
)
FetchContent_MakeAvailable(JoltPhysics)

add_subdirectory(ARLN)
add_subdirectory(meshoptimizer)
add_subdirectory(assimp)

SET_INTERPROCEDURAL_OPTIMIZATION()

target_include_directories(${PROJECT_NAME} PUBLIC stb fast_obj ${JoltPhysics_SOURCE_DIR}/..)
target_link_libraries(${PROJECT_NAME} PUBLIC ARLN meshoptimizer Jolt assimp)