#यदि_अपरिभाषितम् BLAKE3_H
#परिभाषय BLAKE3_H

#अन्तर्भाव <stddef.h>
#अन्तर्भाव <stdint.h>

#यदि !defined(BLAKE3_API)
# यदि defined(_WIN32) || defined(__CYGWIN__)
#   यदि defined(BLAKE3_DLL)
#     यदि defined(BLAKE3_DLL_EXPORTS)
#       define BLAKE3_API __declspec(dllexport)
#     अन्यथा
#       define BLAKE3_API __declspec(dllimport)
#     endif
#     define BLAKE3_वैयक्तिकम्
#   अन्यथा
#     define BLAKE3_API
#     define BLAKE3_वैयक्तिकम्
#   endif
# elif __GNUC__ >= 4
#   define BLAKE3_API __attribute__((visibility("default")))
#   define BLAKE3_वैयक्तिकम् __attribute__((visibility("hidden")))
# अन्यथा
#   define BLAKE3_API
#   define BLAKE3_वैयक्तिकम्
# endif
#यदि_अन्तः

#यदि_परिभाषितम् __cplusplus
बाह्य "C" {
#यदि_अन्तः

#परिभाषय BLAKE3_संस्करणम्_STRING "1.5.0"
#परिभाषय BLAKE3_कुञ्चिका_LEN 32
#परिभाषय BLAKE3_बहिर्गमनम्_LEN 32
#परिभाषय BLAKE3_खण्डम्_LEN 64
#परिभाषय BLAKE3_CHUNK_LEN 1024
#परिभाषय BLAKE3_MAX_गभीरता 54

// This struct is a private implementation detail. It has to be here because
// it's part of blake3_hasher below.
प्रकारनिर्देश संरचना {
  uint32_t cv[8];
  uint64_t chunk_गणक;
  uint8_t buf[BLAKE3_खण्डम्_LEN];
  uint8_t buf_len;
  uint8_t खण्डानि_compressed;
  uint8_t flags;
} blake3_chunk_state;

प्रकारनिर्देश संरचना {
  uint32_t कुञ्चिका[8];
  blake3_chunk_state chunk;
  uint8_t cv_राशिः_len;
  // The stack size is MAX_DEPTH + 1 because we do lazy merging. For example,
  // with 7 chunks, we have 3 entries in the stack. Adding an 8th chunk
  // requires a 4th entry, rather than merging everything down to 1, because we
  // don't know whether more input is coming. This is different from how the
  // reference implementation does things.
  uint8_t cv_राशिः[(BLAKE3_MAX_गभीरता + 1) * BLAKE3_बहिर्गमनम्_LEN];
} blake3_hasher;

BLAKE3_API ध्रुव वर्ण *blake3_संस्करणम्(शून्य);
BLAKE3_API शून्य blake3_hasher_init(blake3_hasher *self);
BLAKE3_API शून्य blake3_hasher_init_keyed(blake3_hasher *self,
                                         ध्रुव uint8_t कुञ्चिका[BLAKE3_कुञ्चिका_LEN]);
BLAKE3_API शून्य blake3_hasher_init_derive_कुञ्चिका(blake3_hasher *self, ध्रुव वर्ण *context);
BLAKE3_API शून्य blake3_hasher_init_derive_कुञ्चिका_raw(blake3_hasher *self, ध्रुव शून्य *context,
                                                  size_t context_len);
BLAKE3_API शून्य blake3_hasher_update(blake3_hasher *self, ध्रुव शून्य *निवेशः,
                                     size_t निवेशः_len);
BLAKE3_API शून्य blake3_hasher_finalize(ध्रुव blake3_hasher *self, uint8_t *बहिर्गमनम्,
                                       size_t बहिर्गमनम्_len);
BLAKE3_API शून्य blake3_hasher_finalize_seek(ध्रुव blake3_hasher *self, uint64_t seek,
                                            uint8_t *बहिर्गमनम्, size_t बहिर्गमनम्_len);
BLAKE3_API शून्य blake3_hasher_reset(blake3_hasher *self);

#यदि_परिभाषितम् __cplusplus
}
#यदि_अन्तः

#यदि_अन्तः /* BLAKE3_H */
