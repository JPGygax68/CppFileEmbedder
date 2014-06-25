cmake_minimum_required(VERSION 3.0)

#------------------------------------------------
# Project + basics
#

project(CppFileEmbedder)

add_executable(${PROJECT_NAME} embedfile.c)

#------------------------------------------------
# INSTALLATION
#

# Note: this requires CMAKE_INSTALL_PREFIX to be defined by the user

install(TARGETS ${PROJECT_NAME} 
  EXPORT ${PROJECT_NAME}-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)

install(EXPORT ${PROJECT_NAME}-targets DESTINATION "cmake")