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
// data18: GUESS(low)
// MagicItemData::GetName, MagicItemData::GetEquipState: COMMUNITY(powerof3/CommonLibSSE@654e55f7979c1733033d5dbef176e43827bac944, GPL-3.0); names only, the bodies are written from the target listing
// PlayerCharacter::singleton: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT); global at address 0x3230778
// GetFormNameOf: GUESS(low); address 0x1e8460, TESForm::GetFormName reached with the form in the first argument
// GetPlayerEquipState: GUESS(low); address 0x940fc0, takes the player, the form and two zero arguments
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
// unk08, unk10: GUESS(low); storage before data18, not read by the compared code
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

    // Members before the pointer read by GetEnabled at offset 0x18.
    std::uint64_t unk08;  // 08
    std::uint64_t unk10;  // 10
    void* data18;  // 18
};
static_assert(sizeof(MagicItemData) == 0x20, "MagicItemData size");
