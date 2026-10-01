#pragma once

#import <assert.h>
#import <js.h>
#import <stdlib.h>
#import <utf.h>

#import <Foundation/Foundation.h>

#import "registry.h"

static bool
bare_core_text__read_number(js_env_t *env, js_value_t *value, const char *name) {
  int err;

  js_value_type_t type;
  err = js_typeof(env, value, &type);
  assert(err == 0);

  if (type == js_number) return true;

  err = js_throw_type_errorf(env, NULL, "Expected a number for '%s'", name);
  assert(err == 0);

  return false;
}

static bool
bare_core_text__read_double(js_env_t *env, js_value_t *value, const char *name, double *result) {
  if (!bare_core_text__read_number(env, value, name)) return false;

  int err = js_get_value_double(env, value, result);
  assert(err == 0);

  return true;
}

static NSString *
bare_core_text__to_string(js_env_t *env, js_value_t *value) {
  int err;

  js_value_type_t type;
  err = js_typeof(env, value, &type);
  assert(err == 0);

  if (type == js_null || type == js_undefined) return nil;

  size_t len;
  err = js_get_value_string_utf8(env, value, NULL, 0, &len);
  assert(err == 0);

  len += 1 /* NULL */;

  char *data = malloc(len);

  err = js_get_value_string_utf8(env, value, (utf8_t *) data, len, &len);
  assert(err == 0);

  NSString *result = [NSString stringWithUTF8String:data];

  free(data);

  return result;
}

static js_value_t *
bare_core_text__from_size(js_env_t *env, CGSize size) {
  int err;

  js_value_t *result;
  err = js_create_object(env, &result);
  assert(err == 0);

#define V(name, n) \
  { \
    js_value_t *val; \
    err = js_create_double(env, n, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, result, name, val); \
    assert(err == 0); \
  }

  V("width", size.width)
  V("height", size.height)
#undef V

  return result;
}
