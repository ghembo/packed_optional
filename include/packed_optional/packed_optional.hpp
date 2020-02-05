#include <exception>

// TODO:
// check noexcept
// add constexpr
// add rvalue ref overloads (maybe not needed since T must be integral?)
// assert T constraints
// add and check specific exception
// add travis on Win

namespace packed_optional {
    template<typename T, T empty_value>
    class packed_optional {
    public:
        packed_optional() = default;
        packed_optional(const packed_optional&) = default;

        packed_optional& operator=(const packed_optional&) = default;

        packed_optional(T value)
            : value_{ value } {
        }

        bool has_value() const noexcept {
            return value_ != empty_value;
        }

        explicit operator bool() const noexcept {
            return has_value();
        }

        T value() const {
            if (!has_value()) {
                throw std::exception();
            }

            return value_;
        }

        T value_or(T default_value) const {
            return has_value() ? value_ : default_value;
        }

        const T& operator*() const {
            return value_;
        }

        T& operator*() {
            return value_;
        }

    private:
        T value_ = empty_value;
    };
}