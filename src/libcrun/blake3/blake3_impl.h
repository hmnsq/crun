#اگر_غیر_تعریف_شدہ BLAKE3_IMPL_H
#تعریف_کرو BLAKE3_IMPL_H

#شامل <assert.h>
#شامل <stdbool.h>
#شامل <stddef.h>
#شامل <stdint.h>
#شامل <string.h>

#شامل "blake3.h"

/* libcrun specific code.  Always force the portable implementation as it is fast enough for us.   */
#تعریف_کرو blake3_compress_اندر_رکھو_portable blake3_compress_اندر_رکھو
#تعریف_کرو blake3_compress_xof_portable blake3_compress_xof
#تعریف_کرو blake3_simd_degree_portable blake3_simd_degree
#تعریف_کرو blake3_compress_subtree_wide_portable blake3_compress_subtree_wide
#تعریف_کرو blake3_hash_many_portable blake3_hash_many
ساکن اندرونی size_t blake3_simd_degree(خالی) {
  واپس 1;
}

// internal flags
شمار blake3_flags {
  CHUNK_آغاز         = 1 << 0,
  CHUNK_انتہا           = 1 << 1,
  والد              = 1 << 2,
  جڑ_2                = 1 << 3,
  KEYED_HASH          = 1 << 4,
  DERIVE_کلید_CONTEXT  = 1 << 5,
  DERIVE_کلید_MATERIAL = 1 << 6,
};

// This C implementation tries to support recent versions of GCC, Clang, and
// MSVC.
#اگر defined(_MSC_VER)
#تعریف_کرو INLINE ساکن __forceinline
#ورنہ
#تعریف_کرو INLINE ساکن اندرونی __attribute__((always_inline))
#اگر_ختم

#اگر defined(__x86_64__) || defined(_M_X64) 
#تعریف_کرو IS_X86
#تعریف_کرو IS_X86_64
#اگر_ختم

#اگر defined(__i386__) || defined(_M_IX86)
#تعریف_کرو IS_X86
#تعریف_کرو IS_X86_32
#اگر_ختم

#اگر defined(__aarch64__) || defined(_M_ARM64)
#تعریف_کرو IS_AARCH64
#اگر_ختم

#اگر defined(IS_X86)
#اگر defined(_MSC_VER)
#شامل <intrin.h>
#اگر_ختم
#اگر_ختم

#اگر !defined(BLAKE3_USE_NEON) 
  // If BLAKE3_USE_NEON not manually set, autodetect based on AArch64ness
  #اگر defined(IS_AARCH64)
    #اگر defined(__ARM_BIG_ENDIAN)
      #تعریف_کرو BLAKE3_USE_NEON 0
    #ورنہ
      #تعریف_کرو BLAKE3_USE_NEON 1
    #اگر_ختم
  #ورنہ
    #تعریف_کرو BLAKE3_USE_NEON 0
  #اگر_ختم
#اگر_ختم

#اگر defined(IS_X86)
#تعریف_کرو MAX_SIMD_DEGREE 16
#وگرنہ_اگر BLAKE3_USE_NEON == 1
#تعریف_کرو MAX_SIMD_DEGREE 4
#ورنہ
#تعریف_کرو MAX_SIMD_DEGREE 1
#اگر_ختم

// There are some places where we want a static size that's equal to the
// MAX_SIMD_DEGREE, but also at least 2.
#تعریف_کرو MAX_SIMD_DEGREE_OR_2 (MAX_SIMD_DEGREE > 2 ? MAX_SIMD_DEGREE : 2)

ساکن ثابت uint32_t IV[8] = {0x6A09E667UL, 0xBB67AE85UL, 0x3C6EF372UL,
                               0xA54FF53AUL, 0x510E527FUL, 0x9B05688CUL,
                               0x1F83D9ABUL, 0x5BE0CD19UL};

