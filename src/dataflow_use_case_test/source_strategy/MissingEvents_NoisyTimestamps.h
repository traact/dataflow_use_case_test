#ifndef TRAACTMULTI_MISSINGEVENTS_NOISYTIMESTAMPS_H
#define TRAACTMULTI_MISSINGEVENTS_NOISYTIMESTAMPS_H

#include "SourceStrategy.h"
namespace dataflow_use_case_test {
class DATAFLOW_USE_CASE_TEST_EXPORT MissingEvents_NoisyTimestamps : public SourceStrategy {
 public:
    MissingEvents_NoisyTimestamps() = default;
    virtual ~MissingEvents_NoisyTimestamps() = default;
    std::vector<ExpectedSource>
    CreateSources(UseCaseProblem problem, bool simulate_sensors, size_t num_events) override;

    BaseProblemList CreateBaseProblems(UseCaseProblem problem, bool do_busy_work) override;
};
}

#endif //TRAACTMULTI_MISSINGEVENTS_NOISYTIMESTAMPS_H
