// NiParticles declarations for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-niparticles
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
// Base form: shared header declarations/NiAVObject.h for NiAVObject.
// NiParticles, NiParticles::AsParticlesGeom, NiParticles::RegisterStreamables, NiParticles::~NiParticles, NiParticles::GetRTTI, NiParticles::CreateClone, NiParticles::LoadBinary, NiParticles::LinkObject, NiParticles::SaveBinary, NiParticles::IsEqual, BSGeometry::~BSGeometry, BSGeometry::GetRTTI, BSGeometry::AsGeometry, BSGeometry::LoadBinary, BSGeometry::LinkObject, BSGeometry::RegisterStreamables, BSGeometry::SaveBinary, BSGeometry::IsEqual, BSGeometry::ProcessClone, BSGeometry::PostLinkObject, BSGeometry::AttachProperty, BSGeometry::SetSelectiveUpdateFlags, BSGeometry::UpdateDownwardPass, BSGeometry::UpdateSelectedDownwardPass, BSGeometry::UpdateRigidDownwardPass, BSGeometry::UpdateWorldBound, BSGeometry::OnVisible, BSGeometry::AsMultiIndexTriShape, BSGeometry::AsSkinnedDecalTriShape, BSGeometry::Unk_37: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
// BSGeometry, BSMultiIndexTriShape, BSSkinnedDecalTriShape: STANDIN COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
// NiParticlesData, NiBound, NiPoint3, NiProperty, NiSkinInstance: STANDIN COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
// BSGeometry::modelBound, properties, skinInstance, rendererData, unk20, vertexDesc, type, pad31, pad32, pad34, NiParticles::particleData, NiParticles::unk160: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
#pragma once

#include <cstdint>
#include <cstddef>
#include "declarations/NiAVObject.h"

class NiProperty;
class NiSkinInstance;
class BSMultiIndexTriShape;
class BSSkinnedDecalTriShape;

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

class NiParticlesData : public NiObject
{
public:
    bool RegisterStreamables(NiStream& a_stream) override;  // 1A
};

class BSGeometry : public NiAVObject
{
public:
    ~BSGeometry() override;  // 00
    const NiRTTI* GetRTTI() const override;  // 02
    BSGeometry* AsGeometry() override;  // 07
    void LoadBinary(NiStream& a_stream) override;  // 18
    void LinkObject(NiStream& a_stream) override;  // 19
    bool RegisterStreamables(NiStream& a_stream) override;  // 1A
    void SaveBinary(NiStream& a_stream) override;  // 1B
    bool IsEqual(NiObject* a_object) override;  // 1C
    void ProcessClone(NiCloningProcess& a_cloning) override;  // 1D
    void PostLinkObject(NiStream& a_stream) override;  // 1E
    void AttachProperty(NiAlphaProperty* a_property) override;  // 27
    void SetSelectiveUpdateFlags(bool& a_selectiveUpdate, bool a_selectiveUpdateTransforms, bool& a_rigid) override;  // 2B
    void UpdateDownwardPass(NiUpdateData& a_data, std::uint32_t a_arg2) override;  // 2C
    void UpdateSelectedDownwardPass(NiUpdateData& a_data, std::uint32_t a_arg2) override;  // 2D
    void UpdateRigidDownwardPass(NiUpdateData& a_data, std::uint32_t a_arg2) override;  // 2E
    void UpdateWorldBound() override;  // 2F
    void OnVisible(NiCullingProcess& a_process) override;  // 34
    virtual BSMultiIndexTriShape* AsMultiIndexTriShape();  // 35
    virtual BSSkinnedDecalTriShape* AsSkinnedDecalTriShape();  // 36
    virtual void Unk_37(void);  // 37
    NiBound modelBound;  // 110
    NiProperty* properties[2];  // 120
    NiSkinInstance* skinInstance;  // 130
    void* rendererData;  // 138
    void* unk20;  // 140
    std::uint64_t vertexDesc;  // 148
    std::uint8_t type;  // 150
    std::uint8_t pad31;  // 151
    std::uint16_t pad32;  // 152
    std::uint32_t pad34;  // 154
};
static_assert(sizeof(BSGeometry) == 0x158, "BSGeometry size");

class NiParticles : public BSGeometry
{
public:
    ~NiParticles() override;  // 00
    const NiRTTI* GetRTTI() const override;  // 02
    virtual NiParticles* AsParticlesGeom();  // compared: 0xf055a0  // slot 10
    NiObject* CreateClone(NiCloningProcess& a_cloning) override;  // 17
    void LoadBinary(NiStream& a_stream) override;  // 18
    void LinkObject(NiStream& a_stream) override;  // 19
    virtual bool RegisterStreamables(NiStream& a_stream);  // compared: 0xf05220  // slot 1A
    void SaveBinary(NiStream& a_stream) override;  // 1B
    bool IsEqual(NiObject* a_object) override;  // 1C

    NiParticlesData* particleData;  // 158
    std::uint64_t unk160;  // 160
};
static_assert(sizeof(NiParticles) == 0x168, "NiParticles size");
static_assert(offsetof(NiParticles, particleData) == 0x158, "NiParticles members");
