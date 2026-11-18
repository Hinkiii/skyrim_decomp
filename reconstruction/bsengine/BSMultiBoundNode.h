// BSMultiBoundNode declarations for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-bsmultiboundnode
// Code origin: names, bases, slot order and member layout adapted from CharmedBaryon/CommonLibSSE-NG (MIT)
// BSMultiBoundNode.h and NiNode.h; declarations written by the contributor, bodies original.
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
// The MIT notice above covers only the names and declarations attributed to
// CharmedBaryon/CommonLibSSE-NG in this file, not the rest of the project.
// Started from a tools/skeleton.py skeleton: stand-ins carry only what the compared code needs, and
// community names are unverified.
// Base form: shared header declarations/NiNode.h for NiNode.
// BSMultiBoundNode, BSMultiBoundNode::IsEqual, BSMultiBoundNode::~BSMultiBoundNode, BSMultiBoundNode::GetRTTI, BSMultiBoundNode::AsMultiBoundNode, BSMultiBoundNode::CreateClone, BSMultiBoundNode::LoadBinary, BSMultiBoundNode::LinkObject, BSMultiBoundNode::RegisterStreamables, BSMultiBoundNode::SaveBinary, BSMultiBoundNode::UpdateDownwardPass, BSMultiBoundNode::UpdateWorldBound, BSMultiBoundNode::OnVisible, BSMultiBoundNode::UpdateUpwardPass, BSMultiBoundNode::GetMultiBoundRoom, BSMultiBoundNode::QPointWithin, BSMultiBoundNode::Unk_40, BSMultiBoundNode::Unk_41, BSNiNode::~BSNiNode, BSNiNode::GetRTTI, BSNiNode::CreateClone: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
// BSNiNode, BSMultiBoundRoom, NiPoint3: STANDIN COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
// multiBound, cullingMode, lastAccumTime: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT)
#pragma once

#include <cstdint>
#include <cstddef>
#include "declarations/NiNode.h"

class BSMultiBoundRoom;
class NiPoint3;

class BSNiNode : public NiNode
{
public:
    ~BSNiNode() override;  // 00
    const NiRTTI* GetRTTI() const override;  // 02
    NiObject* CreateClone(NiCloningProcess& a_cloning) override;  // 17
};
static_assert(sizeof(BSNiNode) == 0x128, "BSNiNode size");

class BSMultiBoundNode : public BSNiNode
{
public:
    ~BSMultiBoundNode() override;  // 00
    const NiRTTI* GetRTTI() const override;  // 02
    BSMultiBoundNode* AsMultiBoundNode() override;  // 06
    NiObject* CreateClone(NiCloningProcess& a_cloning) override;  // 17
    void LoadBinary(NiStream& a_stream) override;  // 18
    void LinkObject(NiStream& a_stream) override;  // 19
    bool RegisterStreamables(NiStream& a_stream) override;  // 1A
    void SaveBinary(NiStream& a_stream) override;  // 1B
    virtual bool IsEqual(NiObject* a_object);  // compared: 0xff2bc0  // slot 1C
    void UpdateDownwardPass(NiUpdateData& a_data, std::uint32_t a_arg2) override;  // 2C
    void UpdateWorldBound() override;  // 2F
    void OnVisible(NiCullingProcess& a_process) override;  // 34
    void UpdateUpwardPass(NiUpdateData& a_data) override;  // 3D
    virtual BSMultiBoundRoom* GetMultiBoundRoom();  // 3E
    virtual bool QPointWithin(const NiPoint3& a_point);  // 3F
    virtual void Unk_40(void);  // 40
    virtual void Unk_41(void);  // 41

    class BSMultiBound* multiBound;  // 128, NG: NiPointer<BSMultiBound>
    std::uint32_t cullingMode;  // 130, NG: BSCullingProcess::BSCPCullingType
    float lastAccumTime;  // 134
};
static_assert(sizeof(BSMultiBoundNode) == 0x138, "BSMultiBoundNode size");
static_assert(offsetof(BSMultiBoundNode, multiBound) == 0x128, "multiBound offset");
