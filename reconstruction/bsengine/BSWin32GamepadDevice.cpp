// BSWin32GamepadDevice functions for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-bswin32gamepaddevice
// Code origin: Reset and Initialize bodies written from the local listings; declarations and stand-in layout added to the existing unit header.
// Source exposure: mixed (names and declarations from community headers; the listing of the pinned
// image).
// MIT License
//
// Copyright (c) 2018 Ryan-rsm-McKenzie
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// Evidence: findings/queue-refill-held-selection.md
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2.json (compile this file alone).
// Drafted by Hinkiii, assisted by claude-sonnet-5-5, starting from
// a tools/skeleton.py skeleton.
// Compared functions:
//   RVA 0xe7b060, 18 bytes: BSWin32GamepadDevice::Reset
//   RVA 0xe7ad60, 46 bytes: BSWin32GamepadDevice::Initialize
//   RVA 0xe7b050, 3 bytes: BSWin32GamepadDevice::Release
//   RVA 0xe7b080, 3 bytes: BSWin32GamepadDevice::SetRumble
// Adapted bodies: none
// A byte match shows that this source and recipe produce the checked code; it does not verify the
// community names or that the original source looked like this.
#include "BSWin32GamepadDevice.h"
#include <cstring>
void BSWin32GamepadDevice::Reset() { std::memset(state, 0, sizeof(state)); }
void BSWin32GamepadDevice::Initialize()
{
    g_gamepadManager->Unk_ceed20(0);
    std::memset(state, 0, sizeof(state));
}

// RVA 0xe7b050, 3 bytes
void BSWin32GamepadDevice::Release()
{
}

// RVA 0xe7b080, 3 bytes
void BSWin32GamepadDevice::SetRumble(float lValue, float rValue)
{
}
