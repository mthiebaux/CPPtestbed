
add_library(algos algos.cpp tree.cpp graph.cpp)

# https://stackoverflow.com/questions/25186755/cmake-and-target-link-libraries-on-library-depending-on-another-lib
#
target_link_libraries(algos util)

target_include_directories(
	algos
	INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

install(TARGETS algos DESTINATION lib)

install(FILES algos.h tree.h graph.h DESTINATION include)
