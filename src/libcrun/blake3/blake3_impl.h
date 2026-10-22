#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ BLAKE3_IMPL_H
#ವ್ಯಾಖ್ಯಾನಿಸು BLAKE3_IMPL_H

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <string.h>

#ಅನ್ತರ್ಭಾವ "blake3.h"

/* libcrun specific code.  Always force the portable implementation as it is fast enough for us.   */
#ವ್ಯಾಖ್ಯಾನಿಸು blake3_compress_ಒಳ_ಇರಿಸು_portable blake3_compress_ಒಳ_ಇರಿಸು
#ವ್ಯಾಖ್ಯಾನಿಸು blake3_compress_xof_portable blake3_compress_xof
#ವ್ಯಾಖ್ಯಾನಿಸು blake3_simd_degree_portable blake3_simd_degree
#ವ್ಯಾಖ್ಯಾನಿಸು blake3_compress_subtree_wide_portable blake3_compress_subtree_wide
#ವ್ಯಾಖ್ಯಾನಿಸು blake3_hash_many_portable blake3_hash_many
ಸ್ಥಿರ ಸಾಲೊಳಗೆ size_t blake3_simd_degree(ಶೂನ್ಯ) {
  ಹಿಂತಿರುಗಿ 1;
}

// internal flags
ಎಣಿಕೆ blake3_flags {
  CHUNK_ಆರಂಭ         = 1 << 0,
  CHUNK_ಕೊನೆ           = 1 << 1,
  ಪೋಷಕ              = 1 << 2,
  ಬೇರು_2                = 1 << 3,
  KEYED_HASH          = 1 << 4,
  DERIVE_ಕೀಲಿ_CONTEXT  = 1 << 5,
  DERIVE_ಕೀಲಿ_MATERIAL = 1 << 6,
};

// This C implementation tries to support recent versions of GCC, Clang, and
// MSVC.
#ಆದರೆ defined(_MSC_VER)
#ವ್ಯಾಖ್ಯಾನಿಸು INLINE ಸ್ಥಿರ __forceinline
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು INLINE ಸ್ಥಿರ ಸಾಲೊಳಗೆ __attribute__((always_inline))
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(__x86_64__) || defined(_M_X64) 
#ವ್ಯಾಖ್ಯಾನಿಸು IS_X86
#ವ್ಯಾಖ್ಯಾನಿಸು IS_X86_64
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(__i386__) || defined(_M_IX86)
#ವ್ಯಾಖ್ಯಾನಿಸು IS_X86
#ವ್ಯಾಖ್ಯಾನಿಸು IS_X86_32
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(__aarch64__) || defined(_M_ARM64)
#ವ್ಯಾಖ್ಯಾನಿಸು IS_AARCH64
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(IS_X86)
#ಆದರೆ defined(_MSC_VER)
#ಅನ್ತರ್ಭಾವ <intrin.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ !defined(BLAKE3_USE_NEON) 
  // If BLAKE3_USE_NEON not manually set, autodetect based on AArch64ness
  #ಆದರೆ defined(IS_AARCH64)
    #ಆದರೆ defined(__ARM_BIG_ENDIAN)
      #ವ್ಯಾಖ್ಯಾನಿಸು BLAKE3_USE_NEON 0
    #ಇಲ್ಲದಿದ್ದರೆ
      #ವ್ಯಾಖ್ಯಾನಿಸು BLAKE3_USE_NEON 1
    #ಆದರೆ_ಅಂತ್ಯ
  #ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು BLAKE3_USE_NEON 0
  #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(IS_X86)
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_SIMD_DEGREE 16
#ಅಥವಾಆದರೆ BLAKE3_USE_NEON == 1
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_SIMD_DEGREE 4
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_SIMD_DEGREE 1
#ಆದರೆ_ಅಂತ್ಯ

