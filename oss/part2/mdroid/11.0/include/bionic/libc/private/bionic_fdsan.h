/*
 * Copyright (C) 2018 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#pragma once

#include <android/fdsan.h>

#include <errno.h>
#include <stdatomic.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/user.h>

struct FdEntry {
  _Atomic(uint64_t) close_tag = _Atomic(uint64_t)NULL;//STDLIBC_INTEGER
};

struct FdTableOverflow {
  size_t len = 0;
  FdEntry entries[0];
};

template <size_t inline_fds>
struct FdTableImpl {
  constexpr FdTableImpl() {}

  uint32_t version = 0;  // currently 0, and hopefully it'll stay that way.
  _Atomic(android_fdsan_error_level) error_level = ANDROID_FDSAN_ERROR_LEVEL_DISABLED;

  FdEntry entries[inline_fds];
  _Atomic(FdTableOverflow*) overflow = nullptr;

  FdEntry* at(size_t idx);
};

using FdTable = FdTableImpl<128>;
