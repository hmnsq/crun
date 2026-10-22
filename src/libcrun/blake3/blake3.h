#ifndef BLAKE3_H
#define BLAKE3_H

#ଅନ୍ତର୍ଭାଵ <stddef.h>
#ଅନ୍ତର୍ଭାଵ <stdint.h>

#ଯଦି !defined(BLAKE3_API)
# ଯଦି defined(_WIN32) || defined(__CYGWIN__)
#   ଯଦି defined(BLAKE3_DLL)
#     ଯଦି defined(BLAKE3_DLL_EXPORTS)
#       define BLAKE3_API __declspec(dllexport)
#     ଅନ୍ୟଥା
#       define BLAKE3_API __declspec(dllimport)
#     endif
#     define BLAKE3_PRIVATE
#   ଅନ୍ୟଥା
#     define BLAKE3_API
#     define BLAKE3_PRIVATE
#   endif
# elif __GNUC__ >= 4
#   define BLAKE3_API __attribute__((visibility("default")))
#   define BLAKE3_PRIVATE __attribute__((visibility("hidden")))
# ଅନ୍ୟଥା
#   define BLAKE3_API
#   define BLAKE3_PRIVATE
# endif
#endif

#ifdef __cplusplus
ବାହ୍ୟ "C" {
#endif

#define BLAKE3_VERSION_STRING "1.5.0"
#define BLAKE3_ଚାବି_LEN 32
#define BLAKE3_OUT_LEN 32
#define BLAKE3_BLOCK_LEN 64
#define BLAKE3_CHUNK_LEN 1024
#define BLAKE3_MAX_DEPTH 54

// This struct is a private implementation detail. It has to be here because
// it's part of blake3_hasher below.
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
  uint32_t cv[8];
  uint64_t chunk_counter;
  uint8_t buf[BLAKE3_BLOCK_LEN];
  uint8_t buf_len;
  uint8_t blocks_compressed;
  uint8_t flags;
} blake3_chunk_state;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
  uint32_t ଚାବି[8];
  blake3_chunk_state chunk;
  uint8_t cv_ଗଦା_len;
  // The stack size is MAX_DEPTH + 1 because we do lazy merging. For example,
  // with 7 chunks, we have 3 entries in the stack. Adding an 8th chunk
  // requires a 4th entry, rather than merging everything down to 1, because we
  // don't know whether more input is coming. This is different from how the
  // reference implementation does things.
  uint8_t cv_ଗଦା[(BLAKE3_MAX_DEPTH + 1) * BLAKE3_OUT_LEN];
} blake3_hasher;

BLAKE3_API ଅଚଳ ଅକ୍ଷର *blake3_version(ଶୂନ୍ୟ);
BLAKE3_API ଶୂନ୍ୟ blake3_hasher_init(blake3_hasher *self);
BLAKE3_API ଶୂନ୍ୟ blake3_hasher_init_keyed(blake3_hasher *self,
                                         ଅଚଳ uint8_t ଚାବି[BLAKE3_ଚାବି_LEN]);
BLAKE3_API ଶୂନ୍ୟ blake3_hasher_init_derive_ଚାବି(blake3_hasher *self, ଅଚଳ ଅକ୍ଷର *context);
BLAKE3_API ଶୂନ୍ୟ blake3_hasher_init_derive_ଚାବି_raw(blake3_hasher *self, ଅଚଳ ଶୂନ୍ୟ *context,
                                                  size_t context_len);
BLAKE3_API ଶୂନ୍ୟ blake3_hasher_update(blake3_hasher *self, ଅଚଳ ଶୂନ୍ୟ *ନିବେଶ,
                                     size_t ନିବେଶ_len);
BLAKE3_API ଶୂନ୍ୟ blake3_hasher_finalize(ଅଚଳ blake3_hasher *self, uint8_t *out,
                                       size_t out_len);
BLAKE3_API ଶୂନ୍ୟ blake3_hasher_finalize_seek(ଅଚଳ blake3_hasher *self, uint64_t seek,
                                            uint8_t *out, size_t out_len);
BLAKE3_API ଶୂନ୍ୟ blake3_hasher_reset(blake3_hasher *self);

#ifdef __cplusplus
}
#endif

#endif /* BLAKE3_H */
