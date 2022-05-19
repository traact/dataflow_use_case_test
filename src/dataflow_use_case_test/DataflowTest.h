#ifndef TRAACTMULTI_DATAFLOWTEST_H
#define TRAACTMULTI_DATAFLOWTEST_H

#include <memory>

#include "DataflowNetwork.h"
#include "UseCase.h"

namespace dataflow_use_case_test {
    class DATAFLOW_USE_CASE_TEST_EXPORT DataflowTest {
    public:
        DataflowTest(std::shared_ptr<DataflowNetwork> network, std::shared_ptr<UseCase> useCase);
        virtual ~DataflowTest() = default;

        bool Test();
    private:
        std::shared_ptr<DataflowNetwork> network_;
        std::shared_ptr<UseCase> useCase_;
    };
}




#endif //TRAACTMULTI_DATAFLOWTEST_H
