set(ENTRY "main")

add_executable(
    ${ENTRY}
    main.cpp
)

target_include_directories(
    ${ENTRY}
    PUBLIC
    "${PROJECT_SOURCE_DIR}/name/include"
)

target_link_libraries(
    ${ENTRY}
    ${courseLib}
    ${nameLib}
    ${study_selfLib}
    ${study_togetherLib}
)











