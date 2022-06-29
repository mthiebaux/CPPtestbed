
add_library(algos algos.cpp)

target_include_directories(
	algos
	INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

install(TARGETS algos DESTINATION lib)

install(FILES algos.h DESTINATION include)
