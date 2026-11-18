// Build: sse-steam-1-7-104
// Labels: v2
// Code origin: declarations extended by hand from the local listings; names follow the community headers.
// Source exposure: mixed
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2.json
// Evidence: findings/ghidra-import.md
// names: GUESS(low); names and signatures are hypotheses.
// TESLevCharacter, TESLeveledList, TESBoundAnimObject, TESBoundObject: COMMUNITY(Ryan-rsm-McKenzie/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT); declarations from include/RE/T/TESLevCharacter.h, include/RE/T/TESLeveledList.h, include/RE/T/TESBoundAnimObject.h and include/RE/T/TESBoundObject.h.
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
// TESLevCharacter::UnClone3D, TESLevCharacter::SaveGame: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT); names only, the bodies are written from the target listing
// UnClone3D return type: GUESS(low); the listing zeroes EAX, the community declaration returns void
// TESLeveledList::SaveGame: GUESS(low); address 0x1ea3a0, called on the base subobject at offset 0x30
#pragma once
class TESForm;
class BGSSaveFormBuffer;
class TESObjectREFR;
class BGSLoadGameBuffer { public: virtual ~BGSLoadGameBuffer(); void* buffer; unsigned long long unknown; unsigned int field18, field1c, field20, position; };
struct BGSLoadFormData { unsigned int formID, size, uncompressedSize, reserved; TESForm* form; unsigned int changeFlags, oldChangeFlags; };
class BGSLoadFormBuffer : public BGSLoadGameBuffer, public BGSLoadFormData {};
class BaseFormComponent { public: virtual ~BaseFormComponent(); };
class TESForm : public BaseFormComponent { public: void SaveGame(BGSSaveFormBuffer*); void LoadGame(BGSLoadFormBuffer*); void Revert(BGSLoadFormBuffer*); void* sourceFiles; unsigned int formFlags; unsigned int formID; unsigned short inGameFormFlags; unsigned char formType; };
struct ShortPoint3 { short x,y,z; }; struct BoundData { ShortPoint3 minimum, maximum; };
class TESBoundObject : public TESForm { public: BoundData bounds; };
class TESBoundAnimObject : public TESBoundObject {};
class TESLeveledList : public BaseFormComponent { public: void SaveGame(BGSSaveFormBuffer*); void LoadGame(BGSLoadFormBuffer*); void Revert(BGSLoadFormBuffer*); };
class TESLevCharacter : public TESBoundAnimObject, public TESLeveledList { public: unsigned int UnClone3D(TESObjectREFR*); void SaveGame(BGSSaveFormBuffer*); void LoadGame(BGSLoadFormBuffer*); void Revert(BGSLoadFormBuffer*); };
