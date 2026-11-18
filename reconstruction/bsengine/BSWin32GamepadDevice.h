// BSWin32GamepadDevice declarations for build sse-steam-1-7-104.
//
// Labels: v2
// Code origin: declarations extended by hand from the local listings; names follow the community headers.
// Task: p92-s-bswin32gamepaddevice
// Source exposure: mixed (names and declarations from community headers; base classes, sizes and
// the compared functions from the pinned image).
// Started from a tools/skeleton.py skeleton: stand-ins carry only what the compared code needs, and
// community names are unverified.
// Base form: local stand-ins (no ancestor has a shared header).
// BSWin32GamepadDevice: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT) EXE_RTTI
// BSIInputDevice, BSInputDevice, BSGamepadDevice, BSPCGamepadDeviceDelegate: STANDIN EXE_RTTI
// BSWin32GamepadDevice::Release, BSWin32GamepadDevice::SetRumble: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
// BSIInputDevice::~BSIInputDevice: GUESS(low)
// BSWin32GamepadDevice::Reset, BSWin32GamepadDevice::Initialize: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT); names only, the bodies are written from the target listing
// g_gamepadManager: GUESS(low); global pointer at address 0x31a5690 that Initialize calls through
// GamepadManager, Unk_ceed20: GUESS(low); address 0xceed20, called on that pointer with zero
// state: GUESS(low); the 0x50-byte buffer at offset 0xD8 that Reset and Initialize clear as a whole
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
// unk08, unk10, unk18, unk20, unk28, unk30, unk38, unk40, unk48, unk50, unk58, unk60, unk68: GUESS(low); storage not read by the compared code
// unk70, unk78, unk80, unk88, unk90, unk98, unkA0, unkA8, unkB0, unkB8, unkC0, unkC8: GUESS(low); storage not read by the compared code
// unkD0: GUESS(low); storage not read by the compared code
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
    std::uint64_t unk08;  // 08
    std::uint64_t unk10;  // 10
    std::uint64_t unk18;  // 18
    std::uint64_t unk20;  // 20
    std::uint64_t unk28;  // 28
    std::uint64_t unk30;  // 30
    std::uint64_t unk38;  // 38
    std::uint64_t unk40;  // 40
    std::uint64_t unk48;  // 48
    std::uint64_t unk50;  // 50
    std::uint64_t unk58;  // 58
    std::uint64_t unk60;  // 60
    std::uint64_t unk68;  // 68
};
static_assert(sizeof(BSInputDevice) == 0x70, "BSInputDevice size");

class BSGamepadDevice : public BSInputDevice
{
public:
    std::uint64_t unk70;  // 70
    std::uint64_t unk78;  // 78
    std::uint64_t unk80;  // 80
    std::uint64_t unk88;  // 88
    std::uint64_t unk90;  // 90
    std::uint64_t unk98;  // 98
    std::uint64_t unkA0;  // A0
    std::uint64_t unkA8;  // A8
    std::uint64_t unkB0;  // B0
    std::uint64_t unkB8;  // B8
    std::uint64_t unkC0;  // C0
    std::uint64_t unkC8;  // C8
};
static_assert(sizeof(BSGamepadDevice) == 0xD0, "BSGamepadDevice size");

class BSPCGamepadDeviceDelegate : public BSGamepadDevice
{
public:
    std::uint64_t unkD0;  // D0
};
static_assert(sizeof(BSPCGamepadDeviceDelegate) == 0xD8, "BSPCGamepadDeviceDelegate size");

class GamepadManager
{
public:
    void Unk_ceed20(std::int32_t a_value);
};
extern GamepadManager* g_gamepadManager;

class BSWin32GamepadDevice : public BSPCGamepadDeviceDelegate
{
public:
    virtual void Release();  // compared: 0xe7b050
    virtual void SetRumble(float lValue, float rValue);  // compared: 0xe7b080
    virtual void Reset();  // compared: 0xe7b060
    virtual void Initialize();  // compared: 0xe7ad60

    std::uint8_t state[0x50];  // D8
};
static_assert(sizeof(BSWin32GamepadDevice) == 0x128, "BSWin32GamepadDevice size");
