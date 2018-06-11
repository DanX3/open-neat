add_executable(genome ${SOURCES} GenomeTest.cpp)
add_test(NAME GenomeTest COMMAND genome)

add_executable(node ${SOURCES} NodeTest.cpp)
add_test(NAME NodeTest COMMAND node)

add_executable(network ${SOURCES} NetworkTest.cpp)
add_test(NAME NetworkTest COMMAND network)

