#నిర్వచితం_కాకపోతే BLAKE3_IMPL_H
#నిర్వచించు BLAKE3_IMPL_H

#అన్తర్భావ <assert.h>
#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <stddef.h>
#అన్తర్భావ <stdint.h>
#అన్తర్భావ <string.h>

#అన్తర్భావ "blake3.h"

/* libcrun specific code.  Always force the portable implementation as it is fast enough for us.   */
#నిర్వచించు blake3_compress_లోపల_ఉంచు_portable blake3_compress_లోపల_ఉంచు
#నిర్వచించు blake3_compress_xof_portable blake3_compress_xof
#నిర్వచించు blake3_simd_degree_portable blake3_simd_degree
#నిర్వచించు blake3_compress_subtree_wide_portable blake3_compress_subtree_wide
#నిర్వచించు blake3_hash_many_portable blake3_hash_many
స్థిర అంతఃవరుస size_t blake3_simd_degree(శూన్య) {
  తిరిగి 1;
}

// internal flags
గణన blake3_flags {
  CHUNK_ప్రారంభం         = 1 << 0,
  CHUNK_చివర           = 1 << 1,
  మాతృ              = 1 << 2,
  మూలం_2                = 1 << 3,
  KEYED_HASH          = 1 << 4,
  DERIVE_కీలకం_CONTEXT  = 1 << 5,
  DERIVE_కీలకం_MATERIAL = 1 << 6,
};

// This C implementation tries to support recent versions of GCC, Clang, and
// MSVC.
#ఒకవేళ defined(_MSC_VER)
#నిర్వచించు INLINE స్థిర __forceinline
#లేకపోతే
#నిర్వచించు INLINE స్థిర అంతఃవరుస __attribute__((always_inline))
#అయితే_ముగింపు

#ఒకవేళ defined(__x86_64__) || defined(_M_X64) 
#నిర్వచించు IS_X86
#నిర్వచించు IS_X86_64
#అయితే_ముగింపు

#ఒకవేళ defined(__i386__) || defined(_M_IX86)
#నిర్వచించు IS_X86
#నిర్వచించు IS_X86_32
#అయితే_ముగింపు

#ఒకవేళ defined(__aarch64__) || defined(_M_ARM64)
#నిర్వచించు IS_AARCH64
#అయితే_ముగింపు

#ఒకవేళ defined(IS_X86)
#ఒకవేళ defined(_MSC_VER)
#అన్తర్భావ <intrin.h>
#అయితే_ముగింపు
#అయితే_ముగింపు

#ఒకవేళ !defined(BLAKE3_USE_NEON) 
  // If BLAKE3_USE_NEON not manually set, autodetect based on AArch64ness
  #ఒకవేళ defined(IS_AARCH64)
    #ఒకవేళ defined(__ARM_BIG_ENDIAN)
      #నిర్వచించు BLAKE3_USE_NEON 0
    #లేకపోతే
      #నిర్వచించు BLAKE3_USE_NEON 1
    #అయితే_ముగింపు
  #లేకపోతే
    #నిర్వచించు BLAKE3_USE_NEON 0
  #అయితే_ముగింపు
#అయితే_ముగింపు

#ఒకవేళ defined(IS_X86)
#నిర్వచించు MAX_SIMD_DEGREE 16
#లేదావేళ BLAKE3_USE_NEON == 1
#నిర్వచించు MAX_SIMD_DEGREE 4
#లేకపోతే
#నిర్వచించు MAX_SIMD_DEGREE 1
#అయితే_ముగింపు

// There are some places where we want a static size that's equal to the
// MAX_SIMD_DEGREE, but also at least 2.
#నిర్వచించు MAX_SIMD_DEGREE_OR_2 (MAX_SIMD_DEGREE > 2 ? MAX_SIMD_DEGREE : 2)

స్థిర స్థిరరాశి uint32_t IV[8] = {0x6A09E667UL, 0xBB67AE85UL, 0x3C6EF372UL,
                               0xA54FF53AUL, 0x510E527FUL, 0x9B05688CUL,
                               0x1F83D9ABUL, 0x5BE0CD19UL};

