
add_library(cwiz cwiz_util.cpp)

target_include_directories(
	cwiz
	INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

install(TARGETS cwiz DESTINATION lib)

install(FILES cwiz_util.hpp cwiz_util_defs.hpp DESTINATION include)
