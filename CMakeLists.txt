cmake_minimum_required(VERSION 3.6)
project(Lab_2_shell)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp PromptConsoleInterface.cpp PromptConsoleInterface.h User.cpp User.h Directory.cpp Directory.h)
add_executable(Lab_2_shell ${SOURCE_FILES})

FIND_PACKAGE( Boost COMPONENTS filesystem program_options system REQUIRED )

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

target_link_libraries( Lab_2_shell ${Boost_LIBRARIES} )
