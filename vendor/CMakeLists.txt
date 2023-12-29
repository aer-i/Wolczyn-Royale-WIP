add_subdirectory(ARLN)
add_subdirectory(fast_obj)
add_subdirectory(meshoptimizer)

target_include_directories(${PROJECT_NAME} PUBLIC fast_obj)
target_link_libraries(${PROJECT_NAME} PUBLIC ARLN meshoptimizer)