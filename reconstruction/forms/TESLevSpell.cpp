// TESLevSpell functions for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-teslevspell
// Code origin: hand-written from the local listings; names and declarations from the community header named in TESLevSpell.h.
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
//   RVA 0x272ee0, 43 bytes: TESLevSpell::SaveGame
//   RVA 0x272f20, 43 bytes: TESLevSpell::LoadGame
//   RVA 0x272f60, 43 bytes: TESLevSpell::Revert
// Adapted bodies: none
// A byte match shows that this source and recipe produce the checked code; it does not verify the
// community names or that the original source looked like this.
#include "TESLevSpell.h"

// TESLeveledList is a secondary base at offset 0x30, so the base calls take this + 0x30.
void TESLevSpell::SaveGame(BGSSaveFormBuffer* a_buf)
{
    TESForm::SaveGame(a_buf);
    TESLeveledList::SaveGame(a_buf);
}

void TESLevSpell::LoadGame(BGSLoadFormBuffer* a_buf)
{
    TESForm::LoadGame(a_buf);
    TESLeveledList::LoadGame(a_buf);
}

void TESLevSpell::Revert(BGSLoadFormBuffer* a_buf)
{
    TESForm::Revert(a_buf);
    TESLeveledList::Revert(a_buf);
}
