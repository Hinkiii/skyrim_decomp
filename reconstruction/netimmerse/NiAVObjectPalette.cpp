// NiAVObjectPalette functions for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-niavobjectpalette
// Code origin: body written by the contributor from the compared function's behaviour; names and
// declarations come from the header (CharmedBaryon/CommonLibSSE-NG, MIT).
// Source exposure: mixed (names and declarations from community headers; the listing of the pinned
// image).
// Declarations derived from powerof3/CommonLibSSE are GPL-3.0 (its licence).
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
// The MIT notice above covers only the names and declarations attributed to
// CharmedBaryon/CommonLibSSE-NG in this file, not the rest of the project.
//
// Declarations derived from powerof3/CommonLibSSE are GPL-3.0-or-later.
// This file is free software: you can redistribute it and/or modify it under the terms
// of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
// This file is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details. A copy is provided in
// licenses/CommonLibSSE-GPL-3.0.txt; see also https://www.gnu.org/licenses/.
// Evidence: findings/p92-unit-supply-selection.md
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2-root-includes.json (compile this file alone).
// Drafted by Hinkiii, assisted by claude-sonnet-5-5, starting from
// a tools/skeleton.py skeleton.
// Compared functions:
//   RVA 0xf0a920, 19 bytes: NiAVObjectPalette::RegisterStreamables
// Adapted bodies: none.
// A byte match shows that this source and recipe produce the checked code; it does not verify the
// community names or that the original source looked like this.
#include "NiAVObjectPalette.h"

bool NiAVObjectPalette::RegisterStreamables(NiStream& a_stream)
{
    if (!NiObject::RegisterStreamables(a_stream)) {
        return false;
    }
    return true;
}
