include(cmake/utils/get_linux_kernel.cmake)
include(GNUInstallDirs)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE INTERNAL "Build as debug" FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(
        -fvisibility=hidden
        -s
        -pedantic
        -Wall
        -Wextra
        -Wcast-align
        -Wcast-qual
        -Wctor-dtor-privacy
        -Wformat=2
        -Winit-self
        -Wlogical-op
        -Wmissing-declarations
        -Wmissing-include-dirs
        -Wnoexcept
        -Woverloaded-virtual
        -Wredundant-decls
        -Wshadow
        -Wsign-promo
        -Wstrict-null-sentinel
        -Wswitch-default
        -Wundef
        -Wno-unused-variable
        -Wno-error=redundant-decls
        -Wno-uninitialized
        -Wno-strict-overflow
        -Ofast
    )
else()
    add_compile_options(
        -Wall
        -Wextra
    )
endif()

# [THREAD FIX]
list(APPEND PROJECT_LIBRARIES_LIST pthread)

# [INSTALL RESOURCES DIRECTORY]
set(PROJECT_INSTALL_DIR "${CMAKE_SOURCE_DIR}/res/install/linux/")

# [EXECUTABLE]
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION ${PROJECT_MAIN_SRC_FILES})
else()
    add_executable(${PROJECT_NAME} ${PROJECT_MAIN_SRC_FILES} ${PROJECT_QRCS_FILES} ${PROJECT_QM_FILES})
endif()

# [INSTALLATION]
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES ${PROJECT_INSTALL_DIR}/desktop/qt-ukr-categorizer-client.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
install(DIRECTORY ${PROJECT_INSTALL_DIR}/icons/ DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor)

if(${QT_VERSION_MAJOR} EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()