// There are some places where we want a static size that's equal to the
// MAX_SIMD_DEGREE, but also at least 2.
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_SIMD_DEGREE_OR_2 (MAX_SIMD_DEGREE > 2 ? MAX_SIMD_DEGREE : 2)

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t IV[8] = {0x6A09E667UL, 0xBB67AE85UL, 0x3C6EF372UL,
                               0xA54FF53AUL, 0x510E527FUL, 0x9B05688CUL,
                               0x1F83D9ABUL, 0x5BE0CD19UL};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint8_t ಸಂದೇಶ_ವೇಳಾಪಟ್ಟಿ[7][16] = {
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
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ highest_one(uint64_t x) {
#ಆದರೆ defined(__GNUC__) || defined(__clang__)
  ಹಿಂತಿರುಗಿ 63 ^ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)__builtin_clzll(x);
#ಅಥವಾಆದರೆ defined(_MSC_VER) && defined(IS_X86_64)
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸೂಚ್ಯಂಕ;
  ಬಿಟ್_ಪರಿಶೀಲನೆ_Reverse64(&ಸೂಚ್ಯಂಕ, x);
  ಹಿಂತಿರುಗಿ ಸೂಚ್ಯಂಕ;
#ಅಥವಾಆದರೆ defined(_MSC_VER) && defined(IS_X86_32)
  ಆದರೆ(x >> 32) {
    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸೂಚ್ಯಂಕ;
    ಬಿಟ್_ಪರಿಶೀಲನೆ_ಹಿಮ್ಮುಖ(&ಸೂಚ್ಯಂಕ, (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)(x >> 32));
    ಹಿಂತಿರುಗಿ 32 + ಸೂಚ್ಯಂಕ;
  } ಇಲ್ಲದಿದ್ದರೆ {
    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸೂಚ್ಯಂಕ;
    ಬಿಟ್_ಪರಿಶೀಲನೆ_ಹಿಮ್ಮುಖ(&ಸೂಚ್ಯಂಕ, (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)x);
    ಹಿಂತಿರುಗಿ ಸೂಚ್ಯಂಕ;
  }
#ಇಲ್ಲದಿದ್ದರೆ
  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ c = 0;
  ಆದರೆ(x & 0xffffffff00000000ULL) { x >>= 32; c += 32; }
  ಆದರೆ(x & 0x00000000ffff0000ULL) { x >>= 16; c += 16; }
  ಆದರೆ(x & 0x000000000000ff00ULL) { x >>=  8; c +=  8; }
  ಆದರೆ(x & 0x00000000000000f0ULL) { x >>=  4; c +=  4; }
  ಆದರೆ(x & 0x000000000000000cULL) { x >>=  2; c +=  2; }
  ಆದರೆ(x & 0x0000000000000002ULL) {           c +=  1; }
  ಹಿಂತಿರುಗಿ c;
#ಆದರೆ_ಅಂತ್ಯ
}

// Count the number of 1 bits.
INLINE ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ popcnt(uint64_t x) {
#ಆದರೆ defined(__GNUC__) || defined(__clang__)
  ಹಿಂತಿರುಗಿ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)__builtin_popcountll(x);
#ಇಲ್ಲದಿದ್ದರೆ
  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
  ಆಗ (x != 0) {
    ಎಣಿಕೆ_2 += 1;
    x &= x - 1;
  }
  ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_2;
#ಆದರೆ_ಅಂತ್ಯ
}

// Largest power of two less than or equal to x. As a special case, returns 1
// when x is 0. 
INLINE uint64_t ಸುತ್ತು_ಕೆಳಕ್ಕೆ_ಗಮ್ಯ_power_ದ_2(uint64_t x) {
  ಹಿಂತಿರುಗಿ 1ULL << highest_one(x | 1);
}

INLINE uint32_t counter_ಕೆಳಗಿನ(uint64_t counter) { ಹಿಂತಿರುಗಿ (uint32_t)counter; }

INLINE uint32_t counter_ಮೇಲಿನ(uint64_t counter) {
  ಹಿಂತಿರುಗಿ (uint32_t)(counter >> 32);
}

