// ActorSpeedBlend: STANDIN GUESS(low); target-bound partial receiver and callee views; original names remain hypotheses.
// Drafted by gpt-6.1-terra; publication metadata normalized by gpt-6.1-sol.
// Compared: RVA 0x692670, 57 bytes; RVA 0x6926b0, 57 bytes.
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2-reconstruction.json
// Origin: original reconstruction; adapted declarations retain their cited source notices.
// ActorSpeedBlend: build sse-steam-1-7-104, RVAs 0x692670 and 0x6926b0, 57 bytes each.
// Labels: v2
// Source exposure: mixed; drafted by gpt-6.1-terra.
// Evidence: findings/ghidra-import.md. Method declarations from CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT.
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
#pragma once
namespace P2071 {
// ActorSpeedBlend: STANDIN GUESS(medium); incomplete actor receiver, layout not asserted.
class ActorSpeedBlend { public:
    // LowerSpeed, UpperSpeed: GUESS(low); direct callee names inferred.
    float LowerSpeed();
    float UpperSpeed();
    // GetJogSpeed, GetFastWalkSpeed: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT)
    float GetJogSpeed();
    float GetFastWalkSpeed();
};
// JogBlend, FastWalkBlend: GUESS(low); named for use, not identified global ownership.
extern float JogBlend;
extern float FastWalkBlend;
}
