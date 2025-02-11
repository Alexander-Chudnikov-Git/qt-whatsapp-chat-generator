cmake_minimum_required(VERSION 3.21.1)

option(CMAKE_DISABLE_SOURCE_CHANGES  "Disable source changes" ON)
option(CMAKE_DISABLE_IN_SOURCE_BUILD "Disable in-source build" ON)

include(cmake/utils/build_protection.cmake)

project(qt-whatsapp-chat-generator
    VERSION     0.0.1
    DESCRIPTION "WhatsApp chat generator"
    LANGUAGES   CXX
)

set(PROJECT_APPLICATION_NAME     "WhatsApp Chat Generator"       CACHE STRING "Application name")
set(PROJECT_ORGANIZATION_NAME    "Redline-Software"              CACHE STRING "Organization's name")
set(PROJECT_ORGANIZATION_WEBSITE "redline-software.moscow"       CACHE STRING "Organization's website")
set(PROJECT_DEVELOPER_NAME       "chooisfox"                     CACHE STRING "Developer's name")
set(PROJECT_DEVELOPER_EMAIL      "admin@redline-software.moscow" CACHE STRING "Developer's email")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(CMAKE_UPX_COMPRESS "Enable UPX compression" ON)

option(PROJECT_BUILD_DOCS     "Build project documentation" OFF)
option(PROJECT_BUILD_TESTS    "Build project tests"         OFF)
option(PROJECT_BUILD_EXAMPLES "Build project examples"      OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(cmake/platform/generic_builder.cmake)
