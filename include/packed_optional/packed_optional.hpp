#include <exception>

// Before C++14 constexpr implies const
#if (__cplusplus >= 201402L)
    #define POPT_CONSTEXPR constexpr
#else
    #define POPT_CONSTEXPR
#endif

// TODO:
// add ctor and copy-ctor from other type and optional
// add comparison to nullopt and type

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
    struct nullopt_t {
        explicit constexpr nullopt_t(int) {}
    };

    constexpr nullopt_t nullopt{ 0 };

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
        constexpr packed_optional() noexcept = default;
        constexpr packed_optional(const packed_optional&) noexcept = default;
        constexpr packed_optional(nullopt_t) noexcept : packed_optional(){}

        POPT_CONSTEXPR packed_optional& operator=(const packed_optional&) noexcept = default;

        constexpr packed_optional(T value) noexcept
            : value_{ value } {
        }

        constexpr bool has_value() const noexcept {
            return value_ != empty_value;
        }

        constexpr explicit operator bool() const noexcept {
            return has_value();
        }

        T value() const {
            if (!has_value()) {
                throw bad_optional_access();
            }

            return value_;
        }

        constexpr T value_or(T default_value) const noexcept {
            return has_value() ? value_ : default_value;
        }

        constexpr const T& operator*() const noexcept {
            return value_;
        }

        POPT_CONSTEXPR T& operator*() noexcept {
            return value_;
        }

    private:
        T value_ = empty_value;
    };

    template<typename T1, typename T2, T1 empty_value1, T2 empty_value2>
    bool operator==(const packed_optional<T1, empty_value1>& p1, 
                    const packed_optional<T2, empty_value2>& p2) noexcept {
        return (!p1.has_value() && !p2.has_value()) ||
               (p1.has_value() && p2.has_value() && *p1 == *p2);
    }

    template<typename T1, typename T2, T1 empty_value1, T2 empty_value2>
    bool operator!=(const packed_optional<T1, empty_value1>& p1, 
                    const packed_optional<T2, empty_value2>& p2) noexcept {
        return !(p1 == p2);
    }
}