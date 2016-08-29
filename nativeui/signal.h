// Copyright 2016 Cheng Zhao. All rights reserved.
// Use of this source code is governed by the license that can be found in the
// LICENSE file.

#ifndef NATIVEUI_SIGNAL_H_
#define NATIVEUI_SIGNAL_H_

#include <list>
#include <utility>

#include "base/callback.h"

namespace nu {

// A simple signal/slot implementation.
template<typename Sig> class Signal;

template<typename... Args>
class Signal<void(Args...)> {
 public:
  using Slot = base::Callback<void(Args...)>;
  using Ref = typename std::list<Slot>::iterator;

  Ref Connect(const Slot& slot) {
    slots_.push_back(slot);
    return --slots_.end();
  }

  void Remove(Ref ref) {
    slots_.erase(ref);
  }

  template<typename... RunArgs>
  void Emit(RunArgs&&... args) {
    for (Slot& slot : slots_)
      slot.Run(std::forward(args)...);
  }

 private:
  std::list<Slot> slots_;
};

}  // namespace nu

#endif  // NATIVEUI_SIGNAL_H_
