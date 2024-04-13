# Add the executable
add_executable(MyProject main.cpp DES.cpp)

# Include directories
target_include_directories(MyProject PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(MyProject PUBLIC "../")

