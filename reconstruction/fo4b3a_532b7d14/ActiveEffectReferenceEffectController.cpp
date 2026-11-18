// ActiveEffectReferenceEffectController functions for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-activeeffectreferenceeffectcontroller
// Code origin: GetManagerHandlesSaveLoad, GetWindPoint and LoadGame bodies written from the local listings; declarations added to the existing unit header.
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
//   RVA 0x5dc9d0, 3 bytes: ActiveEffectReferenceEffectController::GetManagerHandlesSaveLoad
//   RVA 0x5dcae0, 5 bytes: ActiveEffectReferenceEffectController::GetWindPoint
//   RVA 0x5dcb70, 15 bytes: ActiveEffectReferenceEffectController::LoadGame
// Adapted bodies: none
// A byte match shows that this source and recipe produce the checked code; it does not verify the
// community names or that the original source looked like this.
// Build: sse-steam-1-7-104
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2.json
// Drafted by gpt-6.1-terra.
// RVA 0x5dc6b0, 11 bytes: ActiveEffectReferenceEffectController::HandleEvent
// RVA 0x5dc740, 11 bytes: ActiveEffectReferenceEffectController::SwitchAttachedRoot
// RVA 0x5dcac0, 18 bytes: ActiveEffectReferenceEffectController::SetWindPoint
// RVA 0x5dc6b0, 11 bytes: ActiveEffectReferenceEffectController::HandleEvent
// ActiveEffectReferenceEffectController::HandleEvent: SIBLING(fo4-private@1b765c70, author-permission); adaptation of src/Game/Magic/ReferenceEffectControllers.cpp:213.
// RVA 0x5dc740, 11 bytes: ActiveEffectReferenceEffectController::SwitchAttachedRoot
// ActiveEffectReferenceEffectController::SwitchAttachedRoot: SIBLING(fo4-private@1b765c70, author-permission); adaptation of src/Game/Magic/ReferenceEffectControllers.cpp:232.
// RVA 0x5dcac0, 18 bytes: ActiveEffectReferenceEffectController::SetWindPoint
// ActiveEffectReferenceEffectController::SetWindPoint: SIBLING(fo4-private@1b765c70, author-permission); adaptation of src/Game/Magic/ReferenceEffectControllers.cpp:298.
// Semantic names and types: GUESS(low)
// Labels: v2
// Source exposure: mixed.
// Origin: original reconstruction from independently decoded target and anonymous sibling.
// Drafted by gpt-6.1-terra. Evidence: findings/ghidra-import.md
#include "ActiveEffectReferenceEffectController.h"
bool ActiveEffectReferenceEffectController::GetManagerHandlesSaveLoad() const { return false; }
NiPoint3* ActiveEffectReferenceEffectController::GetWindPoint() { return &windPoint; }
void ActiveEffectReferenceEffectController::LoadGame(BGSLoadFormBuffer* a_buf) { LoadRefHandle(a_buf, &target); }
void ActiveEffectReferenceEffectController::HandleEvent(const BSFixedString& event) { effect->HandleEvent(event); }
void ActiveEffectReferenceEffectController::SwitchAttachedRoot(NiNode* from, NiNode* to) { effect->SwitchAttachedRoot(from, to); }
void ActiveEffectReferenceEffectController::SetWindPoint(const NiPoint3& point) { windPoint.x = point.x; windPoint.y = point.y; windPoint.z = point.z; }
