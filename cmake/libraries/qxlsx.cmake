set(CURRENT_LIBRARY_NAME QXlsx)

FetchContent_Declare(
  ${CURRENT_LIBRARY_NAME}
  GIT_REPOSITORY https://github.com/QtExcel/QXlsx.git
  GIT_TAG        v1.5.0
  SOURCE_SUBDIR  QXlsx
)

FetchContent_MakeAvailable(${CURRENT_LIBRARY_NAME})

list(APPEND PROJECT_LIBRARIES_LIST QXlsx::QXlsx)