స్థిర స్థిరరాశి uint8_t సందేశం_కాలపట్టిక[7][16] = {
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
స్థిర గుర్తులేని పూర్ణాంకం highest_one(uint64_t x) {
#ఒకవేళ defined(__GNUC__) || defined(__clang__)
  తిరిగి 63 ^ (గుర్తులేని పూర్ణాంకం)__builtin_clzll(x);
#లేదావేళ defined(_MSC_VER) && defined(IS_X86_64)
  గుర్తులేని దీర్ఘ సూచీ;
  బిట్టు_పరిశీలన_Reverse64(&సూచీ, x);
  తిరిగి సూచీ;
#లేదావేళ defined(_MSC_VER) && defined(IS_X86_32)
  ఒకవేళ(x >> 32) {
    గుర్తులేని దీర్ఘ సూచీ;
    బిట్టు_పరిశీలన_తిరగేయి(&సూచీ, (గుర్తులేని దీర్ఘ)(x >> 32));
    తిరిగి 32 + సూచీ;
  } లేకపోతే {
    గుర్తులేని దీర్ఘ సూచీ;
    బిట్టు_పరిశీలన_తిరగేయి(&సూచీ, (గుర్తులేని దీర్ఘ)x);
    తిరిగి సూచీ;
  }
#లేకపోతే
  గుర్తులేని పూర్ణాంకం c = 0;
  ఒకవేళ(x & 0xffffffff00000000ULL) { x >>= 32; c += 32; }
  ఒకవేళ(x & 0x00000000ffff0000ULL) { x >>= 16; c += 16; }
  ఒకవేళ(x & 0x000000000000ff00ULL) { x >>=  8; c +=  8; }
  ఒకవేళ(x & 0x00000000000000f0ULL) { x >>=  4; c +=  4; }
  ఒకవేళ(x & 0x000000000000000cULL) { x >>=  2; c +=  2; }
  ఒకవేళ(x & 0x0000000000000002ULL) {           c +=  1; }
  తిరిగి c;
#అయితే_ముగింపు
}

// Count the number of 1 bits.
INLINE గుర్తులేని పూర్ణాంకం popcnt(uint64_t x) {
#ఒకవేళ defined(__GNUC__) || defined(__clang__)
  తిరిగి (గుర్తులేని పూర్ణాంకం)__builtin_popcountll(x);
#లేకపోతే
  గుర్తులేని పూర్ణాంకం లెక్క = 0;
  అయితే (x != 0) {
    లెక్క += 1;
    x &= x - 1;
  }
  తిరిగి లెక్క;
#అయితే_ముగింపు
}

// Largest power of two less than or equal to x. As a special case, returns 1
// when x is 0. 
INLINE uint64_t విడత_కిందికి_గమ్యం_power_యొక్క_2(uint64_t x) {
  తిరిగి 1ULL << highest_one(x | 1);
}

INLINE uint32_t counter_దిగువ(uint64_t counter) { తిరిగి (uint32_t)counter; }

INLINE uint32_t counter_ఎగువ(uint64_t counter) {
  తిరిగి (uint32_t)(counter >> 32);
}

INLINE uint32_t load32(స్థిరరాశి శూన్య *src) {
  స్థిరరాశి uint8_t *p = (స్థిరరాశి uint8_t *)src;
  తిరిగి ((uint32_t)(p[0]) << 0) | ((uint32_t)(p[1]) << 8) |
         ((uint32_t)(p[2]) << 16) | ((uint32_t)(p[3]) << 24);
}

INLINE శూన్య ఎక్కించు_కీలకం_పదాలు(స్థిరరాశి uint8_t కీలకం[BLAKE3_కీలకం_LEN],
                           uint32_t కీలకం_పదాలు[8]) {
  కీలకం_పదాలు[0] = load32(&కీలకం[0 * 4]);
  కీలకం_పదాలు[1] = load32(&కీలకం[1 * 4]);
  కీలకం_పదాలు[2] = load32(&కీలకం[2 * 4]);
  కీలకం_పదాలు[3] = load32(&కీలకం[3 * 4]);
  కీలకం_పదాలు[4] = load32(&కీలకం[4 * 4]);
  కీలకం_పదాలు[5] = load32(&కీలకం[5 * 4]);
  కీలకం_పదాలు[6] = load32(&కీలకం[6 * 4]);
  కీలకం_పదాలు[7] = load32(&కీలకం[7 * 4]);
}

INLINE శూన్య store32(శూన్య *dst, uint32_t w) {
  uint8_t *p = (uint8_t *)dst;
  p[0] = (uint8_t)(w >> 0);
  p[1] = (uint8_t)(w >> 8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
}

INLINE శూన్య store_cv_పదాలు(uint8_t బైట్లు_బహిర్గతం[32], uint32_t cv_పదాలు[8]) {
  store32(&బైట్లు_బహిర్గతం[0 * 4], cv_పదాలు[0]);
  store32(&బైట్లు_బహిర్గతం[1 * 4], cv_పదాలు[1]);
  store32(&బైట్లు_బహిర్గతం[2 * 4], cv_పదాలు[2]);
  store32(&బైట్లు_బహిర్గతం[3 * 4], cv_పదాలు[3]);
  store32(&బైట్లు_బహిర్గతం[4 * 4], cv_పదాలు[4]);
  store32(&బైట్లు_బహిర్గతం[5 * 4], cv_పదాలు[5]);
  store32(&బైట్లు_బహిర్గతం[6 * 4], cv_పదాలు[6]);
  store32(&బైట్లు_బహిర్గతం[7 * 4], cv_పదాలు[7]);
}

శూన్య blake3_compress_లోపల_ఉంచు(uint32_t cv[8],
                              స్థిరరాశి uint8_t ఖండం[BLAKE3_ఖండం_LEN],
                              uint8_t ఖండం_len, uint64_t counter,
                              uint8_t flags);

శూన్య blake3_compress_xof(స్థిరరాశి uint32_t cv[8],
                         స్థిరరాశి uint8_t ఖండం[BLAKE3_ఖండం_LEN],
                         uint8_t ఖండం_len, uint64_t counter, uint8_t flags,
                         uint8_t బహిర్గతం[64]);

శూన్య blake3_hash_many(స్థిరరాశి uint8_t *స్థిరరాశి *inputs, size_t అంకె_inputs,
                      size_t ఖండాలు, స్థిరరాశి uint32_t కీలకం[8], uint64_t counter,
                      bool increment_counter, uint8_t flags,
                      uint8_t flags_ప్రారంభం, uint8_t flags_చివర, uint8_t *బహిర్గతం);

size_t blake3_simd_degree(శూన్య);


// Declarations for implementation-specific functions.
శూన్య blake3_compress_లోపల_ఉంచు_portable(uint32_t cv[8],
                                       స్థిరరాశి uint8_t ఖండం[BLAKE3_ఖండం_LEN],
                                       uint8_t ఖండం_len, uint64_t counter,
                                       uint8_t flags);

శూన్య blake3_compress_xof_portable(స్థిరరాశి uint32_t cv[8],
                                  స్థిరరాశి uint8_t ఖండం[BLAKE3_ఖండం_LEN],
                                  uint8_t ఖండం_len, uint64_t counter,
                                  uint8_t flags, uint8_t బహిర్గతం[64]);

శూన్య blake3_hash_many_portable(స్థిరరాశి uint8_t *స్థిరరాశి *inputs, size_t అంకె_inputs,
                               size_t ఖండాలు, స్థిరరాశి uint32_t కీలకం[8],
                               uint64_t counter, bool increment_counter,
                               uint8_t flags, uint8_t flags_ప్రారంభం,
                               uint8_t flags_చివర, uint8_t *బహిర్గతం);

#ఒకవేళ defined(IS_X86)
#ఒకవేళ !defined(BLAKE3_NO_SSE2)
శూన్య blake3_compress_లోపల_ఉంచు_sse2(uint32_t cv[8],
                                   స్థిరరాశి uint8_t ఖండం[BLAKE3_ఖండం_LEN],
                                   uint8_t ఖండం_len, uint64_t counter,
                                   uint8_t flags);
శూన్య blake3_compress_xof_sse2(స్థిరరాశి uint32_t cv[8],
                              స్థిరరాశి uint8_t ఖండం[BLAKE3_ఖండం_LEN],
                              uint8_t ఖండం_len, uint64_t counter,
                              uint8_t flags, uint8_t బహిర్గతం[64]);
శూన్య blake3_hash_many_sse2(స్థిరరాశి uint8_t *స్థిరరాశి *inputs, size_t అంకె_inputs,
                           size_t ఖండాలు, స్థిరరాశి uint32_t కీలకం[8],
                           uint64_t counter, bool increment_counter,
                           uint8_t flags, uint8_t flags_ప్రారంభం,
                           uint8_t flags_చివర, uint8_t *బహిర్గతం);
#అయితే_ముగింపు
#ఒకవేళ !defined(BLAKE3_NO_SSE41)
శూన్య blake3_compress_లోపల_ఉంచు_sse41(uint32_t cv[8],
                                    స్థిరరాశి uint8_t ఖండం[BLAKE3_ఖండం_LEN],
                                    uint8_t ఖండం_len, uint64_t counter,
                                    uint8_t flags);
శూన్య blake3_compress_xof_sse41(స్థిరరాశి uint32_t cv[8],
                               స్థిరరాశి uint8_t ఖండం[BLAKE3_ఖండం_LEN],
                               uint8_t ఖండం_len, uint64_t counter,
                               uint8_t flags, uint8_t బహిర్గతం[64]);
శూన్య blake3_hash_many_sse41(స్థిరరాశి uint8_t *స్థిరరాశి *inputs, size_t అంకె_inputs,
                            size_t ఖండాలు, స్థిరరాశి uint32_t కీలకం[8],
                            uint64_t counter, bool increment_counter,
                            uint8_t flags, uint8_t flags_ప్రారంభం,
                            uint8_t flags_చివర, uint8_t *బహిర్గతం);
#అయితే_ముగింపు
#ఒకవేళ !defined(BLAKE3_NO_AVX2)
శూన్య blake3_hash_many_avx2(స్థిరరాశి uint8_t *స్థిరరాశి *inputs, size_t అంకె_inputs,
                           size_t ఖండాలు, స్థిరరాశి uint32_t కీలకం[8],
                           uint64_t counter, bool increment_counter,
                           uint8_t flags, uint8_t flags_ప్రారంభం,
                           uint8_t flags_చివర, uint8_t *బహిర్గతం);
#అయితే_ముగింపు
#ఒకవేళ !defined(BLAKE3_NO_AVX512)
శూన్య blake3_compress_లోపల_ఉంచు_avx512(uint32_t cv[8],
                                     స్థిరరాశి uint8_t ఖండం[BLAKE3_ఖండం_LEN],
                                     uint8_t ఖండం_len, uint64_t counter,
                                     uint8_t flags);

శూన్య blake3_compress_xof_avx512(స్థిరరాశి uint32_t cv[8],
                                స్థిరరాశి uint8_t ఖండం[BLAKE3_ఖండం_LEN],
                                uint8_t ఖండం_len, uint64_t counter,
                                uint8_t flags, uint8_t బహిర్గతం[64]);

శూన్య blake3_hash_many_avx512(స్థిరరాశి uint8_t *స్థిరరాశి *inputs, size_t అంకె_inputs,
                             size_t ఖండాలు, స్థిరరాశి uint32_t కీలకం[8],
                             uint64_t counter, bool increment_counter,
                             uint8_t flags, uint8_t flags_ప్రారంభం,
                             uint8_t flags_చివర, uint8_t *బహిర్గతం);
#అయితే_ముగింపు
#అయితే_ముగింపు

#ఒకవేళ BLAKE3_USE_NEON == 1
శూన్య blake3_hash_many_neon(స్థిరరాశి uint8_t *స్థిరరాశి *inputs, size_t అంకె_inputs,
                           size_t ఖండాలు, స్థిరరాశి uint32_t కీలకం[8],
                           uint64_t counter, bool increment_counter,
                           uint8_t flags, uint8_t flags_ప్రారంభం,
                           uint8_t flags_చివర, uint8_t *బహిర్గతం);
#అయితే_ముగింపు


#అయితే_ముగింపు /* BLAKE3_IMPL_H */
