// BGSAttackDataMap declarations for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-bgsattackdatamap
// Code origin: hand-written class layout and declarations adapted from the CharmedBaryon/CommonLibSSE-NG headers named below; the compared body is written from the local listing.
// Source exposure: mixed (community declarations and the contributor-owned pack;
// bases, sizes and slots are community hypotheses).
// Sizes and slots are community hypotheses; size agreement alone proves neither fields nor ownership.
// The MIT licence below covers only the names and declarations attributed to CommonLibSSE-NG here.
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
// Started from a tools/skeleton.py skeleton: stand-ins carry only what the compared code needs, and
// community names are unverified.
// Base form: shared header declarations/NiRefObject.h for NiRefObject.
// BGSAttackDataMap, BGSAttackDataMap::GetAttackData, BGSAttackDataMap::~BGSAttackDataMap: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
// BSTHashMap, BGSAttackData, BSFixedString, TESRace, BGSAttackDataMap::attackDataMap, BGSAttackDataMap::defaultDataRace: STANDIN COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
// BSTHashMap::_pad00, BSTHashMap::_pad08, BSTHashMap::_capacity, BSTHashMap::_free, BSTHashMap::_good, BSTHashMap::_sentinel, BSTHashMap::_allocator, BSTHashMap::_entries: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT); BSTScatterTable and its allocator flattened, non-template
// BGSAttackDataMap::GetAttackData parameters and result: GUESS(low); the body only forwards its arguments
#pragma once

#include <cstdint>
#include <cstddef>
#include "declarations/NiRefObject.h"

class BGSAttackData;
class BSFixedString;
class TESRace;

// Non-template stand-in for BSTHashMap<BSFixedString, NiPointer<BGSAttackData>> with the community layout.
struct BSTHashMapAttackData
{
    std::uint64_t pad00;  // 00
    std::uint32_t pad08;  // 08
    std::uint32_t capacity;  // 0C
    std::uint32_t free;  // 10
    std::uint32_t good;  // 14
    const void* sentinel;  // 18
    std::uint64_t allocatorPad00;  // 20
    void* entries;  // 28
};
static_assert(sizeof(BSTHashMapAttackData) == 0x30, "BSTHashMapAttackData size");

class BGSAttackDataMap : public NiRefObject
{
public:
    ~BGSAttackDataMap() override;  // 00
    BGSAttackData* GetAttackData(const BSFixedString& a_event);  // compared: 0x44ab10

    BSTHashMapAttackData attackDataMap;  // 10
    TESRace* defaultDataRace;  // 40
};
static_assert(sizeof(BGSAttackDataMap) == 0x48, "BGSAttackDataMap size");
static_assert(offsetof(BGSAttackDataMap, attackDataMap) == 0x10, "BGSAttackDataMap members");
