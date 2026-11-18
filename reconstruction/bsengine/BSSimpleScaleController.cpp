// BSSimpleScaleController functions for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-bssimplescalecontroller
// Code origin: body written by the contributor from the compared function's behaviour; names and
// declarations come from the header (CharmedBaryon/CommonLibSSE-NG, MIT).
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
// The MIT notice above covers only the names and declarations attributed to
// CharmedBaryon/CommonLibSSE-NG in this file, not the rest of the project.
// Evidence: findings/queue-small-upstream-selection.md
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2-root-includes.json (compile this file alone).
// Drafted by Hinkiii, assisted by claude-sonnet-5-5, starting from
// a tools/skeleton.py skeleton.
// Compared functions:
//   RVA 0x7384e0, 60 bytes: BSSimpleScaleController::TargetIsRequiredType
// Adapted bodies: none.
// A byte match shows that this source and recipe produce the checked code; it does not verify the
// community names or that the original source looked like this.
#include "BSSimpleScaleController.h"

// The scale controller only drives NiAVObject targets: walk the target's type chain looking for it.
bool BSSimpleScaleController::TargetIsRequiredType() const
{
    if (target != nullptr) {
        for (const NiRTTI* rtti = target->GetRTTI(); rtti != nullptr; rtti = rtti->parent) {
            if (rtti == &NiRTTI_NiAVObject) {
                return true;
            }
        }
    }
    return false;
}
