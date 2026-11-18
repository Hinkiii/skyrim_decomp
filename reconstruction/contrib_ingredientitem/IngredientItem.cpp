// IngredientItem functions for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-ingredientitem
// Code origin: GetData2, IsFood and InitChunk bodies written from the local listings; declarations and stand-in layout added to the existing unit header.
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
// Evidence: findings/queue-small-upstream-selection.md
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2.json (compile this file alone).
// Drafted by Hinkiii, assisted by claude-sonnet-5-5, starting from
// a tools/skeleton.py skeleton.
// Compared functions:
//   RVA 0x14f8a0, 29 bytes: IngredientItem::InitChunk
//   RVA 0x14fd00, 8 bytes: IngredientItem::GetData2
//   RVA 0x14fd60, 25 bytes: IngredientItem::IsFood
// Adapted bodies: none
// A byte match shows that this source and recipe produce the checked code; it does not verify the
// community names or that the original source looked like this.
#include "IngredientItem.h"
#include <intrin.h>
void IngredientItem::InitChunk()
{
    // The listing shows a 32-bit byte swap (bswap) of the cost field.
    cost = static_cast<int>(_byteswap_ulong(static_cast<unsigned long>(cost)));
    // The listing shows a 32-bit byte swap (bswap) of the flags field.
    flags = _byteswap_ulong(flags);
}
IngredientData* IngredientItem::GetData2() { return reinterpret_cast<IngredientData*>(&cost); }
bool IngredientItem::IsFood() const { return ((GetData()->flags >> 1) & 1) != 0; }
