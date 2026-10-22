#నిర్వచితం_కాకపోతే BLAKE3_H
#నిర్వచించు BLAKE3_H

#అన్తర్భావ <stddef.h>
#అన్తర్భావ <stdint.h>

#ఒకవేళ !defined(BLAKE3_API)
# ఒకవేళ defined(_WIN32) || defined(__CYGWIN__)
#   ఒకవేళ defined(BLAKE3_DLL)
#     ఒకవేళ defined(BLAKE3_DLL_EXPORTS)
#       define BLAKE3_API __declspec(dllexport)
#     లేకపోతే
#       define BLAKE3_API __declspec(dllimport)
#     endif
#     define BLAKE3_వ్యక్తిగత
#   లేకపోతే
#     define BLAKE3_API
#     define BLAKE3_వ్యక్తిగత
#   endif
# elif __GNUC__ >= 4
#   define BLAKE3_API __attribute__((visibility("default")))
#   define BLAKE3_వ్యక్తిగత __attribute__((visibility("hidden")))
# లేకపోతే
#   define BLAKE3_API
#   define BLAKE3_వ్యక్తిగత
# endif
#అయితే_ముగింపు

#నిర్వచితమైతే __cplusplus
బాహ్య "C" {
#అయితే_ముగింపు

#నిర్వచించు BLAKE3_సంస్కరణ_STRING "1.5.0"
#నిర్వచించు BLAKE3_కీలకం_LEN 32
#నిర్వచించు BLAKE3_బహిర్గతం_LEN 32
#నిర్వచించు BLAKE3_ఖండం_LEN 64
#నిర్వచించు BLAKE3_CHUNK_LEN 1024
#నిర్వచించు BLAKE3_MAX_లోతు 54

// This struct is a private implementation detail. It has to be here because
// it's part of blake3_hasher below.
రకనిర్వచనం నిర్మాణం {
  uint32_t cv[8];
  uint64_t chunk_counter;
  uint8_t buf[BLAKE3_ఖండం_LEN];
  uint8_t buf_len;
  uint8_t ఖండాలు_compressed;
  uint8_t flags;
} blake3_chunk_state;

రకనిర్వచనం నిర్మాణం {
  uint32_t కీలకం[8];
  blake3_chunk_state chunk;
  uint8_t cv_దొంతర_len;
  // The stack size is MAX_DEPTH + 1 because we do lazy merging. For example,
  // with 7 chunks, we have 3 entries in the stack. Adding an 8th chunk
  // requires a 4th entry, rather than merging everything down to 1, because we
  // don't know whether more input is coming. This is different from how the
  // reference implementation does things.
  uint8_t cv_దొంతర[(BLAKE3_MAX_లోతు + 1) * BLAKE3_బహిర్గతం_LEN];
} blake3_hasher;

BLAKE3_API స్థిరరాశి అక్షరం *blake3_సంస్కరణ(శూన్య);
BLAKE3_API శూన్య blake3_hasher_init(blake3_hasher *self);
BLAKE3_API శూన్య blake3_hasher_init_keyed(blake3_hasher *self,
                                         స్థిరరాశి uint8_t కీలకం[BLAKE3_కీలకం_LEN]);
BLAKE3_API శూన్య blake3_hasher_init_derive_కీలకం(blake3_hasher *self, స్థిరరాశి అక్షరం *context);
BLAKE3_API శూన్య blake3_hasher_init_derive_కీలకం_raw(blake3_hasher *self, స్థిరరాశి శూన్య *context,
                                                  size_t context_len);
BLAKE3_API శూన్య blake3_hasher_update(blake3_hasher *self, స్థిరరాశి శూన్య *ఆదానం,
                                     size_t ఆదానం_len);
BLAKE3_API శూన్య blake3_hasher_finalize(స్థిరరాశి blake3_hasher *self, uint8_t *బహిర్గతం,
                                       size_t బహిర్గతం_len);
BLAKE3_API శూన్య blake3_hasher_finalize_seek(స్థిరరాశి blake3_hasher *self, uint64_t seek,
                                            uint8_t *బహిర్గతం, size_t బహిర్గతం_len);
BLAKE3_API శూన్య blake3_hasher_reset(blake3_hasher *self);

#నిర్వచితమైతే __cplusplus
}
#అయితే_ముగింపు

#అయితే_ముగింపు /* BLAKE3_H */
