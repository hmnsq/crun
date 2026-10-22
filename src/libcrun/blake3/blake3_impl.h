#ifndef BLAKE3_IMPL_H
#define BLAKE3_IMPL_H

#अन्तर्भाव <assert.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stddef.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <string.h>

#अन्तर्भाव "blake3.h"

/* libcrun specific code.  Always force the portable implementation as it is fast enough for us.   */
#define blake3_compress_in_place_portable blake3_compress_in_place
#define blake3_compress_xof_portable blake3_compress_xof
#define blake3_simd_degree_portable blake3_simd_degree
#define blake3_compress_subtree_wide_portable blake3_compress_subtree_wide
#define blake3_hash_many_portable blake3_hash_many
स्थिर अन्तःस्थ माप_प्रकार blake3_simd_degree(शून्य) {
  फर्काऊ 1;
}

// internal flags
गणना blake3_flags {
  CHUNK_START         = 1 << 0,
  CHUNK_END           = 1 << 1,
  PARENT              = 1 << 2,
  जरा_2                = 1 << 3,
  KEYED_HASH          = 1 << 4,
  DERIVE_साँचो_CONTEXT  = 1 << 5,
  DERIVE_साँचो_MATERIAL = 1 << 6,
};

// This C implementation tries to support recent versions of GCC, Clang, and
// MSVC.
#यदि defined(_MSC_VER)
#define INLINE स्थिर __forceinline
#अन्यथा
#define INLINE स्थिर अन्तःस्थ __attribute__((always_inline))
#endif

#यदि defined(__x86_64__) || defined(_M_X64) 
#define IS_X86
#define IS_X86_64
#endif

#यदि defined(__i386__) || defined(_M_IX86)
#define IS_X86
#define IS_X86_32
#endif

#यदि defined(__aarch64__) || defined(_M_ARM64)
#define IS_AARCH64
#endif

#यदि defined(IS_X86)
#यदि defined(_MSC_VER)
#अन्तर्भाव <intrin.h>
#endif
#endif

#यदि !defined(BLAKE3_USE_NEON) 
  // If BLAKE3_USE_NEON not manually set, autodetect based on AArch64ness
  #यदि defined(IS_AARCH64)
    #यदि defined(__ARM_BIG_ENDIAN)
      #define BLAKE3_USE_NEON 0
    #अन्यथा
      #define BLAKE3_USE_NEON 1
    #endif
  #अन्यथा
    #define BLAKE3_USE_NEON 0
  #endif
#endif

#यदि defined(IS_X86)
#define MAX_SIMD_DEGREE 16
#अथवायदि BLAKE3_USE_NEON == 1
#define MAX_SIMD_DEGREE 4
#अन्यथा
#define MAX_SIMD_DEGREE 1
#endif

// There are some places where we want a static size that's equal to the
// MAX_SIMD_DEGREE, but also at least 2.
#define MAX_SIMD_DEGREE_OR_2 (MAX_SIMD_DEGREE > 2 ? MAX_SIMD_DEGREE : 2)

स्थिर अचर uint32_t IV[8] = {0x6A09E667UL, 0xBB67AE85UL, 0x3C6EF372UL,
                               0xA54FF53AUL, 0x510E527FUL, 0x9B05688CUL,
                               0x1F83D9ABUL, 0x5BE0CD19UL};

