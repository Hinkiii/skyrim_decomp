// BSWin32KeyboardDevice declarations for build sse-steam-1-7-104.
//
// Labels: v2
// Code origin: declarations extended by hand from the local listings; names follow the community headers.
// Task: p92-s-bswin32keyboarddevice
// Source exposure: mixed (names and declarations from community headers; base classes, sizes and
// the compared functions from the pinned image).
// Started from a tools/skeleton.py skeleton: stand-ins carry only what the compared code needs, and
// community names are unverified.
// Base form: local stand-ins (no ancestor has a shared header).
// BSWin32KeyboardDevice: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT) EXE_RTTI
// BSIInputDevice, BSInputDevice, BSKeyboardDevice: STANDIN EXE_RTTI
// BSWin32KeyboardDevice::Unk_09, BSWin32KeyboardDevice::Unk_0A: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
// BSIInputDevice::~BSIInputDevice, unknown08, unknown70: GUESS(low)
// BSWin32KeyboardDevice::Reset, BSWin32KeyboardDevice::Release, BSDirectInputManager::ReleaseDevice: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT); names only, the bodies are written from the target listing
// g_directInputManager: GUESS(low); global pointer at address 0x32309d0, the manager the device is released through
// device, keyState: GUESS(low); members at 0x70 and 0x168 read by the compared bodies
// The MIT notice below covers only the names and declarations taken from
// CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a; the other code in this file
// is covered by the repository licence and the provenance labels above.
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
#pragma once

#include <cstdint>
#include <cstddef>

class BSIInputDevice
{
public:
    virtual ~BSIInputDevice() = default;
};
static_assert(sizeof(BSIInputDevice) == 0x8, "BSIInputDevice size");

class BSInputDevice : public BSIInputDevice
{
public:
    std::uint8_t unknown08[0x68];  // 08
};
static_assert(sizeof(BSInputDevice) == 0x70, "BSInputDevice size");

class BSKeyboardDevice : public BSInputDevice
{
public:
};
static_assert(sizeof(BSKeyboardDevice) == 0x70, "BSKeyboardDevice size");

class BSDirectInputManager
{
public:
    void ReleaseDevice(void* a_device);
};
extern BSDirectInputManager* g_directInputManager;

class BSWin32KeyboardDevice : public BSKeyboardDevice
{
public:
    virtual void Unk_09();  // compared: 0xe12c20
    virtual void Unk_0A();  // compared: 0xe12c30
    virtual void Reset();  // compared: 0xe12c00
    virtual void Release();  // compared: 0xe12bd0

    void* device;                  // 70
    std::uint8_t unknown78[0xF0];  // 78
    std::uint8_t keyState[0x200];  // 168
    std::uint8_t unknown368[0x8];  // 368
};
static_assert(sizeof(BSWin32KeyboardDevice) == 0x370, "BSWin32KeyboardDevice size");
