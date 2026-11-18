// IngredientItem reconstructed functions.
// Build sse-steam-1-7-104.
// Labels: v2
// Source exposure: mixed (pinned image listing and authorized sibling hints).
// Origin: SIBLING(fo4-private@1b765c70, author-permission)
// Drafted by gpt-6.1-terra
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2.json
// Evidence: findings/ghidra-import.md
// InitializeData: Sibling source: fo4-private@1b765c70 src/Shared/Magic/IngredientItem.cpp:52
// ClearData: Sibling source: fo4-private@1b765c70 src/Shared/Magic/IngredientItem.cpp:60
// RVA 0x14edb0, 25 bytes: IngredientItem::InitializeData
// RVA 0x14edd0, 27 bytes: IngredientItem::ClearData
// IngredientItem, IngredientItem::InitializeData, IngredientItem::ClearData: SIBLING(fo4-private@1b765c70, author-permission)
// unknown00, cost, flags, InitializeMagicData, RemoveAllEffects, ClearFormData: STANDIN GUESS(low)
// Names, signatures, and layout are unverified.
#include "IngredientItem.h"

void IngredientItem::InitializeData() { cost = -1; flags = 0; InitializeMagicData(); }
void IngredientItem::ClearData() { RemoveAllEffects(); ClearFormData(); }
