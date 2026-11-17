// PlayerCharacterMembers: STANDIN GUESS(medium); typed windows into an external player receiver.
// Origin: original reconstruction using published project member declarations.
// Build: sse-steam-1-7-104
// Source exposure: mixed; member offsets are target-bound contracts, not a complete object layout.
// Evidence: findings/ghidra-import.md
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2-reconstruction.json
// Labels: v2
// PlayerCharacterMembers, Get: STANDIN GUESS(medium); access to the named member window of an external receiver.
#pragma once
#include "actors/PlayerCharacterTargetMembers.h"
namespace P2072 {
class PlayerCharacterMembers {
public:
    template<class T, class Binding>
    static T& Get(void* receiver) {
        return *reinterpret_cast<T*>(reinterpret_cast<std::uint8_t*>(receiver) + Binding::offset);
    }
};
}
