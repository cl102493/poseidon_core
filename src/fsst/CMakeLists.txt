#------------------- LIBRARY CONFIGURATION ------------------------------------

set(fsst fsst CACHE INTERNAL "")
set(fsst_SRC
    fsst.cpp
    libfsst.cpp
    fsst_avx512.cpp
)

add_library(${fsst} ${fsst_SRC})

target_include_directories(fsst PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


if ("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
    target_compile_options(${fsst} PRIVATE
        -g -Wall -O0
    )
else()
    target_compile_options(${fsst} PRIVATE
        -w -O3
    )
    target_compile_options(${fsst} PUBLIC
        -Wno-attributes
    )
endif()
