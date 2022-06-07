#ifndef TRAACTMULTI_USECASE_IN0_OUT0_H
#define TRAACTMULTI_USECASE_IN0_OUT0_H

#include "UseCase_Behaviour.h"
namespace dataflow_use_case_test {
class DATAFLOW_USE_CASE_TEST_EXPORT UseCase_In0_Out0 : public UseCase_Behaviour {
 public:
    UseCase_In0_Out0() = default;

    UseCaseProblem GetProblem() const override;

    void CheckInputs(const std::vector<ExpectedSource> &expectedSources,
                     const BaseProblemList &baseProblems) const override;

    void AssignExpectedSources(std::map<size_t, std::pair<int, int>> &source_idx_to_time_domain) const override;

    void CreateResult(const std::pair<TimestampType, int> &ts_td, const std::vector<ExpectedSource> &sources,
                      std::vector<std::vector<std::tuple<bool,
                                                         int,
                                                         TimestampType,
                                                         TestDataType>>> &result_data) override;

    size_t GetTimeDomainCount() const override;

};
}

#endif //TRAACTMULTI_USECASE_IN0_OUT0_H
