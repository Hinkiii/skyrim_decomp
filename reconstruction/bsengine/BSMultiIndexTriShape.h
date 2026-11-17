// Type declarations for the BSMultiIndexTriShape translation unit (build sse-steam-1-7-104).
//
// Labels: v2
// Code origin: original reconstruction from binary analysis by Taylor Turner, assisted by claude-sonnet-5-5; class and member names come from CommonLibSSE-NG (MIT), and no external implementation body or layout is copied.
// Source exposure: binary-analysis (a local listing of the contributor's own copy of the pinned image).
// Community names are unverified. Only the members the compared code needs are declared.
// BSMultiIndexTriShape, BSMultiIndexTriShape::AsMultiIndexTriShape: COMMUNITY(CharmedBaryon/CommonLibSSE-NG@b93280e, MIT)
#pragma once

class BSMultiIndexTriShape {
public:
    virtual ~BSMultiIndexTriShape();

    // The virtual slot order is not compared here; only the body below is.
    virtual BSMultiIndexTriShape* AsMultiIndexTriShape();  // compared: 0x1575450
};
