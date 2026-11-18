// BSCubeMapCamera functions for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-bscubemapcamera
// Code origin: hand-written from the local listing; names and declarations from the community header named in BSCubeMapCamera.h.
// Source exposure: mixed (names and declarations from community headers; the listing of the pinned
// image).
// Declarations derived from powerof3/CommonLibSSE are GPL-3.0 (its licence).
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
//
// Declarations derived from powerof3/CommonLibSSE are GPL-3.0-or-later.
// This file is free software: you can redistribute it and/or modify it under the terms
// of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
// This file is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details. A copy is provided in
// licenses/CommonLibSSE-GPL-3.0.txt; see also https://www.gnu.org/licenses/.
// Evidence: findings/fifth-batch-selection.md
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2-root-includes.json (compile this file alone).
// Drafted by Hinkiii, assisted by claude-sonnet-5-5, starting from
// a tools/skeleton.py skeleton.
// Compared functions:
//   RVA 0x1559a60, 14 bytes: BSCubeMapCamera::RenderCubemap
// Adapted bodies: none
// A byte match shows that this source and recipe produce the checked code; it does not verify the
// community names or that the original source looked like this.
#include "BSCubeMapCamera.h"

// RenderCubemapFaces: STANDIN GUESS(low); the worker called at address 0x1559ea0 with the same arguments
void RenderCubemapFaces(BSCubeMapCamera* a_camera, FaceEnableFlags a_faceMask, bool a_arg2, bool a_clearScenes, bool a_arg4);

void BSCubeMapCamera::RenderCubemap(FaceEnableFlags a_faceMask, bool a_arg2, bool a_clearScenes, bool a_arg4)
{
    RenderCubemapFaces(this, a_faceMask, a_arg2, a_clearScenes, a_arg4);
}
