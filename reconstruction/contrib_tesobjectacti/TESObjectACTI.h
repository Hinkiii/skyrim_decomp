// TESObjectACTI declarations for build sse-steam-1-7-104.
// Code origin: declarations written by hand from the local listings and the community headers; the bodies are in the matching .cpp.
// Labels: v2
// Task: p177fb-ec14f6c6
// Source exposure: mixed (names and declarations from community headers; the listing of the pinned image).
// TESObjectACTI, TESObjectACTI::InitializeData: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT); InitializeData is declared here as a non-virtual member, the community declaration is a virtual override
// soundLoop, soundActivate, waterForm, flags: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT); names and order as declared there, the types here are guesses
// sourceFiles, formFlags, formID, numKeywords: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT); member names and offsets from the community headers, including the base vtable pointer slots; the bases TESForm, TESBoundObject, TESFullName, TESModelTextureSwap, BGSDestructibleObjectForm, BGSOpenCloseForm and BGSKeywordForm are flattened into members at their NG offsets; field types here are simplified
// Supporting types: STANDIN GUESS(low)
// GetWaterType, GetIgnoredBySandbox, IsWater: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT); signatures as declared there
// TESWaterForm: STANDIN COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT)
// The MIT notice below covers only the names and declarations taken from
// CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a; the other code in this file
// is covered by the repository licence and the provenance labels above.
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
#include <cstddef>
#include <cstdint>

class TESWaterForm;
class TESObjectACTI {
public: virtual TESWaterForm* GetWaterType() const; virtual bool GetIgnoredBySandbox() const; virtual bool IsWater() const; void InitializeData();
public:
  // Primary base TESBoundAnimObject (TESForm 08, TESBoundObject 20) flattened at the NG offsets.
  void* sourceFiles;                // 08
  std::uint32_t formFlags;          // 10
  std::uint32_t formID;             // 14
  std::uint16_t inGameFormFlags;    // 18
  std::uint8_t formType;            // 1A
  std::uint8_t pad1B;               // 1B
  std::uint32_t pad1C;              // 1C
  std::int16_t boundMin[3];         // 20
  std::int16_t boundMax[3];         // 26
  std::uint32_t pad2C;              // 2C
  // Secondary bases flattened as (vptr slot, members).
  void* vtblTESFullName;            // 30
  void* fullName;                   // 38, BSFixedString
  void* vtblTESModelTextureSwap;    // 40
  void* model;                      // 48, BSFixedString
  void* textures;                   // 50
  void* addons;                     // 58
  std::uint16_t numTextures;        // 60
  std::uint16_t numAddons;          // 62
  std::uint32_t pad64;              // 64
  void* alternateTextures;          // 68
  std::uint32_t numAlternateTextures;  // 70
  std::uint32_t pad74;              // 74
  void* vtblBGSDestructibleObjectForm;  // 78
  void* destructibleData;           // 80
  void* vtblBGSOpenCloseForm;       // 88
  void* vtblBGSKeywordForm;         // 90
  void** keywords;                  // 98
  std::uint32_t numKeywords;        // A0
  std::uint32_t pad14;              // A4
  void* soundLoop; void* soundActivate; TESWaterForm* waterForm; std::uint16_t flags;  // A8, B0, B8, C0
  std::uint16_t padC2;              // C2
  std::uint32_t padC4;              // C4
};
static_assert(sizeof(TESObjectACTI) == 0xC8, "layout");
static_assert(offsetof(TESObjectACTI, soundLoop) == 0xA8, "layout");
static_assert(offsetof(TESObjectACTI, waterForm) == 0xB8, "layout");
static_assert(offsetof(TESObjectACTI, flags) == 0xC0, "layout");
