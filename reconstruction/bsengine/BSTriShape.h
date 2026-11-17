// Type declarations for the BSTriShape translation unit (build sse-steam-1-7-104).
//
// Labels: v2
// Code origin: original reconstruction from binary analysis by Taylor Turner, assisted by claude-sonnet-5-5; class and member names come from CommonLibSSE-NG (MIT), and no external implementation body or layout is copied.
// Source exposure: binary-analysis (a local listing of the contributor's own copy of the pinned image).
// Community names are unverified. Only the members the compared code needs are declared.
// BSTriShape, BSTriShape::AsTriShape, IsEqual, NiObject: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
#pragma once

class NiObject;

class BSTriShape {
public:
    virtual ~BSTriShape();

    // The virtual slot order is not compared here; only the two bodies below are.
    virtual BSTriShape* AsTriShape();            // compared: 0x26d4d0
    virtual bool        IsEqual(NiObject* a_object);  // compared: 0xef2120
};
