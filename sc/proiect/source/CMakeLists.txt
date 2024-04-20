# Add the executable
add_executable(MyProject main.cpp des/DES.cpp aes/AES.cpp rsa/RSA.cpp OptionParser.cpp)

# Include directories
target_include_directories(MyProject PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(MyProject PUBLIC "../")

find_library(GMP_LIBRARY NAMES gmp)
find_library(GMPXX_LIBRARY NAMES gmpxx)

target_include_directories(${PROJECT_NAME} PRIVATE ${GMP_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${GMP_LIBRARY} ${GMPXX_LIBRARY} )
