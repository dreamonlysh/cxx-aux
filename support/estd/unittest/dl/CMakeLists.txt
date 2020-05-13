project(dl_test)

add_library(al_sum STATIC al_sum.cpp)

add_library(dl_sum MODULE dl_sum.cpp)
set_target_properties(dl_sum PROPERTIES PREFIX "lib")
