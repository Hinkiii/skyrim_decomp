// AIProcess207Pass: STANDIN GUESS(low); partial target-bound receiver/callee view, no complete layout or ownership asserted.
// Origin: original reconstruction; cited adapted declarations retain complete notices.
// Drafted by gpt-6.1-terra; publication metadata normalized by gpt-6.1-sol.
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2-reconstruction.json
// Compared: RVA 0x7023a0, 18 bytes; RVA 0x6e8370, 44 bytes.
// AIProcess207Pass: build sse-steam-1-7-104; 0x6ff050 (66), 0x7023a0 (18), 0x6e8370 (44), 0x720660 (51).
// Labels: v2
// Source exposure: mixed; drafted by gpt-6.1-terra. Evidence: findings/ghidra-import.md.
// Partial declarations adapted from CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT, AIProcess.h, HighProcessData.h, MiddleHighProcessData.h, InventoryEntryData.h, BSPointerHandle.h.
// Unaccessed storage is unknown; no complete allocation size, ownership or semantic name for high+0x1e4 is asserted.
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
#include <cstddef>
#include <cstdint>
#include "actors/ActorValueTargetPrefix.h"
namespace P2071 {
// ObjectRefHandle: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT)
// Handle destructor reflects the public nontrivial handle ABI; the value representation and reset behavior are hypotheses for this target.
struct ObjectRefHandle {
    std::uint32_t value;
    ~ObjectRefHandle() { value = 0; }
};
// HighProcessPrefix: STANDIN GUESS(medium); partial target-bound prefix, no full-layout claim.
struct HighProcessPrefix {
    std::uint8_t unknown000[0x140];
    // headTrackTarget, headTracked: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT)
    ObjectRefHandle headTrackTarget[6];
    bool headTracked[6];
    std::uint8_t unknown15e[0x86];
    // unknown1e4: GUESS(low); target float access only, semantic identity unknown.
    float unknown1e4;
};
// InventoryFormPrefix, InventoryEntryPrefix, MiddleHighPrefix: STANDIN GUESS(medium); partial representations, no pointer ownership claimed.
struct InventoryFormPrefix { P206Target::FormPrefix form; };
struct InventoryEntryPrefix { InventoryFormPrefix* object; };
struct MiddleHighPrefix {
    std::uint8_t unknown000[0x220];
    // leftHand, rightHand: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT)
    InventoryEntryPrefix* leftHand;
    std::uint8_t unknown228[0x38];
    InventoryEntryPrefix* rightHand;
};
// AIProcess207Pass: STANDIN GUESS(medium); target process prefix and receiver, allocation extent not asserted.
struct AIProcess207Pass {
    P206Target::MiddleLowProcessData* middleLow;
    MiddleHighPrefix* middleHigh;
    HighProcessPrefix* high;
    P206Target::ActorPackage currentPackage;
    float unknown48;
    std::uint32_t unknown4c;
    // GetHeadtrackTarget: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT)
    ObjectRefHandle GetHeadtrackTarget() const;
    // SetHighFloat, UpdateTimeBits, GetWeaponEntry: GUESS(low); semantic identities and parameter names inferred.
    void SetHighFloat(float);
    void UpdateTimeBits();
    InventoryEntryPrefix* GetWeaponEntry(bool);
};
// InvalidHandle, TimeFloat, TimeHigh, TimeLow, TimeMiddle: GUESS(low); external values and semantics unconfirmed.
extern std::uint32_t InvalidHandle, TimeHigh, TimeMiddle;
extern std::int8_t TimeLow;
extern float TimeFloat;
static_assert(offsetof(AIProcess207Pass, high)==0x10, "target high pointer");
static_assert(offsetof(AIProcess207Pass, unknown48)==0x48, "target time float");
static_assert(offsetof(AIProcess207Pass, unknown4c)==0x4c, "target time bits");
static_assert(offsetof(HighProcessPrefix, headTrackTarget)==0x140, "target head handles");
static_assert(offsetof(HighProcessPrefix, headTracked)==0x158, "target head flags");
static_assert(offsetof(HighProcessPrefix, unknown1e4)==0x1e4, "target float");
static_assert(offsetof(MiddleHighPrefix, leftHand)==0x220, "target left entry");
static_assert(offsetof(MiddleHighPrefix, rightHand)==0x260, "target right entry");
}
