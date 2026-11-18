// Build: sse-steam-1-7-104
// Labels: v2
// Code origin: declarations extended by hand from the local listings; names follow the community headers.
// Source exposure: mixed
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2.json
// Evidence: findings/ghidra-import.md
// names: GUESS(low); names and signatures are hypotheses.
// BGSProcedureTreeConditionalItem, TESCondition: COMMUNITY(Ryan-rsm-McKenzie/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT); declarations from include/RE/B/BGSProcedureTreeConditionalItem.h and include/RE/T/TESCondition.h.
// TESForm: COMMUNITY(Ryan-rsm-McKenzie/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT); declarations from include/RE/T/TESForm.h and include/RE/B/BaseFormComponent.h.
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
// BGSProcedureTreeConditionalItem::Unk_01, Unk_02, Unk_10: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT); names only, the bodies are written from the target listing
// IProcedureTreeItem::Unk_05, Unk_06: GUESS(low); virtual slots at byte offsets 0x28 and 0x30, each returns a pointer to a pointer
// TESCondition::Unk4a6940, TESCondition::Unk4a6ab0: GUESS(low); addresses 0x4a6940 and 0x4a6ab0, called on the member at offset 0x8
#pragma once
class TESFile { public: void NextChunk(); };
class IProcedureTreeItem { public: virtual ~IProcedureTreeItem(); virtual void Unk_01(); virtual void Unk_02(); virtual void Unk_03(); virtual void Unk_04(); virtual void** Unk_05(); virtual void** Unk_06(); };
class TESCondition { public: void LoadCompact(TESFile*); void Unk4a6940(); void Unk4a6ab0(TESCondition* a_other); void* head; };
class BGSProcedureTreeConditionalItem : public IProcedureTreeItem { public: void Load(TESFile*); void Unk_02() override; virtual void Unk_01(BGSProcedureTreeConditionalItem* a_other); virtual bool Unk_10(IProcedureTreeItem* a_other); TESCondition conditions; };
