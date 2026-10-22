#अगर_अघोषित BLAKE3_IMPL_H
#घोषणा BLAKE3_IMPL_H

#समावेश <assert.h>
#समावेश <stdbool.h>
#समावेश <stddef.h>
#समावेश <stdint.h>
#समावेश <string.h>

#समावेश "blake3.h"

/* libcrun specific code.  Always force the portable implementation as it is fast enough for us.   */
#घोषणा blake3_compress_in_place_portable blake3_compress_in_place
#घोषणा blake3_compress_xof_portable blake3_compress_xof
#घोषणा blake3_simd_degree_portable blake3_simd_degree
#घोषणा blake3_compress_subtree_wide_portable blake3_compress_subtree_wide
#घोषणा blake3_hash_many_portable blake3_hash_many
अटल अंतरभूत माप_प्रकार blake3_simd_degree(व्योम) {
  वापस 1;
}

// internal flags
क्रमागत blake3_flags {
  CHUNK_START         = 1 << 0,
  CHUNK_END           = 1 << 1,
  PARENT              = 1 << 2,
  मूल_2                = 1 << 3,
  KEYED_HASH          = 1 << 4,
  DERIVE_कुंजी_CONTEXT  = 1 << 5,
  DERIVE_कुंजी_MATERIAL = 1 << 6,
};

// This C implementation tries to support recent versions of GCC, Clang, and
// MSVC.
#अगर defined(_MSC_VER)
#घोषणा INLINE अटल __forceinline
#अन्यथा
#घोषणा INLINE अटल अंतरभूत __attribute__((always_inline))
#पूर्ण_अगर

#अगर defined(__x86_64__) || defined(_M_X64) 
#घोषणा IS_X86
#घोषणा IS_X86_64
#पूर्ण_अगर

#अगर defined(__i386__) || defined(_M_IX86)
#घोषणा IS_X86
#घोषणा IS_X86_32
#पूर्ण_अगर

#अगर defined(__aarch64__) || defined(_M_ARM64)
#घोषणा IS_AARCH64
#पूर्ण_अगर

#अगर defined(IS_X86)
#अगर defined(_MSC_VER)
#समावेश <intrin.h>
#पूर्ण_अगर
#पूर्ण_अगर

#अगर !defined(BLAKE3_USE_NEON) 
  // If BLAKE3_USE_NEON not manually set, autodetect based on AArch64ness
  #अगर defined(IS_AARCH64)
    #अगर defined(__ARM_BIG_ENDIAN)
      #घोषणा BLAKE3_USE_NEON 0
    #अन्यथा
      #घोषणा BLAKE3_USE_NEON 1
    #पूर्ण_अगर
  #अन्यथा
    #घोषणा BLAKE3_USE_NEON 0
  #पूर्ण_अगर
#पूर्ण_अगर

#अगर defined(IS_X86)
#घोषणा MAX_SIMD_DEGREE 16
#या_अगर BLAKE3_USE_NEON == 1
#घोषणा MAX_SIMD_DEGREE 4
#अन्यथा
#घोषणा MAX_SIMD_DEGREE 1
#पूर्ण_अगर

// There are some places where we want a static size that's equal to the
// MAX_SIMD_DEGREE, but also at least 2.
#घोषणा MAX_SIMD_DEGREE_OR_2 (MAX_SIMD_DEGREE > 2 ? MAX_SIMD_DEGREE : 2)

अटल स्थिर uint32_t IV[8] = {0x6A09E667UL, 0xBB67AE85UL, 0x3C6EF372UL,
                               0xA54FF53AUL, 0x510E527FUL, 0x9B05688CUL,
                               0x1F83D9ABUL, 0x5BE0CD19UL};

