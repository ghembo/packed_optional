# Tests need to be added as executables first
add_executable(PackedOptionalTest packed_optional_test.cpp)

# I'm using C++17 in the test
target_compile_features(PackedOptionalTest PRIVATE cxx_std_11)
set_target_properties(PackedOptionalTest PROPERTIES CXX_EXTENSIONS OFF)

# Should be linked to the main library, as well as the Catch2 testing library
target_link_libraries(PackedOptionalTest PRIVATE PackedOptional doctest)

# If you register a test, then ctest and make test will run it.
# You can also run examples and check the output, as well.
add_test(NAME PackedOptionalTest COMMAND PackedOptionalTest) # Command can be a target
