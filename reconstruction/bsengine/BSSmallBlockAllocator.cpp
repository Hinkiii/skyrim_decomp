// BSSmallBlockAllocator functions for build sse-steam-1-7-104.
//
// Labels: v2
// Task: contrib-bssmallblockallocator
// Code origin: original drafting by Hinkiii with claude-sonnet-5-5 from a local listing of the pinned image; names and declarations
// from CharmedBaryon/CommonLibSSE-NG@b93280e832f263dbef44e44cbe2936622a02f91a (MIT); no body is copied from any source.
// Source exposure: mixed (names and declarations from community headers; the listing of the pinned
// image).
// The MIT licence below covers only the names and declarations taken from CommonLibSSE-NG, not any other content of this file.
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
// Evidence: findings/queue-refill-held-selection.md
// Recipe: reconstruction/recipes/msvc-19.16.27054-o2.json (compile this file alone).
// Drafted by Hinkiii, assisted by claude-sonnet-5-5, starting from
// a tools/skeleton.py skeleton.
// Compared functions:
//   RVA 0xcea5a0, 8 bytes: BSSmallBlockAllocator::TryAllocateImpl
//   RVA 0xcea710, 24 bytes: BSSmallBlockAllocator::ContainsBlockImpl
//   RVA 0xcea3f0, 43 bytes: BSSmallBlockAllocator::Size
// Adapted bodies: none.
// A byte match shows that this source and recipe produce the checked code; it does not verify the
// community names or that the original source looked like this.
#include "BSSmallBlockAllocator.h"

void* BSSmallBlockAllocator::TryAllocateImpl(std::size_t a_size, std::uint32_t a_alignment)
{
    return AllocateInternal(a_size, a_alignment, false);
}

bool BSSmallBlockAllocator::ContainsBlockImpl(const void* a_block) const
{
    const char* block = static_cast<const char*>(a_block);
    return block >= allocBase && block < blockPageCommit;
}

std::size_t BSSmallBlockAllocator::Size(const void* a_block) const
{
    // Blocks live in 2 MiB mega pages measured from allocBase; each page's descriptor table gives the element size.
    const char* block = static_cast<const char*>(a_block);
    const std::ptrdiff_t offset = block - allocBase;
    const char* page = allocBase + (offset & ~(static_cast<std::ptrdiff_t>(sizeof(MegaBlockPage)) - 1));
    const MegaBlockPage* mega = reinterpret_cast<const MegaBlockPage*>(page);
    return mega->blockPages[(block - page) >> 13].elemSize;
}
