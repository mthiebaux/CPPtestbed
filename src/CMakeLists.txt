cmake_minimum_required(VERSION 3.10)

# set the project name
project(App)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the libraries
add_subdirectory(util)
add_subdirectory(cwiz)
add_subdirectory(algos)

list(APPEND EXTRA_LIBS cwiz algos util)

# add the executable
add_executable(App main.cpp)

target_link_libraries(App PUBLIC cwiz algos util)

# add the binary tree to the search path for include files
target_include_directories(
	App PUBLIC
	"${PROJECT_BINARY_DIR}"
)

install(TARGETS App DESTINATION bin)
