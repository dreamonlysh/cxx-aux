include(FetchContent)

# the defaut fetch content cache dir is ${CMAKE_BINARY_DIR}/_deps,
# and the ${CMAKE_BINARY_DIR} will be removed more frequently.
# So set a directory out of the binary will reduce the case to call git clone
set(FETCH_CONTENT_CACHE_DIR ${CMAKE_SOURCE_DIR}/.cache/_deps CACHE PATH "path to save the fetch contents")

macro(set_fetch_content_dir content_name)
    string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_STRING)
    string(TOUPPER ${content_name} CONTENT_NAME_UPPER_CASE)
    set(${CONTENT_NAME_UPPER_CASE}_FC_SUBBUILD_DIR ${FETCH_CONTENT_CACHE_DIR}/${content_name}_subbuild_${CMAKE_BUILD_TYPE_STRING})
    set(${CONTENT_NAME_UPPER_CASE}_FC_SOURCE_DIR ${FETCH_CONTENT_CACHE_DIR}/${content_name})
    set(${CONTENT_NAME_UPPER_CASE}_FC_BINARY_DIR ${FETCH_CONTENT_CACHE_DIR}/${content_name}_build_${CMAKE_BUILD_TYPE_STRING})
    unset(CONTENT_NAME_UPPER_CASE)
    unset(CMAKE_BUILD_TYPE_STRING)
endmacro()

macro(unset_fetch_content_dir content_name)
    string(TOUPPER ${content_name} CONTENT_NAME_UPPER_CASE)
    unset(${CONTENT_NAME_UPPER_CASE}_FC_SUBBUILD_DIR)
    unset(${CONTENT_NAME_UPPER_CASE}_FC_SOURCE_DIR)
    unset(${CONTENT_NAME_UPPER_CASE}_FC_BINARY_DIR)
    unset(CONTENT_NAME_UPPER_CASE)
endmacro()
