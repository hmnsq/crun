#ifndef BLAKE3_IMPL_H
#define BLAKE3_IMPL_H

#شامل <assert.h>
#شامل <stdbool.h>
#شامل <stddef.h>
#شامل <stdint.h>
#شامل <string.h>

#شامل "blake3.h"

/* libcrun specific code.  Always force the portable implementation as it is fast enough for us.   */
#define blake3_compress_in_place_portable blake3_compress_in_place
#define blake3_compress_xof_portable blake3_compress_xof
#define blake3_simd_degree_portable blake3_simd_degree
#define blake3_compress_subtree_wide_portable blake3_compress_subtree_wide
#define blake3_hash_many_portable blake3_hash_many
ایستا درونخطی size_t blake3_simd_degree(تهی) {
  بازگشت 1;
}

// internal flags
شمارش blake3_flags {
  CHUNK_START         = 1 << 0,
  CHUNK_END           = 1 << 1,
  PARENT              = 1 << 2,
  ریشه_2                = 1 << 3,
  KEYED_HASH          = 1 << 4,
  DERIVE_کلید_CONTEXT  = 1 << 5,
  DERIVE_کلید_MATERIAL = 1 << 6,
};

// This C implementation tries to support recent versions of GCC, Clang, and
// MSVC.
#اگر defined(_MSC_VER)
#define INLINE ایستا __forceinline
#وگرنه
#define INLINE ایستا درونخطی __attribute__((always_inline))
#endif

#اگر defined(__x86_64__) || defined(_M_X64) 
#define IS_X86
#define IS_X86_64
#endif

#اگر defined(__i386__) || defined(_M_IX86)
#define IS_X86
#define IS_X86_32
#endif

#اگر defined(__aarch64__) || defined(_M_ARM64)
#define IS_AARCH64
#endif

#اگر defined(IS_X86)
#اگر defined(_MSC_VER)
#شامل <intrin.h>
#endif
#endif

#اگر !defined(BLAKE3_USE_NEON) 
  // If BLAKE3_USE_NEON not manually set, autodetect based on AArch64ness
  #اگر defined(IS_AARCH64)
    #اگر defined(__ARM_BIG_ENDIAN)
      #define BLAKE3_USE_NEON 0
    #وگرنه
      #define BLAKE3_USE_NEON 1
    #endif
  #وگرنه
    #define BLAKE3_USE_NEON 0
  #endif
#endif

#اگر defined(IS_X86)
#define MAX_SIMD_DEGREE 16
#وگرنهاگر BLAKE3_USE_NEON == 1
#define MAX_SIMD_DEGREE 4
#وگرنه
#define MAX_SIMD_DEGREE 1
#endif

// There are some places where we want a static size that's equal to the
// MAX_SIMD_DEGREE, but also at least 2.
#define MAX_SIMD_DEGREE_OR_2 (MAX_SIMD_DEGREE > 2 ? MAX_SIMD_DEGREE : 2)

ایستا ثابت uint32_t IV[8] = {0x6A09E667UL, 0xBB67AE85UL, 0x3C6EF372UL,
                               0xA54FF53AUL, 0x510E527FUL, 0x9B05688CUL,
                               0x1F83D9ABUL, 0x5BE0CD19UL};

ایستا ثابت uint8_t پیام_SCHEDULE[7][16] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
    {2, 6, 3, 10, 7, 0, 4, 13, 1, 11, 12, 5, 9, 14, 15, 8},
    {3, 4, 10, 12, 13, 2, 7, 14, 6, 5, 9, 0, 11, 15, 8, 1},
    {10, 7, 12, 9, 14, 3, 13, 15, 4, 0, 11, 2, 5, 8, 1, 6},
    {12, 13, 9, 11, 15, 10, 14, 8, 7, 2, 5, 3, 0, 1, 6, 4},
    {9, 14, 11, 5, 8, 12, 15, 1, 13, 3, 0, 10, 2, 6, 4, 7},
    {11, 15, 5, 0, 1, 9, 8, 6, 14, 10, 2, 12, 3, 4, 7, 13},
};

