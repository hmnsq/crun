#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ BLAKE3_H
#ವ್ಯಾಖ್ಯಾನಿಸು BLAKE3_H

#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>

#ಆದರೆ !defined(BLAKE3_API)
# ಆದರೆ defined(_WIN32) || defined(__CYGWIN__)
#   ಆದರೆ defined(BLAKE3_DLL)
#     ಆದರೆ defined(BLAKE3_DLL_EXPORTS)
#       define BLAKE3_API __declspec(dllexport)
#     ಇಲ್ಲದಿದ್ದರೆ
#       define BLAKE3_API __declspec(dllimport)
#     endif
#     define BLAKE3_ಖಾಸಗಿ
#   ಇಲ್ಲದಿದ್ದರೆ
#     define BLAKE3_API
#     define BLAKE3_ಖಾಸಗಿ
#   endif
# elif __GNUC__ >= 4
#   define BLAKE3_API __attribute__((visibility("default")))
#   define BLAKE3_ಖಾಸಗಿ __attribute__((visibility("hidden")))
# ಇಲ್ಲದಿದ್ದರೆ
#   define BLAKE3_API
#   define BLAKE3_ಖಾಸಗಿ
# endif
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
ಬಾಹ್ಯ "C" {
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು BLAKE3_ಆವೃತ್ತಿ_STRING "1.5.0"
#ವ್ಯಾಖ್ಯಾನಿಸು BLAKE3_ಕೀಲಿ_LEN 32
#ವ್ಯಾಖ್ಯಾನಿಸು BLAKE3_ಹೊರಹೋಗು_LEN 32
#ವ್ಯಾಖ್ಯಾನಿಸು BLAKE3_ಖಂಡ_LEN 64
#ವ್ಯಾಖ್ಯಾನಿಸು BLAKE3_CHUNK_LEN 1024
#ವ್ಯಾಖ್ಯಾನಿಸು BLAKE3_MAX_ಆಳ 54

// This struct is a private implementation detail. It has to be here because
// it's part of blake3_hasher below.
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
  uint32_t cv[8];
  uint64_t chunk_counter;
  uint8_t buf[BLAKE3_ಖಂಡ_LEN];
  uint8_t buf_len;
  uint8_t ಖಂಡಗಳು_compressed;
  uint8_t flags;
} blake3_chunk_state;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
  uint32_t ಕೀಲಿ[8];
  blake3_chunk_state chunk;
  uint8_t cv_ರಾಶಿ_len;
  // The stack size is MAX_DEPTH + 1 because we do lazy merging. For example,
  // with 7 chunks, we have 3 entries in the stack. Adding an 8th chunk
  // requires a 4th entry, rather than merging everything down to 1, because we
  // don't know whether more input is coming. This is different from how the
  // reference implementation does things.
  uint8_t cv_ರಾಶಿ[(BLAKE3_MAX_ಆಳ + 1) * BLAKE3_ಹೊರಹೋಗು_LEN];
} blake3_hasher;

BLAKE3_API ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *blake3_ಆವೃತ್ತಿ(ಶೂನ್ಯ);
BLAKE3_API ಶೂನ್ಯ blake3_hasher_init(blake3_hasher *self);
BLAKE3_API ಶೂನ್ಯ blake3_hasher_init_keyed(blake3_hasher *self,
                                         ಸ್ಥಿರಾಂಕ uint8_t ಕೀಲಿ[BLAKE3_ಕೀಲಿ_LEN]);
BLAKE3_API ಶೂನ್ಯ blake3_hasher_init_derive_ಕೀಲಿ(blake3_hasher *self, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *context);
BLAKE3_API ಶೂನ್ಯ blake3_hasher_init_derive_ಕೀಲಿ_raw(blake3_hasher *self, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *context,
                                                  size_t context_len);
BLAKE3_API ಶೂನ್ಯ blake3_hasher_update(blake3_hasher *self, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಒಳಹರಿವು,
                                     size_t ಒಳಹರಿವು_len);
BLAKE3_API ಶೂನ್ಯ blake3_hasher_finalize(ಸ್ಥಿರಾಂಕ blake3_hasher *self, uint8_t *ಹೊರಹೋಗು,
                                       size_t ಹೊರಹೋಗು_len);
BLAKE3_API ಶೂನ್ಯ blake3_hasher_finalize_seek(ಸ್ಥಿರಾಂಕ blake3_hasher *self, uint64_t seek,
                                            uint8_t *ಹೊರಹೋಗು, size_t ಹೊರಹೋಗು_len);
BLAKE3_API ಶೂನ್ಯ blake3_hasher_reset(blake3_hasher *self);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ /* BLAKE3_H */
