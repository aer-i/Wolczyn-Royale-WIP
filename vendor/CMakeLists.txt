add_subdirectory(ARLN)
add_subdirectory(fast_obj)
add_subdirectory(meshoptimizer)

find_package(ReactPhysics3D REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC fast_obj stb)
target_link_libraries(${PROJECT_NAME} PUBLIC ARLN meshoptimizer ReactPhysics3D::ReactPhysics3D)