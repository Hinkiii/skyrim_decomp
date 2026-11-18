// BGSDecalNode functions for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-bgsdecalnode
// Code origin: original drafting by Hinkiii with claude-sonnet-5-5 from a local listing of the pinned image; names, members and declarations
// from CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a (MIT); no body is copied from any source.
// Source exposure: mixed (names and declarations from community headers; the listing of the pinned
// image).
// The MIT licence below covers only the names and declarations taken from CommonLibSSE-NG, not any other content of this file.
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
// Evidence: findings/fifth-batch-selection.md
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2-root-includes.json (compile this file alone).
// Drafted by Hinkiii, assisted by claude-sonnet-5-5, starting from
// a tools/skeleton.py skeleton.
// Compared functions:
//   RVA 0x2030b0, 17 bytes: BGSDecalNode::CreateClone
//   RVA 0x203360, 54 bytes: BGSDecalNode::AttachDecal
// Adapted bodies: none.
// A byte match shows that this source and recipe produce the checked code; it does not verify the
// community names or that the original source looked like this.
#include "BGSDecalNode.h"

NiObject* BGSDecalNode::CreateClone(NiCloningProcess& a_cloning)
{
    return CreateNodeInstance(g_nodeInstanceOwner, nullptr, nullptr);
}

void BGSDecalNode::AttachDecal(BSTempEffect* a_decal, bool a_firstAvail)
{
    NiNode::AttachChild(a_decal->Get3D(), a_firstAvail);
}
