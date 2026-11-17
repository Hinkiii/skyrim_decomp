// ScriptRelationshipPass: STANDIN GUESS(low); partial target-bound receiver/callee view; no full layout or ownership claim.
// Origin: original reconstruction with cited adapted declarations and complete notices.
// Drafted by gpt-6.1-terra; publication metadata normalized by gpt-6.1-sol.
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2-reconstruction.json
// Compared: RVA 0xa00a60, 49 bytes; RVA 0xa00bd0, 49 bytes.
// ScriptRelationship: build sse-steam-1-7-104, RVAs 0xa00a60 (49 bytes), 0xa00bd0 (49 bytes).
// Labels: v2
// Source exposure: mixed; drafted by gpt-6.1-terra.
// Evidence: findings/ghidra-import.md. Prefix declarations from CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT.
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
#pragma once
#include "actors/ActorTargetPrefix.h"
#include <cstdint>
namespace P2071 {
// RelationshipActor, Relationship: STANDIN GUESS(low); incomplete actor and relationship views.
// Represents the already-bound actor prefix only; complete allocation and ownership are unknown.
class RelationshipActor : public P206Target::ActorTargetPrefix<> {};
class Relationship;
// Highest, Lowest, RankIndex, FindRelation, IsFamily, HasAnyFamily, RankValues: GUESS(low); helper contracts and semantics inferred.
Relationship* Highest(P206Target::TESBoundObject*);
Relationship* Lowest(P206Target::TESBoundObject*);
int RankIndex(Relationship*);
Relationship* FindRelation(P206Target::TESBoundObject*, P206Target::TESBoundObject*);
bool IsFamily(Relationship*);
bool HasAnyFamily(P206Target::TESBoundObject*);
extern int RankValues[];
// HighestRank, LowestRank, FamilyRelationship: GUESS(low); scripting signatures inferred.
int HighestRank(void*, unsigned, RelationshipActor*);
int LowestRank(void*, unsigned, RelationshipActor*);
bool FamilyRelationship(void*, unsigned, RelationshipActor*, RelationshipActor*);
}
