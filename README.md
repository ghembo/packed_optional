# packed_optional

A C++ 11 compact Optional type for types with an 'invalid' value.

![example workflow](https://github.com/ghembo/packed_optional/actions/workflows/ubuntu.yml/badge.svg)
![example workflow](https://github.com/ghembo/packed_optional/actions/workflows/windows.yml/badge.svg)

Often the *invalid* or *empty* state of a variable is encoded in a specific value, e.g. `-1` for an `int`. A better alternative would be to use `std::optional`, but if the added space overhead is not viable, `paked_optional` provides a compromise and can be used as a drop-in replacement to `std::optional`.

The *empty* value is encoded as a template non-type parameter and cannot be used as an ordinary value: if used it will set the `paked_optional` as *empty*.

## `packed_optional<T, T empty_value>`

Requirements:
- `T` must be a valid non-type template parameter
- there must exist an 'invalid' `T` value `empty_value`

The `packed_optional` is considered empty whenever its value is `empty_value`
