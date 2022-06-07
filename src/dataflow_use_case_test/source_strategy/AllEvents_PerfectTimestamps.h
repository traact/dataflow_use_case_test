#ifndef TRAACTMULTI_ALLEVENTS_PERFECTTIMESTAMPS_H
#define TRAACTMULTI_ALLEVENTS_PERFECTTIMESTAMPS_H
#include "SourceStrategy.h"
namespace dataflow_use_case_test {
class DATAFLOW_USE_CASE_TEST_EXPORT AllEvents_PerfectTimestamps : public SourceStrategy {
 public:
    AllEvents_PerfectTimestamps() = default;
    virtual ~AllEvents_PerfectTimestamps() = default;
    std::vector<ExpectedSource>
    CreateSources(UseCaseProblem problem, bool simulate_sensors, size_t num_events) override;

    BaseProblemList CreateBaseProblems(UseCaseProblem problem, bool do_busy_work) override;
};
}

#endif //TRAACTMULTI_ALLEVENTS_PERFECTTIMESTAMPS_H
