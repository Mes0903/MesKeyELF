cmake_minimum_required(VERSION 3.11)

project(MesKeyELF)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
  message('OPENGL_FOUND-is-true')
else()
  message('OPENGL_FOUND-is-false')
endif()

if(WIN32)
  if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    MESSAGE("==================== USING MSVC TO COMILE ====================")
    add_compile_options(/wd4819 /wd4244 /wd4267 /wd4305 /utf-8 "/Zc:__cplusplus" "/Zc:char8_t-")
    set(CMAKE_CXX_FLAGS_DEBUG "/O2")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
  else()
    MESSAGE("==================== USING MINGW TO COMILE ====================")
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O3")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
  endif()
else()
  set(CMAKE_CXX_FLAGS "-Wall -Wextra")
  set(CMAKE_CXX_FLAGS_DEBUG "-g -O3")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

set(THIRD_DIR ${PROJECT_SOURCE_DIR}/3rdparty)
set(MesKeyELF_Dir ${PROJECT_SOURCE_DIR}/core)

add_subdirectory(${THIRD_DIR})

add_executable(
  ${PROJECT_NAME} 
  ${MesKeyELF_Dir}/src/showHandler.cpp
  ${MesKeyELF_Dir}/src/showMenu.cpp
  ${MesKeyELF_Dir}/src/showKeyConfigurator.cpp
  ${MesKeyELF_Dir}/src/showKeySender.cpp
  ${MesKeyELF_Dir}/src/showScreen.cpp
  ${PROJECT_SOURCE_DIR}/src/MesKeyELF.cpp
)

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE
    ${MesKeyELF_Dir}/header
    ${THIRD_DIR}/imgui
    ${THIRD_DIR}/implot
    ${THIRD_DIR}/glfw/include
    ${THIRD_DIR}/glad/include
    ${OPENGL_INCLUDE_DIRS}
)

target_link_libraries(
  ${PROJECT_NAME}
  IMGUI_LIB
  IMPLOT_LIB
  glfw
  glad
  ${OPENGL_LIBRARIES}
)

file(COPY ${PROJECT_SOURCE_DIR}/font DESTINATION ${CMAKE_CURRENT_BINARY_DIR})