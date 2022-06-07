#ifndef TRAACTMULTI_USECASEFACTORY_H
#define TRAACTMULTI_USECASEFACTORY_H

#include "TestUtils.h"
#include "UseCase.h"
#include "dataflow_use_case_test/source_strategy/SourceStrategy.h"
namespace dataflow_use_case_test {
class DATAFLOW_USE_CASE_TEST_EXPORT UseCaseFactory {
 public:
    static std::shared_ptr<UseCase>
    Create(UseCaseProblem problem, const std::unique_ptr<SourceStrategy> &source_strategy,
           bool simulate_sensors, bool do_busy_work, size_t num_events);

 private:

};
}

#endif //TRAACTMULTI_USECASEFACTORY_H
