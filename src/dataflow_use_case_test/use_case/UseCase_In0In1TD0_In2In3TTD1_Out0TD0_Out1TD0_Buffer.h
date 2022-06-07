#ifndef TRAACTMULTI_USECASE_IN0IN1TD0_IN2IN3TTD1_OUT0TD0_OUT1TD0_BUFFER_H
#define TRAACTMULTI_USECASE_IN0IN1TD0_IN2IN3TTD1_OUT0TD0_OUT1TD0_BUFFER_H

#include "UseCase_Behaviour.h"

namespace dataflow_use_case_test {
class DATAFLOW_USE_CASE_TEST_EXPORT UseCase_In0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer : public UseCase_Behaviour {
 public:
    UseCaseProblem GetProblem() const override;

    size_t GetTimeDomainCount() const override;

    void CheckInputs(const std::vector<ExpectedSource> &expectedSources,
                     const BaseProblemList &baseProblems) const override;

    void AssignExpectedSources(std::map<size_t, std::pair<int, int>> &source_idx_to_time_domain) const override;

    void CreateResult(const std::pair<TimestampType, int> &ts_td, const std::vector<ExpectedSource> &sources,
                      std::vector<std::vector<std::tuple<bool,
                                                         int,
                                                         TimestampType,
                                                         TestDataType>>> &result_data) override;

 private:
    std::optional<TestDataType> td1_result_buffer_;

};
}

#endif //TRAACTMULTI_USECASE_IN0IN1TD0_IN2IN3TTD1_OUT0TD0_OUT1TD0_BUFFER_H
