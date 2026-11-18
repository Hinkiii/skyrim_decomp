// TESLevSpell declarations for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-teslevspell
// Code origin: hand-written class layouts and declarations adapted from the CharmedBaryon/CommonLibSSE-NG headers named below; the compared bodies are written from the local listings.
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
// Base form: shared header declarations/TESForm.h for TESForm.
// TESObject, TESBoundObject, TESLeveledList, TESLevSpell, LEVELED_OBJECT, SimpleArray, NiNPShortPoint3, TESGlobal: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT)
// TESBoundObject::boundData, TESBoundObject::pad2C, TESLeveledList::entries, TESLeveledList::chanceNone, TESLeveledList::llFlags, TESLeveledList::numEntries, TESLeveledList::unk13, TESLeveledList::pad14, TESLeveledList::unk18, TESLeveledList::chanceGlobal: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT)
// TESLevSpell::SaveGame, TESLevSpell::LoadGame, TESLevSpell::Revert: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT)
// TESLeveledList::SaveGame, TESLeveledList::LoadGame, TESLeveledList::Revert: STANDIN GUESS(low)
// Only the members and virtuals the compared bodies need are declared; the remaining TESObject, TESBoundObject and TESLeveledList virtual slots are omitted.
#pragma once

#include <cstdint>
#include <cstddef>
#include "declarations/TESForm.h"

class TESGlobal;

template <class T>
struct SimpleArray
{
    T* data;  // 00
};
static_assert(sizeof(SimpleArray<int>) == 0x8, "SimpleArray size");

struct LEVELED_OBJECT
{
    TESForm* form;  // 00
    std::uint16_t count;  // 08
    std::uint16_t level;  // 0A
    std::uint32_t pad0C;  // 0C
    void* itemExtra;  // 10
};
static_assert(sizeof(LEVELED_OBJECT) == 0x18, "LEVELED_OBJECT size");

struct NiNPShortPoint3
{
    std::int16_t x;  // 0
    std::int16_t y;  // 2
    std::int16_t z;  // 4
};

class TESObject : public TESForm
{
public:
};
static_assert(sizeof(TESObject) == 0x20, "TESObject size");

class TESBoundObject : public TESObject
{
public:
    struct BOUND_DATA
    {
        NiNPShortPoint3 boundMin;  // 0
        NiNPShortPoint3 boundMax;  // 6
    };
    static_assert(sizeof(BOUND_DATA) == 0xC, "BOUND_DATA size");

    BOUND_DATA boundData;  // 20
    std::uint32_t pad2C;  // 2C
};
static_assert(sizeof(TESBoundObject) == 0x30, "TESBoundObject size");

class TESLeveledList : public BaseFormComponent
{
public:
    ~TESLeveledList() override;  // 00
    virtual bool GetCanContainFormsOfType(FormType a_type) const = 0;  // 07

    void SaveGame(BGSSaveFormBuffer* a_buf);  // address 0x1ea3a0
    void LoadGame(BGSLoadFormBuffer* a_buf);  // address 0x1ea490
    void Revert(BGSLoadFormBuffer* a_buf);  // address 0x1ea6f0

    SimpleArray<LEVELED_OBJECT> entries;  // 08
    std::int8_t chanceNone;  // 10
    std::uint8_t llFlags;  // 11
    std::uint8_t numEntries;  // 12
    std::uint8_t unk13;  // 13
    std::uint32_t pad14;  // 14
    void* unk18;  // 18
    TESGlobal* chanceGlobal;  // 20
};
static_assert(sizeof(TESLeveledList) == 0x28, "TESLeveledList size");

class TESLevSpell : public TESBoundObject, public TESLeveledList
{
public:
    ~TESLevSpell() override;  // 00
    void SaveGame(BGSSaveFormBuffer* a_buf) override;  // 0E, compared: RVA 0x272ee0
    void LoadGame(BGSLoadFormBuffer* a_buf) override;  // 0F, compared: RVA 0x272f20
    void Revert(BGSLoadFormBuffer* a_buf) override;  // 12, compared: RVA 0x272f60
    bool GetCanContainFormsOfType(FormType a_type) const override;  // 07
};
static_assert(sizeof(TESLevSpell) == 0x58, "TESLevSpell size");
static_assert(offsetof(TESLevSpell, entries) == 0x38, "TESLevSpell leveled list members");
