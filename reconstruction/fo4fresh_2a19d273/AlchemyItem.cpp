// AlchemyItem reconstructed functions.
// Build sse-steam-1-7-104.
// Labels: v2
// Source exposure: mixed (pinned image listing and authorized sibling hints).
// Origin: SIBLING(fo4-private@1b765c70, author-permission)
// Drafted by gpt-6.1-terra
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2.json
// Evidence: findings/ghidra-import.md
// InitializeData: Sibling source: fo4-private@1b765c70 src/Shared/Magic/AlchemyItem.cpp:20
// ClearData: Sibling source: fo4-private@1b765c70 src/Shared/Magic/AlchemyItem.cpp:31
// RVA 0x145af0, 43 bytes: AlchemyItem::InitializeData
// RVA 0x145b30, 27 bytes: AlchemyItem::ClearData
// AlchemyItem, AlchemyItem::InitializeData, AlchemyItem::ClearData: SIBLING(fo4-private@1b765c70, author-permission)
// unknown00, cost, flags, InitializeMagicData, RemoveAllEffects, ClearFormData: STANDIN GUESS(low)
// Names, signatures, and layout are unverified.
#include "AlchemyItem.h"

void AlchemyItem::InitializeData() { cost = -1; flags = 0; addictionItem = nullptr; addictionChance = 0.0f; consumptionSound = nullptr; InitializeMagicData(); }
void AlchemyItem::ClearData() { RemoveAllEffects(); ClearFormData(); }
