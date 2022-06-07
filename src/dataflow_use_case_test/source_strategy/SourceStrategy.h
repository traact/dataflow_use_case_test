#ifndef TRAACTMULTI_SOURCESTRATEGY_H
#define TRAACTMULTI_SOURCESTRATEGY_H

#include "dataflow_use_case_test/TestUtils.h"
#include "dataflow_use_case_test/ExpectedSource.h"
namespace dataflow_use_case_test {
class DATAFLOW_USE_CASE_TEST_EXPORT SourceStrategy {
 public:
    SourceStrategy() = default;
    virtual ~SourceStrategy() = default;
    virtual std::vector<ExpectedSource> CreateSources(UseCaseProblem problem,
                                                      bool simulate_sensors,
                                                      size_t num_events) = 0;
    virtual BaseProblemList CreateBaseProblems(UseCaseProblem problem, bool do_busy_work) = 0;
};
}

#endif //TRAACTMULTI_SOURCESTRATEGY_H
