// BSOrderedNode functions for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-bsorderednode
// Code origin: hand-written from the local listings; names and declarations from the community header named in BSOrderedNode.h.
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
// Evidence: findings/p92-unit-supply-selection.md
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2-root-includes.json (compile this file alone).
// Drafted by Hinkiii, assisted by claude-sonnet-5-5, starting from
// a tools/skeleton.py skeleton.
// Compared functions:
//   RVA 0x1570370, 19 bytes: BSOrderedNode::RegisterStreamables
//   RVA 0x1570700, 23 bytes: BSOrderedNode::OnVisible
// Adapted bodies: none
// A byte match shows that this source and recipe produce the checked code; it does not verify the
// community names or that the original source looked like this.
#include <intrin.h>
#include "BSOrderedNode.h"

// NiNode::OnVisible as called with the culling process and the visit index taken from the process counter.
// BSOrderedNode::OnVisible: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT)
// NiNodeOnVisibleIndexed: STANDIN GUESS(low); the target passes a third argument, called at address 0xee2cc0
void NiNodeOnVisibleIndexed(NiNode* a_node, NiCullingProcess& a_process, int a_visitIndex);

// NiNodeRegisterStreamables: STANDIN GUESS(low); the target tests the low byte of the result, so the callee at address 0xee1650 is declared with a byte result
char NiNodeRegisterStreamables(NiNode* a_node, NiStream& a_stream);

// The culling process is a BSCullingProcess; the community header names its member at this offset unk301F0.
// BSCullingProcess::unk301F0: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT)
static const std::size_t kCullingVisitCounterOffset = 0x301F0;

bool BSOrderedNode::RegisterStreamables(NiStream& a_stream)
{
    return NiNodeRegisterStreamables(this, a_stream) != 0;
}

void BSOrderedNode::OnVisible(NiCullingProcess& a_process)
{
    volatile long* counter = reinterpret_cast<volatile long*>(reinterpret_cast<char*>(&a_process) + kCullingVisitCounterOffset);
    // The compared function does a locked increment of the culling counter, so the intrinsic is needed.
    int visitIndex = _InterlockedIncrement(counter);
    NiNodeOnVisibleIndexed(this, a_process, visitIndex);
}
