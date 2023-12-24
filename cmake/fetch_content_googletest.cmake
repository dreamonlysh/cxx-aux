include(${CMAKE_CURRENT_LIST_DIR}/fetch_content.cmake)

set(GTEST_VERSION  "v1.14.0" CACHE STRING "googletest tag(https://github.com/google/googletest/tags)")

set_fetch_content_dir(googletest)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        ${GTEST_VERSION}
  SUBBUILD_DIR   ${GOOGLETEST_FC_SUBBUILD_DIR}
  SOURCE_DIR     ${GOOGLETEST_FC_SOURCE_DIR} 
  BINARY_DIR     ${GOOGLETEST_FC_BINARY_DIR}
  FIND_PACKAGE_ARGS NAMES GTest
)
unset_fetch_content_dir(googletest)

FetchContent_MakeAvailable(googletest)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)

  add_subdirectory(
    ${googletest_SOURCE_DIR}
    ${googletest_BINARY_DIR}
  )
endif()
