// TESObjectACTI declarations for build sse-steam-1-7-104.
// Labels: v2
// Origin: original partial declarations from target listing and sibling type/name hints.
// Task: p177fb-ec14f6c6
// Source exposure: mixed (pinned target listing and sibling hints).
// TESObjectACTI, TESObjectACTI::InitializeData: GUESS(low)
// TESObjectACTI fields: STANDIN GUESS(low)
// Supporting types: STANDIN GUESS(low)
// GetWaterType, GetIgnoredBySandbox, IsWater: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT); signatures as declared there
// TESWaterForm: STANDIN COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT)
#pragma once
#include <cstdint>

class TESWaterForm;
class TESObjectACTI {
public: virtual TESWaterForm* GetWaterType() const; virtual bool GetIgnoredBySandbox() const; virtual bool IsWater() const; void InitializeData();
private: std::uint8_t pad[0xa0]; void* soundLoop; void* soundActivate; TESWaterForm* waterForm; std::uint16_t flags;
};
