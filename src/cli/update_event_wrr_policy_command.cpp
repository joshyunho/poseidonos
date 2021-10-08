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
 *     * Neither the name of Intel Corporation nor the names of its
 *       contributors may be used to endorse or promote products derived
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

#include "src/cli/update_event_wrr_policy_command.h"

#include "src/cli/cli_event_code.h"
#include "src/include/backend_event.h"
#include "src/qos/qos_manager.h"

namespace pos_cli
{
UpdateEventWrrPolicyCommand::UpdateEventWrrPolicyCommand(void)
{
}

UpdateEventWrrPolicyCommand::~UpdateEventWrrPolicyCommand(void)
{
}

string
UpdateEventWrrPolicyCommand::Execute(json& doc, string rid)
{
    JsonFormat jFormat;
    int ret = 0;
    if (doc["param"].contains("name"))
    {
        string eventName = doc["param"]["name"].get<std::string>();
        uint64_t weight = 0;
        if (doc["param"].contains("weight"))
        {
            weight = doc["param"]["weight"].get<uint64_t>();
            if ((1 > weight) || (weight > 3))
            {
                return jFormat.MakeResponse("UPDATEEVENTWRRPOLICY", rid, BADREQUEST, "Wrong Weight", GetPosInfo());
            }
        }
        else
        {
            return jFormat.MakeResponse("UPDATEEVENTWRRPOLICY", rid, BADREQUEST, "Wrong Weight", GetPosInfo());
        }

        BackendEvent event = _GetEventId(eventName);
        if (event == pos::BackendEvent_Unknown)
        {
            return jFormat.MakeResponse("UPDATEEVENTWRRPOLICY", rid, BADREQUEST, "Wrong Event", GetPosInfo());
        }

        QosManagerSingleton::Instance()->SetEventWeightWRR(event, weight);

        if (ret == SUCCESS)
        {
            return jFormat.MakeResponse("UPDATEEVENTWRRPOLICY", rid, ret, "DONE", GetPosInfo());
        }
        else if (ret == QosReturnCode::VOLUME_POLICY_IN_EFFECT)
        {
            return jFormat.MakeResponse("UPDATEEVENTWRRPOLICY", rid, ret, "Volume Min Policy In Effect", GetPosInfo());
        }
        else
        {
            return jFormat.MakeResponse("UPDATEEVENTWRRPOLICY", rid, ret, "FAILED", GetPosInfo());
        }
    }
    else
    {
        return jFormat.MakeResponse("UPDATEEVENTWRRPOLICY", rid, BADREQUEST, "Check parameters", GetPosInfo());
    }
}

pos::BackendEvent
UpdateEventWrrPolicyCommand::_GetEventId(string eventName)
{
    auto search = eventDict.find(eventName);
    if (search != eventDict.end())
    {
        return (search->second);
    }
    return (pos::BackendEvent_Unknown);
}
}; // namespace pos_cli