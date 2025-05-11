cmake_minimum_required(VERSION 3.31)
project(GameEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
find_package(Bullet CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(Qt6 COMPONENTS Widgets Gui Core REQUIRED)

add_executable(GameEngine src/main.cpp
        src/editor/Editor.cpp
        src/editor/startwidget.cpp
        src/editor/startwidget.h
        src/editor/startwidget.ui
        src/editor/Editor.h
        src/editor/newprojectdialog.cpp
        src/editor/newprojectdialog.h
        src/editor/newprojectdialog.ui
        src/Define.h
        src/editor/mainwidget.cpp
        src/editor/mainwidget.h
        assets/main.qrc
)

target_link_libraries(GameEngine PRIVATE OpenAL::OpenAL)
target_link_libraries(GameEngine PRIVATE SDL3::SDL3)
target_link_libraries(GameEngine PRIVATE glm::glm)
target_link_libraries(GameEngine PRIVATE glfw)
target_link_libraries(GameEngine PRIVATE spdlog::spdlog)
target_link_libraries(GameEngine PRIVATE RapidJSON rapidjson)
target_link_libraries(GameEngine PRIVATE ${BULLET_LIBRARIES})
target_link_libraries(GameEngine PRIVATE Bullet3Dynamics)
target_link_libraries(GameEngine PRIVATE imgui::imgui)
target_link_libraries(GameEngine PRIVATE $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>)
target_link_libraries(GameEngine PRIVATE Vulkan::Vulkan)
target_link_libraries(GameEngine PRIVATE Qt6::Widgets
        Qt6::Core
        Qt6::Gui)

