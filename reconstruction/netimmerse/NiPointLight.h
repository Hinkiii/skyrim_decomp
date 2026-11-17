// NiPointLight declarations for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-nipointlight
// Code origin: declarations written for this unit from CommonLibSSE-NG names and layouts (the Ctor
// name from powerof3/CommonLibSSE); the NiObject, NiLight, NiStream and NiBinaryStream stand-ins
// declare only what the compared code needs.
// Source exposure: mixed (community declarations and the contributor-owned pack;
// bases, sizes and slots are community hypotheses).
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
// Base form: NiLight, a local stand-in of the community NiLight 0x140 size over a minimal NiObject;
// the NiObject, NiObjectNET and NiAVObject layers between them are not modelled, and the virtual
// slot order is not modelled (every base call in the compared code is direct, bound by address).
// NiPointLight, NiPointLight::LoadBinary, NiPointLight::~NiPointLight, NiPointLight::SaveBinary, NiPointLight::IsEqual, NiLight::~NiLight, NiLight::LoadBinary, NiLight::SaveBinary, NiLight::IsEqual: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
#pragma once

#include <cstdint>
#include <cstddef>

// NiBinaryStream: STANDIN COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT); only the members the compared code reads
class NiBinaryStream
{
public:
    // ReadFn, WriteFn: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
    using ReadFn = std::uint32_t(NiBinaryStream* a_this, void* a_buffer, std::uint32_t a_bytes, std::uint32_t* a_componentSizes, std::uint32_t a_numComponents);
    using WriteFn = std::uint32_t(NiBinaryStream* a_this, const void* a_buffer, std::uint32_t a_bytes, std::uint32_t* a_componentSizes, std::uint32_t a_numComponents);

    virtual ~NiBinaryStream();  // 00

    // _absoluteCurrentPos, _readFn, _writeFn: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
    std::uint32_t _absoluteCurrentPos;  // 08
    // pad0C: GUESS(low)
    std::uint32_t pad0C;  // 0C
    ReadFn* _readFn;  // 10
    WriteFn* _writeFn;  // 18
};
static_assert(sizeof(NiBinaryStream) == 0x20, "NiBinaryStream size");

// NiStream: STANDIN COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT); a prefix view up to the two stream pointers
class NiStream
{
public:
    // unk000: GUESS(low); members not used here
    std::uint8_t unk000[0x298];  // 000
    // iStr, oStr: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
    NiBinaryStream* iStr;  // 298
    NiBinaryStream* oStr;  // 2A0
};

// NiObject: STANDIN COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT); only the polymorphic root IsEqual takes
class NiObject
{
public:
    virtual ~NiObject();
};

// NiLight: STANDIN COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT); the 0x140 community size
class NiLight : public NiObject
{
public:
    // NiLight::NiLight: GUESS(low); the base constructor the compared constructor calls, declared only, bound by address
    NiLight();
    // NiLight::~NiLight: declared only
    ~NiLight() override;
    // LoadBinary, SaveBinary, IsEqual: declared only; the compared overrides call them directly, bound by address
    virtual void LoadBinary(NiStream& a_stream);
    virtual void SaveBinary(NiStream& a_stream);
    virtual bool IsEqual(NiObject* a_object);

    // unk008: GUESS(low); the NiAVObject and NiLight members, not recovered
    std::uint8_t unk008[0x138];  // 008
};
static_assert(sizeof(NiLight) == 0x140, "NiLight size");

class NiPointLight : public NiLight
{
public:
    // NiPointLight::NiPointLight: GUESS(low); not compared on its own, inlined into Ctor
    NiPointLight();
    // NiPointLight::Ctor: COMMUNITY(powerof3/CommonLibSSE@654e55f, GPL-3.0); constructs in place
    NiPointLight* Ctor();  // compared: 0xefe4e0
    // the complete destructor ??1NiPointLight: GUESS(low); declared only, bound by address to the
    // destructor the listing calls
    ~NiPointLight() override;  // compared: 0xefe5f0 (scalar deleting destructor)
    void LoadBinary(NiStream& a_stream) override;  // compared: 0xefe1f0
    void SaveBinary(NiStream& a_stream) override;  // compared: 0xefe2e0
    bool IsEqual(NiObject* a_object) override;  // compared: 0xefe3b0

    // constAttenuation, linearAttenuation, quadraticAttenuation: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
    float constAttenuation;  // 140
    float linearAttenuation;  // 144
    float quadraticAttenuation;  // 148
    // unk14C: GUESS(low); padding up to the 0x150 size
    std::uint32_t unk14C;  // 14C
};
static_assert(sizeof(NiPointLight) == 0x150, "NiPointLight size");
