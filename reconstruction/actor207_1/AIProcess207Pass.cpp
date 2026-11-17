// AIProcess207Pass: STANDIN GUESS(low); partial target-bound receiver/callee view, no complete layout or ownership asserted.
// Origin: original reconstruction; cited adapted declarations retain complete notices.
// Drafted by gpt-6.1-terra; publication metadata normalized by gpt-6.1-sol.
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2-reconstruction.json
// Compared: RVA 0x7023a0, 18 bytes; RVA 0x6e8370, 44 bytes.
// AIProcess207: build sse-steam-1-7-104; 0x6ff050 (66), 0x7023a0 (18), 0x6e8370 (44), 0x720660 (51).
// Labels: v2
// Source exposure: mixed; drafted by gpt-6.1-terra. Evidence: findings/ghidra-import.md.
// Partial declarations adapted from CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT, AIProcess.h, HighProcessData.h, MiddleHighProcessData.h, InventoryEntryData.h, BSPointerHandle.h.
// Unaccessed storage is unknown; no complete allocation size, ownership or semantic name for high+0x1e4 is asserted.
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
#include "AIProcess207Pass.h"
namespace P2071 {
void AIProcess207Pass::SetHighFloat(float value) {
    if (high) high->unknown1e4 = value;
}
void AIProcess207Pass::UpdateTimeBits() {
    unknown48 = TimeFloat;
    unknown4c = ((TimeHigh << 4) | TimeMiddle) << 9 | static_cast<std::int32_t>(TimeLow);
}
}
