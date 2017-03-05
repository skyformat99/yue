// Copyright 2017 Cheng Zhao. All rights reserved.
// Use of this source code is governed by the license that can be found in the
// LICENSE file.

#include "v8binding/arguments.h"

#include "base/strings/stringprintf.h"

namespace vb {

namespace {

std::string V8TypeAsString(v8::Isolate* isolate, v8::Local<v8::Value> value) {
  if (value.IsEmpty())
    return "<empty handle>";
  if (value->IsUndefined())
    return "undefined";
  if (value->IsNull())
    return "null";
  std::string result;
  if (!FromV8(isolate->GetCurrentContext(), value, &result))
    return std::string();
  return result;
}

}  // namespace

Arguments::Arguments()
    : isolate_(nullptr),
      info_(nullptr),
      next_(0),
      insufficient_arguments_(false) {
}

Arguments::Arguments(const v8::FunctionCallbackInfo<v8::Value>& info)
    : isolate_(info.GetIsolate()),
      info_(&info),
      next_(0),
      insufficient_arguments_(false) {
}

Arguments::~Arguments() {
}

void Arguments::ThrowError() const {
  if (insufficient_arguments_)
    return ThrowTypeError("Insufficient number of arguments.");

  return ThrowTypeError(base::StringPrintf(
      "Error processing argument at index %d, conversion failure from %s",
      next_ - 1, V8TypeAsString(isolate_, (*info_)[next_ - 1]).c_str()));
}

void Arguments::ThrowTypeError(const std::string& message) const {
  isolate_->ThrowException(v8::Exception::TypeError(
      ToV8(isolate_->GetCurrentContext(), message).As<v8::String>()));
}

}  // namespace vb