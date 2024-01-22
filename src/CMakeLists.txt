set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PUBLIC
    main.cpp
    Engine.cpp
    Renderer.cpp
    Editor.cpp
    Scene.cpp
    SceneLoader.cpp
    Camera.cpp
    Model.cpp
    Sky.cpp
    ThreadPool.cpp
    Physics.cpp
    MeshImporter.cpp
)

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)