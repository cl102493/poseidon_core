if(POLICY CMP0063)
    cmake_policy(SET CMP0063 NEW)
endif()

find_package(Threads REQUIRED)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)

add_library(poseidon_fsst STATIC libfsst.cpp)

target_include_directories(poseidon_fsst PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
set_target_properties(poseidon_fsst PROPERTIES EXPORT_NAME poseidon_fsst)

install(TARGETS poseidon_fsst
        EXPORT "${POSEIDON_EXPORT_SET}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}")

#disable_target_warnings(poseidon_fsst)
