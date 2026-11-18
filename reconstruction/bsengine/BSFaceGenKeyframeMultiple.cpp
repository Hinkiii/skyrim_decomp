// BSFaceGenKeyframeMultiple functions for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-bsfacegenkeyframemultiple
// Code origin: GetModifierName, Unk_08 and Unk_09 bodies written from the local listings; declarations added to the existing unit header.
// Source exposure: mixed (names and declarations from community headers; the listing of the pinned
// image).
// Declarations derived from powerof3/CommonLibSSE are GPL-3.0.
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
// Declarations derived from powerof3/CommonLibSSE are GPL-3.0.
// This file is free software: you can redistribute it and/or modify it under the terms
// of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
// This file is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details. A copy is provided in
// licenses/CommonLibSSE-GPL-3.0.txt; see also https://www.gnu.org/licenses/.
// Evidence: findings/p92-unit-supply-selection.md
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2.json (compile this file alone).
// Drafted by Hinkiii, assisted by claude-sonnet-5-5, starting from
// a tools/skeleton.py skeleton.
// Compared functions:
//   RVA 0x43acc0, 23 bytes: BSFaceGenKeyframeMultiple::GetModifierName
//   RVA 0x43aca0, 23 bytes: BSFaceGenKeyframeMultiple::GetExpressionName
//   RVA 0x43ace0, 23 bytes: BSFaceGenKeyframeMultiple::GetPhonemeName
//   RVA 0x432080, 39 bytes: BSFaceGenKeyframeMultiple::Unk_0E
//   RVA 0x432030, 68 bytes: BSFaceGenKeyframeMultiple::Unk_0B
// Adapted bodies: none
// A byte match shows that this source and recipe produce the checked code; it does not verify the
// community names or that the original source looked like this.
#include "BSFaceGenKeyframeMultiple.h"
const char* BSFaceGenKeyframeMultiple::GetModifierName(std::uint32_t a_index)
{
    if (a_index <= 0x10) {
        const std::int32_t tableIndex = static_cast<std::int32_t>(a_index);
        return g_modifierNameTable[tableIndex];
    }
    return nullptr;
}

float BSFaceGenKeyframeMultiple::Unk_08()
{
    float result = 0.0f;
    for (std::uint32_t i = 0; i < count; ++i) {
        if (Unk_0E(i)) {
            const float weight = values[i];
            result = weight > result ? weight : result;
        }
    }
    return result;
}

bool BSFaceGenKeyframeMultiple::Unk_09()
{
    for (std::uint32_t i = 0; i < count; ++i) {
        if (Unk_0E(i) && !(values[i] <= 0.0f)) {
            return false;
        }
    }
    return true;
}

// RVA 0x43ad00, 22 bytes: only index 0 has a name.


// RVA 0x43aca0, 23 bytes: indices up to 16 have names.
const char* BSFaceGenKeyframeMultiple::GetExpressionName(std::uint32_t a_index)
{
    if (a_index <= 0x10) {
        // The public unsigned input has been bounded; its signed table index is representable.
        const std::int32_t tableIndex = static_cast<std::int32_t>(a_index);
        return g_expressionNameTable[tableIndex];
    }
    return nullptr;
}

// RVA 0x43ace0, 23 bytes: indices up to 15 have names.
const char* BSFaceGenKeyframeMultiple::GetPhonemeName(std::uint32_t a_index)
{
    if (a_index <= 0xF) {
        // The public unsigned input has been bounded; its signed table index is representable.
        const std::int32_t tableIndex = static_cast<std::int32_t>(a_index);
        return g_phonemeNameTable[tableIndex];
    }
    return nullptr;
}

// RVA 0x432080, 39 bytes: the weight at the index is in [0, 1].
bool BSFaceGenKeyframeMultiple::Unk_0E(std::uint32_t a_index)
{
    if (a_index < count) {
        float weight = values[a_index];
        if (weight >= 0.0f) {
            if (weight > 1.0f) {
                return false;
            }
            return true;
        }
    }
    return false;
}

// RVA 0x432030, 68 bytes: true when some weight is in (0, 1].
bool BSFaceGenKeyframeMultiple::Unk_0B()
{
    for (std::uint32_t i = 0; i < count; ++i) {
        float weight = values[i];
        if (weight >= 0.0f) {
            if (weight > 1.0f) {
                continue;
            }
            if (weight > 0.0f) {
                return true;
            }
        }
    }
    return false;
}

// RVA 0x431da0, 100 bytes: false when a valid index has a positive weight.


// RVA 0x431d20, 108 bytes: the largest weight among the valid indices, or zero.


// Accepted leaf retained from the integrated version.
// Unk_0C: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
std::uint8_t BSFaceGenKeyframeMultiple::Unk_0C() const
{
    return 0x1;
}
