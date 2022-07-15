
#add_library(util timer.cpp)

# http://mariobadr.com/creating-a-header-only-library-with-cmake.html
#
add_library(util INTERFACE)

target_include_directories(
	util
	INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

#install(TARGETS util DESTINATION lib)

install(FILES timer.hpp DESTINATION include)
