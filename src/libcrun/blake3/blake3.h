#अगर_अघोषित BLAKE3_H
#घोषणा BLAKE3_H

#समावेश <stddef.h>
#समावेश <stdint.h>

#अगर !defined(BLAKE3_API)
# अगर defined(_WIN32) || defined(__CYGWIN__)
#   अगर defined(BLAKE3_DLL)
#     अगर defined(BLAKE3_DLL_EXPORTS)
#       define BLAKE3_API __declspec(dllexport)
#     अन्यथा
#       define BLAKE3_API __declspec(dllimport)
#     endif
#     define BLAKE3_PRIVATE
#   अन्यथा
#     define BLAKE3_API
#     define BLAKE3_PRIVATE
#   endif
# elif __GNUC__ >= 4
#   define BLAKE3_API __attribute__((visibility("default")))
#   define BLAKE3_PRIVATE __attribute__((visibility("hidden")))
# अन्यथा
#   define BLAKE3_API
#   define BLAKE3_PRIVATE
# endif
#पूर्ण_अगर

#अगर_घोषित __cplusplus
बाह्य "C" {
#पूर्ण_अगर

#घोषणा BLAKE3_VERSION_STRING "1.5.0"
#घोषणा BLAKE3_कुंजी_LEN 32
#घोषणा BLAKE3_OUT_LEN 32
#घोषणा BLAKE3_BLOCK_LEN 64
#घोषणा BLAKE3_CHUNK_LEN 1024
#घोषणा BLAKE3_MAX_DEPTH 54

// This struct is a private implementation detail. It has to be here because
// it's part of blake3_hasher below.
प्रकार काष्ठा {
  uint32_t cv[8];
  uint64_t chunk_गणक;
  uint8_t buf[BLAKE3_BLOCK_LEN];
  uint8_t buf_len;
  uint8_t blocks_compressed;
  uint8_t flags;
} blake3_chunk_state;

प्रकार काष्ठा {
  uint32_t कुंजी[8];
  blake3_chunk_state chunk;
  uint8_t cv_ढेर_len;
  // The stack size is MAX_DEPTH + 1 because we do lazy merging. For example,
  // with 7 chunks, we have 3 entries in the stack. Adding an 8th chunk
  // requires a 4th entry, rather than merging everything down to 1, because we
  // don't know whether more input is coming. This is different from how the
  // reference implementation does things.
  uint8_t cv_ढेर[(BLAKE3_MAX_DEPTH + 1) * BLAKE3_OUT_LEN];
} blake3_hasher;

BLAKE3_API स्थिर अक्षर *blake3_version(व्योम);
BLAKE3_API व्योम blake3_hasher_init(blake3_hasher *self);
BLAKE3_API व्योम blake3_hasher_init_keyed(blake3_hasher *self,
                                         स्थिर uint8_t कुंजी[BLAKE3_कुंजी_LEN]);
BLAKE3_API व्योम blake3_hasher_init_derive_कुंजी(blake3_hasher *self, स्थिर अक्षर *context);
BLAKE3_API व्योम blake3_hasher_init_derive_कुंजी_raw(blake3_hasher *self, स्थिर व्योम *context,
                                                  माप_प्रकार context_len);
BLAKE3_API व्योम blake3_hasher_update(blake3_hasher *self, स्थिर व्योम *निवेश,
                                     माप_प्रकार निवेश_len);
BLAKE3_API व्योम blake3_hasher_finalize(स्थिर blake3_hasher *self, uint8_t *out,
                                       माप_प्रकार out_len);
BLAKE3_API व्योम blake3_hasher_finalize_seek(स्थिर blake3_hasher *self, uint64_t seek,
                                            uint8_t *out, माप_प्रकार out_len);
BLAKE3_API व्योम blake3_hasher_reset(blake3_hasher *self);

#अगर_घोषित __cplusplus
}
#पूर्ण_अगर

#पूर्ण_अगर /* BLAKE3_H */
