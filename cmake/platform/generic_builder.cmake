include(FetchContent)
include(ExternalProject)

include(cmake/utils/list_all_subdirectories.cmake)

message(STATUS "CXX compiler:      ${CMAKE_CXX_COMPILER_ID}")

# [SOURCE DIRECTORIES]
set(PROJECT_MAIN_SRC_DIR   "${CMAKE_SOURCE_DIR}/src")

file(GLOB PROJECT_MAIN_SRC_FILES CONFIGURE_DEPENDS
    "${PROJECT_MAIN_SRC_DIR}/*.hpp"
    "${PROJECT_MAIN_SRC_DIR}/*.cpp"
)

list(APPEND PROJECT_INCLUDE_DIRS ${PROJECT_MAIN_SRC_DIR})

# [INCLUDE DIRECTORIES]
set(PROJECT_INCLUDE_DIRS)

# [LIBRARIES LIST]
set(PROJECT_LIBRARIES_LIST)

# [DIRECTORIES LIST]
set(PROJECT_DIRECTORIES_LIST)

# [TRANSLATION]
set(PROJECT_TS_FILE_DIR "${CMAKE_SOURCE_DIR}/translations")
set(PROJECT_TRANSLATION_TARGETS ${PROJECT_NAME})
file(GLOB PROJECT_TS_FILES CONFIGURE_DEPENDS
    "${PROJECT_TS_FILE_DIR}/*.ts"
)

# [RESOURCES]
file(GLOB PROJECT_QRC_FILES CONFIGURE_DEPENDS
    "res/*.qrc"
)

# [LIBRARIES]
include(cmake/libraries/fmt.cmake)
include(cmake/libraries/spdlog.cmake)
include(cmake/libraries/qt.cmake)
include(cmake/libraries/qxlsx.cmake)
include(cmake/libraries/utils.cmake)
include(cmake/libraries/app.cmake)

# LINUX does not exclude MACOS, so we need to check it first
if(MACOS)
    message(FATAL_ERROR "Unsupported OS: ${CMAKE_SYSTEM_NAME}")
elseif(LINUX)
    include(cmake/platform/linux_builder.cmake)
else()
    include(cmake/platform/windows_builder.cmake)
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_standard_project_setup(I18N_TRANSLATED_LANGUAGES en)
    set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY QT_EXCLUDE_FROM_TRANSLATION ON)
    qt_add_translations(${PROJECT_NAME} TS_FILES ${PROJECT_TS_FILES} SOURCE_TARGETS ${PROJECT_TRANSLATION_TARGETS})
    add_dependencies(${PROJECT_NAME} update_translations)

    qt_add_resources(${PROJECT_NAME} ${PROJECT_QRC_FILES})
else()
    # Implement later
endif()

# [TARGET LNKING]
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_DIRECTORIES_LIST})
target_link_directories(${PROJECT_NAME}    PRIVATE ${PROJECT_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}      PRIVATE ${PROJECT_LIBRARIES_LIST})

target_compile_definitions(${PROJECT_NAME} PRIVATE SYSTEM_IS_TILING="${CMAKE_SYSTEM_TILING}")

# [SOURCE GROUPS]
source_group("Main"         FILES ${PROJECT_MAIN_SRC_FILES})
source_group("Resources"    FILES ${PROJECT_QRC_FILES})
source_group("Translations" FILES ${PROJECT_TS_FILES})

include(cmake/utils/upx_compress.cmake)
