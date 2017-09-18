#pragma once
#include "KTL.Type.TypeTraits.h"

namespace ktl
{
    inline namespace random
    {
        //
        // Mersenne Twister Home Page
        // See. http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
        //

        //
        // w: UInt size (in number of bits)
        // n: degree of recurrence
        // m : middle word, an offset used in the recurrence relation defining the series x, 1 ¡Ü m < n
        // r : separation point of one word, or the number of bits of the lower bitmask, 0 ¡Ü r ¡Ü w - 1
        // a : coefficients of the rational normal form twist matrix
        // b, c : TGFSR(R) tempering bitmasks
        // s, t : TGFSR(R) tempering bit shifts
        // u, d, l : additional Mersenne Twister tempering bit shifts / masks
        //
        // See. https://en.wikipedia.org/wiki/Mersenne_Twister#C.2FC.2B.2B_implementation
        //

        template <
            typename _UIntType,
            ktl::usize __w,
            ktl::usize __n,
            ktl::usize __m,
            ktl::usize __r,
            _UIntType __a,
            ktl::usize __u, _UIntType __d,
            ktl::usize __s, _UIntType __b,
            ktl::usize __t, _UIntType __c,
            ktl::usize __l, _UIntType __f
        >
        class mersenne_twister
        {
            static constexpr _UIntType _MaxBound = (((1ULL << (__w - 1)) - 1) << 1) + 1;

            static_assert(ktl::is_unsigned<_UIntType>::value,
                "result_type must be an unsigned integral type");
            static_assert(1u <= __m && __m <= __n, "template argument substituting __m out of bounds");
            static_assert(__r <= __w, "template argument substituting __r out of bound");
            static_assert(__u <= __w, "template argument substituting __u out of bound");
            static_assert(__s <= __w, "template argument substituting __s out of bound");
            static_assert(__t <= __w, "template argument substituting __t out of bound");
            static_assert(__l <= __w, "template argument substituting __l out of bound");
            static_assert(__w <= sizeof(_UIntType) * 8, "template argument substituting __w out of bound");
            static_assert(__a <= _MaxBound, "template argument substituting __a out of bound");
            static_assert(__b <= _MaxBound, "template argument substituting __b out of bound");
            static_assert(__c <= _MaxBound, "template argument substituting __c out of bound");
            static_assert(__d <= _MaxBound, "template argument substituting __d out of bound");
            static_assert(__f <= _MaxBound, "template argument substituting __f out of bound");

        public:
            using result_type = _UIntType;

            // parameter values
            static constexpr ktl::usize     word_size = __w;
            static constexpr ktl::usize     state_size = __n;
            static constexpr ktl::usize     shift_size = __m;
            static constexpr ktl::usize     mask_bits = __r;
            static constexpr result_type    xor_mask = __a;
            static constexpr ktl::usize     tempering_u = __u;
            static constexpr result_type    tempering_d = __d;
            static constexpr ktl::usize     tempering_s = __s;
            static constexpr result_type    tempering_b = __b;
            static constexpr ktl::usize     tempering_t = __t;
            static constexpr result_type    tempering_c = __c;
            static constexpr ktl::usize     tempering_l = __l;
            static constexpr result_type    initialization_multiplier = __f;
            static constexpr result_type    default_seed = 5489u;

            static constexpr result_type _w_mask = ~((~result_type(0) << (__w - 1)) << 1);

            void seed(result_type aSeed = default_seed)
            {
                m_State[0] = aSeed & _w_mask;

                for (ktl::usize vIndex = 1; vIndex < state_size; ++vIndex)
                {
                    result_type __x = m_State[vIndex - 1];
                    __x ^= __x >> (__w - 2);
                    __x *= __f;
                    __x += vIndex;
                    m_State[vIndex] = __x & _w_mask;
                    
                }
                m_StateIndex = state_size;
            }

            static constexpr result_type min_rand()
            {
                return 0;
            }

            static constexpr result_type max_rand()
            {
                return (_w_mask);
            }

            void discard(ktl::u64 aCount)
            {
                for (; 0 < aCount; --aCount) (*this)();
            }

            result_type operator()()
            {
                if (m_StateIndex >= state_size - 1)
                    refill_rand();

                result_type vRand = m_State[m_StateIndex++];
                vRand ^= (vRand >> __u) & __d;
                vRand ^= (vRand << __s) & __b;
                vRand ^= (vRand << __t) & __c;
                vRand ^= (vRand >> __l);

                return vRand;
            }

        private:

            void refill_rand()
            {
                const result_type __upper_mask = (~(result_type())) << __r;
                const result_type __lower_mask = ~__upper_mask;

                // if seed() has not been called, a default initial seed is used 
                if (m_StateIndex == state_size + 1)
                    seed();

                for (ktl::usize vIndex = 0; vIndex < (__n - __m); ++vIndex)
                {
                    result_type vTemp = ((m_State[vIndex] & __upper_mask) | (m_State[vIndex + 1] & __lower_mask));
                    m_State[vIndex] = (m_State[vIndex + __m] ^ (vTemp >> 1) ^ ((vTemp & 0x01) ? __a : 0));
                }

                for (ktl::usize vIndex = (__n - __m); vIndex < (__n - 1); ++vIndex)
                {
                    result_type vTemp = ((m_State[vIndex] & __upper_mask) | (m_State[vIndex + 1] & __lower_mask));
                    m_State[vIndex] = (m_State[vIndex + (__m - __n)] ^ (vTemp >> 1) ^ ((vTemp & 0x01) ? __a : 0));
                }

                result_type vTemp = ((m_State[__n - 1] & __upper_mask) | (m_State[0] & __lower_mask));
                m_State[__n - 1] = (m_State[__m - 1] ^ (vTemp >> 1) ^ ((vTemp & 0x01) ? __a : 0));

                m_StateIndex = 0;
            }

            result_type m_State[state_size];
            ktl::usize  m_StateIndex = state_size + 1;
        };

        using mt19937 = mersenne_twister<
            ktl::u32,
            sizeof(ktl::u32) * 8,
            624, 397,
            31, 
            0x9908b0df,
            11, 0xffffffff,
            7, 0x9d2c5680, 
            15, 0xefc60000,
            18, 
            1812433253>;

        using mt19937_64 = mersenne_twister<
            ktl::u64, 
            sizeof(ktl::u64) * 8,
            312, 156,
            31,
            0xb5026f5aa96619e9, 29,
            0x5555555555555555, 17,
            0x71d67fffeda60000, 37,
            0xfff7eee000000000, 
            43,
            6364136223846793005>;

    }
}