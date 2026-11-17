// BSMultiIndexTriShape functions, build sse-steam-1-7-104.
//
// Labels: v2
// Code origin: original reconstruction from binary analysis by Taylor Turner, assisted by claude-sonnet-5-5; class and member names come from CommonLibSSE-NG (MIT), and no external implementation body or layout is copied.
// Evidence: findings/p92-unit-supply-selection.md (BSMultiIndexTriShape, 0x1575450).
// Source exposure: binary-analysis. Drafted by Taylor Turner, assisted by claude-sonnet-5-5.
// A byte match shows that this source and recipe produce the checked code; it does not verify the
// community name or that the original source looked like this.
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2.json (compile this file alone).
// Compared functions:
//   RVA 0x1575450, 4 bytes: BSMultiIndexTriShape::AsMultiIndexTriShape
//
// Consulted (private, not in repo): Ghidra decompile of the contributor's own copy.
#include "BSMultiIndexTriShape.h"

BSMultiIndexTriShape* BSMultiIndexTriShape::AsMultiIndexTriShape()
{
    return this;
}
