# Tests need to be added as executables first
add_executable(PackedOptionalTest 
			   packed_optional_test.cpp
			   ${CMAKE_SOURCE_DIR}/include/packed_optional/packed_optional.hpp)

# I'm using C++17 in the test
target_compile_features(PackedOptionalTest PRIVATE cxx_std_11)
set_target_properties(PackedOptionalTest PROPERTIES CXX_EXTENSIONS OFF)

# Should be linked to the main library, as well as the Catch2 testing library
target_link_libraries(PackedOptionalTest PRIVATE PackedOptional doctest)

if(MSVC)
	target_compile_options(PackedOptionalTest PRIVATE 
							/W4 
							/w14265 
							/w14287 
							/we4289 
							/w14296 
							/w14311 
							/w14545 
							/w14546 
							/w14547 
							/w14549 
							/w14555 
							/w14619 
							/w14640 
							/w14826 
							/w14928 
							/w44061 
							/w44062 
							/wd4514 
							/wd4710 
							/wd4711 
							/wd4571 
							/wd4820)

	#target_compile_definitions(Iris PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
	target_compile_options(PackedOptionalTest PRIVATE 
							-W
							-Wall
							-Wcast-align
							-Wconversion
							-Wdouble-promotion
							-Wduplicated-branches
							-Wduplicated-cond
							-Werror
							-Wextra
							-Wno-unknown-pragmas
							-Wnon-virtual-dtor
							-Wnull-dereference
							-Wold-style-cast
							-Woverloaded-virtual
							-Wpedantic
							-Wshadow
							-Wsign-conversion
							-Wunused
							-Wuseless-cast)
endif()


# If you register a test, then ctest and make test will run it.
# You can also run examples and check the output, as well.
add_test(NAME PackedOptionalTest COMMAND PackedOptionalTest) # Command can be a target
