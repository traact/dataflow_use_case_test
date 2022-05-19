#ifndef TRAACTMULTI_USECASE_BEHAVIOUR_H
#define TRAACTMULTI_USECASE_BEHAVIOUR_H

#include <map>
#include "dataflow_use_case_test/TestUtils.h"
#include "dataflow_use_case_test/ExpectedSource.h"

namespace dataflow_use_case_test {
    class DATAFLOW_USE_CASE_TEST_EXPORT UseCase_Behaviour
    {
    public:
        virtual ~UseCase_Behaviour() = default;
        virtual UseCaseProblem GetProblem() const = 0;
        virtual size_t GetTimeDomainCount() const = 0;
        virtual void CheckInputs(const std::vector<ExpectedSource>& expectedSources, const BaseProblemList& baseProblems) const = 0;
        virtual void AssignExpectedSources(std::map<size_t, std::pair<int, int>> &source_idx_to_time_domain) const = 0;
        virtual void CreateResult(const std::pair<TimestampType, int>& ts_td, const std::vector<ExpectedSource>& sources, std::vector<std::vector<std::tuple<bool, int, TimestampType , TestDataType >>>& result_data) = 0;
    };
}

#endif //TRAACTMULTI_USECASE_BEHAVIOUR_H
