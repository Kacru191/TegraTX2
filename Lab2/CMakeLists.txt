cmake_minimum_required (VERSION 3.5)

project(first_app)

find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED) 


pkg_check_modules(GST REQUIRED gstreamer-1.0>=1.4 
                               gstreamer-sdp-1.0>=1.4 
                               gstreamer-video-1.0>=1.4 
                               gstreamer-app-1.0>=1.4) 

include_directories(${OpenCV_INCLUDE_DIRS}, ${GST_INCLUDE_DIRS})
add_definitions(${GSTREAMER_DEFINITIONS})

add_executable(first_app test.cpp)

MESSAGE( STATUS "GST_INCLUDE_DIRS:         " ${GST_INCLUDE_DIRS} )
MESSAGE( STATUS "GST_LIBRARIES:         " ${GST_LIBRARIES} )

target_link_libraries(first_app ${OpenCV_LIBS})
target_link_libraries(first_app ${GST_LIBRARIES})
