#include <exception>

// Before C++14 constexpr implies const
#if (__cplusplus >= 201402L)
    #define POPT_CONSTEXPR constexpr
#else
    #define POPT_CONSTEXPR
#endif

// TODO:
// add ctor and copy-ctor from other type

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
    namespace details{
        template <class Source> struct NoConvert { 
            operator Source() const = delete;
            operator Source const&() const;
        };
        

        #define HAS(function_name, macro_name) \
        template<typename T1, typename T2, typename T3 = void> \
        struct Has##macro_name \
            : public std::false_type {}; \
        template<typename T1, typename T2>\
        struct Has##macro_name<T1, T2, decltype(function_name(NoConvert<T1>(), NoConvert<T2>()), void())>\
            : public std::true_type {}
        
        HAS(equal, Equal);
        HAS(not_equal, NotEqual);
        HAS(less_than, Less);


        template<typename T1>
        struct Equatable{
            template<typename T2>
            friend bool equal(const T1& op1, const T2& op2) noexcept {
                static_assert(HasEqual<T1, T2>::value, "No operator== defined");

                return (op1 == op2);
            }

            template<typename T2>
            friend bool not_equal(const T1& op1, const T2& op2) noexcept {
                static_assert(HasEqual<T1, T2>::value, "No operator== defined");

                return !(op1 == op2);
            }

            template<typename T2>
            friend bool operator!=(const T1& op1, const T2& op2) noexcept {
                static_assert(HasEqual<T1, T2>::value, "No operator== defined");

                return not_equal(op1, op2);
            }
            
            template<typename T2>
            friend enable_if_t<!HasEqual<T2, T1>::value, bool> operator==(const T2& op1, const T1& op2) noexcept {
                static_assert(HasEqual<T1, T2>::value, "No operator== defined");

                return (op2 == op1);
            }

            template<typename T2>
            friend enable_if_t<!HasNotEqual<T2, T1>::value, bool> operator!=(const T2& op1, const T1& op2) noexcept {
                return (op2 != op1);
            }
        };
        

        template<typename T1>
        struct Comparable{
            template<typename T2>
            friend bool less_than(const T1& op1, const T2& op2) noexcept {
                static_assert(HasLess<T1, T2>::value, "No operator< defined");

                return (op1 < op2);
            }

            template<typename T2>
            friend bool operator<=(const T1& op1, const T2& op2) noexcept {
                static_assert(HasEqual<T1, T2>::value, "No operator== defined");
                
                return less_than(op1, op2) || (op1 == op2);
            }

            template<typename T2>
            friend bool operator>(const T1& op1, const T2& op2) noexcept {
                return !(op1 <= op2);
            }

            template<typename T2>
            friend bool operator>=(const T1& op1, const T2& op2) noexcept {
                return !less_than(op1, op2);
            }
            
            template<typename T2>
            friend enable_if_t<!HasLess<T2, T1>::value, bool> operator<(const T2& op1, const T1& op2) noexcept {
                return (op2 > op1);
            }
            
            template<typename T2>
            friend enable_if_t<!HasLess<T2, T1>::value, bool> operator<=(const T2& op1, const T1& op2) noexcept {
                return (op2 >= op1);
            }
            
            template<typename T2>
            friend enable_if_t<!HasLess<T2, T1>::value, bool> operator>(const T2& op1, const T1& op2) noexcept {
                return (op2 < op1);
            }
            
            template<typename T2>
            friend enable_if_t<!HasLess<T2, T1>::value, bool> operator>=(const T2& op1, const T1& op2) noexcept {
                return (op2 <= op1);
            }
        };
    }


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
    class packed_optional
        : public details::Equatable<packed_optional<T, empty_value>>,
          public details::Comparable<packed_optional<T, empty_value>>{
        static_assert(is_valid_optional<T>::value, 
                      "T must be a valid non-type parameter type");

    public:
        using value_type = T;

        constexpr packed_optional() noexcept = default;
        constexpr packed_optional(const packed_optional&) noexcept = default;
        constexpr packed_optional(nullopt_t) noexcept : packed_optional(){}
        
        template<typename T2, valid_optional_t<T2> empty_value2>
        constexpr packed_optional(const packed_optional<T2, empty_value2>& other) noexcept
            : value_{ other.has_value() ? *other : empty_value }{
        }

        POPT_CONSTEXPR packed_optional& operator=(const packed_optional&) noexcept = default;
        
        template<typename T2, valid_optional_t<T2> empty_value2>
        POPT_CONSTEXPR packed_optional& operator=(const packed_optional<T2, empty_value2>& other) noexcept {
            *this = packed_optional(other);
            return *this;
        }

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
    bool operator==(const packed_optional<T1, empty_value1>& op1, 
                    const packed_optional<T2, empty_value2>& op2) noexcept {
        return (!op1.has_value() && !op2.has_value()) ||
               (op1.has_value() && op2.has_value() && *op1 == *op2);
    }

    template<typename T, T empty_value>
    bool operator==(const packed_optional<T, empty_value>& op, 
                    nullopt_t) noexcept {
        return (!op.has_value());
    }

    template<typename T, typename T2, T empty_value, typename Dummy = valid_optional_t<T2>>
    bool operator==(const packed_optional<T, empty_value>& op, 
                    T2 value) noexcept {
        return (op.has_value() && *op == value);
    }    

    template<typename T1, typename T2, T1 empty_value1, T2 empty_value2>
    bool operator<(const packed_optional<T1, empty_value1>& op1, 
                    const packed_optional<T2, empty_value2>& op2) noexcept {
        return (!op1.has_value() && op2.has_value()) ||
               (op1.has_value() && op2.has_value() && *op1 < *op2);
    }    

    template<typename T, T empty_value>
    bool operator<(const packed_optional<T, empty_value>&, 
                    nullopt_t) noexcept {
        return false;
    }    

    template<typename T, typename T2, T empty_value, typename Dummy = valid_optional_t<T2>>
    bool operator<(const packed_optional<T, empty_value>& op, 
                    T2 value) noexcept {
        return (!op.has_value() || (*op < value));
    }
}