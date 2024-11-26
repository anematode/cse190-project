#ifdef _SAFE
#feature on safety
#define _STD std2
#else
#define _STD std
#define safe
#define unsafe
#include <limits>
#include <string>
#include <string_view>
namespace std {
template <typename T>
struct string_constant {
//    const char* ptr;
//    size_t size;
//
//    template <size_t S>
//    string_constant(const char (&ptr)[S]): ptr(ptr), size(S) {}
//
//    string_view text() {
//        return string_view(ptr, size);
//    }
    int text() { return 0; }
};
}
#endif

namespace _STD {

inline void println(string_view sv) noexcept safe
{
  unsafe { printf("%.*s\n", sv.size(), sv.data()); }
}

#ifdef _SAFE
inline void println(string_constant<char> sc) noexcept safe
{
  println(sc.text());
}
#endif

#ifdef _SAFE
inline void println(const std::string^ s) noexcept safe
{
  unsafe { printf("%s\n", s->c_str()); }
}
#endif

#define __X_TYPES_AND_FORMAT          \
    X(char,                   "%c")   \
    X(short,                  "%hd")  \
    X(int,                    "%d")   \
    X(long int,               "%ld")  \
    X(long long int,          "%lld") \
    X(unsigned char,          "%hhu") \
    X(unsigned short,         "%hu")  \
    X(unsigned int,           "%u")   \
    X(long unsigned int,      "%lu")  \
    X(long long unsigned int, "%llu") \
    X(float,                  "%f")   \
    X(double,                 "%f")   \
    X(long double,            "%Lf")  \

#define X(TYPE, PRI) \
    inline void println(TYPE x) noexcept safe \
    { unsafe { printf(PRI "\n", x); } }

__X_TYPES_AND_FORMAT

#undef X

inline void print(string_view sv) noexcept safe
{
  unsafe { printf("%.*s", sv.size(), sv.data()); }
}

#ifdef _SAFE
inline void print(string_constant<char> sc) noexcept safe
{
  print(sc.text());
}
#endif

#ifdef _SAFE
inline void print(const std::string^ s) noexcept safe
{
  unsafe { printf("%s", s->c_str()); }
}
#endif

#define X(TYPE, PRI) \
    inline void print(TYPE x) noexcept safe \
    { unsafe { printf(PRI, x); } }

__X_TYPES_AND_FORMAT

#undef X

}

#feature off safety
#include <iostream>

// NOTE: must use in unsafe context
template <typename T>
inline void input(T& value)
{
    while (!(std::cin >> value)) {
        std::cout << "Invalid input. Please enter an integer: ";
        std::cin.clear(); // Clear error state
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard remaining input
    }
}

#feature on safety

