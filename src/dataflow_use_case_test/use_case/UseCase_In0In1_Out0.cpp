/**
 *   Copyright (C) 2022  Frieder Pankratz <frieder.pankratz@gmail.com>
 *
 *   License in root folder
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
    if (expectedSources.size() != 2)
        throw std::runtime_error("UseCase_In0In1_Out0 expects exactly 2 source");
    if (baseProblems.size() != 1)
        throw std::runtime_error("UseCase_In0In1_Out0 expects exactly 1 base problems");
}

void UseCase_In0In1_Out0::AssignExpectedSources(std::map<size_t,
                                                         std::pair<int, int>> &source_idx_to_time_domain) const {
    source_idx_to_time_domain[0] = std::make_pair(0, 0);
    source_idx_to_time_domain[1] = std::make_pair(0, 1);

}

void UseCase_In0In1_Out0::CreateResult(const std::pair<TimestampType, int> &ts_td,
                                       const std::vector<ExpectedSource> &sources,
                                       std::vector<std::vector<std::tuple<bool,
                                                                          int,
                                                                          TimestampType,
                                                                          TestDataType>>> &result_data) {
    auto &result_list = result_data[ts_td.second];
    auto ts1 = sources[0].GetTimestamp(ts_td.first);
    auto ts2 = sources[1].GetTimestamp(ts_td.first);
    if (ts1.has_value() && ts2.has_value()) {
        TestDataType val1 = sources[0].GetPose(ts_td.first);
        TestDataType val2 = sources[1].GetPose(ts_td.first);
        TestDataType pose = val1 * val2;
        result_list.emplace_back(std::make_tuple(true, 0, ts_td.first, pose));
    } else
        result_list.emplace_back(std::make_tuple(false, 0, ts_td.first, TestDataType::Identity()));
}
}