#import <assert.h>
#import <bare.h>
#import <js.h>

#import "lib/framesetter.h"

static js_value_t *
bare_core_text_exports(js_env_t *env, js_value_t *exports) {
  int err;

  bare_foundation_registry_t *registry = bare_foundation_registry_create(env, exports);

#define V(name, fn) \
  { \
    js_value_t *val; \
    err = js_create_function(env, name, -1, fn, registry, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, exports, name, val); \
    assert(err == 0); \
  }

  V("claim", bare_foundation_claim)
  V("wrapper", bare_foundation_wrapper)
  V("registrySize", bare_foundation_registry_size)
  V("handle", bare_foundation_handle)
  V("adopt", bare_foundation_adopt)

  V("framesetterInit", bare_core_text_framesetter_init)
  V("framesetterMeasure", bare_core_text_framesetter_measure)
  V("framesetterMeasureAttributed", bare_core_text_framesetter_measure_attributed)
#undef V

  return exports;
}

BARE_MODULE(bare_core_text, bare_core_text_exports)
