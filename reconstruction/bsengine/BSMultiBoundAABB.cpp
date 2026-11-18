// BSMultiBoundAABB functions for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-bsmultiboundaabb
// Code origin: SetCenter and RegisterStreamables bodies written from the local listings; declarations added to the existing unit header.
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
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2.json (compile this file alone).
// Drafted by Hinkiii, assisted by claude-sonnet-5-5, starting from
// a tools/skeleton.py skeleton.
// Compared functions:
//   RVA 0xfceab0, 18 bytes: BSMultiBoundAABB::SetCenter
//   RVA 0xfce8c0, 19 bytes: BSMultiBoundAABB::RegisterStreamables
//   RVA 0x213ab0, 50 bytes: BSMultiBoundAABB::~BSMultiBoundAABB
//   RVA 0xfce870, 55 bytes: BSMultiBoundAABB::LoadBinary
// Adapted bodies: none
// A byte match shows that this source and recipe produce the checked code; it does not verify the
// community names or that the original source looked like this.
// Public naming source notice:
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
#include "BSMultiBoundAABB.h"

void BSMultiBoundAABB::SetCenter(const NiPoint3& a_center)
{
    unk1C.x = a_center.x;
    unk1C.y = a_center.y;
    unk1C.z = a_center.z;
}

bool BSMultiBoundAABB::RegisterStreamables(NiStream& a_stream)
{
    return RegisterStreamablesBase(this, a_stream) != 0;
}

// Not compared: defined so that the compiler emits the vftable and, beside it, the
// deleting destructor that is compared (cookbook rule 14).
BSMultiBoundAABB::BSMultiBoundAABB()
{
}

// Body done: RVA 0x213ab0, 50 bytes, BSMultiBoundAABB::~BSMultiBoundAABB; compared as the scalar
// deleting destructor. The target does the NiRefObject cleanup itself, so keep this empty
// definition and the constructor: the inline base destructors in the header make the compiler
// produce the same body.
BSMultiBoundAABB::~BSMultiBoundAABB()
{
}

// Body done: RVA 0xfce870, 55 bytes, BSMultiBoundAABB::LoadBinary.
void BSMultiBoundAABB::LoadBinary(NiStream& a_stream)
{
    NiObject::LoadBinary(a_stream);
    unk1C.LoadTriple(a_stream);
    unk2C.LoadTriple(a_stream);
}

// Retained generated leaf; its byte-match evidence is renewed with this unit.
// GetType: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
std::uint32_t BSMultiBoundAABB::GetType() const
{
    return 0x1;
}
