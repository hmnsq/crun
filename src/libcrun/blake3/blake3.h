#اگر_تعریف_نشده BLAKE3_H
#تعریف_کن BLAKE3_H

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
#     define BLAKE3_خصوصی
#   وإلا
#     define BLAKE3_API
#     define BLAKE3_خصوصی
#   endif
# elif __GNUC__ >= 4
#   define BLAKE3_API __attribute__((visibility("default")))
#   define BLAKE3_خصوصی __attribute__((visibility("hidden")))
# وإلا
#   define BLAKE3_API
#   define BLAKE3_خصوصی
# endif
#پایان_اگر

#اگر_تعریف_شده __cplusplus
خارجي "C" {
#پایان_اگر

#تعریف_کن BLAKE3_نسخه_STRING "1.5.0"
#تعریف_کن BLAKE3_کلید_LEN 32
#تعریف_کن BLAKE3_بیرون_LEN 32
#تعریف_کن BLAKE3_بلوک_LEN 64
#تعریف_کن BLAKE3_CHUNK_LEN 1024
#تعریف_کن BLAKE3_MAX_عمق 54

// This struct is a private implementation detail. It has to be here because
// it's part of blake3_hasher below.
تعريفنوع بنية {
  uint32_t cv[8];
  uint64_t chunk_counter;
  uint8_t buf[BLAKE3_بلوک_LEN];
  uint8_t buf_len;
  uint8_t بلوکها_compressed;
  uint8_t flags;
} blake3_chunk_state;

تعريفنوع بنية {
  uint32_t کلید[8];
  blake3_chunk_state chunk;
  uint8_t cv_پشته_len;
  // The stack size is MAX_DEPTH + 1 because we do lazy merging. For example,
  // with 7 chunks, we have 3 entries in the stack. Adding an 8th chunk
  // requires a 4th entry, rather than merging everything down to 1, because we
  // don't know whether more input is coming. This is different from how the
  // reference implementation does things.
  uint8_t cv_پشته[(BLAKE3_MAX_عمق + 1) * BLAKE3_بیرون_LEN];
} blake3_hasher;

BLAKE3_API ثابت حرف *blake3_نسخه(فراغ);
BLAKE3_API فراغ blake3_hasher_init(blake3_hasher *self);
BLAKE3_API فراغ blake3_hasher_init_keyed(blake3_hasher *self,
                                         ثابت uint8_t کلید[BLAKE3_کلید_LEN]);
BLAKE3_API فراغ blake3_hasher_init_derive_کلید(blake3_hasher *self, ثابت حرف *context);
BLAKE3_API فراغ blake3_hasher_init_derive_کلید_raw(blake3_hasher *self, ثابت فراغ *context,
                                                  size_t context_len);
BLAKE3_API فراغ blake3_hasher_update(blake3_hasher *self, ثابت فراغ *درون_داد,
                                     size_t درون_داد_len);
BLAKE3_API فراغ blake3_hasher_finalize(ثابت blake3_hasher *self, uint8_t *بیرون,
                                       size_t بیرون_len);
BLAKE3_API فراغ blake3_hasher_finalize_seek(ثابت blake3_hasher *self, uint64_t seek,
                                            uint8_t *بیرون, size_t بیرون_len);
BLAKE3_API فراغ blake3_hasher_reset(blake3_hasher *self);

#اگر_تعریف_شده __cplusplus
}
#پایان_اگر

#پایان_اگر /* BLAKE3_H */
