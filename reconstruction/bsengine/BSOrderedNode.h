// BSOrderedNode declarations for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-bsorderednode
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
// Started from a tools/skeleton.py skeleton: stand-ins carry only what the compared code needs, and
// community names are unverified.
// Base form: shared header declarations/NiNode.h for NiNode.
// BSOrderedNode, BSOrderedNode::RegisterStreamables, BSOrderedNode::OnVisible, BSOrderedNode::~BSOrderedNode, BSOrderedNode::GetRTTI, BSOrderedNode::CreateClone, BSOrderedNode::LoadBinary, BSOrderedNode::LinkObject, BSOrderedNode::SaveBinary, BSOrderedNode::IsEqual, BSOrderedNode::UpdateDownwardPass, BSOrderedNode::UpdateSelectedDownwardPass, BSNiNode::~BSNiNode, BSNiNode::GetRTTI, BSNiNode::CreateClone: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
// BSNiNode: STANDIN COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
// NiBound, NiPoint3: STANDIN COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
// BSOrderedNode::orderedBound, BSOrderedNode::useStaticBound, BSOrderedNode::pad13C: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
#pragma once

#include <cstdint>
#include <cstddef>
#include "declarations/NiNode.h"

struct NiPoint3
{
    float x;  // 0
    float y;  // 4
    float z;  // 8
};

struct NiBound
{
    NiPoint3 center;  // 00
    float radius;  // 0C
};
static_assert(sizeof(NiBound) == 0x10, "NiBound size");

class BSNiNode : public NiNode
{
public:
    ~BSNiNode() override;  // 00
    const NiRTTI* GetRTTI() const override;  // 02
    NiObject* CreateClone(NiCloningProcess& a_cloning) override;  // 17
};
static_assert(sizeof(BSNiNode) == 0x128, "BSNiNode size");

class BSOrderedNode : public BSNiNode
{
public:
    ~BSOrderedNode() override;  // 00
    const NiRTTI* GetRTTI() const override;  // 02
    NiObject* CreateClone(NiCloningProcess& a_cloning) override;  // 17
    void LoadBinary(NiStream& a_stream) override;  // 18
    void LinkObject(NiStream& a_stream) override;  // 19
    virtual bool RegisterStreamables(NiStream& a_stream);  // compared: 0x1570370  // slot 1A
    void SaveBinary(NiStream& a_stream) override;  // 1B
    bool IsEqual(NiObject* a_object) override;  // 1C
    void UpdateDownwardPass(NiUpdateData& a_data, std::uint32_t a_arg2) override;  // 2C
    void UpdateSelectedDownwardPass(NiUpdateData& a_data, std::uint32_t a_arg2) override;  // 2D
    virtual void OnVisible(NiCullingProcess& a_process);  // compared: 0x1570700  // slot 34

    NiBound orderedBound;  // 128
    bool useStaticBound;  // 138
    std::uint32_t pad13C;  // 13C
};
static_assert(sizeof(BSOrderedNode) == 0x140, "BSOrderedNode size");
static_assert(offsetof(BSOrderedNode, orderedBound) == 0x128, "BSOrderedNode members");