INLINE uint32_t load32(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *src) {
  ಸ್ಥಿರಾಂಕ uint8_t *p = (ಸ್ಥಿರಾಂಕ uint8_t *)src;
  ಹಿಂತಿರುಗಿ ((uint32_t)(p[0]) << 0) | ((uint32_t)(p[1]) << 8) |
         ((uint32_t)(p[2]) << 16) | ((uint32_t)(p[3]) << 24);
}

INLINE ಶೂನ್ಯ ತುಂಬು_ಕೀಲಿ_ಪದಗಳು(ಸ್ಥಿರಾಂಕ uint8_t ಕೀಲಿ[BLAKE3_ಕೀಲಿ_LEN],
                           uint32_t ಕೀಲಿ_ಪದಗಳು[8]) {
  ಕೀಲಿ_ಪದಗಳು[0] = load32(&ಕೀಲಿ[0 * 4]);
  ಕೀಲಿ_ಪದಗಳು[1] = load32(&ಕೀಲಿ[1 * 4]);
  ಕೀಲಿ_ಪದಗಳು[2] = load32(&ಕೀಲಿ[2 * 4]);
  ಕೀಲಿ_ಪದಗಳು[3] = load32(&ಕೀಲಿ[3 * 4]);
  ಕೀಲಿ_ಪದಗಳು[4] = load32(&ಕೀಲಿ[4 * 4]);
  ಕೀಲಿ_ಪದಗಳು[5] = load32(&ಕೀಲಿ[5 * 4]);
  ಕೀಲಿ_ಪದಗಳು[6] = load32(&ಕೀಲಿ[6 * 4]);
  ಕೀಲಿ_ಪದಗಳು[7] = load32(&ಕೀಲಿ[7 * 4]);
}

