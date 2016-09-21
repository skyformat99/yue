// Copyright 2016 Cheng Zhao. All rights reserved.
// Use of this source code is governed by the license that can be found in the
// LICENSE file.

#include "nativeui/label.h"

#import <Cocoa/Cocoa.h>

#include "base/strings/sys_string_conversions.h"
#include "nativeui/gfx/geometry/size_conversions.h"
#include "nativeui/gfx/mac/painter_mac.h"
#include "nativeui/gfx/text.h"
#include "third_party/css-layout/CSSLayout/CSSLayout.h"

@interface LabelView : NSView {
 @private
  std::string text_;
  nu::Color background_color_;
}
- (void)setText:(const std::string&)text;
- (std::string)text;
- (void)setBackgroundColor:(nu::Color)color;
@end

@implementation LabelView

- (void)setText:(const std::string&)text {
  text_ = text;
  [self setNeedsDisplay:YES];
}

- (std::string)text {
  return text_;
}

- (void)setBackgroundColor:(nu::Color)color {
  background_color_ = color;
  [self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)dirtyRect {
  nu::PainterMac painter;
  painter.FillRect(nu::RectF(dirtyRect), background_color_);
  painter.DrawStringWithFlags(text_, nu::Font(), nu::Color(),
                              nu::RectF([self frame]),
                              nu::Painter::TextAlignCenter);
}

@end

namespace nu {

namespace {

SizeF GetPreferredSizeForText(const std::string& text) {
  SizeF size = MeasureText(Font(), text);
  size.Enlarge(1, 1);  // leave space for border
  return size;
}

}  // namespace

Label::Label(const std::string& text) {
  TakeOverView([[LabelView alloc] init]);
  SetText(text);
}

Label::~Label() {
}

void Label::SetText(const std::string& text) {
  [static_cast<LabelView*>(view()) setText:text];
  SetDefaultStyle(GetPreferredSizeForText(text));
}

std::string Label::GetText() {
  return [static_cast<LabelView*>(view()) text];
}

void Label::SetBackgroundColor(Color color) {
  [static_cast<LabelView*>(view()) setBackgroundColor:color];
}

}  // namespace nu
