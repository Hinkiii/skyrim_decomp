// TESObjectACTI declarations for build sse-steam-1-7-104.
// Labels: v2
// Origin: original partial declarations from target listing and sibling type/name hints.
// Task: p177fb-ec14f6c6
// Source exposure: mixed (pinned target listing and sibling hints).
// TESObjectACTI, TESObjectACTI::InitializeData: GUESS(low)
// TESObjectACTI fields: STANDIN GUESS(low)
// Supporting types: STANDIN GUESS(low)
#pragma once
#include <cstdint>

class TESObjectACTI {
public: void InitializeData();
private: std::uint8_t pad[0xa8]; void* soundLoop; void* soundActivate; void* waterForm; std::uint16_t flags;
};
