cmake_minimum_required(VERSION 3.8)
project(diagnostic_monitor)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(diagnostic_msgs REQUIRED)

# Include headers
include_directories(include)

# Executable
add_executable(diagnostic_monitor
    src/diagnostic_monitor.cpp
    src/node_monitor.cpp
    src/topic_monitor.cpp
    src/system_monitor.cpp
    src/analyzer.cpp
)



# Dependencies
ament_target_dependencies(
    diagnostic_monitor
    rclcpp
    std_msgs
    diagnostic_msgs
)

# Include directory
target_include_directories(diagnostic_monitor PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# Install executable
install(TARGETS
  diagnostic_monitor
  DESTINATION lib/${PROJECT_NAME}
)

# Install headers
install(
  DIRECTORY include/
  DESTINATION include
)
install(
  DIRECTORY
    launch
    config
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
