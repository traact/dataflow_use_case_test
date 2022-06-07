#ifndef TRAACTMULTI_USECASE_H
#define TRAACTMULTI_USECASE_H

#include <map>
#include "TestUtils.h"
#include "ExpectedSource.h"
namespace dataflow_use_case_test {
class DATAFLOW_USE_CASE_TEST_EXPORT UseCase {
 public:
    using TS_TD = std::pair<TimestampType, int>;
    UseCase() = default;
    virtual ~UseCase() = default;
    virtual UseCaseProblem GetProblem() = 0;
    virtual size_t GetTimeDomainCount() = 0;
    virtual void Init(std::vector<ExpectedSource> expectedSources, BaseProblemList baseProblems) = 0;
    BaseProblemList GetBaseProblems();
    const DataList &GetSourceData();
    size_t GetExpectedResultCount();
    const std::vector<std::vector<std::tuple<bool, int, TimestampType, TestDataType >>> &GetExpectedResults();
    const std::vector<UseCase::TS_TD> &GetCleanTimestamps();
    virtual TimeDurationType GetMaxTimestampOffset(int time_domain, int port) = 0;
 protected:

    std::vector<ExpectedSource> expected_sources_;
    std::map<size_t, std::pair<int, int>> source_idx_to_time_domain_and_port;
    BaseProblemList base_problems_;

    std::vector<TS_TD> clean_timestamps_;
    DataList source_data_;
    std::vector<std::vector<std::tuple<bool, int, TimestampType, TestDataType >>> expected_results_;

    void GatherAllTimestamps();
    TimeDurationType GetDelay(const TS_TD &ts_td) const;
};
}

#endif //TRAACTMULTI_USECASE_H
