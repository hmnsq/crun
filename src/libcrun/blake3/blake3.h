#اگر_غیر_تعریف_شدہ BLAKE3_H
#تعریف_کرو BLAKE3_H

#شامل <stddef.h>
#شامل <stdint.h>

#اگر !defined(BLAKE3_API)
# اگر defined(_WIN32) || defined(__CYGWIN__)
#   اگر defined(BLAKE3_DLL)
#     اگر defined(BLAKE3_DLL_EXPORTS)
#       define BLAKE3_API __declspec(dllexport)
#     ورنہ
#       define BLAKE3_API __declspec(dllimport)
#     endif
#     define BLAKE3_نجی
#   ورنہ
#     define BLAKE3_API
#     define BLAKE3_نجی
#   endif
# elif __GNUC__ >= 4
#   define BLAKE3_API __attribute__((visibility("default")))
#   define BLAKE3_نجی __attribute__((visibility("hidden")))
# ورنہ
#   define BLAKE3_API
#   define BLAKE3_نجی
# endif
#اگر_ختم

#اگر_تعریف_شدہ __cplusplus
خارجی "C" {
#اگر_ختم

#تعریف_کرو BLAKE3_نسخہ_STRING "1.5.0"
#تعریف_کرو BLAKE3_کلید_LEN 32
#تعریف_کرو BLAKE3_برآمد_LEN 32
#تعریف_کرو BLAKE3_بلاک_LEN 64
#تعریف_کرو BLAKE3_CHUNK_LEN 1024
#تعریف_کرو BLAKE3_MAX_گہرائی 54

// This struct is a private implementation detail. It has to be here because
// it's part of blake3_hasher below.
قسمتعریف ساخت {
  uint32_t cv[8];
  uint64_t chunk_شمار;
  uint8_t buf[BLAKE3_بلاک_LEN];
  uint8_t buf_len;
  uint8_t بلاکیں_compressed;
  uint8_t flags;
} blake3_chunk_state;

قسمتعریف ساخت {
  uint32_t کلید[8];
  blake3_chunk_state chunk;
  uint8_t cv_ڈھیر_len;
  // The stack size is MAX_DEPTH + 1 because we do lazy merging. For example,
  // with 7 chunks, we have 3 entries in the stack. Adding an 8th chunk
  // requires a 4th entry, rather than merging everything down to 1, because we
  // don't know whether more input is coming. This is different from how the
  // reference implementation does things.
  uint8_t cv_ڈھیر[(BLAKE3_MAX_گہرائی + 1) * BLAKE3_برآمد_LEN];
} blake3_hasher;

BLAKE3_API ثابت حرف *blake3_نسخہ(خالی);
BLAKE3_API خالی blake3_hasher_init(blake3_hasher *self);
BLAKE3_API خالی blake3_hasher_init_keyed(blake3_hasher *self,
                                         ثابت uint8_t کلید[BLAKE3_کلید_LEN]);
BLAKE3_API خالی blake3_hasher_init_derive_کلید(blake3_hasher *self, ثابت حرف *context);
BLAKE3_API خالی blake3_hasher_init_derive_کلید_raw(blake3_hasher *self, ثابت خالی *context,
                                                  size_t context_len);
BLAKE3_API خالی blake3_hasher_update(blake3_hasher *self, ثابت خالی *داخلہ,
                                     size_t داخلہ_len);
BLAKE3_API خالی blake3_hasher_finalize(ثابت blake3_hasher *self, uint8_t *برآمد,
                                       size_t برآمد_len);
BLAKE3_API خالی blake3_hasher_finalize_seek(ثابت blake3_hasher *self, uint64_t seek,
                                            uint8_t *برآمد, size_t برآمد_len);
BLAKE3_API خالی blake3_hasher_reset(blake3_hasher *self);

#اگر_تعریف_شدہ __cplusplus
}
#اگر_ختم

#اگر_ختم /* BLAKE3_H */
