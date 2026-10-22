#جيڪڏهن_وصف_نه_ٿيل BLAKE3_H
#وصف_ڪريو BLAKE3_H

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
#     define BLAKE3_ذاتي
#   وإلا
#     define BLAKE3_API
#     define BLAKE3_ذاتي
#   endif
# elif __GNUC__ >= 4
#   define BLAKE3_API __attribute__((visibility("default")))
#   define BLAKE3_ذاتي __attribute__((visibility("hidden")))
# وإلا
#   define BLAKE3_API
#   define BLAKE3_ذاتي
# endif
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل __cplusplus
خارجي "C" {
#جيڪڏهن_پورو

#وصف_ڪريو BLAKE3_نسخو_STRING "1.5.0"
#وصف_ڪريو BLAKE3_ڪنجي_LEN 32
#وصف_ڪريو BLAKE3_ٻاهريون_LEN 32
#وصف_ڪريو BLAKE3_بلاڪ_LEN 64
#وصف_ڪريو BLAKE3_CHUNK_LEN 1024
#وصف_ڪريو BLAKE3_MAX_اونهائي 54

// This struct is a private implementation detail. It has to be here because
// it's part of blake3_hasher below.
تعريفنوع بنية {
  uint32_t cv[8];
  uint64_t chunk_counter;
  uint8_t buf[BLAKE3_بلاڪ_LEN];
  uint8_t buf_len;
  uint8_t بلاڪون_compressed;
  uint8_t flags;
} blake3_chunk_state;

تعريفنوع بنية {
  uint32_t ڪنجي[8];
  blake3_chunk_state chunk;
  uint8_t cv_ڍير_len;
  // The stack size is MAX_DEPTH + 1 because we do lazy merging. For example,
  // with 7 chunks, we have 3 entries in the stack. Adding an 8th chunk
  // requires a 4th entry, rather than merging everything down to 1, because we
  // don't know whether more input is coming. This is different from how the
  // reference implementation does things.
  uint8_t cv_ڍير[(BLAKE3_MAX_اونهائي + 1) * BLAKE3_ٻاهريون_LEN];
} blake3_hasher;

BLAKE3_API ثابت حرف *blake3_نسخو(فراغ);
BLAKE3_API فراغ blake3_hasher_init(blake3_hasher *self);
BLAKE3_API فراغ blake3_hasher_init_keyed(blake3_hasher *self,
                                         ثابت uint8_t ڪنجي[BLAKE3_ڪنجي_LEN]);
BLAKE3_API فراغ blake3_hasher_init_derive_ڪنجي(blake3_hasher *self, ثابت حرف *context);
BLAKE3_API فراغ blake3_hasher_init_derive_ڪنجي_raw(blake3_hasher *self, ثابت فراغ *context,
                                                  size_t context_len);
BLAKE3_API فراغ blake3_hasher_update(blake3_hasher *self, ثابت فراغ *داخلا,
                                     size_t داخلا_len);
BLAKE3_API فراغ blake3_hasher_finalize(ثابت blake3_hasher *self, uint8_t *ٻاهريون,
                                       size_t ٻاهريون_len);
BLAKE3_API فراغ blake3_hasher_finalize_seek(ثابت blake3_hasher *self, uint64_t seek,
                                            uint8_t *ٻاهريون, size_t ٻاهريون_len);
BLAKE3_API فراغ blake3_hasher_reset(blake3_hasher *self);

#جيڪڏهن_وصف_ٿيل __cplusplus
}
#جيڪڏهن_پورو

#جيڪڏهن_پورو /* BLAKE3_H */
