
#pragma once
extern "C" {
#include "AdHoc.h"
}
#include <iterator>

namespace org
{
    namespace unirail
    {
        namespace utils
        {

#define EMPTY_ARG
#define FLAGS(T) T;\
    static inline T    operator    &    (T x, T y)        {    return static_cast<T>    (static_cast<uintptr_t>(x) & static_cast<uintptr_t>(y));    }; \
    static inline T    operator    |    (T x, T y)        {    return static_cast<T>    (static_cast<uintptr_t>(x) | static_cast<uintptr_t>(y));    }; \
    static inline T    operator    ^    (T x, T y)        {    return static_cast<T>    (static_cast<uintptr_t>(x) ^ static_cast<uintptr_t>(y));    }; \
    static inline T    operator    ~    (T x)             {    return static_cast<T>    (~static_cast<uintptr_t>(x));                            }; \
    static inline T&   operator    &=    (T& x, T y)      {    x = x & y;    return x;    }; \
    static inline T&   operator    |=    (T& x, T y)      {    x = x | y;    return x;    }; \
    static inline T&   operator    ^=    (T& x, T y)      {    x = x ^ y;    return x;    }; \
    static inline bool T##_bool(T x)                      {    return static_cast<uintptr_t>(x) != 0;}; \
    enum class T


            template<typename T, typename SRC>
            struct iterator
            {
                typedef iterator                self_type;
                typedef T                       value_type;
                typedef T                       &reference;
                typedef T                       *pointer;
                typedef uint32_t                difference_type;
                typedef std::input_iterator_tag iterator_category;


                self_type operator++()
                {
                    self_type i = *this;
                    ++index;
                    return i;
                }

                T operator*() { return src.get(index); }

                pointer operator->() { return src.bytes + index; }

                bool operator==(const self_type &rhs) { return src.bytes == rhs.src.bytes && index = rhs.index; }

                bool operator!=(const self_type &rhs) { return src.bytes != rhs.src.bytes || index != rhs.index; }

                iterator(const SRC &src, size_t index) : index(index), src(src) {}

                iterator(const SRC &src) : src(src) {}

            private:
                size_t    index = 0;
                const SRC &src;
            };

            extern inline Cursor* wrap_pack(Pack* pack, Cursor dst[])
            {
                dst->base.pack  = pack;
                dst->base.meta  = pack->meta;
                dst->base.bytes = pack->bytes;
                reset_cursor(dst);
                return dst;
            }
            extern inline bool is_equal_data(Cursor &a,  Cursor &b)
            {
                return  &a == &b ||
                        (a.base.bytes != nullptr &&
                         b.base.bytes != nullptr &&
                         pack_size(&a.base) == pack_size(&b.base) &&
                         memcmp(a.base.bytes, b.base.bytes, a.base.LAST_BYTE) == 0);
            }


            constexpr int RECEIVE_REQ_MAX_BYTES = 2000;
            constexpr int RECEIVE_FULL_MAX_BYTES = 2000;



            constexpr int SEND_REQ_MAX_BYTES = 257;
            constexpr int SEND_FULL_MAX_BYTES = 257;


        }
    }
}
namespace org
{
    namespace company
    {

    }
}


