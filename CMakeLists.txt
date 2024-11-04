cmake_minimum_required(VERSION 3.20)
project(poseidon_core)

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
    message(PROJECT_NAME:${PROJECT_NAME})
#
# Set this only if this project is built as a standalone project.
#
#-------------------------------------------------------------------------------
# Set the mount path of your pmem device where the tests/benchmarks are stored
if(NOT PMEM_MNT_PATH)
  set(PMEM_MNT_PATH "/mnt/pmem0/poseidon")
endif()

option(USE_LLVM         "Use LLVM for query compilation"         OFF)
option(BUILD_PYTHON     "Build python interface for poseidon"    ON)
option(QOP_PROFILING    "Enable query profiling"                 ON)
#-------------------------------------------------------------------------------
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(LLVM_DIR "/usr/local/Cellar/llvm/18.1.5/lib/cmake/llvm")
  set(ANTLR_EXECUTABLE  "/usr/local/Homebrew/antlr-4.13.1-complete.jar")
else()
  set(ANTLR_EXECUTABLE "${CMAKE_SOURCE_DIR}/jars/antlr4-4.13.1-complete.jar")
endif()


set (FETCHCONTENT_FULLY_DISCONNECTED "OFF")

# Set the searching location for cmake 'include' locations
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 20)

include(FetchContent REQUIRED)

#-------------------------------------------------------------------------------
# ANTLR4 Parser generator
#
find_package(ANTLR)
if(ANTLR_FOUND)
message("ANTLR found")
  antlr_target(grammar src/query/parser/poseidon.g4 
    COMPILE_FLAGS -Xexact-output-dir -visitor
    OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/src/query/antlr4_generated
)
else()
  set(ANTLR_grammar_CXX_OUTPUTS 
    src/query/antlr4_generated/poseidonLexer.h
    src/query/antlr4_generated/poseidonLexer.cpp
    src/query/antlr4_generated/poseidonParser.h
    src/query/antlr4_generated/poseidonParser.cpp)

endif()

# message("grammar: ${ANTLR_grammar_CXX_OUTPUTS}")

#-------------------------------------------------------------------------------
if(QOP_PROFILING MATCHES ON)
  add_definitions( "-DQOP_PROFILING" )
endif()

if(USE_LLVM MATCHES ON)
  add_definitions( "-DUSE_LLVM" )
endif()

configure_file(
  ${PROJECT_SOURCE_DIR}/src/config.h.in
  ${PROJECT_BINARY_DIR}/generated/config.h
)
#---------------------------------------------------------------------------
include_directories(${PROJECT_SOURCE_DIR}/src/vec)
include_directories(${PROJECT_SOURCE_DIR}/src/storage)
include_directories(${PROJECT_SOURCE_DIR}/src/query/codegen)
include_directories(${PROJECT_SOURCE_DIR}/src/query/expr)
include_directories(${PROJECT_SOURCE_DIR}/src/query/interp)
include_directories(${PROJECT_SOURCE_DIR}/src/query/parser)
include_directories(${PROJECT_SOURCE_DIR}/src/query/plan_op)
include_directories(${PROJECT_SOURCE_DIR}/src/query/utils)
include_directories(${PROJECT_SOURCE_DIR}/src/bp_file)
include_directories(${PROJECT_SOURCE_DIR}/src/pool)
include_directories(${PROJECT_SOURCE_DIR}/src/dict)
include_directories(${PROJECT_SOURCE_DIR}/src/tx)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/antlr4_cpp_runtime/src)


#---------------------------------------------------------------------------

set(THIRD_PARTY_DIR "${PROJECT_BINARY_DIR}/_deps")

# C++ compiler flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -Wall -Wno-deprecated -O3 -Wsign-compare")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Wall -Wno-deprecated -O0 -g -Wsign-compare")
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-local-typedefs -Wno-#pragma-messages")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-local-typedefs -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0 -Wno-unused")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -wd488 -wd597")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-unused -Wno-uninitialized")

if(CMAKE_BUILD_TYPE MATCHES Release)
  # this disables asserts for release builds
  add_definitions("-DNDEBUG")
endif()

#---------------------------------------------------------------------------

#-----------------------------------------------------------------------------------------


# CSVParser =================================================================== #
FetchContent_Declare(
  CSV
  GIT_REPOSITORY https://github.com/AriaFallah/csv-parser.git
  #GIT_TAG        1.2.1
)

FetchContent_MakeAvailable(CSV)

# spdlog =================================================================== #
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.12.0
  )
  
FetchContent_MakeAvailable(spdlog)

# linenoise =================================================================== #
FetchContent_Declare(
  linenoise
  GIT_REPOSITORY https://github.com/yhirose/cpp-linenoise.git
 #  GIT_TAG        1.2.1
  )

FetchContent_MakeAvailable(linenoise)

# fmt =================================================================== #
set (BUILD_SHARED_LIBS ON)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        9.1.0
)

FetchContent_MakeAvailable(fmt)

# pybind11 =================================================================== #


#---------------------------------------------------------------------------

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
  set(DYLIB_LIBRARY "-ldl")
else()
  set(DYLIB_LIBRARY "")
endif()

#---------------------------------------------------------------------------




######################
# Boost C++ library
######################
#
SET(BOOST_MIN_VERSION "1.81.0")
set(Boost_INCLUDE_DIR "/usr/local/opt/boost/include")
find_package(Boost ${BOOST_MIN_VERSION} COMPONENTS
    program_options
    filesystem
    date_time
    thread
    REQUIRED)

if (NOT Boost_FOUND)
     message(FATAL_ERROR "Fatal error: Boost (version >= ${BOOST_MIN_VERSION}) required.\n"
 )
endif (NOT Boost_FOUND)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    add_definitions( "-DHAS_BOOST" )
endif()

#---------------------------------------------------------------------------
include_directories("${csv_SOURCE_DIR}")
include_directories("${spdlog_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${linenoise_SOURCE_DIR}")
include_directories("${fmt_SOURCE_DIR}/include")


###########################
# Python Interface
###########################
#
#-----------------------------------------------------------------------------------------

set(src_files
  src/storage/nodes.cpp
  src/storage/properties.cpp
  src/storage/relationships.cpp
  src/storage/graph_db.cpp
  src/storage/graph_db_import.cpp
  src/bp_file/bufferpool.cpp
  src/bp_file/paged_file.cpp
  src/bp_file/lru_list.cpp
  src/pool/graph_pool.cpp
  src/dict/dict.cpp
  src/dict/code_table.cpp
  src/dict/paged_string_pool.cpp
  src/query/interp/query_ctx.cpp
  src/query/parser/query_proc.cpp
  ${ANTLR_grammar_CXX_OUTPUTS}
  
  )

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
add_library(poseidon_obj OBJECT ${src_files})
else()
add_library(poseidon_obj OBJECT SHARED ${src_files})
endif()
  
# Poseidon core library for building C++ applications
add_library(poseidon_core SHARED
  $<TARGET_OBJECTS:poseidon_obj>
)

target_link_libraries(poseidon_core 
  antlr4_cpp_runtime
  Boost::filesystem
  Boost::date_time
  Boost::thread
  spdlog::spdlog_header_only
  fmt::fmt)

target_include_directories( ${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/include
)

#-----------------------------------------------------------------------------------------

add_subdirectory(src/antlr4_cpp_runtime)

#-----------------------------------------------------------------------------------------

add_executable(pcli  src/cmds/pcli.cpp)

target_link_libraries(pcli 
  poseidon_core
  Boost::program_options)
