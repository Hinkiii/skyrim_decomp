// NiSourceTexture functions for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-nisourcetexture
// Code origin: hand-written from the local listing; names and declarations from the community header named in NiSourceTexture.h.
// Source exposure: mixed (names and declarations from community headers; the listing of the pinned
// image).
// MIT License
//
// Copyright (c) 2018 Ryan-rsm-McKenzie
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// Evidence: findings/queue-small-2-selection.md
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2-root-includes.json (compile this file alone).
// Drafted by Hinkiii, assisted by claude-sonnet-5-5, starting from
// a tools/skeleton.py skeleton.
// Compared functions:
//   RVA 0xef3500, 24 bytes: NiSourceTexture::Unk_25
// Adapted bodies: none
// A byte match shows that this source and recipe produce the checked code; it does not verify the
// community names or that the original source looked like this.
#include "NiSourceTexture.h"

// TextureLookupState, g_textureLookupState, LookupRendererTexture: STANDIN GUESS(low); names invented, the object is at address 0x3331f40 and the function at address 0x100f370
struct TextureLookupState;
extern TextureLookupState g_textureLookupState;
std::uint64_t LookupRendererTexture(TextureLookupState* a_state, BSGraphicsTexture* a_texture);

// Unk_25 is the community name; the community header says it returns 0, the listing consults the renderer texture.
std::uint64_t NiSourceTexture::Unk_25()
{
    if (rendererTexture) {
        return LookupRendererTexture(&g_textureLookupState, rendererTexture);
    }
    return 0;
}
