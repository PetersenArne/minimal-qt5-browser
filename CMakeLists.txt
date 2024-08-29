cmake_minimum_required(VERSION 3.16)

project(MyQt6Project VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets WebEngineCore WebEngineWidgets)

add_executable(my_qt6_app main.cpp)

target_link_libraries(my_qt6_app PRIVATE Qt6::Widgets Qt6::WebEngineCore Qt6::WebEngineWidgets)

set_target_properties(my_qt6_app PROPERTIES
    AUTOMOC ON
    AUTORCC ON
    AUTOUIC ON
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
