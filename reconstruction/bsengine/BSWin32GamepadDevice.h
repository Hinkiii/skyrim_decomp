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
// BSIInputDevice::~BSIInputDevice, unknown08, unknown70, unknownD0, unknownD8: GUESS(low)
// BSWin32GamepadDevice::Reset, BSWin32GamepadDevice::Initialize: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT); names only, the bodies are written from the target listing
// g_gamepadManager: GUESS(low); global pointer at RVA 0x31a5690 that Initialize calls through
// GamepadManager, Unk_ceed20: GUESS(low); RVA 0xceed20, called on that pointer with zero
// state: GUESS(low); the 0x50 bytes at offset 0xD8 that Reset and Initialize clear
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

class BSGamepadDevice : public BSInputDevice
{
public:
    std::uint8_t unknown70[0x60];  // 70
};
static_assert(sizeof(BSGamepadDevice) == 0xD0, "BSGamepadDevice size");

class BSPCGamepadDeviceDelegate : public BSGamepadDevice
{
public:
    std::uint8_t unknownD0[0x8];  // D0
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
