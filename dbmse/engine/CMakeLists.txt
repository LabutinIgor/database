cmake_minimum_required(VERSION 3.7)
project(engine)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/demo/bin")

set(CMAKE_CXX_STANDARD 11)

include_directories(interface)

set(SOURCE_FILES
        demo/src/demo.cpp
        demo/src/pgetnextnode.cpp
        demo/src/pgetnextnode.h
        demo/src/PJoinNLNode.cpp
        demo/src/PJoinNLNode.h
        demo/src/pselectnode.cpp
        demo/src/pselectnode.h
        demo/data/table1
        demo/data/table2
        interface/basics.h
        interface/interface.cpp
        interface/interface.h demo/src/PCrossProductNode.cpp demo/src/PCrossProductNode.h demo/src/PProjectNode.cpp demo/src/PProjectNode.h demo/src/PUniqueNode.cpp demo/src/PUniqueNode.h demo/src/PUnionNode.cpp demo/src/PUnionNode.h demo/src/PSortJoinNode.cpp demo/src/PSortJoinNode.h demo/src/PHashJoinNode.cpp demo/src/PHashJoinNode.h demo/src/PSortNode.cpp demo/src/PSortNode.h)

add_executable(engine ${SOURCE_FILES})