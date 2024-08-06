set(CMAKE_CXX_STANDARD 14)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/. CPP_SOURCE_FILES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Template/. CPP_SOURCE_FILES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Math/. CPP_SOURCE_FILES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Render/. CPP_SOURCE_FILES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Core/. CPP_SOURCE_FILES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Cauto/. CPP_SOURCE_FILES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Birth/. CPP_SOURCE_FILES)

add_executable(${PROJECT_NAME} ${CPP_SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

find_package(PkgConfig REQUIRED)

pkg_check_modules(VULKAN vulkan REQUIRED IMPORTED_TARGET)
target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::VULKAN)

pkg_check_modules(SDL2 sdl2 REQUIRED IMPORTED_TARGET)
target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::SDL2)

