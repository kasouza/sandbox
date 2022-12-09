if(MSVC)
	set(COMPILE_OPTIONS /W4)
	# target_compile_options(${LIB_NAME} PRIVATE /W4)
else()
	set(COMPILE_OPTIONS -Wall -Wextra -pedantic)
	# target_compile_options(${LIB_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

set(SOURCES
	main.c
	renderer.c
	utils.c
	world.c
)

file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC c_std_17)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw glad stb_image)
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_OPTIONS})
