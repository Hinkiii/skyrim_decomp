// TESObjectREFR: STANDIN GUESS(low); partial target-bound receiver/callee view; no full layout or ownership claim.
// Origin: original reconstruction with cited adapted declarations and complete notices.
// Drafted by gpt-6.1-terra; publication metadata normalized by gpt-6.1-sol.
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2-reconstruction.json
// Compared: RVA 0x300b20, 52 bytes.
// Build: sse-steam-1-7-104; compared RVA 0x300b20, 52 bytes.
// Evidence: findings/ghidra-import.md
// Source exposure: mixed; Drafted by gpt-6.1-terra.
// Origin: original implementation informed by pinned community declarations.
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2-reconstruction.json
// Labels: v2
// TESObjectREFR, TESObjectCELL, TESWorldSpace, BGSEncounterZone, parentCell, encounterZone, GetWorldspace: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT)
// GetEncounterZone, GetCellZone, unknownPrefix: STANDIN GUESS(medium); direct callee name and ownership inferred.
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
#include "TESObjectREFR.h"
namespace P2073 {
BGSEncounterZone* TESObjectREFR::GetEncounterZone() {
    if (parentCell) return parentCell->GetCellZone();
    TESWorldSpace* world = GetWorldspace();
    return world ? world->encounterZone : nullptr;
}
}
