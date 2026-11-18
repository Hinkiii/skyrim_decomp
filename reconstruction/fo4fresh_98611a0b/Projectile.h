// Projectile declarations for build sse-steam-1-7-104.
// Labels: v2
// Code origin: declarations extended by hand from the local listings; names follow the community headers.
// Source exposure: mixed (pinned image listing and anonymous sibling hints).
// Origin: target listing and anonymous sibling hints.
// Drafted by gpt-6.1-terra.
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2.json
// Evidence: findings/ghidra-import.md
// Projectile, GetWeaponSpeedMult: GUESS(low)
// IsMissileProjectile, IsGrenadeProjectile: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a, MIT); names only, the bodies are written from the target listing
#pragma once
class Projectile {
public:
    virtual bool IsMissileProjectile() const;
    virtual bool IsGrenadeProjectile() const;
    float GetWeaponSpeedMult() const;
};
