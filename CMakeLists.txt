cmake_minimum_required(VERSION 3.31)
project(GameEngine)

set(CMAKE_CXX_STANDARD 20)

find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
find_package(Bullet CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)

add_executable(GameEngine src/main.cpp)

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

