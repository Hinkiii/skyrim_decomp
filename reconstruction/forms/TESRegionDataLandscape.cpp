// TESRegionDataLandscape functions for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-tesregiondatalandscape
// Code origin: names and declarations from CharmedBaryon/CommonLibSSE-NG b93280e (MIT); bodies written from the listing of the pinned image
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
// Evidence: findings/queue-small-2-selection.md
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2-root-includes.json (compile this file alone).
// Drafted by Hinkiii, assisted by claude-sonnet-5-5, starting from
// a tools/skeleton.py skeleton.
// Compared functions:
//   RVA 0x250dd0, 40 bytes: TESRegionDataLandscape::IsLoaded
//   RVA 0x250980, 45 bytes: TESRegionDataLandscape::GetIconFileName
//   RVA 0x250bc0, 55 bytes: TESRegionDataLandscape::Unk_05
// Adapted bodies: none
// A byte match shows that this source and recipe produce the checked code; it does not verify the
// community names or that the original source looked like this.
#include "TESRegionDataLandscape.h"

bool TESRegionDataLandscape::IsLoaded() const
{
    return icon && icon->textureName.HasValue();
}

void TESRegionDataLandscape::GetIconFileName(BSString* a_dst)
{
    if (!icon) {
        a_dst->Set(BSStringT::EMPTY, 0);
        return;
    }
    a_dst->Set(icon->textureName._data, 0);
}

TESRegionData* TESRegionDataLandscape::Unk_05(TESRegionData* a_src)
{
    TESRegionData* result = ConstructSelf();
    result->CopyFrom(a_src, 2);
    return result;
}
