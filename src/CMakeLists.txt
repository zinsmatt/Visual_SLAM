cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

project(Visual_SLAM)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package( OpenCV REQUIRED )
find_package(Pangolin REQUIRED)

add_executable(${PROJECT_NAME}
  io.h
  io.cxx
  main.cxx
  visualization.h
  visualization.cxx
  )
target_link_libraries(${PROJECT_NAME}
  Eigen3::Eigen
  ${OpenCV_LIBS}
  ${Pangolin_LIBRARIES})
