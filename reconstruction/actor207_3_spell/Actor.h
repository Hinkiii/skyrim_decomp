// Actor: STANDIN GUESS(low); partial target-bound receiver/callee view; no full layout or ownership claim.
// Origin: original reconstruction with cited adapted declarations and complete notices.
// Drafted by gpt-6.1-terra; publication metadata normalized by gpt-6.1-sol.
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2-reconstruction.json
// Compared: RVA 0x6d6d00, 48 bytes.
// Build: sse-steam-1-7-104; compared RVA 0x6d6d00, 48 bytes.
// Evidence: findings/ghidra-import.md
// Source exposure: mixed; Drafted by gpt-6.1-terra.
// Origin: original implementation informed by pinned community declarations.
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2-reconstruction.json
// Labels: v2
// Actor, MagicItem, GetSpellType, selectedSpells: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT)
// HasSelectedSpell, unknownPrefix, UnknownSlot: STANDIN GUESS(medium); meaning of type 6 and wrapper name remain unknown.
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
#include <cstdint>
namespace P2073 {
class MagicItem {
public:
    virtual void UnknownSlot000();
    virtual void UnknownSlot001();
    virtual void UnknownSlot002();
    virtual void UnknownSlot003();
    virtual void UnknownSlot004();
    virtual void UnknownSlot005();
    virtual void UnknownSlot006();
    virtual void UnknownSlot007();
    virtual void UnknownSlot008();
    virtual void UnknownSlot009();
    virtual void UnknownSlot010();
    virtual void UnknownSlot011();
    virtual void UnknownSlot012();
    virtual void UnknownSlot013();
    virtual void UnknownSlot014();
    virtual void UnknownSlot015();
    virtual void UnknownSlot016();
    virtual void UnknownSlot017();
    virtual void UnknownSlot018();
    virtual void UnknownSlot019();
    virtual void UnknownSlot020();
    virtual void UnknownSlot021();
    virtual void UnknownSlot022();
    virtual void UnknownSlot023();
    virtual void UnknownSlot024();
    virtual void UnknownSlot025();
    virtual void UnknownSlot026();
    virtual void UnknownSlot027();
    virtual void UnknownSlot028();
    virtual void UnknownSlot029();
    virtual void UnknownSlot030();
    virtual void UnknownSlot031();
    virtual void UnknownSlot032();
    virtual void UnknownSlot033();
    virtual void UnknownSlot034();
    virtual void UnknownSlot035();
    virtual void UnknownSlot036();
    virtual void UnknownSlot037();
    virtual void UnknownSlot038();
    virtual void UnknownSlot039();
    virtual void UnknownSlot040();
    virtual void UnknownSlot041();
    virtual void UnknownSlot042();
    virtual void UnknownSlot043();
    virtual void UnknownSlot044();
    virtual void UnknownSlot045();
    virtual void UnknownSlot046();
    virtual void UnknownSlot047();
    virtual void UnknownSlot048();
    virtual void UnknownSlot049();
    virtual void UnknownSlot050();
    virtual void UnknownSlot051();
    virtual void UnknownSlot052();
    virtual void UnknownSlot053();
    virtual void UnknownSlot054();
    virtual void UnknownSlot055();
    virtual void UnknownSlot056();
    virtual void UnknownSlot057();
    virtual void UnknownSlot058();
    virtual void UnknownSlot059();
    virtual void UnknownSlot060();
    virtual void UnknownSlot061();
    virtual void UnknownSlot062();
    virtual void UnknownSlot063();
    virtual void UnknownSlot064();
    virtual void UnknownSlot065();
    virtual void UnknownSlot066();
    virtual void UnknownSlot067();
    virtual void UnknownSlot068();
    virtual void UnknownSlot069();
    virtual void UnknownSlot070();
    virtual void UnknownSlot071();
    virtual void UnknownSlot072();
    virtual void UnknownSlot073();
    virtual void UnknownSlot074();
    virtual void UnknownSlot075();
    virtual void UnknownSlot076();
    virtual void UnknownSlot077();
    virtual void UnknownSlot078();
    virtual void UnknownSlot079();
    virtual void UnknownSlot080();
    virtual void UnknownSlot081();
    virtual void UnknownSlot082();
    virtual std::int32_t GetSpellType() const;
};
class Actor {
public:
    bool HasSelectedSpell(std::int32_t slot);
    std::uint8_t unknownPrefix[0x1c8];
    MagicItem* selectedSpells[4];
};
}
