// MagicItem, build sse-steam-1-7-104.
// Labels: v2
// RVA 0x1501d0, 4 bytes: MagicItem::GetFixedCastDuration
// RVA 0x1501e0, 4 bytes: MagicItem::GetRange
// Source exposure: mixed (target listing, anonymous sibling and unverified names).
// Origin: SIBLING(fo4-private@1b765c70, author-permission)
// Sibling source: fo4-private@1b765c70 src/Shared/Magic/MagicItem.cpp:50 (MagicItem::GetFixedCastDuration)
// Sibling source: fo4-private@1b765c70 src/Shared/Magic/MagicItem.cpp:56 (MagicItem::GetRange)
// Evidence: findings/ghidra-import.md
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2.json
// Drafted by gpt-6.1-terra.
// MagicItem, GetFixedCastDuration, GetRange: GUESS(low)
#include "MagicItem.h"

float MagicItem::GetFixedCastDuration() const
{
    return 0.0f;
}

float MagicItem::GetRange() const
{
    return 0.0f;
}

