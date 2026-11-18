// BSSimpleScaleController declarations for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-bssimplescalecontroller
// Code origin: names, bases, slot order and member layout adapted from CharmedBaryon/CommonLibSSE-NG (MIT)
// BSSimpleScaleController.h and NiTimeController.h; declarations written by the contributor, body original.
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
// Base form: shared header declarations/NiObject.h for NiObject.
// BSSimpleScaleController, BSSimpleScaleController::TargetIsRequiredType, BSSimpleScaleController::~BSSimpleScaleController, BSSimpleScaleController::GetRTTI, BSSimpleScaleController::CreateClone, BSSimpleScaleController::Update, NiTimeController::~NiTimeController, NiTimeController::GetRTTI, NiTimeController::LoadBinary, NiTimeController::LinkObject, NiTimeController::RegisterStreamables, NiTimeController::SaveBinary, NiTimeController::IsEqual, NiTimeController::ProcessClone, NiTimeController::Start, NiTimeController::Stop, NiTimeController::Update, NiTimeController::SetTarget, NiTimeController::IsTransformController, NiTimeController::IsVertexController, NiTimeController::ComputeScaledTime, NiTimeController::OnPreDisplay, NiTimeController::IsStreamable, NiTimeController::TargetIsRequiredType: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
// NiTimeController, NiObjectNET: STANDIN COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
// NiTimeController::flags, frequency, phase, loKeyTime, hiKeyTime, startTime, lastTime, weightedLastTime, scaledTime, target, next, BSSimpleScaleController::scale3D, refScale, scaleAnimDuration, NiRTTI::name, NiRTTI_NiAVObject: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT)
// NiRTTI::parent: OBSERVED; NiRTTI_NiAVObject is the runtime type object that the image's NiNode type object names as its parent
#pragma once

#include <cstdint>
#include <cstddef>
#include "declarations/NiObject.h"
#include "declarations/NiObjectNET.h"

class NiObjectNET;

// Runtime type descriptor: a name and the parent type, which the kind-of walk follows.
class NiRTTI
{
public:
    const char* name;  // 00
    const NiRTTI* parent;  // 08
};
static_assert(sizeof(NiRTTI) == 0x10, "NiRTTI size");

// Runtime type object for NiAVObject (address 0x331c4a8), defined elsewhere.
extern NiRTTI NiRTTI_NiAVObject;

class NiTimeController : public NiObject
{
public:
    ~NiTimeController() override;  // 00
    const NiRTTI* GetRTTI() const override;  // 02
    void LoadBinary(NiStream& a_stream) override;  // 18
    void LinkObject(NiStream& a_stream) override;  // 19
    bool RegisterStreamables(NiStream& a_stream) override;  // 1A
    void SaveBinary(NiStream& a_stream) override;  // 1B
    bool IsEqual(NiObject* a_object) override;  // 1C
    void ProcessClone(NiCloningProcess& a_cloning) override;  // 1D
    virtual void Start(float a_time);  // 25
    virtual void Stop();  // 26
    virtual void Update(float a_time) = 0;  // 27
    virtual void SetTarget(NiObjectNET* a_target);  // 28
    virtual bool IsTransformController() const;  // 29
    virtual bool IsVertexController() const;  // 2A
    virtual float ComputeScaledTime(float a_time);  // 2B
    virtual void OnPreDisplay();  // 2C
    virtual bool IsStreamable() const;  // 2D
    virtual bool TargetIsRequiredType() const = 0;  // 2E
    std::uint16_t flags;  // 10
    std::uint16_t pad12;  // 12
    float frequency;  // 14
    float phase;  // 18
    float loKeyTime;  // 1C
    float hiKeyTime;  // 20
    float startTime;  // 24
    float lastTime;  // 28
    float weightedLastTime;  // 2C
    float scaledTime;  // 30
    std::uint32_t pad34;  // 34
    NiObjectNET* target;  // 38
    NiTimeController* next;  // 40, NG: NiPointer<NiTimeController>
};
static_assert(sizeof(NiTimeController) == 0x48, "NiTimeController size");

class BSSimpleScaleController : public NiTimeController
{
public:
    ~BSSimpleScaleController() override;  // 00
    const NiRTTI* GetRTTI() const override;  // 02
    NiObject* CreateClone(NiCloningProcess& a_cloning) override;  // 17
    void Update(float a_time) override;  // 27
    bool TargetIsRequiredType() const override;  // 2E, compared: 0x7384e0

    float scale3D;  // 48
    float refScale;  // 4C
    float scaleAnimDuration;  // 50
    std::uint32_t pad54;  // 54
};
static_assert(sizeof(BSSimpleScaleController) == 0x58, "BSSimpleScaleController size");
static_assert(offsetof(NiTimeController, target) == 0x38, "target offset");
