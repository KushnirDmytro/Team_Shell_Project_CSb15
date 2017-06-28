cmake_minimum_required(VERSION 3.5)
project(mShell)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS  "-pthread")
#SET(GCC_COVERAGE_LINK_FLAGS  "-lrt")


set(SOURCE_FILES core_main.cpp Shell_core/EmbeddedFunc.cpp Shell_core/EmbeddedFunc.h
        Shell_core/coreFuncLib.cpp Shell_core/coreFuncLib.h
        Env/ConsoleView.cpp Env/ConsoleView.h
        Env/User.cpp Env/User.h
        Env/Env.cpp Env/Env.h
        Env/Directory.cpp Env/Directory.h
        Shell_core/Utils/LineSplitter.cpp Shell_core/Utils/LineSplitter.h
        Shell_core/Utils/Tokenizer.cpp Shell_core/Utils/Tokenizer.h
        Shell_core/LaneInterpreter.cpp Shell_core/LaneInterpreter.h
        Shell_core/Utils/FileLaneIterator.cpp Shell_core/Utils/FileLaneIterator.h
        ExternalFunc.cpp ExternalFunc.h
        #my_ls/ExternLS.cpp my_ls/ExternLS.h
        #my_mkdir/ExternGrep.cpp my_mkdir/ExternGrep.h
        Shell_core/ReducerToTasks.cpp Shell_core/ReducerToTasks.h)
add_executable(mShell ${SOURCE_FILES} Env/Env.cpp Env/Env.h)

set(SOURCE_FILES server.cpp Shell_core/EmbeddedFunc.cpp Shell_core/EmbeddedFunc.h
        Shell_core/coreFuncLib.cpp Shell_core/coreFuncLib.h
        Env/ConsoleView.cpp Env/ConsoleView.h
        Env/User.cpp Env/User.h
        Env/Env.cpp Env/Env.h
        Env/Directory.cpp Env/Directory.h
        Shell_core/Utils/LineSplitter.cpp Shell_core/Utils/LineSplitter.h
        Shell_core/Utils/Tokenizer.cpp

        Shell_core/Utils/Tokenizer.h
        Shell_core/LaneInterpreter.cpp Shell_core/LaneInterpreter.h
        Shell_core/Utils/FileLaneIterator.cpp Shell_core/Utils/FileLaneIterator.h
        ExternalFunc.cpp ExternalFunc.h
        my_ls/ExternLS.cpp my_ls/ExternLS.h
        my_mkdir/ExternGrep.cpp my_mkdir/ExternMkdir.h
        Shell_core/ReducerToTasks.cpp Shell_core/ReducerToTasks.h

        ${GCC_COVERAGE_LINK_FLAGS})
add_executable(shell_server ${SOURCE_FILES} Env/Env.cpp Env/Env.h)


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
        my_mkdir/ExternGrep.cpp my_mkdir/ExternMkdir.h
        Shell_core/EmbeddedFunc.cpp Shell_core/EmbeddedFunc.h
        Env/ConsoleView.cpp Env/ConsoleView.h
        Env/User.cpp Env/User.h
        Env/Env.cpp Env/Env.h
        Env/Directory.cpp Env/Directory.h
        ExternalFunc.cpp ExternalFunc.h)
add_executable(mmkdir ${SOURCE_FILES})

set(SOURCE_FILES my_grep/mgrep_main.cpp
        my_grep/ExternGrep.cpp my_grep/ExternGrep.h
        Shell_core/EmbeddedFunc.cpp Shell_core/EmbeddedFunc.h
        ExternalFunc.cpp ExternalFunc.h
        Env/ConsoleView.cpp Env/ConsoleView.h
        Env/User.cpp Env/User.h
        Env/Env.cpp Env/Env.h
        Env/Directory.cpp Env/Directory.h
        )
add_executable(mgrep ${SOURCE_FILES})



set(SOURCE_FILES clientTCP.cpp
        Env/User.cpp Env/User.h
        Env/Env.cpp Env/Env.h
        Env/Directory.cpp Env/Directory.h
        Env/ConsoleView.cpp Env/ConsoleView.h
        Shell_core/Utils/FileLaneIterator.cpp Shell_core/Utils/FileLaneIterator.h

        ${GCC_COVERAGE_LINK_FLAGS})
add_executable(clientTCP ${SOURCE_FILES})

set(SOURCE_FILES ServerBoost.cpp ${GCC_COVERAGE_LINK_FLAGS})
add_executable(serverBoost ${SOURCE_FILES})



FIND_PACKAGE( Boost COMPONENTS filesystem program_options system thread REQUIRED )
set(CMAKE_VERBOSE_MAKEFILE ON)

link_directories(${Boost_LIBRARY_DIR})
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

target_link_libraries( serverBoost ${Boost_LIBRARIES} )

target_link_libraries( clientTCP ${Boost_LIBRARIES} )


#FIND_PACKAGE( Boost COMPONENTS filesystem program_options system REQUIRED )

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

target_link_libraries( mShell  ${Boost_LIBRARIES} )
target_link_libraries( shell_server  ${Boost_LIBRARIES} rt)
#target_link_libraries( myLsStaticLauncher ${Boost_LIBRARIES} )
target_link_libraries( mrm ${Boost_LIBRARIES} )
target_link_libraries( mls ${Boost_LIBRARIES} )
target_link_libraries( mmkdir ${Boost_LIBRARIES} )
target_link_libraries( mgrep ${Boost_LIBRARIES} )




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


#
#cmake_minimum_required(VERSION 3.6)
#project(socket_server)
#
#SET(CMAKE_CXX_FLAGS  "-pthread")
#
#set(CMAKE_CXX_STANDARD 11)
#
#set(SOURCE_FILES clientTCP.cpp ${GCC_COVERAGE_LINK_FLAGS})
#
#add_executable(clientTCP ${SOURCE_FILES})
#set(SOURCE_FILES ServerBoost.cpp ${GCC_COVERAGE_LINK_FLAGS})
#
#add_executable(serverBoost ${SOURCE_FILES})
#
#
#
#FIND_PACKAGE( Boost COMPONENTS filesystem program_options system thread REQUIRED )
#set(CMAKE_VERBOSE_MAKEFILE ON)
#
#link_directories(${Boost_LIBRARY_DIR})
#INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
#
#target_link_libraries( serverBoost ${Boost_LIBRARIES} )
#
#target_link_libraries( clientTCP ${Boost_LIBRARIES} )