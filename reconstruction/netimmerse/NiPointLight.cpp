// NiPointLight functions for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-nipointlight
// Code origin: bodies written for this unit from the listings of the pinned image; names and
// declarations from CommonLibSSE-NG, and the Ctor name from powerof3/CommonLibSSE. No community or
// sibling body is adapted.
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
// Declarations derived from powerof3/CommonLibSSE are GPL-3.0-or-later.
// This file is free software: you can redistribute it and/or modify it under the terms
// of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
// This file is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details. A copy is provided in
// licenses/CommonLibSSE-GPL-3.0.txt; see also https://www.gnu.org/licenses/.
// Evidence: findings/fifth-batch-selection.md
// Recipe: reconstruction/recipes/o2-fpfast-root-includes.json (compile this file alone). The fast
// floating-point model, because IsEqual tests float equality without the NaN parity branch that the
// precise model emits; the other functions do no float compare or arithmetic. The recipe follows the
// selection note for this task in findings/fifth-batch-selection.md.
// Drafted by v-macrev, assisted by claude-opus-5-5, starting from
// a tools/skeleton.py skeleton.
// Compared functions:
//   RVA 0xefe1f0, 192 bytes: NiPointLight::LoadBinary
//   RVA 0xefe2e0, 192 bytes: NiPointLight::SaveBinary
//   RVA 0xefe3b0, 102 bytes: NiPointLight::IsEqual
//   RVA 0xefe4e0, 52 bytes: NiPointLight::Ctor
//   RVA 0xefe5f0, 52 bytes: NiPointLight::~NiPointLight
// Adapted bodies: none
// A byte match shows that this source and recipe produce the checked code; it does not verify the
// community names or that the original source looked like this.
#include "NiPointLight.h"

#include <new>

// StreamRead, StreamWrite: GUESS(low); one raw read or write of a member through the stream's
// function pointer, advancing the stream position by the bytes moved.
template <class T>
static inline bool StreamRead(NiBinaryStream* a_in, T* a_value, std::uint32_t a_count)
{
    std::uint32_t size = sizeof(T);
    std::uint32_t toRead = a_count * size;
    std::uint32_t bytesRead = a_in->_readFn(a_in, a_value, toRead, &size, 1);
    a_in->_absoluteCurrentPos += bytesRead;
    return bytesRead == toRead;
}

template <class T>
static inline bool StreamWrite(NiBinaryStream* a_out, const T* a_value, std::uint32_t a_count)
{
    std::uint32_t size = sizeof(T);
    std::uint32_t toWrite = a_count * size;
    std::uint32_t bytesWritten = a_out->_writeFn(a_out, a_value, toWrite, &size, 1);
    a_out->_absoluteCurrentPos += bytesWritten;
    return bytesWritten == toWrite;
}

// RVA 0xefe1f0, 192 bytes.
void NiPointLight::LoadBinary(NiStream& a_stream)
{
    NiLight::LoadBinary(a_stream);
    StreamRead(a_stream.iStr, &constAttenuation, 1);
    StreamRead(a_stream.iStr, &linearAttenuation, 1);
    StreamRead(a_stream.iStr, &quadraticAttenuation, 1);
}

// RVA 0xefe2e0, 192 bytes.
void NiPointLight::SaveBinary(NiStream& a_stream)
{
    NiLight::SaveBinary(a_stream);
    StreamWrite(a_stream.oStr, &constAttenuation, 1);
    StreamWrite(a_stream.oStr, &linearAttenuation, 1);
    StreamWrite(a_stream.oStr, &quadraticAttenuation, 1);
}

// RVA 0xefe3b0, 102 bytes.
bool NiPointLight::IsEqual(NiObject* a_object)
{
    if (!NiLight::IsEqual(a_object)) {
        return false;
    }
    NiPointLight* other = static_cast<NiPointLight*>(a_object);
    if (constAttenuation != other->constAttenuation) {
        return false;
    }
    if (linearAttenuation != other->linearAttenuation) {
        return false;
    }
    if (quadraticAttenuation != other->quadraticAttenuation) {
        return false;
    }
    return true;
}

// Not compared on its own: inlined into Ctor below. Defining it also makes the compiler emit the
// vftable and, beside it, the scalar deleting destructor compared at RVA 0xefe5f0.
NiPointLight::NiPointLight() :
    constAttenuation(0.0f),
    linearAttenuation(1.0f),
    quadraticAttenuation(0.0f)
{
}

// RVA 0xefe4e0, 52 bytes: constructs the light in place.
NiPointLight* NiPointLight::Ctor()
{
    return new (this) NiPointLight;
}
