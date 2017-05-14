cmake_minimum_required(VERSION 3.5)
project(Lab_2_shell)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES core_main.cpp Shell_core/EmbeddedFunc.cpp Shell_core/EmbeddedFunc.h
        Env/ConsoleView.cpp Env/ConsoleView.h
        Env/User.cpp Env/User.h
        Env/Env.cpp Env/Env.h
        Env/Directory.cpp Env/Directory.h
        Shell_core/Utils/LineSplitter.cpp Shell_core/Utils/LineSplitter.h
        Shell_core/LaneInterpreter.cpp Shell_core/LaneInterpreter.h
        Shell_core/Utils/FileLaneIterator.cpp Shell_core/Utils/FileLaneIterator.h
        ExternalFunc.cpp ExternalFunc.h
        my_ls/ExternLS.cpp my_ls/ExternLS.h
        Env/shell_environment.h)
add_executable(Lab_2_shell ${SOURCE_FILES} Env/Env.cpp Env/Env.h Env/shell_environment.h)


set(SOURCE_FILES my_ls/main_ls.cpp
        Shell_core/EmbeddedFunc.cpp Shell_core/EmbeddedFunc.h
        Env/ConsoleView.cpp Env/ConsoleView.h
        Env/User.cpp Env/User.h
        Env/Env.cpp Env/Env.h
        Env/Directory.cpp Env/Directory.h
        Shell_core/Utils/LineSplitter.cpp Shell_core/Utils/LineSplitter.h
        Shell_core/LaneInterpreter.cpp Shell_core/LaneInterpreter.h
        Shell_core/Utils/FileLaneIterator.cpp Shell_core/Utils/FileLaneIterator.h
        ExternalFunc.cpp ExternalFunc.h
        my_ls/ExternLS.cpp my_ls/ExternLS.h
        Env/shell_environment.h )
add_executable(ls ${SOURCE_FILES})



set(SOURCE_FILES my_rm/remove_main.cpp my_rm/deleter.cpp my_rm/deleter.h)

add_executable(rm ${SOURCE_FILES})


FIND_PACKAGE( Boost COMPONENTS filesystem program_options system REQUIRED )

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

target_link_libraries( Lab_2_shell  ${Boost_LIBRARIES} )
#target_link_libraries( my_ls ${Boost_LIBRARIES} )
target_link_libraries( rm ${Boost_LIBRARIES} )
target_link_libraries( ls ${Boost_LIBRARIES} )




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