#import <CoreText/CoreText.h>

#import "bridging.h"

// A layout engine measures the same text at several widths, so the
// framesetter is kept for as long as the text does not change.
@interface BareFramesetter : NSObject {
@public
  CTFramesetterRef framesetter;
  NSAttributedString *text;
}

@end

@implementation BareFramesetter

- (void)dealloc {
  if (framesetter != NULL) CFRelease(framesetter);

  [text release];

  [super dealloc];
}

- (void)setText:(NSAttributedString *)value {
  if (text != nil && [text isEqualToAttributedString:value]) return;

  if (framesetter != NULL) CFRelease(framesetter);

  [text release];

  text = [value retain];
  framesetter = CTFramesetterCreateWithAttributedString((CFAttributedStringRef) value);
}

@end

static js_value_t *
bare_core_text_framesetter_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, NULL, NULL, NULL, (void **) &registry);
  assert(err == 0);

  js_value_t *result;

  @autoreleasepool {
    result = bare_foundation_bridge(env, registry, [[[BareFramesetter alloc] init] autorelease]);
  }

  return result;
}

// The tallest font in the string. It gives the frame enough room that no line
// is cut off before it is counted.
static CGFloat
bare_core_text__line_height(NSAttributedString *text) {
  __block CGFloat result = 0;

  [text enumerateAttribute:(__bridge NSString *) kCTFontAttributeName
                   inRange:NSMakeRange(0, text.length)
                   options:0
                usingBlock:^(id value, NSRange range, BOOL *stop) {
                   if (value == nil || CFGetTypeID((__bridge CFTypeRef) value) != CTFontGetTypeID()) return;

                   CTFontRef font = (__bridge CTFontRef) value;

                   CGFloat height = CTFontGetAscent(font) + CTFontGetDescent(font) + CTFontGetLeading(font);

                   if (height > result) result = height;
                 }];

  return result;
}

// The line count is reported instead of a final height, because each control
// places its lines on whole pixels in its own way.
static CFIndex
bare_core_text__lines(CTFramesetterRef framesetter, CGSize measured, CGFloat line_height) {
  CGPathRef path = CGPathCreateWithRect(
    CGRectMake(0, 0, measured.width, measured.height + line_height * 2), NULL);

  CTFrameRef frame = CTFramesetterCreateFrame(framesetter, CFRangeMake(0, 0), path, NULL);

  CFIndex result = CFArrayGetCount(CTFrameGetLines(frame));

  CFRelease(frame);
  CGPathRelease(path);

  return result;
}

// The font arrives as a family and a size rather than as an object, so that
// measuring one string at many widths allocates nothing.
static js_value_t *
bare_core_text_framesetter_measure(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 6;
  js_value_t *argv[6];

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry);
  assert(err == 0);

  assert(argc == 6);

  void *handle;
  if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL;

  double size;
  if (!bare_core_text__read_double(env, argv[3], "size", &size)) return NULL;

  double width;
  if (!bare_core_text__read_double(env, argv[4], "width", &width)) return NULL;

  double height;
  if (!bare_core_text__read_double(env, argv[5], "height", &height)) return NULL;

  js_value_t *result;

  @autoreleasepool {
    BareFramesetter *framesetter = (__bridge BareFramesetter *) handle;

    NSString *string = bare_core_text__to_string(env, argv[1]);
    NSString *family = bare_core_text__to_string(env, argv[2]);

    // No family means the system font, because that is what a control draws
    // with when no font is set.
    CTFontRef font = family
      ? CTFontCreateWithName((__bridge CFStringRef) family, size, NULL)
      : CTFontCreateUIFontForLanguage(kCTFontUIFontSystem, size, NULL);

    NSDictionary *attributes = @{(__bridge NSString *) kCTFontAttributeName: (__bridge id) font};

    [framesetter setText:[[[NSAttributedString alloc]
      initWithString:string ? string : @"" attributes:attributes] autorelease]];

    CGSize measured = CTFramesetterSuggestFrameSizeWithConstraints(
      framesetter->framesetter, CFRangeMake(0, 0), NULL, CGSizeMake(width, height), NULL);

    CGFloat line_height = CTFontGetAscent(font) + CTFontGetDescent(font) + CTFontGetLeading(font);

    CFIndex lines = bare_core_text__lines(framesetter->framesetter, measured, line_height);

    measured.width = ceil(measured.width);

    CFRelease(font);

    result = bare_core_text__from_size(env, measured);

    js_value_t *count;
    err = js_create_int32(env, (int32_t) lines, &count);
    assert(err == 0);

    err = js_set_named_property(env, result, "lines", count);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_core_text_framesetter_measure_attributed(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 4;
  js_value_t *argv[4];

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry);
  assert(err == 0);

  assert(argc == 4);

  void *handle;
  if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL;

  void *text;
  if (bare_foundation_read_tag(env, registry, argv[1], "text", &text) < 0) return NULL;

  double width;
  if (!bare_core_text__read_double(env, argv[2], "width", &width)) return NULL;

  double height;
  if (!bare_core_text__read_double(env, argv[3], "height", &height)) return NULL;

  js_value_t *result;

  @autoreleasepool {
    BareFramesetter *framesetter = (__bridge BareFramesetter *) handle;

    NSAttributedString *string = (__bridge NSAttributedString *) text;

    [framesetter setText:string];

    CGSize measured = CTFramesetterSuggestFrameSizeWithConstraints(
      framesetter->framesetter, CFRangeMake(0, 0), NULL, CGSizeMake(width, height), NULL);

    CFIndex lines = bare_core_text__lines(
      framesetter->framesetter, measured, bare_core_text__line_height(string));

    measured.width = ceil(measured.width);

    result = bare_core_text__from_size(env, measured);

    js_value_t *count;
    err = js_create_int32(env, (int32_t) lines, &count);
    assert(err == 0);

    err = js_set_named_property(env, result, "lines", count);
    assert(err == 0);
  }

  return result;
}