/* Find index of the highest set bit */
/* x is assumed to be nonzero.       */
ایستا unsigned صحیح highest_one(uint64_t x) {
#اگر defined(__GNUC__) || defined(__clang__)
  بازگشت 63 ^ (unsigned صحیح)__builtin_clzll(x);
#وگرنهاگر defined(_MSC_VER) && defined(IS_X86_64)
  unsigned long index;
  _BitScanReverse64(&index, x);
  بازگشت index;
#وگرنهاگر defined(_MSC_VER) && defined(IS_X86_32)
  اگر(x >> 32) {
    unsigned long index;
    _BitScanReverse(&index, (unsigned long)(x >> 32));
    بازگشت 32 + index;
  } وگرنه {
    unsigned long index;
    _BitScanReverse(&index, (unsigned long)x);
    بازگشت index;
  }
#وگرنه
  unsigned صحیح c = 0;
  اگر(x & 0xffffffff00000000ULL) { x >>= 32; c += 32; }
  اگر(x & 0x00000000ffff0000ULL) { x >>= 16; c += 16; }
  اگر(x & 0x000000000000ff00ULL) { x >>=  8; c +=  8; }
  اگر(x & 0x00000000000000f0ULL) { x >>=  4; c +=  4; }
  اگر(x & 0x000000000000000cULL) { x >>=  2; c +=  2; }
  اگر(x & 0x0000000000000002ULL) {           c +=  1; }
  بازگشت c;
#endif
}

// Count the number of 1 bits.
INLINE unsigned صحیح popcnt(uint64_t x) {
#اگر defined(__GNUC__) || defined(__clang__)
  بازگشت (unsigned صحیح)__builtin_popcountll(x);
#وگرنه
  unsigned صحیح شمار = 0;
  وقتی (x != 0) {
    شمار += 1;
    x &= x - 1;
  }
  بازگشت شمار;
#endif
}

// Largest power of two less than or equal to x. As a special case, returns 1
// when x is 0. 
INLINE uint64_t round_down_to_power_of_2(uint64_t x) {
  بازگشت 1ULL << highest_one(x | 1);
}

INLINE uint32_t counter_کم(uint64_t counter) { بازگشت (uint32_t)counter; }

INLINE uint32_t counter_بالا(uint64_t counter) {
  بازگشت (uint32_t)(counter >> 32);
}

INLINE uint32_t load32(ثابت تهی *src) {
  ثابت uint8_t *p = (ثابت uint8_t *)src;
  بازگشت ((uint32_t)(p[0]) << 0) | ((uint32_t)(p[1]) << 8) |
         ((uint32_t)(p[2]) << 16) | ((uint32_t)(p[3]) << 24);
}

INLINE تهی load_کلید_واژگان(ثابت uint8_t کلید[BLAKE3_کلید_LEN],
                           uint32_t کلید_واژگان[8]) {
  کلید_واژگان[0] = load32(&کلید[0 * 4]);
  کلید_واژگان[1] = load32(&کلید[1 * 4]);
  کلید_واژگان[2] = load32(&کلید[2 * 4]);
  کلید_واژگان[3] = load32(&کلید[3 * 4]);
  کلید_واژگان[4] = load32(&کلید[4 * 4]);
  کلید_واژگان[5] = load32(&کلید[5 * 4]);
  کلید_واژگان[6] = load32(&کلید[6 * 4]);
  کلید_واژگان[7] = load32(&کلید[7 * 4]);
}

