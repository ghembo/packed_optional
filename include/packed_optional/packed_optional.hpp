#include <exception>

// TODO:
// check noexcept
// add constexpr

namespace{
    template<bool C, typename T1, typename T2>
    using conditional_t = typename std::conditional<C, T1, T2>::type;

    template<bool C, typename T>
    using enable_if_t = typename std::enable_if<C, T>::type;
    
    template<class...> struct disjunction : std::true_type { };
    template<class B1> struct disjunction<B1> : B1 { };
    template<class B1, class... Bn>
    struct disjunction<B1, Bn...> 
    : conditional_t<bool(B1::value), B1, disjunction<Bn...>> {};
}

namespace packed_optional {
    class bad_optional_access : public std::exception{
        const char* what() const noexcept override{
            return "Empty packed_optional";
        }
    };

    template<typename T>
    using is_valid_optional = disjunction<std::is_integral<T>, 
                                          std::is_pointer<T>, 
                                          std::is_member_pointer<T>, 
                                          std::is_enum<T>,
                                          std::is_same<T, std::nullptr_t>>;

    template<typename T>
    using valid_optional_t = enable_if_t<is_valid_optional<T>::value,T>;
                                    
    template<typename T, valid_optional_t<T> empty_value>
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
                throw bad_optional_access();
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