अटल स्थिर uint8_t संदेश_SCHEDULE[7][16] = {
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
अटल अचिन्हित पूर्णांक highest_one(uint64_t x) {
#अगर defined(__GNUC__) || defined(__clang__)
  वापस 63 ^ (अचिन्हित पूर्णांक)__builtin_clzll(x);
#या_अगर defined(_MSC_VER) && defined(IS_X86_64)
  अचिन्हित दीर्घ सूचकांक;
  _BitScanReverse64(&सूचकांक, x);
  वापस सूचकांक;
#या_अगर defined(_MSC_VER) && defined(IS_X86_32)
  अगर(x >> 32) {
    अचिन्हित दीर्घ सूचकांक;
    _BitScanReverse(&सूचकांक, (अचिन्हित दीर्घ)(x >> 32));
    वापस 32 + सूचकांक;
  } अन्यथा {
    अचिन्हित दीर्घ सूचकांक;
    _BitScanReverse(&सूचकांक, (अचिन्हित दीर्घ)x);
    वापस सूचकांक;
  }
#अन्यथा
  अचिन्हित पूर्णांक c = 0;
  अगर(x & 0xffffffff00000000ULL) { x >>= 32; c += 32; }
  अगर(x & 0x00000000ffff0000ULL) { x >>= 16; c += 16; }
  अगर(x & 0x000000000000ff00ULL) { x >>=  8; c +=  8; }
  अगर(x & 0x00000000000000f0ULL) { x >>=  4; c +=  4; }
  अगर(x & 0x000000000000000cULL) { x >>=  2; c +=  2; }
  अगर(x & 0x0000000000000002ULL) {           c +=  1; }
  वापस c;
#पूर्ण_अगर
}

// Count the number of 1 bits.
INLINE अचिन्हित पूर्णांक popcnt(uint64_t x) {
#अगर defined(__GNUC__) || defined(__clang__)
  वापस (अचिन्हित पूर्णांक)__builtin_popcountll(x);
#अन्यथा
  अचिन्हित पूर्णांक गिनती = 0;
  जबतक (x != 0) {
    गिनती += 1;
    x &= x - 1;
  }
  वापस गिनती;
#पूर्ण_अगर
}

// Largest power of two less than or equal to x. As a special case, returns 1
// when x is 0. 
INLINE uint64_t round_down_to_power_of_2(uint64_t x) {
  वापस 1ULL << highest_one(x | 1);
}

INLINE uint32_t गणक_निचला(uint64_t गणक) { वापस (uint32_t)गणक; }

INLINE uint32_t गणक_ऊपरी(uint64_t गणक) {
  वापस (uint32_t)(गणक >> 32);
}

INLINE uint32_t load32(स्थिर व्योम *src) {
  स्थिर uint8_t *p = (स्थिर uint8_t *)src;
  वापस ((uint32_t)(p[0]) << 0) | ((uint32_t)(p[1]) << 8) |
         ((uint32_t)(p[2]) << 16) | ((uint32_t)(p[3]) << 24);
}

INLINE व्योम load_कुंजी_शब्दगण(स्थिर uint8_t कुंजी[BLAKE3_कुंजी_LEN],
                           uint32_t कुंजी_शब्दगण[8]) {
  कुंजी_शब्दगण[0] = load32(&कुंजी[0 * 4]);
  कुंजी_शब्दगण[1] = load32(&कुंजी[1 * 4]);
  कुंजी_शब्दगण[2] = load32(&कुंजी[2 * 4]);
  कुंजी_शब्दगण[3] = load32(&कुंजी[3 * 4]);
  कुंजी_शब्दगण[4] = load32(&कुंजी[4 * 4]);
  कुंजी_शब्दगण[5] = load32(&कुंजी[5 * 4]);
  कुंजी_शब्दगण[6] = load32(&कुंजी[6 * 4]);
  कुंजी_शब्दगण[7] = load32(&कुंजी[7 * 4]);
}

INLINE व्योम store32(व्योम *dst, uint32_t w) {
  uint8_t *p = (uint8_t *)dst;
  p[0] = (uint8_t)(w >> 0);
  p[1] = (uint8_t)(w >> 8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
}

INLINE व्योम store_cv_शब्दगण(uint8_t bytes_out[32], uint32_t cv_शब्दगण[8]) {
  store32(&bytes_out[0 * 4], cv_शब्दगण[0]);
  store32(&bytes_out[1 * 4], cv_शब्दगण[1]);
  store32(&bytes_out[2 * 4], cv_शब्दगण[2]);
  store32(&bytes_out[3 * 4], cv_शब्दगण[3]);
  store32(&bytes_out[4 * 4], cv_शब्दगण[4]);
  store32(&bytes_out[5 * 4], cv_शब्दगण[5]);
  store32(&bytes_out[6 * 4], cv_शब्दगण[6]);
  store32(&bytes_out[7 * 4], cv_शब्दगण[7]);
}

व्योम blake3_compress_in_place(uint32_t cv[8],
                              स्थिर uint8_t block[BLAKE3_BLOCK_LEN],
                              uint8_t block_len, uint64_t गणक,
                              uint8_t flags);

व्योम blake3_compress_xof(स्थिर uint32_t cv[8],
                         स्थिर uint8_t block[BLAKE3_BLOCK_LEN],
                         uint8_t block_len, uint64_t गणक, uint8_t flags,
                         uint8_t out[64]);

व्योम blake3_hash_many(स्थिर uint8_t *स्थिर *inputs, माप_प्रकार अंक_inputs,
                      माप_प्रकार blocks, स्थिर uint32_t कुंजी[8], uint64_t गणक,
                      bool increment_गणक, uint8_t flags,
                      uint8_t flags_start, uint8_t flags_end, uint8_t *out);

माप_प्रकार blake3_simd_degree(व्योम);


// Declarations for implementation-specific functions.
व्योम blake3_compress_in_place_portable(uint32_t cv[8],
                                       स्थिर uint8_t block[BLAKE3_BLOCK_LEN],
                                       uint8_t block_len, uint64_t गणक,
                                       uint8_t flags);

व्योम blake3_compress_xof_portable(स्थिर uint32_t cv[8],
                                  स्थिर uint8_t block[BLAKE3_BLOCK_LEN],
                                  uint8_t block_len, uint64_t गणक,
                                  uint8_t flags, uint8_t out[64]);

व्योम blake3_hash_many_portable(स्थिर uint8_t *स्थिर *inputs, माप_प्रकार अंक_inputs,
                               माप_प्रकार blocks, स्थिर uint32_t कुंजी[8],
                               uint64_t गणक, bool increment_गणक,
                               uint8_t flags, uint8_t flags_start,
                               uint8_t flags_end, uint8_t *out);

#अगर defined(IS_X86)
#अगर !defined(BLAKE3_NO_SSE2)
व्योम blake3_compress_in_place_sse2(uint32_t cv[8],
                                   स्थिर uint8_t block[BLAKE3_BLOCK_LEN],
                                   uint8_t block_len, uint64_t गणक,
                                   uint8_t flags);
व्योम blake3_compress_xof_sse2(स्थिर uint32_t cv[8],
                              स्थिर uint8_t block[BLAKE3_BLOCK_LEN],
                              uint8_t block_len, uint64_t गणक,
                              uint8_t flags, uint8_t out[64]);
व्योम blake3_hash_many_sse2(स्थिर uint8_t *स्थिर *inputs, माप_प्रकार अंक_inputs,
                           माप_प्रकार blocks, स्थिर uint32_t कुंजी[8],
                           uint64_t गणक, bool increment_गणक,
                           uint8_t flags, uint8_t flags_start,
                           uint8_t flags_end, uint8_t *out);
#पूर्ण_अगर
#अगर !defined(BLAKE3_NO_SSE41)
व्योम blake3_compress_in_place_sse41(uint32_t cv[8],
                                    स्थिर uint8_t block[BLAKE3_BLOCK_LEN],
                                    uint8_t block_len, uint64_t गणक,
                                    uint8_t flags);
व्योम blake3_compress_xof_sse41(स्थिर uint32_t cv[8],
                               स्थिर uint8_t block[BLAKE3_BLOCK_LEN],
                               uint8_t block_len, uint64_t गणक,
                               uint8_t flags, uint8_t out[64]);
व्योम blake3_hash_many_sse41(स्थिर uint8_t *स्थिर *inputs, माप_प्रकार अंक_inputs,
                            माप_प्रकार blocks, स्थिर uint32_t कुंजी[8],
                            uint64_t गणक, bool increment_गणक,
                            uint8_t flags, uint8_t flags_start,
                            uint8_t flags_end, uint8_t *out);
#पूर्ण_अगर
#अगर !defined(BLAKE3_NO_AVX2)
व्योम blake3_hash_many_avx2(स्थिर uint8_t *स्थिर *inputs, माप_प्रकार अंक_inputs,
                           माप_प्रकार blocks, स्थिर uint32_t कुंजी[8],
                           uint64_t गणक, bool increment_गणक,
                           uint8_t flags, uint8_t flags_start,
                           uint8_t flags_end, uint8_t *out);
#पूर्ण_अगर
#अगर !defined(BLAKE3_NO_AVX512)
व्योम blake3_compress_in_place_avx512(uint32_t cv[8],
                                     स्थिर uint8_t block[BLAKE3_BLOCK_LEN],
                                     uint8_t block_len, uint64_t गणक,
                                     uint8_t flags);

व्योम blake3_compress_xof_avx512(स्थिर uint32_t cv[8],
                                स्थिर uint8_t block[BLAKE3_BLOCK_LEN],
                                uint8_t block_len, uint64_t गणक,
                                uint8_t flags, uint8_t out[64]);

व्योम blake3_hash_many_avx512(स्थिर uint8_t *स्थिर *inputs, माप_प्रकार अंक_inputs,
                             माप_प्रकार blocks, स्थिर uint32_t कुंजी[8],
                             uint64_t गणक, bool increment_गणक,
                             uint8_t flags, uint8_t flags_start,
                             uint8_t flags_end, uint8_t *out);
#पूर्ण_अगर
#पूर्ण_अगर

#अगर BLAKE3_USE_NEON == 1
व्योम blake3_hash_many_neon(स्थिर uint8_t *स्थिर *inputs, माप_प्रकार अंक_inputs,
                           माप_प्रकार blocks, स्थिर uint32_t कुंजी[8],
                           uint64_t गणक, bool increment_गणक,
                           uint8_t flags, uint8_t flags_start,
                           uint8_t flags_end, uint8_t *out);
#पूर्ण_अगर


#पूर्ण_अगर /* BLAKE3_IMPL_H */
