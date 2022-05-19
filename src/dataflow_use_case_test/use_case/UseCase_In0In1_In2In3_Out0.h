#ifndef TRAACTMULTI_USECASE_IN0IN1_IN2IN3_OUT0_H
#define TRAACTMULTI_USECASE_IN0IN1_IN2IN3_OUT0_H

#include "UseCase_Behaviour.h"

namespace dataflow_use_case_test {
    class DATAFLOW_USE_CASE_TEST_EXPORT UseCase_In0In1_In2In3_Out0 : public UseCase_Behaviour{
    public:
        UseCaseProblem GetProblem() const override;

        size_t GetTimeDomainCount() const override;

        void CheckInputs(const std::vector<ExpectedSource> &expectedSources,
                         const BaseProblemList &baseProblems) const override;

        void AssignExpectedSources(std::map<size_t, std::pair<int, int>> &source_idx_to_time_domain) const override;

        void CreateResult(const std::pair<TimestampType, int> &ts_td, const std::vector<ExpectedSource> &sources,
                          std::vector<std::vector<std::tuple<bool, int, TimestampType, TestDataType>>> &result_data) override;

    };
}


#endif //TRAACTMULTI_USECASE_IN0IN1_IN2IN3_OUT0_H
