#که_نه_تعریف_شوی BLAKE3_H
#تعریف_کړه BLAKE3_H

#ضمن <stddef.h>
#ضمن <stdint.h>

#إذا !defined(BLAKE3_API)
# إذا defined(_WIN32) || defined(__CYGWIN__)
#   إذا defined(BLAKE3_DLL)
#     إذا defined(BLAKE3_DLL_EXPORTS)
#       define BLAKE3_API __declspec(dllexport)
#     وإلا
#       define BLAKE3_API __declspec(dllimport)
#     endif
#     define BLAKE3_شخصي
#   وإلا
#     define BLAKE3_API
#     define BLAKE3_شخصي
#   endif
# elif __GNUC__ >= 4
#   define BLAKE3_API __attribute__((visibility("default")))
#   define BLAKE3_شخصي __attribute__((visibility("hidden")))
# وإلا
#   define BLAKE3_API
#   define BLAKE3_شخصي
# endif
#د_که_پای

#که_تعریف_شوی __cplusplus
خارجي "C" {
#د_که_پای

#تعریف_کړه BLAKE3_نسخه_STRING "1.5.0"
#تعریف_کړه BLAKE3_کلی_LEN 32
#تعریف_کړه BLAKE3_بهر_LEN 32
#تعریف_کړه BLAKE3_بلاک_LEN 64
#تعریف_کړه BLAKE3_CHUNK_LEN 1024
#تعریف_کړه BLAKE3_MAX_ژوروالی 54

// This struct is a private implementation detail. It has to be here because
// it's part of blake3_hasher below.
تعريفنوع بنية {
  uint32_t cv[8];
  uint64_t chunk_counter;
  uint8_t buf[BLAKE3_بلاک_LEN];
  uint8_t buf_len;
  uint8_t بلاکونه_compressed;
  uint8_t flags;
} blake3_chunk_state;

تعريفنوع بنية {
  uint32_t کلی[8];
  blake3_chunk_state chunk;
  uint8_t cv_ډېری_len;
  // The stack size is MAX_DEPTH + 1 because we do lazy merging. For example,
  // with 7 chunks, we have 3 entries in the stack. Adding an 8th chunk
  // requires a 4th entry, rather than merging everything down to 1, because we
  // don't know whether more input is coming. This is different from how the
  // reference implementation does things.
  uint8_t cv_ډېری[(BLAKE3_MAX_ژوروالی + 1) * BLAKE3_بهر_LEN];
} blake3_hasher;

BLAKE3_API ثابت حرف *blake3_نسخه(فراغ);
BLAKE3_API فراغ blake3_hasher_init(blake3_hasher *self);
BLAKE3_API فراغ blake3_hasher_init_keyed(blake3_hasher *self,
                                         ثابت uint8_t کلی[BLAKE3_کلی_LEN]);
BLAKE3_API فراغ blake3_hasher_init_derive_کلی(blake3_hasher *self, ثابت حرف *context);
BLAKE3_API فراغ blake3_hasher_init_derive_کلی_raw(blake3_hasher *self, ثابت فراغ *context,
                                                  size_t context_len);
BLAKE3_API فراغ blake3_hasher_update(blake3_hasher *self, ثابت فراغ *ننوت,
                                     size_t ننوت_len);
BLAKE3_API فراغ blake3_hasher_finalize(ثابت blake3_hasher *self, uint8_t *بهر,
                                       size_t بهر_len);
BLAKE3_API فراغ blake3_hasher_finalize_seek(ثابت blake3_hasher *self, uint64_t seek,
                                            uint8_t *بهر, size_t بهر_len);
BLAKE3_API فراغ blake3_hasher_reset(blake3_hasher *self);

#که_تعریف_شوی __cplusplus
}
#د_که_پای

#د_که_پای /* BLAKE3_H */
