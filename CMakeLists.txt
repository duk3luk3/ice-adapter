cmake_minimum_required(VERSION 3.0)

project(faf-ice-adapter
  VERSION 0.1
  LANGUAGES CXX C
  )

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" ICE_ADAPTER_COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" ICE_ADAPTER_COMPILER_SUPPORTS_CXX0X)
if(ICE_ADAPTER_COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(ICE_ADAPTER_COMPILER_SUPPORTS_CXX0X)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused")

if(NOT PKGCONFIG_FOUND)
  find_package(PkgConfig REQUIRED)
endif()
pkg_search_module(NICE REQUIRED nice)
pkg_search_module(JSON REQUIRED jsoncpp)

include_directories(${NICE_INCLUDE_DIRS})
include_directories(${JSON_INCLUDE_DIRS})

find_package(Boost REQUIRED COMPONENTS log program_options thread system)

add_custom_target(docs SOURCES README.md LICENSE)

pkg_search_module(GIOMM REQUIRED giomm-2.4)
include_directories(${GIOMM_INCLUDE_DIRS})

add_executable(faf-ice-adapter
  IceAdapter.cpp
  IceAdapterOptions.cpp
  IceAgent.cpp
  JsonRpcTcpServer.cpp
  JsonRpcTcpSession.cpp
  GPGNetServer.cpp
  GPGNetConnection.cpp
  GPGNetMessage.h
  logging.cpp
  main.cpp
  PeerRelay.cpp
)

target_link_libraries(faf-ice-adapter
  ${GIOMM_LIBRARIES}
  ${Boost_LIBRARIES}
  ${JSON_LIBRARIES}
  ${NICE_LIBRARIES}
)

target_compile_definitions(faf-ice-adapter PRIVATE BOOST_SYSTEM_NO_DEPRECATED)

if(NOT MINGW)
  target_link_libraries(faf-ice-adapter pthread)
  target_compile_definitions(faf-ice-adapter PRIVATE BOOST_LOG_DYN_LINK)
endif()

# Strip exe to reduce file size
add_custom_command(TARGET faf-ice-adapter
  POST_BUILD
  COMMAND ${CMAKE_STRIP} -s "$<TARGET_FILE:faf-ice-adapter>"
  COMMENT "Stripping executable $<TARGET_FILE_NAME:faf-ice-adapter>"
  )
