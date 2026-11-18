// BGSDecalNode declarations for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-bgsdecalnode
// Code origin: original drafting by Hinkiii with claude-sonnet-5-5 from a local listing of the pinned image; names, members and declarations
// from CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a (MIT); no body is copied from any source.
// Source exposure: mixed (community declarations and the contributor-owned pack;
// bases, sizes and slots are community hypotheses).
// Sizes and slots are community hypotheses: verify C1-C5 locally before drafting.
// Size agreement alone proves neither fields nor ownership.
// The MIT licence below covers only the names and declarations taken from CommonLibSSE-NG, not any other content of this file.
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
// BGSDecalNode, BGSDecalNode::CreateClone, BGSDecalNode::AttachDecal, BGSDecalNode::~BGSDecalNode, BGSDecalNode::GetRTTI, BGSDecalNode::ProcessClone, BGSDecalNode::OnVisible, BGSDecalNode::Unk_3E: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT)
// BSTempEffect, BSTArray, NiPointer: STANDIN COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT)
// BSTArray::_data, BSTArray::_capacity, BSTArray::_size, BGSDecalNode::decals, BGSDecalNode::isSkinned, BSTempEffect::Get3D: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT)
// NiPointer::_ptr, BSTArray::_pad0C, BSTArray::_pad14: GUESS(low) STANDIN
// CreateNodeInstance, g_nodeInstanceOwner: GUESS(low) STANDIN
#pragma once

#include <cstdint>
#include <cstddef>
#include "declarations/NiNode.h"

class NiAVObject;

template <class T>
class NiPointer
{
public:
    T* _ptr;  // 00
};

// Layout of NG's BSTArray with the default heap allocator: data and capacity first, then the element count.
template <class T>
class BSTArray
{
public:
    T* _data;  // 00
    std::uint32_t _capacity;  // 08
    std::uint32_t _pad0C;  // 0C
    std::uint32_t _size;  // 10
    std::uint32_t _pad14;  // 14
};

// Only the slots up to Get3D are declared; the decal reads the node through it.
class BSTempEffect : public NiObject
{
public:
    virtual void Initialize();  // 25
    virtual void Attach();  // 26
    virtual void Detach();  // 27
    virtual bool Update(float a_arg1);  // 28
    virtual NiAVObject* Get3D() const;  // 29
};

class BGSDecalNode : public NiNode
{
public:
    ~BGSDecalNode() override;  // 00
    const NiRTTI* GetRTTI() const override;  // 02
    NiObject* CreateClone(NiCloningProcess& a_cloning) override;  // 17, compared: 0x2030b0
    void ProcessClone(NiCloningProcess& a_cloning) override;  // 1D
    void OnVisible(NiCullingProcess& a_process) override;  // 34
    virtual std::uint8_t Unk_3E();  // 3E
    virtual void AttachDecal(BSTempEffect* a_decal, bool a_firstAvail);  // 3F, compared: 0x203360

    BSTArray<NiPointer<BSTempEffect>> decals;  // 128
    bool isSkinned;  // 140
};
static_assert(sizeof(BGSDecalNode) == 0x148, "BGSDecalNode size");
static_assert(offsetof(BGSDecalNode, isSkinned) == 0x140, "isSkinned offset");

// address 0x1ff7e0: routine CreateClone tail-calls with the global below (names and argument meaning are guesses)
NiObject* CreateNodeInstance(void* a_owner, void* a_arg1, void* a_arg2);
extern void* g_nodeInstanceOwner;  // address 0x21a3df8
