#include "UseCase_In0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer.h"

namespace dataflow_use_case_test {
    UseCaseProblem UseCase_In0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer::GetProblem() const {
        return UseCaseProblem::In0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer;
    }

    size_t UseCase_In0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer::GetTimeDomainCount() const {
        return 2;
    }

    void
    UseCase_In0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer::CheckInputs(const std::vector<ExpectedSource> &expectedSources,
                                                                     const BaseProblemList &baseProblems) const {
        if(expectedSources.size() != 4)
            throw std::runtime_error("UseCase_In0In1_In2In3_Out0 expects exactly 4 source");
        if(baseProblems.size() != 3)
            throw std::runtime_error("UseCase_In0In1_In2In3_Out0 expects exactly 3 base problems");
    }

    void UseCase_In0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer::AssignExpectedSources(
            std::map<size_t, std::pair<int, int>> &source_idx_to_time_domain) const {
        source_idx_to_time_domain[0] = std::make_pair(0,0);
        source_idx_to_time_domain[1] = std::make_pair(0,1);
        source_idx_to_time_domain[2] = std::make_pair(1,0);
        source_idx_to_time_domain[3] = std::make_pair(1,1);
    }

    void UseCase_In0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer::CreateResult(const std::pair<TimestampType, int> &ts_td,
                                                                           const std::vector<ExpectedSource> &sources,
                                                                           std::vector<std::vector<std::tuple<bool, int, TimestampType, TestDataType>>> &result_data) {
        auto max_offset = sources[0].configuration.time_delta/2;
        if(ts_td.second == 1){
            auto ts3 = sources[2].GetTimestamp(ts_td.first);
            auto ts4 = sources[3].GetTimestamp(ts_td.first);
            if(ts3.has_value() && ts4.has_value() && TimestampsMatch(ts3.value(), ts4.value(), max_offset))
                td1_result_buffer_ = sources[2].GetPose(ts_td.first) * sources[3].GetPose(ts_td.first);


            return;
        }
        auto& result_list = result_data[ts_td.second];
        auto ts1 = sources[0].GetTimestamp(ts_td.first);
        auto ts2 = sources[1].GetTimestamp(ts_td.first);

        if(ts1.has_value() && ts2.has_value() && TimestampsMatch(ts1.value(), ts2.value(), max_offset)){
            TestDataType val1 = sources[0].GetPose(ts_td.first);
            TestDataType val2 = sources[1].GetPose(ts_td.first);
            TestDataType pose1 = val1 * val2;
            result_list.emplace_back(std::make_tuple(true,0, ts_td.first, pose1));
            if(td1_result_buffer_.has_value()){
                TestDataType pose2 = pose1 * td1_result_buffer_.value();
                result_list.emplace_back(std::make_tuple(true,1, ts_td.first, pose2));
            } else {
                result_list.emplace_back(std::make_tuple(false,1, ts_td.first, TestDataType::Identity()));
            }
        }
        else
            result_list.emplace_back(std::make_tuple(false,0, ts_td.first, TestDataType::Identity()));
    }
}