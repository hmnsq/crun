#यदि_अपरिभाषितम् BLAKE3_IMPL_H
#परिभाषय BLAKE3_IMPL_H

#अन्तर्भाव <assert.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stddef.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <string.h>

#अन्तर्भाव "blake3.h"

/* libcrun specific code.  Always force the portable implementation as it is fast enough for us.   */
#परिभाषय blake3_compress_अन्तर्_place_portable blake3_compress_अन्तर्_place
#परिभाषय blake3_compress_xof_portable blake3_compress_xof
#परिभाषय blake3_simd_degree_portable blake3_simd_degree
#परिभाषय blake3_compress_subtree_wide_portable blake3_compress_subtree_wide
#परिभाषय blake3_hash_many_portable blake3_hash_many
स्थिर अन्तःस्थ size_t blake3_simd_degree(शून्य) {
  प्रत्यागम 1;
}

// internal flags
गणना blake3_flags {
  CHUNK_START         = 1 << 0,
  CHUNK_अन्तः           = 1 << 1,
  PARENT              = 1 << 2,
  मूलम्_2                = 1 << 3,
  KEYED_HASH          = 1 << 4,
  DERIVE_कुञ्चिका_CONTEXT  = 1 << 5,
  DERIVE_कुञ्चिका_MATERIAL = 1 << 6,
};

// This C implementation tries to support recent versions of GCC, Clang, and
// MSVC.
#यदि defined(_MSC_VER)
#परिभाषय INLINE स्थिर __forceinline
#अन्यथा
#परिभाषय INLINE स्थिर अन्तःस्थ __attribute__((always_inline))
#यदि_अन्तः

#यदि defined(__x86_64__) || defined(_M_X64) 
#परिभाषय IS_X86
#परिभाषय IS_X86_64
#यदि_अन्तः

#यदि defined(__i386__) || defined(_M_IX86)
#परिभाषय IS_X86
#परिभाषय IS_X86_32
#यदि_अन्तः

#यदि defined(__aarch64__) || defined(_M_ARM64)
#परिभाषय IS_AARCH64
#यदि_अन्तः

#यदि defined(IS_X86)
#यदि defined(_MSC_VER)
#अन्तर्भाव <intrin.h>
#यदि_अन्तः
#यदि_अन्तः

#यदि !defined(BLAKE3_USE_NEON) 
  // If BLAKE3_USE_NEON not manually set, autodetect based on AArch64ness
  #यदि defined(IS_AARCH64)
    #यदि defined(__ARM_BIG_ENDIAN)
      #परिभाषय BLAKE3_USE_NEON 0
    #अन्यथा
      #परिभाषय BLAKE3_USE_NEON 1
    #यदि_अन्तः
  #अन्यथा
    #परिभाषय BLAKE3_USE_NEON 0
  #यदि_अन्तः
#यदि_अन्तः

#यदि defined(IS_X86)
#परिभाषय MAX_SIMD_DEGREE 16
#नोचेद्यदि BLAKE3_USE_NEON == 1
#परिभाषय MAX_SIMD_DEGREE 4
#अन्यथा
#परिभाषय MAX_SIMD_DEGREE 1
#यदि_अन्तः

// There are some places where we want a static size that's equal to the
// MAX_SIMD_DEGREE, but also at least 2.
#परिभाषय MAX_SIMD_DEGREE_OR_2 (MAX_SIMD_DEGREE > 2 ? MAX_SIMD_DEGREE : 2)

स्थिर ध्रुव uint32_t IV[8] = {0x6A09E667UL, 0xBB67AE85UL, 0x3C6EF372UL,
                               0xA54FF53AUL, 0x510E527FUL, 0x9B05688CUL,
                               0x1F83D9ABUL, 0x5BE0CD19UL};

