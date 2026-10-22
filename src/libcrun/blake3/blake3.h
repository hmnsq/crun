#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ BLAKE3_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLAKE3_H

#ਅਨ੍ਤਰ੍ਭਾਵ <stddef.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>

#ਜੇ !defined(BLAKE3_API)
# ਜੇ defined(_WIN32) || defined(__CYGWIN__)
#   ਜੇ defined(BLAKE3_DLL)
#     ਜੇ defined(BLAKE3_DLL_EXPORTS)
#       define BLAKE3_API __declspec(dllexport)
#     ਨਹੀਂਤਾਂ
#       define BLAKE3_API __declspec(dllimport)
#     endif
#     define BLAKE3_ਨਿੱਜੀ
#   ਨਹੀਂਤਾਂ
#     define BLAKE3_API
#     define BLAKE3_ਨਿੱਜੀ
#   endif
# elif __GNUC__ >= 4
#   define BLAKE3_API __attribute__((visibility("default")))
#   define BLAKE3_ਨਿੱਜੀ __attribute__((visibility("hidden")))
# ਨਹੀਂਤਾਂ
#   define BLAKE3_API
#   define BLAKE3_ਨਿੱਜੀ
# endif
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLAKE3_ਸੰਸਕਰਣ_STRING "1.5.0"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLAKE3_ਕੁੰਜੀ_LEN 32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLAKE3_ਬਾਹਰਲਾ_LEN 32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLAKE3_ਖੰਡ_LEN 64
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLAKE3_CHUNK_LEN 1024
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLAKE3_MAX_ਡੂੰਘਾਈ 54

// This struct is a private implementation detail. It has to be here because
// it's part of blake3_hasher below.
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
  uint32_t cv[8];
  uint64_t chunk_counter;
  uint8_t buf[BLAKE3_ਖੰਡ_LEN];
  uint8_t buf_len;
  uint8_t ਖੰਡਾਂ_compressed;
  uint8_t flags;
} blake3_chunk_state;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
  uint32_t ਕੁੰਜੀ[8];
  blake3_chunk_state chunk;
  uint8_t cv_ਢੇਰ_len;
  // The stack size is MAX_DEPTH + 1 because we do lazy merging. For example,
  // with 7 chunks, we have 3 entries in the stack. Adding an 8th chunk
  // requires a 4th entry, rather than merging everything down to 1, because we
  // don't know whether more input is coming. This is different from how the
  // reference implementation does things.
  uint8_t cv_ਢੇਰ[(BLAKE3_MAX_ਡੂੰਘਾਈ + 1) * BLAKE3_ਬਾਹਰਲਾ_LEN];
} blake3_hasher;

BLAKE3_API ਅਚਲ ਅੱਖਰ *blake3_ਸੰਸਕਰਣ(ਖਾਲੀ);
BLAKE3_API ਖਾਲੀ blake3_hasher_init(blake3_hasher *self);
BLAKE3_API ਖਾਲੀ blake3_hasher_init_keyed(blake3_hasher *self,
                                         ਅਚਲ uint8_t ਕੁੰਜੀ[BLAKE3_ਕੁੰਜੀ_LEN]);
BLAKE3_API ਖਾਲੀ blake3_hasher_init_derive_ਕੁੰਜੀ(blake3_hasher *self, ਅਚਲ ਅੱਖਰ *context);
BLAKE3_API ਖਾਲੀ blake3_hasher_init_derive_ਕੁੰਜੀ_raw(blake3_hasher *self, ਅਚਲ ਖਾਲੀ *context,
                                                  size_t context_len);
BLAKE3_API ਖਾਲੀ blake3_hasher_update(blake3_hasher *self, ਅਚਲ ਖਾਲੀ *ਨਿਵੇਸ਼,
                                     size_t ਨਿਵੇਸ਼_len);
BLAKE3_API ਖਾਲੀ blake3_hasher_finalize(ਅਚਲ blake3_hasher *self, uint8_t *ਬਾਹਰਲਾ,
                                       size_t ਬਾਹਰਲਾ_len);
BLAKE3_API ਖਾਲੀ blake3_hasher_finalize_seek(ਅਚਲ blake3_hasher *self, uint64_t seek,
                                            uint8_t *ਬਾਹਰਲਾ, size_t ਬਾਹਰਲਾ_len);
BLAKE3_API ਖਾਲੀ blake3_hasher_reset(blake3_hasher *self);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* BLAKE3_H */
