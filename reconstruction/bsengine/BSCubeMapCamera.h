// BSCubeMapCamera declarations for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-bscubemapcamera
// Code origin: hand-written class layout and declarations adapted from the powerof3/CommonLibSSE headers named below (GPL-3.0); the compared body is written from the local listing.
// Source exposure: mixed (community declarations and the contributor-owned pack;
// bases, sizes and slots are community hypotheses).
// Sizes and slots are community hypotheses; size agreement alone proves neither fields nor ownership.
// Declarations derived from powerof3/CommonLibSSE are GPL-3.0 (its licence).
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
//
// Declarations derived from powerof3/CommonLibSSE are GPL-3.0-or-later.
// This file is free software: you can redistribute it and/or modify it under the terms
// of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
// This file is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details. A copy is provided in
// licenses/CommonLibSSE-GPL-3.0.txt; see also https://www.gnu.org/licenses/.
// Started from a tools/skeleton.py skeleton: stand-ins carry only what the compared code needs, and
// community names are unverified.
// Base form: shared header declarations/NiAVObject.h for NiAVObject; NiCamera is declared here with its community members.
// BSCubeMapCamera, BSCubeMapCamera::RenderCubemap, BSCubeMapCamera::~BSCubeMapCamera, BSCubeMapCamera::GetRTTI: COMMUNITY(powerof3/CommonLibSSE@654e55f, GPL-3.0)
// NiRTTI: STANDIN COMMUNITY(powerof3/CommonLibSSE@654e55f, GPL-3.0)
// NiCamera, NiFrustum, NiRect, NiCamera::worldToCam, viewFrustum, minNearPlaneDist, maxFarNearRatio, port, lodAdjust: COMMUNITY(powerof3/CommonLibSSE@654e55f, GPL-3.0)
// FaceEnableFlags, BSTArray, NiColorA, BSShaderAccumulator, NiAVObject, BSCubeMapCamera::scenes, accumulator, secondAccumulator, unk1B0, silhouetteColor: COMMUNITY(powerof3/CommonLibSSE@654e55f, GPL-3.0)
#pragma once

#include <cstdint>
#include <cstddef>
#include "declarations/NiAVObject.h"

class NiRTTI;

struct NiFrustum
{
    float fLeft;  // 00
    float fRight;  // 04
    float fTop;  // 08
    float fBottom;  // 0C
    float fNear;  // 10
    float fFar;  // 14
    bool bOrtho;  // 18
};
static_assert(sizeof(NiFrustum) == 0x1C, "NiFrustum size");

struct NiRectFloat
{
    float left;  // 00
    float right;  // 04
    float top;  // 08
    float bottom;  // 0C
};

class NiCamera : public NiAVObject
{
public:
    ~NiCamera() override;  // 00
    const NiRTTI* GetRTTI() const override;  // 02

    float worldToCam[4][4];  // 110
    NiFrustum viewFrustum;  // 150
    float minNearPlaneDist;  // 16C
    float maxFarNearRatio;  // 170
    NiRectFloat port;  // 174
    float lodAdjust;  // 184
};
static_assert(sizeof(NiCamera) == 0x188, "NiCamera size");

class BSShaderAccumulator;
class NiAVObject;

enum class FaceEnableFlags : std::uint32_t
{
    kEnablePosX = 1 << 0,
    kEnableNegX = 1 << 1,
    kEnablePosY = 1 << 2,
    kEnableNegY = 1 << 3,
    kEnablePosZ = 1 << 4,
    kEnableNegZ = 1 << 5,
    kEnableAll = 0x3F
};

struct BSTArrayOfNiAVObject
{
    NiAVObject** data;  // 00
    std::uint32_t capacity;  // 08
    std::uint32_t pad0C;  // 0C
    std::uint32_t size;  // 10
    std::uint32_t pad14;  // 14
};
static_assert(sizeof(BSTArrayOfNiAVObject) == 0x18, "BSTArray size");

struct NiColorA
{
    float r;  // 00
    float g;  // 04
    float b;  // 08
    float a;  // 0C
};

class BSCubeMapCamera : public NiCamera
{
public:
    ~BSCubeMapCamera() override;  // 00
    const NiRTTI* GetRTTI() const override;  // 02
    virtual void RenderCubemap(FaceEnableFlags a_faceMask, bool a_arg2, bool a_clearScenes, bool a_arg4);  // compared: 0x1559a60  // slot 35

    BSTArrayOfNiAVObject scenes;  // 188
    BSShaderAccumulator* accumulator;  // 1A0
    BSShaderAccumulator* secondAccumulator;  // 1A8
    NiCamera* unk1B0;  // 1B0
    NiColorA silhouetteColor;  // 1B8
};
static_assert(sizeof(BSCubeMapCamera) == 0x1C8, "BSCubeMapCamera size");
static_assert(offsetof(BSCubeMapCamera, silhouetteColor) == 0x1B8, "BSCubeMapCamera members");
