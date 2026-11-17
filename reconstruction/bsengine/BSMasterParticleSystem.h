// Type declarations for the BSMasterParticleSystem translation unit (build sse-steam-1-7-104).
//
// Labels: v2
// Code origin: original reconstruction from binary analysis by Taylor Turner, assisted by claude-sonnet-5-5; class and member names come from CommonLibSSE-NG (MIT), and no external implementation body or layout is copied.
// Source exposure: binary-analysis (a local listing of the contributor's own copy of the pinned image).
// Community names are unverified. Only the members the compared code needs are declared.
// BSMasterParticleSystem, BSMasterParticleSystem::IsEqual, NiObject: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
#pragma once

class NiObject;

class BSMasterParticleSystem {
public:
    virtual ~BSMasterParticleSystem();

    // The virtual slot order is not compared here; only the body below is.
    virtual bool IsEqual(NiObject* a_object);  // compared: 0xffb470
};
