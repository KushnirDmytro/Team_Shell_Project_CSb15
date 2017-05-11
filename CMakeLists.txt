cmake_minimum_required(VERSION 3.5)
project(Lab_2_shell)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Shell_core/Embedded_func.cpp Shell_core/Embedded_func.h
        Env/ConsoleView.cpp Env/ConsoleView.h
        Env/User.cpp Env/User.h
        Env/Env.cpp Env/Env.h
        Env/Directory.cpp Env/Directory.h
        Shell_core/Utils/LineSplitter.cpp Shell_core/Utils/LineSplitter.h
        Shell_core/LineInterpreter.cpp Shell_core/LineInterpreter.h
        Shell_core/Utils/FileLaneIterator.cpp Shell_core/Utils/FileLaneIterator.h
        External_func.cpp External_func.h
        Extern_LS.cpp Extern_LS.h
        Env/shell_environment.h)
add_executable(Lab_2_shell ${SOURCE_FILES} Env/Env.cpp Env/Env.h Env/shell_environment.h)


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