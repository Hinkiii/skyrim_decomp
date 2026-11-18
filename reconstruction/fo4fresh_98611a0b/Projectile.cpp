// Projectile function for build sse-steam-1-7-104.
// Labels: v2
// Source exposure: mixed (pinned image listing and anonymous sibling hints).
// Origin: anonymous sibling and target listing.
// GetWeaponSpeedMult: SIBLING(fo4-private@1b765c70, author-permission)
// Sibling source: fo4-private@1b765c70 src/Game/AI/Combat/Projectile.cpp:60
// Drafted by gpt-6.1-terra.
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2.json
// Evidence: findings/ghidra-import.md
// RVA 0x7fc030, 9 bytes: Projectile::GetWeaponSpeedMult
#include "Projectile.h"
float Projectile::GetWeaponSpeedMult() const { return 1.0f; }
