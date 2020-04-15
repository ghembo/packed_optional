# packed_optional

[![Build Status](https://travis-ci.com/ghembo/packed_optional.svg?branch=master)](https://travis-ci.com/ghembo/packed_optional)

A C++ 11 compact Optional type for types with an 'invalid' value

## `packed_optional<T, T empty_value>`

Requirements:
- `T` must be a valid non-type template parameter
- there must exist an 'invalid' `T` value `empty_value`

The `packed_optional` is considered empty whenever its value is `empty_value`
