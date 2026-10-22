#ifndef BLAKE3_H
#define BLAKE3_H

#अन्तर्भाव <stddef.h>
#अन्तर्भाव <stdint.h>

#जर !defined(BLAKE3_API)
# जर defined(_WIN32) || defined(__CYGWIN__)
#   जर defined(BLAKE3_DLL)
#     जर defined(BLAKE3_DLL_EXPORTS)
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
#endif

#ifdef __cplusplus
बाह्य "C" {
#endif

#define BLAKE3_VERSION_STRING "1.5.0"
#define BLAKE3_किल्ली_LEN 32
#define BLAKE3_OUT_LEN 32
#define BLAKE3_BLOCK_LEN 64
#define BLAKE3_CHUNK_LEN 1024
#define BLAKE3_MAX_DEPTH 54

// This struct is a private implementation detail. It has to be here because
// it's part of blake3_hasher below.
प्रकारव्याख्या संरचना {
  uint32_t cv[8];
  uint64_t chunk_counter;
  uint8_t buf[BLAKE3_BLOCK_LEN];
  uint8_t buf_len;
  uint8_t blocks_compressed;
  uint8_t flags;
} blake3_chunk_state;

प्रकारव्याख्या संरचना {
  uint32_t किल्ली[8];
  blake3_chunk_state chunk;
  uint8_t cv_चळत_len;
  // The stack size is MAX_DEPTH + 1 because we do lazy merging. For example,
  // with 7 chunks, we have 3 entries in the stack. Adding an 8th chunk
  // requires a 4th entry, rather than merging everything down to 1, because we
  // don't know whether more input is coming. This is different from how the
  // reference implementation does things.
  uint8_t cv_चळत[(BLAKE3_MAX_DEPTH + 1) * BLAKE3_OUT_LEN];
} blake3_hasher;

BLAKE3_API स्थिरांक अक्षर *blake3_version(शून्य);
BLAKE3_API शून्य blake3_hasher_init(blake3_hasher *self);
BLAKE3_API शून्य blake3_hasher_init_keyed(blake3_hasher *self,
                                         स्थिरांक uint8_t किल्ली[BLAKE3_किल्ली_LEN]);
BLAKE3_API शून्य blake3_hasher_init_derive_किल्ली(blake3_hasher *self, स्थिरांक अक्षर *context);
BLAKE3_API शून्य blake3_hasher_init_derive_किल्ली_raw(blake3_hasher *self, स्थिरांक शून्य *context,
                                                  माप_प्रकार context_len);
BLAKE3_API शून्य blake3_hasher_update(blake3_hasher *self, स्थिरांक शून्य *आदान,
                                     माप_प्रकार आदान_len);
BLAKE3_API शून्य blake3_hasher_finalize(स्थिरांक blake3_hasher *self, uint8_t *out,
                                       माप_प्रकार out_len);
BLAKE3_API शून्य blake3_hasher_finalize_seek(स्थिरांक blake3_hasher *self, uint64_t seek,
                                            uint8_t *out, माप_प्रकार out_len);
BLAKE3_API शून्य blake3_hasher_reset(blake3_hasher *self);

#ifdef __cplusplus
}
#endif

#endif /* BLAKE3_H */