ساکن ثابت uint8_t پیغام_جدول_وقت[7][16] = {
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
ساکن غیرعلامتی صحیح highest_one(uint64_t x) {
#اگر defined(__GNUC__) || defined(__clang__)
  واپس 63 ^ (غیرعلامتی صحیح)__builtin_clzll(x);
#وگرنہ_اگر defined(_MSC_VER) && defined(IS_X86_64)
  غیرعلامتی طویل اشاریہ;
  ثنائی_ہندسہ_جائزہ_Reverse64(&اشاریہ, x);
  واپس اشاریہ;
#وگرنہ_اگر defined(_MSC_VER) && defined(IS_X86_32)
  اگر(x >> 32) {
    غیرعلامتی طویل اشاریہ;
    ثنائی_ہندسہ_جائزہ_الٹ(&اشاریہ, (غیرعلامتی طویل)(x >> 32));
    واپس 32 + اشاریہ;
  } ورنہ {
    غیرعلامتی طویل اشاریہ;
    ثنائی_ہندسہ_جائزہ_الٹ(&اشاریہ, (غیرعلامتی طویل)x);
    واپس اشاریہ;
  }
#ورنہ
  غیرعلامتی صحیح c = 0;
  اگر(x & 0xffffffff00000000ULL) { x >>= 32; c += 32; }
  اگر(x & 0x00000000ffff0000ULL) { x >>= 16; c += 16; }
  اگر(x & 0x000000000000ff00ULL) { x >>=  8; c +=  8; }
  اگر(x & 0x00000000000000f0ULL) { x >>=  4; c +=  4; }
  اگر(x & 0x000000000000000cULL) { x >>=  2; c +=  2; }
  اگر(x & 0x0000000000000002ULL) {           c +=  1; }
  واپس c;
#اگر_ختم
}

// Count the number of 1 bits.
INLINE غیرعلامتی صحیح popcnt(uint64_t x) {
#اگر defined(__GNUC__) || defined(__clang__)
  واپس (غیرعلامتی صحیح)__builtin_popcountll(x);
#ورنہ
  غیرعلامتی صحیح گنتی = 0;
  جب (x != 0) {
    گنتی += 1;
    x &= x - 1;
  }
  واپس گنتی;
#اگر_ختم
}

// Largest power of two less than or equal to x. As a special case, returns 1
// when x is 0. 
INLINE uint64_t باری_نیچے_کو_مقصد_power_کا_2(uint64_t x) {
  واپس 1ULL << highest_one(x | 1);
}

INLINE uint32_t شمار_کم(uint64_t شمار_2) { واپس (uint32_t)شمار_2; }

INLINE uint32_t شمار_بلند(uint64_t شمار_2) {
  واپس (uint32_t)(شمار_2 >> 32);
}

INLINE uint32_t load32(ثابت خالی *src) {
  ثابت uint8_t *p = (ثابت uint8_t *)src;
  واپس ((uint32_t)(p[0]) << 0) | ((uint32_t)(p[1]) << 8) |
         ((uint32_t)(p[2]) << 16) | ((uint32_t)(p[3]) << 24);
}

INLINE خالی بھرو_کلید_الفاظ(ثابت uint8_t کلید[BLAKE3_کلید_LEN],
                           uint32_t کلید_الفاظ[8]) {
  کلید_الفاظ[0] = load32(&کلید[0 * 4]);
  کلید_الفاظ[1] = load32(&کلید[1 * 4]);
  کلید_الفاظ[2] = load32(&کلید[2 * 4]);
  کلید_الفاظ[3] = load32(&کلید[3 * 4]);
  کلید_الفاظ[4] = load32(&کلید[4 * 4]);
  کلید_الفاظ[5] = load32(&کلید[5 * 4]);
  کلید_الفاظ[6] = load32(&کلید[6 * 4]);
  کلید_الفاظ[7] = load32(&کلید[7 * 4]);
}

INLINE خالی store32(خالی *dst, uint32_t w) {
  uint8_t *p = (uint8_t *)dst;
  p[0] = (uint8_t)(w >> 0);
  p[1] = (uint8_t)(w >> 8);
  p[2] = (uint8_t)(w >> 16);
  p[3] = (uint8_t)(w >> 24);
}

INLINE خالی store_cv_الفاظ(uint8_t بائٹیں_برآمد[32], uint32_t cv_الفاظ[8]) {
  store32(&بائٹیں_برآمد[0 * 4], cv_الفاظ[0]);
  store32(&بائٹیں_برآمد[1 * 4], cv_الفاظ[1]);
  store32(&بائٹیں_برآمد[2 * 4], cv_الفاظ[2]);
  store32(&بائٹیں_برآمد[3 * 4], cv_الفاظ[3]);
  store32(&بائٹیں_برآمد[4 * 4], cv_الفاظ[4]);
  store32(&بائٹیں_برآمد[5 * 4], cv_الفاظ[5]);
  store32(&بائٹیں_برآمد[6 * 4], cv_الفاظ[6]);
  store32(&بائٹیں_برآمد[7 * 4], cv_الفاظ[7]);
}

خالی blake3_compress_اندر_رکھو(uint32_t cv[8],
                              ثابت uint8_t بلاک[BLAKE3_بلاک_LEN],
                              uint8_t بلاک_len, uint64_t شمار_2,
                              uint8_t flags);

خالی blake3_compress_xof(ثابت uint32_t cv[8],
                         ثابت uint8_t بلاک[BLAKE3_بلاک_LEN],
                         uint8_t بلاک_len, uint64_t شمار_2, uint8_t flags,
                         uint8_t برآمد[64]);

خالی blake3_hash_many(ثابت uint8_t *ثابت *inputs, size_t عدد_inputs,
                      size_t بلاکیں, ثابت uint32_t کلید[8], uint64_t شمار_2,
                      bool increment_شمار, uint8_t flags,
                      uint8_t flags_آغاز, uint8_t flags_انتہا, uint8_t *برآمد);

size_t blake3_simd_degree(خالی);


// Declarations for implementation-specific functions.
خالی blake3_compress_اندر_رکھو_portable(uint32_t cv[8],
                                       ثابت uint8_t بلاک[BLAKE3_بلاک_LEN],
                                       uint8_t بلاک_len, uint64_t شمار_2,
                                       uint8_t flags);

خالی blake3_compress_xof_portable(ثابت uint32_t cv[8],
                                  ثابت uint8_t بلاک[BLAKE3_بلاک_LEN],
                                  uint8_t بلاک_len, uint64_t شمار_2,
                                  uint8_t flags, uint8_t برآمد[64]);

خالی blake3_hash_many_portable(ثابت uint8_t *ثابت *inputs, size_t عدد_inputs,
                               size_t بلاکیں, ثابت uint32_t کلید[8],
                               uint64_t شمار_2, bool increment_شمار,
                               uint8_t flags, uint8_t flags_آغاز,
                               uint8_t flags_انتہا, uint8_t *برآمد);

#اگر defined(IS_X86)
#اگر !defined(BLAKE3_NO_SSE2)
خالی blake3_compress_اندر_رکھو_sse2(uint32_t cv[8],
                                   ثابت uint8_t بلاک[BLAKE3_بلاک_LEN],
                                   uint8_t بلاک_len, uint64_t شمار_2,
                                   uint8_t flags);
خالی blake3_compress_xof_sse2(ثابت uint32_t cv[8],
                              ثابت uint8_t بلاک[BLAKE3_بلاک_LEN],
                              uint8_t بلاک_len, uint64_t شمار_2,
                              uint8_t flags, uint8_t برآمد[64]);
خالی blake3_hash_many_sse2(ثابت uint8_t *ثابت *inputs, size_t عدد_inputs,
                           size_t بلاکیں, ثابت uint32_t کلید[8],
                           uint64_t شمار_2, bool increment_شمار,
                           uint8_t flags, uint8_t flags_آغاز,
                           uint8_t flags_انتہا, uint8_t *برآمد);
#اگر_ختم
#اگر !defined(BLAKE3_NO_SSE41)
خالی blake3_compress_اندر_رکھو_sse41(uint32_t cv[8],
                                    ثابت uint8_t بلاک[BLAKE3_بلاک_LEN],
                                    uint8_t بلاک_len, uint64_t شمار_2,
                                    uint8_t flags);
خالی blake3_compress_xof_sse41(ثابت uint32_t cv[8],
                               ثابت uint8_t بلاک[BLAKE3_بلاک_LEN],
                               uint8_t بلاک_len, uint64_t شمار_2,
                               uint8_t flags, uint8_t برآمد[64]);
خالی blake3_hash_many_sse41(ثابت uint8_t *ثابت *inputs, size_t عدد_inputs,
                            size_t بلاکیں, ثابت uint32_t کلید[8],
                            uint64_t شمار_2, bool increment_شمار,
                            uint8_t flags, uint8_t flags_آغاز,
                            uint8_t flags_انتہا, uint8_t *برآمد);
#اگر_ختم
#اگر !defined(BLAKE3_NO_AVX2)
خالی blake3_hash_many_avx2(ثابت uint8_t *ثابت *inputs, size_t عدد_inputs,
                           size_t بلاکیں, ثابت uint32_t کلید[8],
                           uint64_t شمار_2, bool increment_شمار,
                           uint8_t flags, uint8_t flags_آغاز,
                           uint8_t flags_انتہا, uint8_t *برآمد);
#اگر_ختم
#اگر !defined(BLAKE3_NO_AVX512)
خالی blake3_compress_اندر_رکھو_avx512(uint32_t cv[8],
                                     ثابت uint8_t بلاک[BLAKE3_بلاک_LEN],
                                     uint8_t بلاک_len, uint64_t شمار_2,
                                     uint8_t flags);

خالی blake3_compress_xof_avx512(ثابت uint32_t cv[8],
                                ثابت uint8_t بلاک[BLAKE3_بلاک_LEN],
                                uint8_t بلاک_len, uint64_t شمار_2,
                                uint8_t flags, uint8_t برآمد[64]);

خالی blake3_hash_many_avx512(ثابت uint8_t *ثابت *inputs, size_t عدد_inputs,
                             size_t بلاکیں, ثابت uint32_t کلید[8],
                             uint64_t شمار_2, bool increment_شمار,
                             uint8_t flags, uint8_t flags_آغاز,
                             uint8_t flags_انتہا, uint8_t *برآمد);
#اگر_ختم
#اگر_ختم

#اگر BLAKE3_USE_NEON == 1
خالی blake3_hash_many_neon(ثابت uint8_t *ثابت *inputs, size_t عدد_inputs,
                           size_t بلاکیں, ثابت uint32_t کلید[8],
                           uint64_t شمار_2, bool increment_شمار,
                           uint8_t flags, uint8_t flags_آغاز,
                           uint8_t flags_انتہا, uint8_t *برآمد);
#اگر_ختم


#اگر_ختم /* BLAKE3_IMPL_H */
