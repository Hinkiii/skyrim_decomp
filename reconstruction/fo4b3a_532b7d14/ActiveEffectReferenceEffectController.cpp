// Build: sse-steam-1-7-104
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2.json
// Drafted by gpt-6.1-terra.
// RVA 0x5dc6b0, 11 bytes: ActiveEffectReferenceEffectController::HandleEvent
// RVA 0x5dc740, 11 bytes: ActiveEffectReferenceEffectController::SwitchAttachedRoot
// RVA 0x5dcac0, 18 bytes: ActiveEffectReferenceEffectController::SetWindPoint
// RVA 0x5dc6b0, 11 bytes: ActiveEffectReferenceEffectController::HandleEvent
// ActiveEffectReferenceEffectController::HandleEvent: SIBLING(fo4-private@1b765c70, author-permission); adaptation of src/Game/Magic/ReferenceEffectControllers.cpp:213.
// RVA 0x5dc740, 11 bytes: ActiveEffectReferenceEffectController::SwitchAttachedRoot
// ActiveEffectReferenceEffectController::SwitchAttachedRoot: SIBLING(fo4-private@1b765c70, author-permission); adaptation of src/Game/Magic/ReferenceEffectControllers.cpp:232.
// RVA 0x5dcac0, 18 bytes: ActiveEffectReferenceEffectController::SetWindPoint
// ActiveEffectReferenceEffectController::SetWindPoint: SIBLING(fo4-private@1b765c70, author-permission); adaptation of src/Game/Magic/ReferenceEffectControllers.cpp:298.
// Semantic names and types: GUESS(low)
// Labels: v2
// Source exposure: mixed.
// Origin: original reconstruction from independently decoded target and anonymous sibling.
// Drafted by gpt-6.1-terra. Evidence: findings/ghidra-import.md
#include "ActiveEffectReferenceEffectController.h"
void ActiveEffectReferenceEffectController::HandleEvent(const BSFixedString& event) { effect->HandleEvent(event); }
void ActiveEffectReferenceEffectController::SwitchAttachedRoot(NiNode* from, NiNode* to) { effect->SwitchAttachedRoot(from, to); }
void ActiveEffectReferenceEffectController::SetWindPoint(const NiPoint3& point) { windPoint.x = point.x; windPoint.y = point.y; windPoint.z = point.z; }
