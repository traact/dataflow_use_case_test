/*  BSD 3-Clause License
 *
 *  Copyright (c) 2020, FriederPankratz <frieder.pankratz@gmail.com>
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, this
 *     list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *  3. Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/

#include "UseCase_In0In1_Out0.h"

namespace dataflow_use_case_test {

    UseCaseProblem UseCase_In0In1_Out0::GetProblem() const {
        return UseCaseProblem::In0In1_Out0;
    }

    size_t UseCase_In0In1_Out0::GetTimeDomainCount() const {
        return 1;
    }

    void UseCase_In0In1_Out0::CheckInputs(const std::vector<ExpectedSource> &expectedSources,
                                          const BaseProblemList &baseProblems) const {
        if(expectedSources.size() != 2)
            throw std::runtime_error("UseCase_In0In1_Out0 expects exactly 2 source");
        if(baseProblems.size() != 1)
            throw std::runtime_error("UseCase_In0In1_Out0 expects exactly 1 base problems");
    }

    void UseCase_In0In1_Out0::AssignExpectedSources(std::map<size_t, std::pair<int, int>> &source_idx_to_time_domain) const {
        source_idx_to_time_domain[0] = std::make_pair(0,0);
        source_idx_to_time_domain[1] = std::make_pair(0,1);

    }

    void UseCase_In0In1_Out0::CreateResult(const std::pair<TimestampType, int> &ts_td,
                                           const std::vector<ExpectedSource> &sources,
                                           std::vector<std::vector<std::tuple<bool, int, TimestampType, TestDataType>>> &result_data) {
        auto& result_list = result_data[ts_td.second];
        auto ts1 = sources[0].GetTimestamp(ts_td.first);
        auto ts2 = sources[1].GetTimestamp(ts_td.first);
        if(ts1.has_value() && ts2.has_value()){
            TestDataType val1 = sources[0].GetPose(ts_td.first);
            TestDataType val2 = sources[1].GetPose(ts_td.first);
            TestDataType pose = val1 * val2;
            result_list.emplace_back(std::make_tuple(true,0, ts_td.first, pose));
        }
        else
            result_list.emplace_back(std::make_tuple(false,0, ts_td.first, TestDataType::Identity()));
    }
}