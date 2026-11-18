// NiSkinData declarations for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-niskindata
// Code origin: names, bases, virtual slots and member offsets from CharmedBaryon/CommonLibSSE-NG b93280e (MIT); the body is written from the listing of the pinned image
// Source exposure: mixed (community declarations and the contributor-owned pack;
// bases, sizes and slots are community hypotheses).
// Sizes and slots are community hypotheses: verify C1-C5 locally before drafting.
// Size agreement alone proves neither fields nor ownership.
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
// The MIT notice above covers only the names and declarations taken from CommonLibSSE-NG.
// Base form: shared header declarations/NiObject.h for NiObject.
// NiSkinData, NiSkinData::RegisterStreamables, ::~NiSkinData, ::GetRTTI, ::LoadBinary, ::LinkObject, ::SaveBinary, ::IsEqual, ::skinPartition, ::rootParentToSkin, ::boneData, ::bones, ::pad5C: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
// NiSkinPartition, NiPointer, NiTransform, NiPoint3, NiSkinData::BoneData: STANDIN COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT); reduced to the layout the class needs (BoneData is only a pointer target)
// NiTransform::rotate, ::translate, ::scale: GUESS(low); NG's NiMatrix3 is stored as nine floats
#pragma once

#include <cstdint>
#include <cstddef>
#include "declarations/NiObject.h"

class NiSkinPartition;

template <class T>
class NiPointer
{
public:
    T* object;  // 00
};

struct NiPoint3
{
    float x;  // 00
    float y;  // 04
    float z;  // 08
};

struct NiTransform
{
    float    rotate[3][3];  // 00
    NiPoint3 translate;     // 24
    float    scale;         // 30
};
static_assert(sizeof(NiTransform) == 0x34, "NiTransform size");

class NiSkinData : public NiObject
{
public:
    struct BoneData;

    ~NiSkinData() override;  // 00
    const NiRTTI* GetRTTI() const override;  // 02
    void LoadBinary(NiStream& a_stream) override;  // 18
    void LinkObject(NiStream& a_stream) override;  // 19
    bool RegisterStreamables(NiStream& a_stream) override;  // 1A - compared: 0xf0db60
    void SaveBinary(NiStream& a_stream) override;  // 1B
    bool IsEqual(NiObject* a_object) override;  // 1C

    NiPointer<NiSkinPartition> skinPartition;     // 10
    NiTransform                rootParentToSkin;  // 18
    BoneData*                  boneData;          // 50
    std::uint32_t              bones;             // 58
    std::uint32_t              pad5C;             // 5C
};
static_assert(sizeof(NiSkinData) == 0x60, "NiSkinData size");
static_assert(offsetof(NiSkinData, boneData) == 0x50, "NiSkinData::boneData offset");