INLINE تهی store32(تهی *dst, uint32_t w) {
  uint8_t *p = (uint8_t *)dst;
  p[0] = (uint8_t)(w >> 0);
  p[1] = (uint8_t)(w >> 8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
}

INLINE تهی store_cv_واژگان(uint8_t bytes_out[32], uint32_t cv_واژگان[8]) {
  store32(&bytes_out[0 * 4], cv_واژگان[0]);
  store32(&bytes_out[1 * 4], cv_واژگان[1]);
  store32(&bytes_out[2 * 4], cv_واژگان[2]);
  store32(&bytes_out[3 * 4], cv_واژگان[3]);
  store32(&bytes_out[4 * 4], cv_واژگان[4]);
  store32(&bytes_out[5 * 4], cv_واژگان[5]);
  store32(&bytes_out[6 * 4], cv_واژگان[6]);
  store32(&bytes_out[7 * 4], cv_واژگان[7]);
}

تهی blake3_compress_in_place(uint32_t cv[8],
                              ثابت uint8_t block[BLAKE3_BLOCK_LEN],
                              uint8_t block_len, uint64_t counter,
                              uint8_t flags);

تهی blake3_compress_xof(ثابت uint32_t cv[8],
                         ثابت uint8_t block[BLAKE3_BLOCK_LEN],
                         uint8_t block_len, uint64_t counter, uint8_t flags,
                         uint8_t out[64]);

تهی blake3_hash_many(ثابت uint8_t *ثابت *inputs, size_t رقم_inputs,
                      size_t blocks, ثابت uint32_t کلید[8], uint64_t counter,
                      bool increment_counter, uint8_t flags,
                      uint8_t flags_start, uint8_t flags_end, uint8_t *out);

size_t blake3_simd_degree(تهی);


// Declarations for implementation-specific functions.
تهی blake3_compress_in_place_portable(uint32_t cv[8],
                                       ثابت uint8_t block[BLAKE3_BLOCK_LEN],
                                       uint8_t block_len, uint64_t counter,
                                       uint8_t flags);

تهی blake3_compress_xof_portable(ثابت uint32_t cv[8],
                                  ثابت uint8_t block[BLAKE3_BLOCK_LEN],
                                  uint8_t block_len, uint64_t counter,
                                  uint8_t flags, uint8_t out[64]);

تهی blake3_hash_many_portable(ثابت uint8_t *ثابت *inputs, size_t رقم_inputs,
                               size_t blocks, ثابت uint32_t کلید[8],
                               uint64_t counter, bool increment_counter,
                               uint8_t flags, uint8_t flags_start,
                               uint8_t flags_end, uint8_t *out);

#اگر defined(IS_X86)
#اگر !defined(BLAKE3_NO_SSE2)
تهی blake3_compress_in_place_sse2(uint32_t cv[8],
                                   ثابت uint8_t block[BLAKE3_BLOCK_LEN],
                                   uint8_t block_len, uint64_t counter,
                                   uint8_t flags);
تهی blake3_compress_xof_sse2(ثابت uint32_t cv[8],
                              ثابت uint8_t block[BLAKE3_BLOCK_LEN],
                              uint8_t block_len, uint64_t counter,
                              uint8_t flags, uint8_t out[64]);
تهی blake3_hash_many_sse2(ثابت uint8_t *ثابت *inputs, size_t رقم_inputs,
                           size_t blocks, ثابت uint32_t کلید[8],
                           uint64_t counter, bool increment_counter,
                           uint8_t flags, uint8_t flags_start,
                           uint8_t flags_end, uint8_t *out);
#endif
#اگر !defined(BLAKE3_NO_SSE41)
تهی blake3_compress_in_place_sse41(uint32_t cv[8],
                                    ثابت uint8_t block[BLAKE3_BLOCK_LEN],
                                    uint8_t block_len, uint64_t counter,
                                    uint8_t flags);
تهی blake3_compress_xof_sse41(ثابت uint32_t cv[8],
                               ثابت uint8_t block[BLAKE3_BLOCK_LEN],
                               uint8_t block_len, uint64_t counter,
                               uint8_t flags, uint8_t out[64]);
تهی blake3_hash_many_sse41(ثابت uint8_t *ثابت *inputs, size_t رقم_inputs,
                            size_t blocks, ثابت uint32_t کلید[8],
                            uint64_t counter, bool increment_counter,
                            uint8_t flags, uint8_t flags_start,
                            uint8_t flags_end, uint8_t *out);
#endif
#اگر !defined(BLAKE3_NO_AVX2)
تهی blake3_hash_many_avx2(ثابت uint8_t *ثابت *inputs, size_t رقم_inputs,
                           size_t blocks, ثابت uint32_t کلید[8],
                           uint64_t counter, bool increment_counter,
                           uint8_t flags, uint8_t flags_start,
                           uint8_t flags_end, uint8_t *out);
#endif
#اگر !defined(BLAKE3_NO_AVX512)
تهی blake3_compress_in_place_avx512(uint32_t cv[8],
                                     ثابت uint8_t block[BLAKE3_BLOCK_LEN],
                                     uint8_t block_len, uint64_t counter,
                                     uint8_t flags);

تهی blake3_compress_xof_avx512(ثابت uint32_t cv[8],
                                ثابت uint8_t block[BLAKE3_BLOCK_LEN],
                                uint8_t block_len, uint64_t counter,
                                uint8_t flags, uint8_t out[64]);

تهی blake3_hash_many_avx512(ثابت uint8_t *ثابت *inputs, size_t رقم_inputs,
                             size_t blocks, ثابت uint32_t کلید[8],
                             uint64_t counter, bool increment_counter,
                             uint8_t flags, uint8_t flags_start,
                             uint8_t flags_end, uint8_t *out);
#endif
#endif

#اگر BLAKE3_USE_NEON == 1
تهی blake3_hash_many_neon(ثابت uint8_t *ثابت *inputs, size_t رقم_inputs,
                           size_t blocks, ثابت uint32_t کلید[8],
                           uint64_t counter, bool increment_counter,
                           uint8_t flags, uint8_t flags_start,
                           uint8_t flags_end, uint8_t *out);
#endif


#endif /* BLAKE3_IMPL_H */
