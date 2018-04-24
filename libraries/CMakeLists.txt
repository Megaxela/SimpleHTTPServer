if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/ALogger/CMakeLists.txt)
    add_subdirectory(ALogger)
endif()

if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/json/CMakeLists.txt)
    add_subdirectory(json)
endif()