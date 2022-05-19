#include <set>
#include "UseCase.h"

namespace dataflow_use_case_test {

    BaseProblemList UseCase::GetBaseProblems() {
        return base_problems_;
    }

    const DataList &UseCase::GetSourceData() {
        return source_data_;
    }

    std::size_t UseCase::GetExpectedResultCount() {
        std::size_t result_count = 0;
        for(const auto& result_list : expected_results_)
            result_count = std::max(result_count, result_list.size());
        return result_count;
    }

    const std::vector<std::vector<std::tuple<bool, int, TimestampType, TestDataType >>> &UseCase::GetExpectedResults() {
        return expected_results_;
    }

    const std::vector<UseCase::TS_TD> &UseCase::GetCleanTimestamps() {
        return clean_timestamps_;
    }

    void UseCase::GatherAllTimestamps() {

        std::set<std::pair<TimestampType, int>> all_ts;
        for (const auto& source_td : source_idx_to_time_domain_and_port) {
            for(const auto& ts : expected_sources_[source_td.first].all_timestamps){
                all_ts.emplace(std::make_pair(ts, source_td.second.first));
            }

        }
        clean_timestamps_.clear();
        clean_timestamps_.reserve(all_ts.size());
        std::copy(all_ts.begin(), all_ts.end(), std::back_inserter(clean_timestamps_));
        std::sort(clean_timestamps_.begin(), clean_timestamps_.end());


    }

    TimeDurationType UseCase::GetDelay(const UseCase::TS_TD &ts_td) const {
        auto current_result = std::find(clean_timestamps_.cbegin(), clean_timestamps_.cend(), ts_td);

        if(current_result != clean_timestamps_.cbegin()) {
            auto prev_result = std::prev(current_result);
            return current_result->first - prev_result->first;
        } else {
            return TimeDurationType (0);
        }
    }


}