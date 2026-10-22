#ifndef BLAKE3_H
#define BLAKE3_H

#અન્તર્ભાવ <stddef.h>
#અન્તર્ભાવ <stdint.h>

#જો !defined(BLAKE3_API)
# જો defined(_WIN32) || defined(__CYGWIN__)
#   જો defined(BLAKE3_DLL)
#     જો defined(BLAKE3_DLL_EXPORTS)
#       define BLAKE3_API __declspec(dllexport)
#     નહીંતર
#       define BLAKE3_API __declspec(dllimport)
#     endif
#     define BLAKE3_PRIVATE
#   નહીંતર
#     define BLAKE3_API
#     define BLAKE3_PRIVATE
#   endif
# elif __GNUC__ >= 4
#   define BLAKE3_API __attribute__((visibility("default")))
#   define BLAKE3_PRIVATE __attribute__((visibility("hidden")))
# નહીંતર
#   define BLAKE3_API
#   define BLAKE3_PRIVATE
# endif
#endif

#ifdef __cplusplus
બાહ્ય "C" {
#endif

#define BLAKE3_VERSION_STRING "1.5.0"
#define BLAKE3_ચાવી_LEN 32
#define BLAKE3_OUT_LEN 32
#define BLAKE3_BLOCK_LEN 64
#define BLAKE3_CHUNK_LEN 1024
#define BLAKE3_MAX_DEPTH 54

// This struct is a private implementation detail. It has to be here because
// it's part of blake3_hasher below.
પ્રકારવ્યાખ્યા સંરચના {
  uint32_t cv[8];
  uint64_t chunk_counter;
  uint8_t buf[BLAKE3_BLOCK_LEN];
  uint8_t buf_len;
  uint8_t blocks_compressed;
  uint8_t flags;
} blake3_chunk_state;

પ્રકારવ્યાખ્યા સંરચના {
  uint32_t ચાવી[8];
  blake3_chunk_state chunk;
  uint8_t cv_ઢગલો_len;
  // The stack size is MAX_DEPTH + 1 because we do lazy merging. For example,
  // with 7 chunks, we have 3 entries in the stack. Adding an 8th chunk
  // requires a 4th entry, rather than merging everything down to 1, because we
  // don't know whether more input is coming. This is different from how the
  // reference implementation does things.
  uint8_t cv_ઢગલો[(BLAKE3_MAX_DEPTH + 1) * BLAKE3_OUT_LEN];
} blake3_hasher;

BLAKE3_API અચલ અક્ષર *blake3_version(શૂન્ય);
BLAKE3_API શૂન્ય blake3_hasher_init(blake3_hasher *self);
BLAKE3_API શૂન્ય blake3_hasher_init_keyed(blake3_hasher *self,
                                         અચલ uint8_t ચાવી[BLAKE3_ચાવી_LEN]);
BLAKE3_API શૂન્ય blake3_hasher_init_derive_ચાવી(blake3_hasher *self, અચલ અક્ષર *context);
BLAKE3_API શૂન્ય blake3_hasher_init_derive_ચાવી_raw(blake3_hasher *self, અચલ શૂન્ય *context,
                                                  size_t context_len);
BLAKE3_API શૂન્ય blake3_hasher_update(blake3_hasher *self, અચલ શૂન્ય *આગત,
                                     size_t આગત_len);
BLAKE3_API શૂન્ય blake3_hasher_finalize(અચલ blake3_hasher *self, uint8_t *out,
                                       size_t out_len);
BLAKE3_API શૂન્ય blake3_hasher_finalize_seek(અચલ blake3_hasher *self, uint64_t seek,
                                            uint8_t *out, size_t out_len);
BLAKE3_API શૂન્ય blake3_hasher_reset(blake3_hasher *self);

#ifdef __cplusplus
}
#endif

#endif /* BLAKE3_H */
