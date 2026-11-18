// MagicItemData declarations for build sse-steam-1-7-104.
//
// Labels: v2
// Code origin: declarations extended by hand from the local listings; names follow the community headers.
// Task: p92-s-magicitemdata
// Source exposure: mixed (names and declarations from community headers; base classes, sizes and
// the compared functions from the pinned image).
// Declarations derived from powerof3/CommonLibSSE are GPL-3.0 (its licence).
// Started from a tools/skeleton.py skeleton: stand-ins carry only what the compared code needs, and
// community names are unverified.
// MagicItemData: COMMUNITY(powerof3/CommonLibSSE@654e55f, GPL-3.0) EXE_RTTI
// MagicItemData::GetCount, MagicItemData::GetEnabled: COMMUNITY(powerof3/CommonLibSSE@654e55f, GPL-3.0)
// unknown08: GUESS(low)
// data18: GUESS(low)
// MagicItemData::GetName, MagicItemData::GetEquipState: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT); names only, the bodies are written from the target listing
// PlayerCharacter::singleton: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT); global at RVA 0x3230778
// GetFormNameOf: GUESS(low); RVA 0x1e8460, TESForm::GetFormName reached with the form in the first argument
// GetPlayerEquipState: GUESS(low); RVA 0x940fc0, takes the player, the form and two zero arguments
#pragma once

#include <cstdint>
#include <cstddef>

class PlayerCharacter
{
public:
    static PlayerCharacter* singleton;
};

const char* GetFormNameOf(void* a_form);
std::uint32_t GetPlayerEquipState(PlayerCharacter* a_player, void* a_form, std::int32_t a_arg3, std::int32_t a_arg4);

class MagicItemData
{
public:
    virtual const char* GetName();  // compared: 0x951d00
    virtual std::uint32_t GetEquipState();  // compared: 0x951d10
    virtual std::uint32_t GetCount();  // compared: 0x956d90
    virtual bool GetEnabled();  // compared: 0x951e80

    // Padding up to the pointer read by GetEnabled at offset 0x18.
    std::uint8_t unknown08[0x10];  // 08
    void* data18;  // 18
};
static_assert(sizeof(MagicItemData) == 0x20, "MagicItemData size");
