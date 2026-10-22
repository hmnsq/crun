#ifndef BLAKE3_H
#define BLAKE3_H

#شامل <stddef.h>
#شامل <stdint.h>

#اگر !defined(BLAKE3_API)
# اگر defined(_WIN32) || defined(__CYGWIN__)
#   اگر defined(BLAKE3_DLL)
#     اگر defined(BLAKE3_DLL_EXPORTS)
#       define BLAKE3_API __declspec(dllexport)
#     وگرنه
#       define BLAKE3_API __declspec(dllimport)
#     endif
#     define BLAKE3_PRIVATE
#   وگرنه
#     define BLAKE3_API
#     define BLAKE3_PRIVATE
#   endif
# elif __GNUC__ >= 4
#   define BLAKE3_API __attribute__((visibility("default")))
#   define BLAKE3_PRIVATE __attribute__((visibility("hidden")))
# وگرنه
#   define BLAKE3_API
#   define BLAKE3_PRIVATE
# endif
#endif

#ifdef __cplusplus
بیرونی "C" {
#endif

#define BLAKE3_VERSION_STRING "1.5.0"
#define BLAKE3_کلید_LEN 32
#define BLAKE3_OUT_LEN 32
#define BLAKE3_BLOCK_LEN 64
#define BLAKE3_CHUNK_LEN 1024
#define BLAKE3_MAX_DEPTH 54

// This struct is a private implementation detail. It has to be here because
// it's part of blake3_hasher below.
تعریفنوع ساختار {
  uint32_t cv[8];
  uint64_t chunk_counter;
  uint8_t buf[BLAKE3_BLOCK_LEN];
  uint8_t buf_len;
  uint8_t blocks_compressed;
  uint8_t flags;
} blake3_chunk_state;

تعریفنوع ساختار {
  uint32_t کلید[8];
  blake3_chunk_state chunk;
  uint8_t cv_پشته_len;
  // The stack size is MAX_DEPTH + 1 because we do lazy merging. For example,
  // with 7 chunks, we have 3 entries in the stack. Adding an 8th chunk
  // requires a 4th entry, rather than merging everything down to 1, because we
  // don't know whether more input is coming. This is different from how the
  // reference implementation does things.
  uint8_t cv_پشته[(BLAKE3_MAX_DEPTH + 1) * BLAKE3_OUT_LEN];
} blake3_hasher;

BLAKE3_API ثابت نویسه *blake3_version(تهی);
BLAKE3_API تهی blake3_hasher_init(blake3_hasher *self);
BLAKE3_API تهی blake3_hasher_init_keyed(blake3_hasher *self,
                                         ثابت uint8_t کلید[BLAKE3_کلید_LEN]);
BLAKE3_API تهی blake3_hasher_init_derive_کلید(blake3_hasher *self, ثابت نویسه *context);
BLAKE3_API تهی blake3_hasher_init_derive_کلید_raw(blake3_hasher *self, ثابت تهی *context,
                                                  size_t context_len);
BLAKE3_API تهی blake3_hasher_update(blake3_hasher *self, ثابت تهی *درون_داد,
                                     size_t درون_داد_len);
BLAKE3_API تهی blake3_hasher_finalize(ثابت blake3_hasher *self, uint8_t *out,
                                       size_t out_len);
BLAKE3_API تهی blake3_hasher_finalize_seek(ثابت blake3_hasher *self, uint64_t seek,
                                            uint8_t *out, size_t out_len);
BLAKE3_API تهی blake3_hasher_reset(blake3_hasher *self);

#ifdef __cplusplus
}
#endif

#endif /* BLAKE3_H */
