// TESObjectACTI functions for build sse-steam-1-7-104.
// Labels: v2
// Task: p177fb-ec14f6c6
// Source exposure: mixed (pinned target listing and sibling hints).
// Evidence: findings/ghidra-import.md
// RVA 0x279750, 31 bytes: TESObjectACTI::InitializeData
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2.json
// Drafted by gpt-6.1-terra.
// Origin: adapted sibling source and target listing.
// TESObjectACTI::InitializeData: SIBLING(fo4-private@1b765c70, author-permission)
// Sibling source: fo4-private@1b765c70 src/Shared/TESForms/Objects/TESObjectACTI.cpp:57
// TESObjectACTI, TESObjectACTI::InitializeData: GUESS(low)
// TESObjectACTI fields: STANDIN GUESS(low)
#include "TESObjectACTI.h"
void TESObjectACTI::InitializeData() { soundLoop = nullptr; soundActivate = nullptr; waterForm = nullptr; flags = 0; }