स्थिर अचर uint8_t सन्देश_SCHEDULE[7][16] = {
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
स्थिर अचिन्हित पूर्णाङ्क highest_one(uint64_t x) {
#यदि defined(__GNUC__) || defined(__clang__)
  फर्काऊ 63 ^ (अचिन्हित पूर्णाङ्क)__builtin_clzll(x);
#अथवायदि defined(_MSC_VER) && defined(IS_X86_64)
  अचिन्हित दीर्घ index;
  _BitScanReverse64(&index, x);
  फर्काऊ index;
#अथवायदि defined(_MSC_VER) && defined(IS_X86_32)
  यदि(x >> 32) {
    अचिन्हित दीर्घ index;
    _BitScanReverse(&index, (अचिन्हित दीर्घ)(x >> 32));
    फर्काऊ 32 + index;
  } अन्यथा {
    अचिन्हित दीर्घ index;
    _BitScanReverse(&index, (अचिन्हित दीर्घ)x);
    फर्काऊ index;
  }
#अन्यथा
  अचिन्हित पूर्णाङ्क c = 0;
  यदि(x & 0xffffffff00000000ULL) { x >>= 32; c += 32; }
  यदि(x & 0x00000000ffff0000ULL) { x >>= 16; c += 16; }
  यदि(x & 0x000000000000ff00ULL) { x >>=  8; c +=  8; }
  यदि(x & 0x00000000000000f0ULL) { x >>=  4; c +=  4; }
  यदि(x & 0x000000000000000cULL) { x >>=  2; c +=  2; }
  यदि(x & 0x0000000000000002ULL) {           c +=  1; }
  फर्काऊ c;
#endif
}

// Count the number of 1 bits.
INLINE अचिन्हित पूर्णाङ्क popcnt(uint64_t x) {
#यदि defined(__GNUC__) || defined(__clang__)
  फर्काऊ (अचिन्हित पूर्णाङ्क)__builtin_popcountll(x);
#अन्यथा
  अचिन्हित पूर्णाङ्क गन्ती = 0;
  जबसम्म (x != 0) {
    गन्ती += 1;
    x &= x - 1;
  }
  फर्काऊ गन्ती;
#endif
}

// Largest power of two less than or equal to x. As a special case, returns 1
// when x is 0. 
INLINE uint64_t round_down_to_power_of_2(uint64_t x) {
  फर्काऊ 1ULL << highest_one(x | 1);
}

INLINE uint32_t counter_तल्लो(uint64_t counter) { फर्काऊ (uint32_t)counter; }

INLINE uint32_t counter_माथिल्लो(uint64_t counter) {
  फर्काऊ (uint32_t)(counter >> 32);
}

INLINE uint32_t load32(अचर शून्य *src) {
  अचर uint8_t *p = (अचर uint8_t *)src;
  फर्काऊ ((uint32_t)(p[0]) << 0) | ((uint32_t)(p[1]) << 8) |
         ((uint32_t)(p[2]) << 16) | ((uint32_t)(p[3]) << 24);
}

INLINE शून्य load_साँचो_शब्दहरू(अचर uint8_t साँचो[BLAKE3_साँचो_LEN],
                           uint32_t साँचो_शब्दहरू[8]) {
  साँचो_शब्दहरू[0] = load32(&साँचो[0 * 4]);
  साँचो_शब्दहरू[1] = load32(&साँचो[1 * 4]);
  साँचो_शब्दहरू[2] = load32(&साँचो[2 * 4]);
  साँचो_शब्दहरू[3] = load32(&साँचो[3 * 4]);
  साँचो_शब्दहरू[4] = load32(&साँचो[4 * 4]);
  साँचो_शब्दहरू[5] = load32(&साँचो[5 * 4]);
  साँचो_शब्दहरू[6] = load32(&साँचो[6 * 4]);
  साँचो_शब्दहरू[7] = load32(&साँचो[7 * 4]);
}

INLINE शून्य store32(शून्य *dst, uint32_t w) {
  uint8_t *p = (uint8_t *)dst;
  p[0] = (uint8_t)(w >> 0);
  p[1] = (uint8_t)(w >> 8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
}

INLINE शून्य store_cv_शब्दहरू(uint8_t bytes_out[32], uint32_t cv_शब्दहरू[8]) {
  store32(&bytes_out[0 * 4], cv_शब्दहरू[0]);
  store32(&bytes_out[1 * 4], cv_शब्दहरू[1]);
  store32(&bytes_out[2 * 4], cv_शब्दहरू[2]);
  store32(&bytes_out[3 * 4], cv_शब्दहरू[3]);
  store32(&bytes_out[4 * 4], cv_शब्दहरू[4]);
  store32(&bytes_out[5 * 4], cv_शब्दहरू[5]);
  store32(&bytes_out[6 * 4], cv_शब्दहरू[6]);
  store32(&bytes_out[7 * 4], cv_शब्दहरू[7]);
}

शून्य blake3_compress_in_place(uint32_t cv[8],
                              अचर uint8_t block[BLAKE3_BLOCK_LEN],
                              uint8_t block_len, uint64_t counter,
                              uint8_t flags);

शून्य blake3_compress_xof(अचर uint32_t cv[8],
                         अचर uint8_t block[BLAKE3_BLOCK_LEN],
                         uint8_t block_len, uint64_t counter, uint8_t flags,
                         uint8_t out[64]);

शून्य blake3_hash_many(अचर uint8_t *अचर *inputs, माप_प्रकार अङ्क_inputs,
                      माप_प्रकार blocks, अचर uint32_t साँचो[8], uint64_t counter,
                      bool increment_counter, uint8_t flags,
                      uint8_t flags_start, uint8_t flags_end, uint8_t *out);

माप_प्रकार blake3_simd_degree(शून्य);


// Declarations for implementation-specific functions.
शून्य blake3_compress_in_place_portable(uint32_t cv[8],
                                       अचर uint8_t block[BLAKE3_BLOCK_LEN],
                                       uint8_t block_len, uint64_t counter,
                                       uint8_t flags);

शून्य blake3_compress_xof_portable(अचर uint32_t cv[8],
                                  अचर uint8_t block[BLAKE3_BLOCK_LEN],
                                  uint8_t block_len, uint64_t counter,
                                  uint8_t flags, uint8_t out[64]);

शून्य blake3_hash_many_portable(अचर uint8_t *अचर *inputs, माप_प्रकार अङ्क_inputs,
                               माप_प्रकार blocks, अचर uint32_t साँचो[8],
                               uint64_t counter, bool increment_counter,
                               uint8_t flags, uint8_t flags_start,
                               uint8_t flags_end, uint8_t *out);

#यदि defined(IS_X86)
#यदि !defined(BLAKE3_NO_SSE2)
शून्य blake3_compress_in_place_sse2(uint32_t cv[8],
                                   अचर uint8_t block[BLAKE3_BLOCK_LEN],
                                   uint8_t block_len, uint64_t counter,
                                   uint8_t flags);
शून्य blake3_compress_xof_sse2(अचर uint32_t cv[8],
                              अचर uint8_t block[BLAKE3_BLOCK_LEN],
                              uint8_t block_len, uint64_t counter,
                              uint8_t flags, uint8_t out[64]);
शून्य blake3_hash_many_sse2(अचर uint8_t *अचर *inputs, माप_प्रकार अङ्क_inputs,
                           माप_प्रकार blocks, अचर uint32_t साँचो[8],
                           uint64_t counter, bool increment_counter,
                           uint8_t flags, uint8_t flags_start,
                           uint8_t flags_end, uint8_t *out);
#endif
#यदि !defined(BLAKE3_NO_SSE41)
शून्य blake3_compress_in_place_sse41(uint32_t cv[8],
                                    अचर uint8_t block[BLAKE3_BLOCK_LEN],
                                    uint8_t block_len, uint64_t counter,
                                    uint8_t flags);
शून्य blake3_compress_xof_sse41(अचर uint32_t cv[8],
                               अचर uint8_t block[BLAKE3_BLOCK_LEN],
                               uint8_t block_len, uint64_t counter,
                               uint8_t flags, uint8_t out[64]);
शून्य blake3_hash_many_sse41(अचर uint8_t *अचर *inputs, माप_प्रकार अङ्क_inputs,
                            माप_प्रकार blocks, अचर uint32_t साँचो[8],
                            uint64_t counter, bool increment_counter,
                            uint8_t flags, uint8_t flags_start,
                            uint8_t flags_end, uint8_t *out);
#endif
#यदि !defined(BLAKE3_NO_AVX2)
शून्य blake3_hash_many_avx2(अचर uint8_t *अचर *inputs, माप_प्रकार अङ्क_inputs,
                           माप_प्रकार blocks, अचर uint32_t साँचो[8],
                           uint64_t counter, bool increment_counter,
                           uint8_t flags, uint8_t flags_start,
                           uint8_t flags_end, uint8_t *out);
#endif
#यदि !defined(BLAKE3_NO_AVX512)
शून्य blake3_compress_in_place_avx512(uint32_t cv[8],
                                     अचर uint8_t block[BLAKE3_BLOCK_LEN],
                                     uint8_t block_len, uint64_t counter,
                                     uint8_t flags);

शून्य blake3_compress_xof_avx512(अचर uint32_t cv[8],
                                अचर uint8_t block[BLAKE3_BLOCK_LEN],
                                uint8_t block_len, uint64_t counter,
                                uint8_t flags, uint8_t out[64]);

शून्य blake3_hash_many_avx512(अचर uint8_t *अचर *inputs, माप_प्रकार अङ्क_inputs,
                             माप_प्रकार blocks, अचर uint32_t साँचो[8],
                             uint64_t counter, bool increment_counter,
                             uint8_t flags, uint8_t flags_start,
                             uint8_t flags_end, uint8_t *out);
#endif
#endif

#यदि BLAKE3_USE_NEON == 1
शून्य blake3_hash_many_neon(अचर uint8_t *अचर *inputs, माप_प्रकार अङ्क_inputs,
                           माप_प्रकार blocks, अचर uint32_t साँचो[8],
                           uint64_t counter, bool increment_counter,
                           uint8_t flags, uint8_t flags_start,
                           uint8_t flags_end, uint8_t *out);
#endif


#endif /* BLAKE3_IMPL_H */
