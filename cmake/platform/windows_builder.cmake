if(CMAKE_RELEASE)
    set(CMAKE_BUILD_TYPE "Release" CACHE INTERNAL "Build as release" FORCE)
    add_compile_options(
        -fvisibility=hidden
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
    set(CMAKE_BUILD_TYPE "Debug" CACHE INTERNAL "Build as debug" FORCE)
    add_compile_options(
        -Wall
        -Wextra
    )
endif()

# [EXECUTABLE]
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION ${PROJECT_MAIN_SRC_FILES})
else()
    add_executable(${PROJECT_NAME} ${PROJECT_MAIN_SRC_FILES} ${PROJECT_QRCS_FILES} ${PROJECT_QM_FILES})
endif()

# [INSTALLATION]

if(${QT_VERSION_MAJOR} EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()

