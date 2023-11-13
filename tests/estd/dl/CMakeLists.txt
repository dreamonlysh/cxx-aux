add_library(dll0 MODULE dll0.cpp)
add_library(dll1 MODULE dll1.cpp)
set_target_properties(dll0 PROPERTIES PREFIX "lib")
set_target_properties(dll1 PROPERTIES PREFIX "lib")
