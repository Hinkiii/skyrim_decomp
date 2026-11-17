// ActorVector: build sse-steam-1-7-104, RVA 0x6a7050, 61 bytes.
// Labels: v2
// Source exposure: mixed; Drafted by gpt-6.1-sol.
// Evidence: findings/ghidra-import.md
// Declaration evidence: original target-bound caller and callee analysis; identities remain inferred.
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2-reconstruction.json
// Origin: original reconstruction; types adapted from pinned public MIT declarations.
// ActorVector: STANDIN GUESS(medium); partial research declaration, not a recovered original name.
// Community names unverified; ordinary receiver and return ABI are hypotheses.
// Only the named entry is compared; declarations have no stand-in bodies.
// No complete object size or ownership is asserted.
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
#include "NiPoint3.h"
namespace P207SolRetry {
// ActorVector: STANDIN GUESS(medium); typed actor receiver prefix; ordinary owner inferred.
class ActorVector : public P206Target::ActorTargetPrefix<> {
public:
    // ResolveController: GUESS(medium); direct helper returns actor-associated movement receiver.
    MovementControllerNPC* ResolveController();
    // ReadVector: GUESS(medium); hidden result pointer inferred from three-float result stores.
    NiPoint3 ReadVector();
};
// VectorX, VectorY, VectorZ: GUESS(low); three external float components, meaning unknown.
extern float VectorX, VectorY, VectorZ;
}