स्थिर ध्रुव uint8_t सन्देशः_कालसारणी[7][16] = {
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
स्थिर अचिह्नित पूर्णाङ्क highest_one(uint64_t x) {
#यदि defined(__GNUC__) || defined(__clang__)
  प्रत्यागम 63 ^ (अचिह्नित पूर्णाङ्क)__builtin_clzll(x);
#नोचेद्यदि defined(_MSC_VER) && defined(IS_X86_64)
  अचिह्नित दीर्घ सूचकांक;
  Bit_अवलोकनम्_Reverse64(&सूचकांक, x);
  प्रत्यागम सूचकांक;
#नोचेद्यदि defined(_MSC_VER) && defined(IS_X86_32)
  यदि(x >> 32) {
    अचिह्नित दीर्घ सूचकांक;
    Bit_अवलोकनम्_Reverse(&सूचकांक, (अचिह्नित दीर्घ)(x >> 32));
    प्रत्यागम 32 + सूचकांक;
  } अन्यथा {
    अचिह्नित दीर्घ सूचकांक;
    Bit_अवलोकनम्_Reverse(&सूचकांक, (अचिह्नित दीर्घ)x);
    प्रत्यागम सूचकांक;
  }
#अन्यथा
  अचिह्नित पूर्णाङ्क c = 0;
  यदि(x & 0xffffffff00000000ULL) { x >>= 32; c += 32; }
  यदि(x & 0x00000000ffff0000ULL) { x >>= 16; c += 16; }
  यदि(x & 0x000000000000ff00ULL) { x >>=  8; c +=  8; }
  यदि(x & 0x00000000000000f0ULL) { x >>=  4; c +=  4; }
  यदि(x & 0x000000000000000cULL) { x >>=  2; c +=  2; }
  यदि(x & 0x0000000000000002ULL) {           c +=  1; }
  प्रत्यागम c;
#यदि_अन्तः
}

// Count the number of 1 bits.
INLINE अचिह्नित पूर्णाङ्क popcnt(uint64_t x) {
#यदि defined(__GNUC__) || defined(__clang__)
  प्रत्यागम (अचिह्नित पूर्णाङ्क)__builtin_popcountll(x);
#अन्यथा
  अचिह्नित पूर्णाङ्क गणनम् = 0;
  यावत् (x != 0) {
    गणनम् += 1;
    x &= x - 1;
  }
  प्रत्यागम गणनम्;
#यदि_अन्तः
}

// Largest power of two less than or equal to x. As a special case, returns 1
// when x is 0. 
INLINE uint64_t आवर्तनम्_अधस्तात्_गन्तव्यम्_power_सम्बन्धि_2(uint64_t x) {
  प्रत्यागम 1ULL << highest_one(x | 1);
}

INLINE uint32_t गणक_नीचः(uint64_t गणक) { प्रत्यागम (uint32_t)गणक; }

INLINE uint32_t गणक_उच्चः(uint64_t गणक) {
  प्रत्यागम (uint32_t)(गणक >> 32);
}

INLINE uint32_t load32(ध्रुव शून्य *src) {
  ध्रुव uint8_t *p = (ध्रुव uint8_t *)src;
  प्रत्यागम ((uint32_t)(p[0]) << 0) | ((uint32_t)(p[1]) << 8) |
         ((uint32_t)(p[2]) << 16) | ((uint32_t)(p[3]) << 24);
}

INLINE शून्य आरोपय_कुञ्चिका_शब्दाः(ध्रुव uint8_t कुञ्चिका[BLAKE3_कुञ्चिका_LEN],
                           uint32_t कुञ्चिका_शब्दाः[8]) {
  कुञ्चिका_शब्दाः[0] = load32(&कुञ्चिका[0 * 4]);
  कुञ्चिका_शब्दाः[1] = load32(&कुञ्चिका[1 * 4]);
  कुञ्चिका_शब्दाः[2] = load32(&कुञ्चिका[2 * 4]);
  कुञ्चिका_शब्दाः[3] = load32(&कुञ्चिका[3 * 4]);
  कुञ्चिका_शब्दाः[4] = load32(&कुञ्चिका[4 * 4]);
  कुञ्चिका_शब्दाः[5] = load32(&कुञ्चिका[5 * 4]);
  कुञ्चिका_शब्दाः[6] = load32(&कुञ्चिका[6 * 4]);
  कुञ्चिका_शब्दाः[7] = load32(&कुञ्चिका[7 * 4]);
}

INLINE शून्य store32(शून्य *dst, uint32_t w) {
  uint8_t *p = (uint8_t *)dst;
  p[0] = (uint8_t)(w >> 0);
  p[1] = (uint8_t)(w >> 8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
}

INLINE शून्य store_cv_शब्दाः(uint8_t अष्टकानि_बहिर्गमनम्[32], uint32_t cv_शब्दाः[8]) {
  store32(&अष्टकानि_बहिर्गमनम्[0 * 4], cv_शब्दाः[0]);
  store32(&अष्टकानि_बहिर्गमनम्[1 * 4], cv_शब्दाः[1]);
  store32(&अष्टकानि_बहिर्गमनम्[2 * 4], cv_शब्दाः[2]);
  store32(&अष्टकानि_बहिर्गमनम्[3 * 4], cv_शब्दाः[3]);
  store32(&अष्टकानि_बहिर्गमनम्[4 * 4], cv_शब्दाः[4]);
  store32(&अष्टकानि_बहिर्गमनम्[5 * 4], cv_शब्दाः[5]);
  store32(&अष्टकानि_बहिर्गमनम्[6 * 4], cv_शब्दाः[6]);
  store32(&अष्टकानि_बहिर्गमनम्[7 * 4], cv_शब्दाः[7]);
}

शून्य blake3_compress_अन्तर्_place(uint32_t cv[8],
                              ध्रुव uint8_t खण्डम्[BLAKE3_खण्डम्_LEN],
                              uint8_t खण्डम्_len, uint64_t गणक,
                              uint8_t flags);

शून्य blake3_compress_xof(ध्रुव uint32_t cv[8],
                         ध्रुव uint8_t खण्डम्[BLAKE3_खण्डम्_LEN],
                         uint8_t खण्डम्_len, uint64_t गणक, uint8_t flags,
                         uint8_t बहिर्गमनम्[64]);

शून्य blake3_hash_many(ध्रुव uint8_t *ध्रुव *inputs, size_t अङ्कः_inputs,
                      size_t खण्डानि, ध्रुव uint32_t कुञ्चिका[8], uint64_t गणक,
                      bool increment_गणक, uint8_t flags,
                      uint8_t flags_start, uint8_t flags_अन्तः, uint8_t *बहिर्गमनम्);

size_t blake3_simd_degree(शून्य);


// Declarations for implementation-specific functions.
शून्य blake3_compress_अन्तर्_place_portable(uint32_t cv[8],
                                       ध्रुव uint8_t खण्डम्[BLAKE3_खण्डम्_LEN],
                                       uint8_t खण्डम्_len, uint64_t गणक,
                                       uint8_t flags);

शून्य blake3_compress_xof_portable(ध्रुव uint32_t cv[8],
                                  ध्रुव uint8_t खण्डम्[BLAKE3_खण्डम्_LEN],
                                  uint8_t खण्डम्_len, uint64_t गणक,
                                  uint8_t flags, uint8_t बहिर्गमनम्[64]);

शून्य blake3_hash_many_portable(ध्रुव uint8_t *ध्रुव *inputs, size_t अङ्कः_inputs,
                               size_t खण्डानि, ध्रुव uint32_t कुञ्चिका[8],
                               uint64_t गणक, bool increment_गणक,
                               uint8_t flags, uint8_t flags_start,
                               uint8_t flags_अन्तः, uint8_t *बहिर्गमनम्);

#यदि defined(IS_X86)
#यदि !defined(BLAKE3_NO_SSE2)
शून्य blake3_compress_अन्तर्_place_sse2(uint32_t cv[8],
                                   ध्रुव uint8_t खण्डम्[BLAKE3_खण्डम्_LEN],
                                   uint8_t खण्डम्_len, uint64_t गणक,
                                   uint8_t flags);
शून्य blake3_compress_xof_sse2(ध्रुव uint32_t cv[8],
                              ध्रुव uint8_t खण्डम्[BLAKE3_खण्डम्_LEN],
                              uint8_t खण्डम्_len, uint64_t गणक,
                              uint8_t flags, uint8_t बहिर्गमनम्[64]);
शून्य blake3_hash_many_sse2(ध्रुव uint8_t *ध्रुव *inputs, size_t अङ्कः_inputs,
                           size_t खण्डानि, ध्रुव uint32_t कुञ्चिका[8],
                           uint64_t गणक, bool increment_गणक,
                           uint8_t flags, uint8_t flags_start,
                           uint8_t flags_अन्तः, uint8_t *बहिर्गमनम्);
#यदि_अन्तः
#यदि !defined(BLAKE3_NO_SSE41)
शून्य blake3_compress_अन्तर्_place_sse41(uint32_t cv[8],
                                    ध्रुव uint8_t खण्डम्[BLAKE3_खण्डम्_LEN],
                                    uint8_t खण्डम्_len, uint64_t गणक,
                                    uint8_t flags);
शून्य blake3_compress_xof_sse41(ध्रुव uint32_t cv[8],
                               ध्रुव uint8_t खण्डम्[BLAKE3_खण्डम्_LEN],
                               uint8_t खण्डम्_len, uint64_t गणक,
                               uint8_t flags, uint8_t बहिर्गमनम्[64]);
शून्य blake3_hash_many_sse41(ध्रुव uint8_t *ध्रुव *inputs, size_t अङ्कः_inputs,
                            size_t खण्डानि, ध्रुव uint32_t कुञ्चिका[8],
                            uint64_t गणक, bool increment_गणक,
                            uint8_t flags, uint8_t flags_start,
                            uint8_t flags_अन्तः, uint8_t *बहिर्गमनम्);
#यदि_अन्तः
#यदि !defined(BLAKE3_NO_AVX2)
शून्य blake3_hash_many_avx2(ध्रुव uint8_t *ध्रुव *inputs, size_t अङ्कः_inputs,
                           size_t खण्डानि, ध्रुव uint32_t कुञ्चिका[8],
                           uint64_t गणक, bool increment_गणक,
                           uint8_t flags, uint8_t flags_start,
                           uint8_t flags_अन्तः, uint8_t *बहिर्गमनम्);
#यदि_अन्तः
#यदि !defined(BLAKE3_NO_AVX512)
शून्य blake3_compress_अन्तर्_place_avx512(uint32_t cv[8],
                                     ध्रुव uint8_t खण्डम्[BLAKE3_खण्डम्_LEN],
                                     uint8_t खण्डम्_len, uint64_t गणक,
                                     uint8_t flags);

शून्य blake3_compress_xof_avx512(ध्रुव uint32_t cv[8],
                                ध्रुव uint8_t खण्डम्[BLAKE3_खण्डम्_LEN],
                                uint8_t खण्डम्_len, uint64_t गणक,
                                uint8_t flags, uint8_t बहिर्गमनम्[64]);

शून्य blake3_hash_many_avx512(ध्रुव uint8_t *ध्रुव *inputs, size_t अङ्कः_inputs,
                             size_t खण्डानि, ध्रुव uint32_t कुञ्चिका[8],
                             uint64_t गणक, bool increment_गणक,
                             uint8_t flags, uint8_t flags_start,
                             uint8_t flags_अन्तः, uint8_t *बहिर्गमनम्);
#यदि_अन्तः
#यदि_अन्तः

#यदि BLAKE3_USE_NEON == 1
शून्य blake3_hash_many_neon(ध्रुव uint8_t *ध्रुव *inputs, size_t अङ्कः_inputs,
                           size_t खण्डानि, ध्रुव uint32_t कुञ्चिका[8],
                           uint64_t गणक, bool increment_गणक,
                           uint8_t flags, uint8_t flags_start,
                           uint8_t flags_अन्तः, uint8_t *बहिर्गमनम्);
#यदि_अन्तः


#यदि_अन्तः /* BLAKE3_IMPL_H */
