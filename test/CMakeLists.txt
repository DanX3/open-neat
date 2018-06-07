add_executable(genome ${SOURCES} GenomeTest.cpp)
add_test(NAME GenomeTest COMMAND genome)

add_executable(node ${SOURCES} NodeTest.cpp)
add_test(NAME NodeTest COMMAND node)
