cmake_minimum_required(VERSION 3.5)
project(Lab_2_shell)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Embedded_func.cpp Embedded_func.h
        Env/ConsoleView.cpp Env/ConsoleView.h Env/User.cpp Env/User.h
        Env/Directory.cpp Env/Directory.h Env/Line_splitter.cpp
        Env/Line_splitter.h Env/Interpreter.cpp Env/Interpreter.h
        Env/FileLaneIterator.cpp Env/FileLaneIterator.h
        External_func.cpp External_func.h
        Extern_LS.cpp Extern_LS.h
        Environment_objects.cpp)
add_executable(Lab_2_shell ${SOURCE_FILES})


set(SOURCE_FILES my_rm/remove_main.cpp my_rm/deleter.cpp my_rm/deleter.h)

add_executable(rm ${SOURCE_FILES})


FIND_PACKAGE( Boost COMPONENTS filesystem program_options system REQUIRED )

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

target_link_libraries( Lab_2_shell  ${Boost_LIBRARIES} )
#target_link_libraries( my_ls ${Boost_LIBRARIES} )
target_link_libraries( rm ${Boost_LIBRARIES} )




include(PVS-Studio.cmake)

#pvs_studio_add_target(TARGET analyze ALL
 #       FORMAT errorfile
  #      PREPROCESSOR gcc
#        LOG "Lab_2_shell.tasks"
   #     ANALYZE Lab_2_shell
    #    )

pvs_studio_add_target(TARGET analyze ALL
        OUTPUT FORMAT errorfile
        ANALYZE Lab_2_shell
        )