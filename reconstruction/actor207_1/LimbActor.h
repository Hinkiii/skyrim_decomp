// LimbActor: STANDIN GUESS(low); partial target-bound receiver/callee view, no complete layout or ownership asserted.
// Origin: original reconstruction; cited adapted declarations retain complete notices.
// Drafted by gpt-6.1-terra; publication metadata normalized by gpt-6.1-sol.
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2-reconstruction.json
// Compared: RVA 0x2ef760, 43 bytes.
// LimbActor: build sse-steam-1-7-104, RVA 0x2ef760, size 43.
// Labels: v2
// Source exposure: mixed; drafted by gpt-6.1-terra.
// Evidence: findings/ghidra-import.md. Prefix adapted from CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT.
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
#include "actors/ActorTargetPrefix.h"
#include <cstdint>
namespace P2071 {
// LimbActor: STANDIN GUESS(medium); partial actor receiver.
class LimbActor : public P206Target::ActorTargetPrefix<> { public:
    // IsLimbGone: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT)
    bool IsLimbGone(std::uint32_t);
};
// LimbData: STANDIN GUESS(low); helper result identity unknown.
class LimbData;
// FindLimbData, CheckLimb: GUESS(low); helper signatures and meaning inferred.
LimbData* FindLimbData(P206Target::ExtraDataListPrefix*);
bool CheckLimb(LimbData*, std::uint32_t);
}
