// MagicTarget: STANDIN GUESS(low); partial target view; original identities and ownership remain hypotheses.
// Compared: RVA 0x74a500, 41 bytes.
// Build: sse-steam-1-7-104; compared RVA 0x74a500, 41 bytes.
// Evidence: findings/ghidra-import.md
// Source exposure: mixed; Drafted by gpt-6.1-terra.
// Origin: original implementation informed by pinned community declarations.
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2-reconstruction.json
// Labels: v2
// MagicTarget: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT)
// PlayerReceiver, QueryGuard, invulnerabilityOverride: STANDIN GUESS(medium); complete receiver and global identities inferred.
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
namespace P2073 {
class MagicTarget : public P206Target::MagicTargetPrefix {
public:
    bool IsInvulnerable();
};
// reference, magicTarget: STANDIN GUESS(medium); target-bound containing receiver prefix.
class PlayerReceiver {
public:
    P206Target::ReferencePrefix<> reference;
    MagicTarget magicTarget;
    bool QueryGuard();
    static PlayerReceiver* FromMagicTarget(MagicTarget* target) {
        return reinterpret_cast<PlayerReceiver*>(reinterpret_cast<char*>(target) - offsetof(PlayerReceiver, magicTarget));
    }
};
static_assert(offsetof(PlayerReceiver, magicTarget) == 0xa0, "target secondary receiver anchor");
extern bool invulnerabilityOverride;
}
