// IngredientItem declarations for build sse-steam-1-7-104 (standalone unit for GetData2, IsFood and InitChunk).
// Labels: v2
// Task: contrib-ingredientitem
// Code origin: declarations written by hand from the local listings and the community headers; the bodies are in the matching .cpp.
// Source exposure: mixed (names and declarations from community headers; the listing of the pinned image).
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2.json
// Drafted by Hinkiii, assisted by claude-sonnet-5-5.
// RVA 0x14f8a0, 29 bytes: IngredientItem::InitChunk
// RVA 0x14fd00, 8 bytes: IngredientItem::GetData2
// RVA 0x14fd60, 25 bytes: IngredientItem::IsFood
// IngredientItem::GetData2, IngredientItem::IsFood, IngredientItem::InitChunk: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT); names only, the bodies are written from the target listing
// GetData: GUESS(low); virtual slot 0x6C (byte offset 0x360), returns the record whose flags IsFood reads
// Unk_NN: GUESS(low); placeholders that only keep GetData at the vtable offset the image encodes
// IngredientData, cost, flags: STANDIN GUESS(low); the first two members of the record at 0x130 (cost, flags)
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
// Layout members are named after CommonLibSSE-NG and sit at its offsets; secondary bases are flattened as vptr slot plus fields.
#pragma once
#include <cstddef>
#include <cstdint>
struct IngredientData {
    std::int32_t  cost;   // 00
    std::uint32_t flags;  // 04
};
class IngredientItem {
public:
    virtual void Unk_00();
    virtual void Unk_01();
    virtual void Unk_02();
    virtual void Unk_03();
    virtual void Unk_04();
    virtual void Unk_05();
    virtual void Unk_06();
    virtual void Unk_07();
    virtual void Unk_08();
    virtual void Unk_09();
    virtual void Unk_0A();
    virtual void Unk_0B();
    virtual void Unk_0C();
    virtual void Unk_0D();
    virtual void Unk_0E();
    virtual void Unk_0F();
    virtual void Unk_10();
    virtual void Unk_11();
    virtual void Unk_12();
    virtual void Unk_13();
    virtual void Unk_14();
    virtual void Unk_15();
    virtual void Unk_16();
    virtual void Unk_17();
    virtual void Unk_18();
    virtual void Unk_19();
    virtual void Unk_1A();
    virtual void Unk_1B();
    virtual void Unk_1C();
    virtual void Unk_1D();
    virtual void Unk_1E();
    virtual void Unk_1F();
    virtual void Unk_20();
    virtual void Unk_21();
    virtual void Unk_22();
    virtual void Unk_23();
    virtual void Unk_24();
    virtual void Unk_25();
    virtual void Unk_26();
    virtual void Unk_27();
    virtual void Unk_28();
    virtual void Unk_29();
    virtual void Unk_2A();
    virtual void Unk_2B();
    virtual void Unk_2C();
    virtual void Unk_2D();
    virtual void Unk_2E();
    virtual void Unk_2F();
    virtual void Unk_30();
    virtual void Unk_31();
    virtual void Unk_32();
    virtual void Unk_33();
    virtual void Unk_34();
    virtual void Unk_35();
    virtual void Unk_36();
    virtual void Unk_37();
    virtual void Unk_38();
    virtual void Unk_39();
    virtual void Unk_3A();
    virtual void Unk_3B();
    virtual void Unk_3C();
    virtual void Unk_3D();
    virtual void Unk_3E();
    virtual void Unk_3F();
    virtual void Unk_40();
    virtual void Unk_41();
    virtual void Unk_42();
    virtual void Unk_43();
    virtual void Unk_44();
    virtual void Unk_45();
    virtual void Unk_46();
    virtual void Unk_47();
    virtual void Unk_48();
    virtual void Unk_49();
    virtual void Unk_4A();
    virtual void Unk_4B();
    virtual void Unk_4C();
    virtual void Unk_4D();
    virtual void Unk_4E();
    virtual void Unk_4F();
    virtual void Unk_50();
    virtual void Unk_51();
    virtual void Unk_52();
    virtual void Unk_53();
    virtual void Unk_54();
    virtual void Unk_55();
    virtual void Unk_56();
    virtual void Unk_57();
    virtual void Unk_58();
    virtual void Unk_59();
    virtual void Unk_5A();
    virtual void Unk_5B();
    virtual void Unk_5C();
    virtual void Unk_5D();
    virtual void Unk_5E();
    virtual void Unk_5F();
    virtual void Unk_60();
    virtual void Unk_61();
    virtual void Unk_62();
    virtual void Unk_63();
    virtual void Unk_64();
    virtual void Unk_65();
    virtual void Unk_66();
    virtual void Unk_67();
    virtual void Unk_68();
    virtual void Unk_69();
    virtual void Unk_6A();
    virtual void Unk_6B();
    virtual IngredientData* GetData() const;  // 6C
    virtual IngredientData* GetData2();
    virtual bool IsFood() const;
    virtual void InitChunk();

private:
public:
    // flattened secondary bases at their CommonLibSSE-NG offsets (vptr slot plus fields)
    void*         sourceFiles;           // 08 TESForm
    std::uint32_t formFlags;             // 10
    std::uint32_t formID;                // 14
    std::uint16_t inGameFormFlags;       // 18
    std::uint8_t  formType;              // 1A
    std::uint8_t  pad1B;                 // 1B
    std::uint32_t pad1C;                 // 1C
    std::int16_t  boundData[6];          // 20 TESBoundObject OBND
    std::uint32_t pad2C;                 // 2C
    void*         vptrFullName;          // 30 TESFullName
    void*         fullName;              // 38
    void*         vptrKeywordForm;       // 40 BGSKeywordForm
    void**        keywords;              // 48
    std::uint32_t numKeywords;           // 50
    std::uint32_t pad54;                 // 54
    void*         effectsBegin;          // 58 MagicItem::effects (BSTArray)
    void*         effectsPad;            // 60
    std::uint64_t effectsSizeCapacity;   // 68
    std::int32_t  hostileCount;          // 70
    std::uint32_t pad74;                 // 74
    void*         avEffectSetting;       // 78
    std::uint32_t preloadCount;          // 80
    std::uint32_t pad84;                 // 84
    void*         preloadedItem;         // 88
    void*         vptrModel;             // 90 TESModelTextureSwap
    void*         model;                 // 98
    void*         textures;              // A0
    void*         addons;                // A8
    std::uint16_t numTextures;           // B0
    std::uint16_t numAddons;             // B2
    std::uint32_t pad24;                 // B4
    void*         alternateTextures;     // B8
    std::uint32_t numAlternateTextures;  // C0
    std::uint32_t pad34;                 // C4
    void*         vptrIcon;              // C8 TESIcon
    void*         textureName;           // D0
    void*         vptrWeightForm;        // D8 TESWeightForm
    float         weight;                // E0
    std::uint32_t pad0C;                 // E4
    void*         vptrEquipType;         // E8 BGSEquipType
    void*         equipSlot;             // F0
    void*         vptrDestructible;      // F8 BGSDestructibleObjectForm
    void*         destructibleData;      // 100
    void*         vptrPickupPutdown;     // 108 BGSPickupPutdownSounds
    void*         pickupSound;           // 110
    void*         putdownSound;          // 118
    void*         vptrValueForm;         // 120 TESValueForm
    std::int32_t  value;                 // 128
    std::uint32_t padValue;              // 12C
    int cost;                            // 130 ENIT costOverride
    std::uint32_t flags;                 // 134
    std::uint16_t knownEffectFlags;      // 138 GameData
    std::uint16_t playerUses;            // 13A
    std::uint32_t pad13C;                // 13C
};
static_assert(offsetof(IngredientItem, cost) == 0x130, "cost at 0x130");
static_assert(sizeof(IngredientItem) == 0x140, "size");
