// ActorSpeedPrefix: STANDIN GUESS(low); target-bound partial receiver and callee views; original names remain hypotheses.
// Drafted by gpt-6.1-terra; publication metadata normalized by gpt-6.1-sol.
// Compared: RVA 0x6a7ce0, 61 bytes; RVA 0x6a7d30, 61 bytes; RVA 0x6a7d80, 61 bytes.
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
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2-reconstruction.json
// Origin: original reconstruction; adapted declarations retain their cited source notices.
// ActorSpeed: build sse-steam-1-7-104, RVAs 0x6a7ce0, 0x6a7d30, 0x6a7d80, 61 bytes each.
// Labels: v2
// Source exposure: mixed; target packs and pinned community headers as recorded in declaration-evidence.json.
// Drafted by gpt-6.1-terra. Original bodies; declarations adapted from CommonLibSSE-NG, MIT.
// Evidence: findings/ghidra-import.md. Receiver identity is inferred.
#include "ActorSpeedPrefix.h"
namespace P2071 {
void ActorSpeedPrefix::UpdateFastWalkCache() {
    float speed = GetFastWalkSpeed();
    auto* process = currentProcess;
    if (process && process->cachedValues) {
        process->cachedValues->cachedFastWalkSpeed = speed;
        process->cachedValues->flags |= 0x400;
    }
}
void ActorSpeedPrefix::UpdateJogCache() {
    float speed = GetJogSpeed();
    auto* process = currentProcess;
    if (process && process->cachedValues) {
        process->cachedValues->cachedJogSpeed = speed;
        process->cachedValues->flags |= 0x800;
    }
}
void ActorSpeedPrefix::UpdateRunCache() {
    float speed = GetRunSpeed();
    auto* process = currentProcess;
    if (process && process->cachedValues) {
        process->cachedValues->cachedRunSpeed = speed;
        process->cachedValues->flags |= 0x2000;
    }
}
}
