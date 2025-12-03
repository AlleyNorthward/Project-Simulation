add_library(mathlib
    math/mymath.cpp
)

target_include_directories(mathlib PUBLIC
    math
)

target_link_libraries(mathlib
    PRIVATE utils
)












