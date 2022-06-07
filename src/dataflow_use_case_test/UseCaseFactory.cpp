#include "UseCaseFactory.h"
#include "dataflow_use_case_test/use_case/UseCase_Generic.h"
#include "dataflow_use_case_test/use_case/UseCase_Behaviour.h"
#include "dataflow_use_case_test/use_case/UseCase_In0_Out0.h"
#include "dataflow_use_case_test/use_case/UseCase_In0In1_Out0.h"
#include "dataflow_use_case_test/use_case/UseCase_In0In1_In2_Out0.h"
#include "dataflow_use_case_test/use_case/UseCase_In0In1_In2In3_Out0.h"
#include "dataflow_use_case_test/use_case/UseCase_In0In1_In2In3_Out0_Out1.h"
#include "dataflow_use_case_test/use_case/UseCase_In0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer.h"

namespace dataflow_use_case_test {

static std::unique_ptr<UseCase_Behaviour> CreateBehaviour(UseCaseProblem problem) {
    switch (problem) {
        case UseCaseProblem::In0_Out0:return std::make_unique<UseCase_In0_Out0>();
        case UseCaseProblem::In0In1_Out0:return std::make_unique<UseCase_In0In1_Out0>();
        case UseCaseProblem::In0In1_In2_Out0:return std::make_unique<UseCase_In0In1_In2_Out0>();
        case UseCaseProblem::In0In1_In2In3_Out0:return std::make_unique<UseCase_In0In1_In2In3_Out0>();
        case UseCaseProblem::In0In1_In2In3_Out0_Out1:return std::make_unique<UseCase_In0In1_In2In3_Out0_Out1>();
        case UseCaseProblem::In0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer:
            return std::make_unique<UseCase_In0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer>();
        default:throw std::runtime_error("UseCaseFactory unsupported UseCaseProblem");
    }
}

std::shared_ptr<UseCase>
UseCaseFactory::Create(UseCaseProblem problem, const std::unique_ptr<SourceStrategy> &source_strategy,
                       bool simulate_sensors, bool do_busy_work, size_t num_events) {
    auto use_case = std::make_shared<UseCase_Generic>(CreateBehaviour(problem));
    use_case->Init(source_strategy->CreateSources(problem, simulate_sensors, num_events),
                   source_strategy->CreateBaseProblems(problem,
                                                       do_busy_work));
    return use_case;
}
}