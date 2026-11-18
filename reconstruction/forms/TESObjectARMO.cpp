// TESObjectARMO functions for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-tesobjectarmo
// Code origin: bodies written by the contributor from the compared functions' behaviour; names and
// declarations come from the header (CharmedBaryon/CommonLibSSE-NG, MIT).
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
// The MIT notice above covers only the names and declarations attributed to
// CharmedBaryon/CommonLibSSE-NG in this file, not the rest of the project.
// Evidence: findings/queue-refill-held-selection.md
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2-root-includes.json (compile this file alone).
// Drafted by Hinkiii, assisted by claude-sonnet-5-5, starting from
// a tools/skeleton.py skeleton.
// Compared functions:
//   RVA 0x27f1b0, 24 bytes: TESObjectARMO::GetDescriptionOwnerFile
//   RVA 0x27ef24, 9 bytes: TESObjectARMO::dtor
//   RVA 0x27eef4, 12 bytes: TESObjectARMO::dtor
//   RVA 0x27ef6c, 12 bytes: TESObjectARMO::dtor
//   RVA 0x27dae0, 20 bytes: TESObjectARMO::InitializeData
//   RVA 0x27ee20, 43 bytes: TESObjectARMO::SaveGame
//   RVA 0x27ee60, 43 bytes: TESObjectARMO::LoadGame
// Adapted bodies: none.
// A byte match shows that this source and recipe produce the checked code; it does not verify the
// community names or that the original source looked like this.
#include "TESObjectARMO.h"

// ResolveOwnerFile: GUESS(low); address 0x1e5ae0 takes the form and a 32-bit file index (-1 for the
// latest owner) and returns the file; declared, not defined here.
TESFile* ResolveOwnerFile(const TESForm* a_form, std::uint32_t a_index);

// Defined so that the compiler emits the vftables of every base and, beside them, the deleting
// destructor and its adjustor thunks that are compared (cookbook rules 14 and 25).
TESObjectARMO::TESObjectARMO()
{
}

TESFile* TESObjectARMO::GetDescriptionOwnerFile() const
{
    return ResolveOwnerFile(templateArmor != nullptr ? templateArmor : this, 0xFFFFFFFFu);
}

void TESObjectARMO::InitializeData()
{
    armorRating = 0;
    templateArmor = nullptr;
    amountofEnchantment = 0;
}

void TESObjectARMO::SaveGame(BGSSaveFormBuffer* a_buf)
{
    TESForm::SaveGame(a_buf);
    TESValueForm::SaveGame(a_buf);
}

void TESObjectARMO::LoadGame(BGSLoadFormBuffer* a_buf)
{
    TESForm::LoadGame(a_buf);
    TESValueForm::LoadGame(a_buf);
}
