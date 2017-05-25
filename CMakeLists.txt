cmake_minimum_required(VERSION 3.5)
project(mShell)

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
        Shell_core/coreFuncLib.cpp Shell_core/coreFuncLib.h my_mkdir/ExternMkdir.cpp my_mkdir/ExternMkdir.h)
add_executable(mShell ${SOURCE_FILES} Env/Env.cpp Env/Env.h)


set(SOURCE_FILES my_ls/main_ls.cpp
        Shell_core/EmbeddedFunc.cpp Shell_core/EmbeddedFunc.h
        Env/ConsoleView.cpp Env/ConsoleView.h
        Env/User.cpp Env/User.h
        Env/Env.cpp Env/Env.h
        Env/Directory.cpp Env/Directory.h
        ExternalFunc.cpp ExternalFunc.h
        my_ls/ExternLS.cpp my_ls/ExternLS.h
        )
add_executable(mls ${SOURCE_FILES})



set(SOURCE_FILES my_rm/remove_main.cpp my_rm/deleter.cpp my_rm/deleter.h)

add_executable(mrm ${SOURCE_FILES})

set(SOURCE_FILES mv_master/mv_main.cpp)
add_executable(mmv ${SOURCE_FILES})

set(SOURCE_FILES cp_master/cp_main.cpp )
add_executable(mcp ${SOURCE_FILES})

set(SOURCE_FILES my_mkdir/mkdir_main.cpp
        my_mkdir/ExternMkdir.cpp my_mkdir/ExternMkdir.h
        Shell_core/EmbeddedFunc.cpp Shell_core/EmbeddedFunc.h
        Env/ConsoleView.cpp Env/ConsoleView.h
        Env/User.cpp Env/User.h
        Env/Env.cpp Env/Env.h
        Env/Directory.cpp Env/Directory.h
        ExternalFunc.cpp ExternalFunc.h)
add_executable(mmkdir ${SOURCE_FILES})


FIND_PACKAGE( Boost COMPONENTS filesystem program_options system REQUIRED )

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

target_link_libraries( mShell  ${Boost_LIBRARIES} )
#target_link_libraries( myLsStaticLauncher ${Boost_LIBRARIES} )
target_link_libraries( mrm ${Boost_LIBRARIES} )
target_link_libraries( mls ${Boost_LIBRARIES} )
target_link_libraries( mmkdir ${Boost_LIBRARIES} )




include(PVS-Studio.cmake)

#pvs_studio_add_target(TARGET analyze ALL
 #       FORMAT errorfile
  #      PREPROCESSOR gcc
#        LOG "Lab_2_shell.tasks"
   #     ANALYZE Lab_2_shell
    #    )

pvs_studio_add_target(TARGET analyze ALL
        OUTPUT FORMAT errorfile
        ANALYZE mShell
        )