cmake_minimum_required(VERSION 2.8.3)
project(hope)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

#Comment this if don't want debug
#set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O1")

find_package(catkin REQUIRED COMPONENTS
  message_generation
  cv_bridge
  geometry_msgs
  image_transport
  pcl_ros
  rospy
  roscpp
  sensor_msgs
  std_msgs
  tf2
  tf2_ros
  visualization_msgs
  peanut_ods
)

# Set BOOST_LIBRARYDIR accordingly in case that PCL could not find Boost
set(BOOST_LIBRARYDIR /usr/lib/x86_64-linux-gnu)

find_package(Boost COMPONENTS system filesystem regex REQUIRED)

find_package(PCL 1.7 REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

find_package(OpenCV REQUIRED)
message("Found OpenCV ${OpenCV_VERSION}")

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_message_files(
  FILES
  Subsections.msg
)

generate_messages(
   DEPENDENCIES
   geometry_msgs
   std_msgs
)

catkin_package(
  CATKIN_DEPENDS 
  message_runtime
  cv_bridge
  INCLUDE_DIRS
)

# Declare a C++ library
add_library(${PROJECT_NAME}
  src/lib/get_cloud.cpp
  src/lib/z_growing.cpp
  src/lib/transform.cpp
  src/lib/plane_segment.cpp
  src/lib/utilities.cpp

  src/lib/get_cloud.h
  src/lib/z_growing.h
  src/lib/transform.h
  src/lib/plane_segment.h
  src/lib/utilities.h
)
target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
)
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_generate_message_cpp ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})

# Declare C++ executables

add_executable(${PROJECT_NAME}_node src/hope_node.cpp)
target_link_libraries(${PROJECT_NAME}_node
  ${catkin_LIBRARIES}
  ${GLOG_LIB}
  ${OpenCV_LIBS}
  ${PCL_LIBRARIES}
  ${PROJECT_NAME}
)
add_dependencies(${PROJECT_NAME}_node ${PROJECT_NAME}_node_generate_message_cpp ${${PROJECT_NAME}_EXPORTED_TARGETS})

add_executable(${PROJECT_NAME}_ros src/hope_ros.cpp)
target_link_libraries(${PROJECT_NAME}_ros
  ${catkin_LIBRARIES}
  ${PROJECT_NAME}
  ${GLOG_LIB}
  ${OpenCV_LIBS}
  ${PCL_LIBRARIES}
)
add_dependencies(${PROJECT_NAME}_ros ${PROJECT_NAME}_ros_generate_message_cpp ${${PROJECT_NAME}_EXPORTED_TARGETS})

add_executable(view_cloud src/view_cloud.cpp)
target_link_libraries(view_cloud ${PCL_LIBRARIES})
