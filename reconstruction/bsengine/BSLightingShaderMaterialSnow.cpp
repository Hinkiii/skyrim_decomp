// BSLightingShaderMaterialSnow functions for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-bslightingshadermaterialsnow
// Code origin: SaveBinary and LoadBinary bodies written from the local listings; declarations added to the existing unit header.
// Source exposure: mixed (names and declarations from community headers; the listing of the pinned
// image).
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
// Evidence: findings/p92-unit-supply-selection.md
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2.json (compile this file alone).
// Drafted by Hinkiii, assisted by claude-sonnet-5-5, starting from
// a tools/skeleton.py skeleton.
// Compared functions:
//   RVA 0x1528120, 46 bytes: BSLightingShaderMaterialSnow::SaveBinary
//   RVA 0x1528160, 46 bytes: BSLightingShaderMaterialSnow::LoadBinary
//   RVA 0x1528110, 6 bytes: BSLightingShaderMaterialSnow::GetFeature
//   RVA 0x1528090, 46 bytes: BSLightingShaderMaterialSnow::CopyMembers
//   RVA 0x1527fc0, 47 bytes: BSLightingShaderMaterialSnow::Ctor
//   RVA 0x15280d0, 55 bytes: BSLightingShaderMaterialSnow::ComputeCRC32
// Adapted bodies: none
// A byte match shows that this source and recipe produce the checked code; it does not verify the
// community names or that the original source looked like this.
#include "BSLightingShaderMaterialSnow.h"
void BSLightingShaderMaterialSnow::SaveBinary(NiStream& a_stream)
{
    BSLightingShaderMaterialBase::SaveBinary(a_stream);
    unknownA0.SaveStream(a_stream);
}
void BSLightingShaderMaterialSnow::LoadBinary(NiStream& a_stream)
{
    BSLightingShaderMaterialBase::LoadBinary(a_stream);
    unknownA0.LoadStream(a_stream);
}
#include <new>

// SnowDefaults: STANDIN GUESS(low)
using SnowDefaults = BSLightingShaderMaterialSnow::SnowData;
// g_snowDefaults: GUESS(low)
extern SnowDefaults g_snowDefaults;
// BaseCopyHelper: GUESS(low)
void BaseCopyHelper(BSLightingShaderMaterialBase* a_this, BSShaderMaterial* a_src);
// SnowCRCSeed: GUESS(low)
std::uint32_t SnowCRCSeed();
// CRC32Size: GUESS(low)
void CRC32Size(std::uint32_t* a_out, const void* a_data, std::uint32_t a_size);

BSShaderMaterial::Feature BSLightingShaderMaterialSnow::GetFeature() const
{
    return static_cast<Feature>(0xE);
}

void BSLightingShaderMaterialSnow::CopyMembers(BSShaderMaterial* a_src)
{
    BaseCopyHelper(this, a_src);
    unknownA0 = static_cast<BSLightingShaderMaterialSnow*>(a_src)->unknownA0;
}

BSLightingShaderMaterialSnow::BSLightingShaderMaterialSnow() :
    unknownA0(g_snowDefaults)
{}

BSLightingShaderMaterialSnow* BSLightingShaderMaterialSnow::Ctor()
{
    return new (this) BSLightingShaderMaterialSnow;
}

std::uint32_t BSLightingShaderMaterialSnow::ComputeCRC32()
{
    struct SnowBlock {
        SnowData data;
        std::uint32_t seed;
    } block{unknownA0, SnowCRCSeed()};
    std::uint32_t result;
    CRC32Size(&result, &block, 0x14);
    return result;
}
