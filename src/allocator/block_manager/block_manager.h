/*
 *   BSD LICENSE
 *   Copyright (c) 2021 Samsung Electronics Corporation
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *     * Neither the name of Samsung Electronics Corporation nor the names of
 *       its contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <atomic>
#include <string>
#include <utility>

#include "src/allocator/context_manager/context_manager.h"
#include "src/allocator/i_block_allocator.h"
#include "src/include/smart_ptr_type.h"

namespace pos
{
class BlockAllocationStatus;
class TelemetryPublisher;
class StripeManager;

class BlockManager : public IBlockAllocator
{
public:
    BlockManager(void) = default;
    BlockManager(TelemetryPublisher* tp_, AllocatorCtx* alloCtx_,
        BlockAllocationStatus* allocStatus, AllocatorAddressInfo* info,
        ContextManager* ctxMgr, int arrayId);
    BlockManager(TelemetryPublisher* tp_, AllocatorAddressInfo* info,
        ContextManager* ctxMgr, int arrayId);
    virtual ~BlockManager(void) = default;
    virtual void Init(StripeManager* stripeManager);

    virtual std::pair<VirtualBlks, StripeId> AllocateWriteBufferBlks(uint32_t volumeId, uint32_t numBlks) override;
    virtual StripeSmartPtr AllocateGcDestStripe(uint32_t volumeId);
    virtual void ProhibitUserBlkAlloc(void) override;
    virtual void PermitUserBlkAlloc(void) override;

    virtual bool BlockAllocating(uint32_t volumeId) override;
    virtual void UnblockAllocating(uint32_t volumeId) override;
    virtual void TurnOffBlkAllocation(void);
    virtual void TurnOnBlkAllocation(void);

    virtual bool TryRdLock(uint32_t volumeId)
    {
        return allocStatus->TryRdLock(volumeId);
    }
    virtual bool Unlock(uint32_t volumeId)
    {
        return allocStatus->Unlock(volumeId);
    }
    virtual void Lock(void)
    {
        allocStatus->Lock();
    }
    virtual void Unlock(void)
    {
        allocStatus->Unlock();
    }

protected:
    std::pair<VirtualBlks, StripeId> _AllocateBlks(ASTailArrayIdx asTailArrayIdx, int numBlks);
    VirtualBlks _AllocateBlocksFromActiveStripe(ASTailArrayIdx asTailArrayIdx, int numBlks);

    bool _IsStripeFull(VirtualBlkAddr addr)
    {
        return addr.offset == addrInfo->GetblksPerStripe();
    }
    bool _IsValidOffset(uint64_t stripeOffset)
    {
        return stripeOffset < addrInfo->GetblksPerStripe();
    }

    // DOCs
    AllocatorAddressInfo* addrInfo;
    ContextManager* contextManager;
    StripeManager* stripeManager;
    BlockAllocationStatus* allocStatus;

    int arrayId;

    AllocatorCtx* allocCtx;
    TelemetryPublisher* tp;
};

} // namespace pos
