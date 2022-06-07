#ifndef TRAACTMULTI_USECASE_GENERIC_H
#define TRAACTMULTI_USECASE_GENERIC_H

#include "dataflow_use_case_test/UseCase.h"
#include "UseCase_Behaviour.h"

namespace dataflow_use_case_test {
class DATAFLOW_USE_CASE_TEST_EXPORT UseCase_Generic : public UseCase {
 public:
    UseCase_Generic(std::unique_ptr<UseCase_Behaviour> &&behaviour);

    UseCaseProblem GetProblem() override;

    size_t GetTimeDomainCount() override;

    void Init(std::vector<ExpectedSource> expectedSources, BaseProblemList baseProblems) override;

    TimeDurationType GetMaxTimestampOffset(int time_domain, int port) override;

 private:
    std::unique_ptr<UseCase_Behaviour> behaviour_;

    void CreateDataEvents(const std::map<bool, std::map<size_t, TimestampType>> &source_data,
                          const TS_TD &ts_td);

    void CreateData(const TS_TD &ts_td, const TimeDurationType &delay,
                    const std::map<size_t, TimestampType> &data);
};
}

#endif //TRAACTMULTI_USECASE_GENERIC_H
