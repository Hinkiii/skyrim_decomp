// Build: sse-steam-1-7-104; RVA 0x155380; 26 bytes; RVA 0x1553a0; 26 bytes; RVA 0x1553c0; 26 bytes
// Labels: v2
// Source exposure: mixed
// Origin: independently decoded target, anonymous sibling and public declarations.
// Drafted by gpt-6.1-terra
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2.json
// Evidence: findings/ghidra-import.md
// SpellItem::IgnoreLOS, SpellItem::GetNoAbsorb, SpellItem::GetNoDualCastModifications: SIBLING(fo4-private@1b765c70, author-permission); adapted to target observations.
// Sibling source: fo4-private@1b765c70 src/Shared/Magic/SpellItem.cpp:214
// Sibling source: fo4-private@1b765c70 src/Shared/Magic/SpellItem.cpp:220
// Sibling source: fo4-private@1b765c70 src/Shared/Magic/SpellItem.cpp:226
// SpellItem, MagicItem: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT); declarations adapted from include/RE/S/SpellItem.h, include/RE/M/MagicItem.h, include/RE/T/TESBoundObject.h, include/RE/T/TESForm.h, include/RE/T/TESFullName.h and include/RE/B/BGSKeywordForm.h, include/RE/B/BGSEquipType.h, include/RE/B/BGSMenuDisplayObject.h and include/RE/T/TESDescription.h.
// names: GUESS(low); names and original signatures are tentative.
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
#include "SpellItem.h"
bool SpellItem::IgnoreLOS() const { return (GetData1()->flags >> 19) & 1; }
bool SpellItem::GetNoAbsorb() const { return (GetData1()->flags >> 21) & 1; }
bool SpellItem::GetNoDualCastModifications() const { return (GetData1()->flags >> 23) & 1; }
