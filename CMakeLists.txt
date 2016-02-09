CMAKE_MINIMUM_REQUIRED(VERSION 3.4)
#Because target platform version is only set on cmake v3.4+ (required for directx 12 headers/libs)
PROJECT(DirectX12FromScratch)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
SET(OUTPUT_DIR ${PROJECT_BINARY_DIR}/bin)

find_package(assimp REQUIRED)
if (ASSIMP_FOUND)
MESSAGE(STATUS "Assimp found")
add_library(assimp UNKNOWN IMPORTED)
set_property(TARGET assimp PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${ASSIMP_INCLUDE_DIR})
set_property(TARGET assimp PROPERTY IMPORTED_LOCATION_RELEASE ${ASSIMP_LIBRARY_RELEASE})
set_property(TARGET assimp PROPERTY IMPORTED_LOCATION_DEBUG ${ASSIMP_LIBRARY_DEBUG})
else()
MESSAGE(FATAL_ERROR "Assimp not found, please set ASSIMP_ROOT_DIR and check lib32/lib64 contents")
endif()

add_subdirectory(DirectX12FromScratch)