INLINE ಶೂನ್ಯ store32(ಶೂನ್ಯ *dst, uint32_t w) {
  uint8_t *p = (uint8_t *)dst;
  p[0] = (uint8_t)(w >> 0);
  p[1] = (uint8_t)(w >> 8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
}

INLINE ಶೂನ್ಯ store_cv_ಪದಗಳು(uint8_t ಬೈಟುಗಳು_ಹೊರಹೋಗು[32], uint32_t cv_ಪದಗಳು[8]) {
  store32(&ಬೈಟುಗಳು_ಹೊರಹೋಗು[0 * 4], cv_ಪದಗಳು[0]);
  store32(&ಬೈಟುಗಳು_ಹೊರಹೋಗು[1 * 4], cv_ಪದಗಳು[1]);
  store32(&ಬೈಟುಗಳು_ಹೊರಹೋಗು[2 * 4], cv_ಪದಗಳು[2]);
  store32(&ಬೈಟುಗಳು_ಹೊರಹೋಗು[3 * 4], cv_ಪದಗಳು[3]);
  store32(&ಬೈಟುಗಳು_ಹೊರಹೋಗು[4 * 4], cv_ಪದಗಳು[4]);
  store32(&ಬೈಟುಗಳು_ಹೊರಹೋಗು[5 * 4], cv_ಪದಗಳು[5]);
  store32(&ಬೈಟುಗಳು_ಹೊರಹೋಗು[6 * 4], cv_ಪದಗಳು[6]);
  store32(&ಬೈಟುಗಳು_ಹೊರಹೋಗು[7 * 4], cv_ಪದಗಳು[7]);
}

ಶೂನ್ಯ blake3_compress_ಒಳ_ಇರಿಸು(uint32_t cv[8],
                              ಸ್ಥಿರಾಂಕ uint8_t ಖಂಡ[BLAKE3_ಖಂಡ_LEN],
                              uint8_t ಖಂಡ_len, uint64_t counter,
                              uint8_t flags);

ಶೂನ್ಯ blake3_compress_xof(ಸ್ಥಿರಾಂಕ uint32_t cv[8],
                         ಸ್ಥಿರಾಂಕ uint8_t ಖಂಡ[BLAKE3_ಖಂಡ_LEN],
                         uint8_t ಖಂಡ_len, uint64_t counter, uint8_t flags,
                         uint8_t ಹೊರಹೋಗು[64]);

ಶೂನ್ಯ blake3_hash_many(ಸ್ಥಿರಾಂಕ uint8_t *ಸ್ಥಿರಾಂಕ *inputs, size_t ಅಂಕೆ_inputs,
                      size_t ಖಂಡಗಳು, ಸ್ಥಿರಾಂಕ uint32_t ಕೀಲಿ[8], uint64_t counter,
                      bool increment_counter, uint8_t flags,
                      uint8_t flags_ಆರಂಭ, uint8_t flags_ಕೊನೆ, uint8_t *ಹೊರಹೋಗು);

size_t blake3_simd_degree(ಶೂನ್ಯ);


// Declarations for implementation-specific functions.
ಶೂನ್ಯ blake3_compress_ಒಳ_ಇರಿಸು_portable(uint32_t cv[8],
                                       ಸ್ಥಿರಾಂಕ uint8_t ಖಂಡ[BLAKE3_ಖಂಡ_LEN],
                                       uint8_t ಖಂಡ_len, uint64_t counter,
                                       uint8_t flags);

ಶೂನ್ಯ blake3_compress_xof_portable(ಸ್ಥಿರಾಂಕ uint32_t cv[8],
                                  ಸ್ಥಿರಾಂಕ uint8_t ಖಂಡ[BLAKE3_ಖಂಡ_LEN],
                                  uint8_t ಖಂಡ_len, uint64_t counter,
                                  uint8_t flags, uint8_t ಹೊರಹೋಗು[64]);

ಶೂನ್ಯ blake3_hash_many_portable(ಸ್ಥಿರಾಂಕ uint8_t *ಸ್ಥಿರಾಂಕ *inputs, size_t ಅಂಕೆ_inputs,
                               size_t ಖಂಡಗಳು, ಸ್ಥಿರಾಂಕ uint32_t ಕೀಲಿ[8],
                               uint64_t counter, bool increment_counter,
                               uint8_t flags, uint8_t flags_ಆರಂಭ,
                               uint8_t flags_ಕೊನೆ, uint8_t *ಹೊರಹೋಗು);

#ಆದರೆ defined(IS_X86)
#ಆದರೆ !defined(BLAKE3_NO_SSE2)
ಶೂನ್ಯ blake3_compress_ಒಳ_ಇರಿಸು_sse2(uint32_t cv[8],
                                   ಸ್ಥಿರಾಂಕ uint8_t ಖಂಡ[BLAKE3_ಖಂಡ_LEN],
                                   uint8_t ಖಂಡ_len, uint64_t counter,
                                   uint8_t flags);
ಶೂನ್ಯ blake3_compress_xof_sse2(ಸ್ಥಿರಾಂಕ uint32_t cv[8],
                              ಸ್ಥಿರಾಂಕ uint8_t ಖಂಡ[BLAKE3_ಖಂಡ_LEN],
                              uint8_t ಖಂಡ_len, uint64_t counter,
                              uint8_t flags, uint8_t ಹೊರಹೋಗು[64]);
ಶೂನ್ಯ blake3_hash_many_sse2(ಸ್ಥಿರಾಂಕ uint8_t *ಸ್ಥಿರಾಂಕ *inputs, size_t ಅಂಕೆ_inputs,
                           size_t ಖಂಡಗಳು, ಸ್ಥಿರಾಂಕ uint32_t ಕೀಲಿ[8],
                           uint64_t counter, bool increment_counter,
                           uint8_t flags, uint8_t flags_ಆರಂಭ,
                           uint8_t flags_ಕೊನೆ, uint8_t *ಹೊರಹೋಗು);
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ !defined(BLAKE3_NO_SSE41)
ಶೂನ್ಯ blake3_compress_ಒಳ_ಇರಿಸು_sse41(uint32_t cv[8],
                                    ಸ್ಥಿರಾಂಕ uint8_t ಖಂಡ[BLAKE3_ಖಂಡ_LEN],
                                    uint8_t ಖಂಡ_len, uint64_t counter,
                                    uint8_t flags);
ಶೂನ್ಯ blake3_compress_xof_sse41(ಸ್ಥಿರಾಂಕ uint32_t cv[8],
                               ಸ್ಥಿರಾಂಕ uint8_t ಖಂಡ[BLAKE3_ಖಂಡ_LEN],
                               uint8_t ಖಂಡ_len, uint64_t counter,
                               uint8_t flags, uint8_t ಹೊರಹೋಗು[64]);
ಶೂನ್ಯ blake3_hash_many_sse41(ಸ್ಥಿರಾಂಕ uint8_t *ಸ್ಥಿರಾಂಕ *inputs, size_t ಅಂಕೆ_inputs,
                            size_t ಖಂಡಗಳು, ಸ್ಥಿರಾಂಕ uint32_t ಕೀಲಿ[8],
                            uint64_t counter, bool increment_counter,
                            uint8_t flags, uint8_t flags_ಆರಂಭ,
                            uint8_t flags_ಕೊನೆ, uint8_t *ಹೊರಹೋಗು);
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ !defined(BLAKE3_NO_AVX2)
ಶೂನ್ಯ blake3_hash_many_avx2(ಸ್ಥಿರಾಂಕ uint8_t *ಸ್ಥಿರಾಂಕ *inputs, size_t ಅಂಕೆ_inputs,
                           size_t ಖಂಡಗಳು, ಸ್ಥಿರಾಂಕ uint32_t ಕೀಲಿ[8],
                           uint64_t counter, bool increment_counter,
                           uint8_t flags, uint8_t flags_ಆರಂಭ,
                           uint8_t flags_ಕೊನೆ, uint8_t *ಹೊರಹೋಗು);
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ !defined(BLAKE3_NO_AVX512)
ಶೂನ್ಯ blake3_compress_ಒಳ_ಇರಿಸು_avx512(uint32_t cv[8],
                                     ಸ್ಥಿರಾಂಕ uint8_t ಖಂಡ[BLAKE3_ಖಂಡ_LEN],
                                     uint8_t ಖಂಡ_len, uint64_t counter,
                                     uint8_t flags);

ಶೂನ್ಯ blake3_compress_xof_avx512(ಸ್ಥಿರಾಂಕ uint32_t cv[8],
                                ಸ್ಥಿರಾಂಕ uint8_t ಖಂಡ[BLAKE3_ಖಂಡ_LEN],
                                uint8_t ಖಂಡ_len, uint64_t counter,
                                uint8_t flags, uint8_t ಹೊರಹೋಗು[64]);

ಶೂನ್ಯ blake3_hash_many_avx512(ಸ್ಥಿರಾಂಕ uint8_t *ಸ್ಥಿರಾಂಕ *inputs, size_t ಅಂಕೆ_inputs,
                             size_t ಖಂಡಗಳು, ಸ್ಥಿರಾಂಕ uint32_t ಕೀಲಿ[8],
                             uint64_t counter, bool increment_counter,
                             uint8_t flags, uint8_t flags_ಆರಂಭ,
                             uint8_t flags_ಕೊನೆ, uint8_t *ಹೊರಹೋಗು);
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ BLAKE3_USE_NEON == 1
ಶೂನ್ಯ blake3_hash_many_neon(ಸ್ಥಿರಾಂಕ uint8_t *ಸ್ಥಿರಾಂಕ *inputs, size_t ಅಂಕೆ_inputs,
                           size_t ಖಂಡಗಳು, ಸ್ಥಿರಾಂಕ uint32_t ಕೀಲಿ[8],
                           uint64_t counter, bool increment_counter,
                           uint8_t flags, uint8_t flags_ಆರಂಭ,
                           uint8_t flags_ಕೊನೆ, uint8_t *ಹೊರಹೋಗು);
#ಆದರೆ_ಅಂತ್ಯ


#ಆದರೆ_ಅಂತ್ಯ /* BLAKE3_IMPL_H */
