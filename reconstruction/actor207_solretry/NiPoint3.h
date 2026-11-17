// MotionTypes: build sse-steam-1-7-104, RVA 0x6a6b20 / 0x6a7050, size 62 / 61.
// Labels: v2
// Source exposure: mixed; Drafted by gpt-6.1-sol.
// Evidence: findings/ghidra-import.md
// Declaration evidence: original target-bound caller and callee analysis; identities remain inferred.
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2-reconstruction.json
// Origin: original reconstruction; types adapted from pinned public MIT declarations.
// NiPoint3: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT)
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
namespace P207SolRetry {
// P207SolRetry: GUESS(low); namespace isolates private partial declarations.
// NiPoint3, x, y, z: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT)
class NiPoint3 {
public:
    constexpr NiPoint3(float a_x, float a_y, float a_z) noexcept : x(a_x), y(a_y), z(a_z) {}
    float x, y, z;
};
// MovementControllerNPC: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT)
class MovementControllerNPC {
public:
    // Forward: GUESS(low); direct receiver, point and three scalar argument ABI inferred.
    void Forward(const NiPoint3&, float, float, float);
};
}
