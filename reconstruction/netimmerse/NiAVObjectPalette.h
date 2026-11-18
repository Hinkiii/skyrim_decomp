// NiAVObjectPalette declarations for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-niavobjectpalette
// Code origin: names, base, slot order and size adapted from CharmedBaryon/CommonLibSSE-NG (MIT)
// NiAVObjectPalette.h and NiObject.h; declarations written by the contributor, body original.
// Source exposure: mixed (community declarations and the contributor-owned pack;
// bases, sizes and slots are community hypotheses).
// Sizes and slots are community hypotheses: verify C1-C5 locally before drafting.
// Size agreement alone proves neither fields nor ownership.
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
// The MIT notice above covers only the names and declarations attributed to
// CharmedBaryon/CommonLibSSE-NG in this file, not the rest of the project.
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
// Base form: shared header declarations/NiObject.h for NiObject.
// NiAVObjectPalette, NiAVObjectPalette::RegisterStreamables, NiAVObjectPalette::~NiAVObjectPalette, NiAVObjectPalette::GetRTTI, NiAVObjectPalette::LoadBinary, NiAVObjectPalette::LinkObject, NiAVObjectPalette::SaveBinary, NiAVObjectPalette::GetAVObject, NiAVObjectPalette::SetAVObject, NiAVObjectPalette::ResetAndFillFromScenegraph: COMMUNITY(powerof3/CommonLibSSE@654e55f, GPL-3.0)
// NiAVObject, BSFixedString: STANDIN COMMUNITY(powerof3/CommonLibSSE@654e55f, GPL-3.0)
#pragma once

#include <cstdint>
#include <cstddef>
#include "declarations/NiObject.h"

class NiAVObject;
class BSFixedString;

class NiAVObjectPalette : public NiObject
{
public:
    ~NiAVObjectPalette() override;  // 00
    const NiRTTI* GetRTTI() const override;  // 02
    void LoadBinary( NiStream& a_stream) override;  // 18
    void LinkObject( NiStream& a_stream) override;  // 19
    void SaveBinary( NiStream& a_stream) override;  // 1B
    virtual NiAVObject* GetAVObject(BSFixedString& a_name) = 0;  // 25
    virtual void SetAVObject(BSFixedString& a_name, NiAVObject* a_object) = 0;  // 26
    virtual void ResetAndFillFromScenegraph();  // 27
    bool RegisterStreamables(NiStream& a_stream) override;  // 1A, compared: 0xf0a920
};
static_assert(sizeof(NiAVObjectPalette) == 0x10, "NiAVObjectPalette size");
