#ifndef TRAACTMULTI_dataflow_use_case_test_DATAFLOWNETWORK_H
#define TRAACTMULTI_dataflow_use_case_test_DATAFLOWNETWORK_H

#include <functional>
#include <map>
#include "TestUtils.h"

namespace dataflow_use_case_test {

    class DATAFLOW_USE_CASE_TEST_EXPORT DataflowNetwork {
    public:

        virtual ~DataflowNetwork() = default;

        virtual void Init(dataflow_use_case_test::UseCaseProblem problem, const dataflow_use_case_test::BaseProblemList &problem_list,
                          const dataflow_use_case_test::ResultCallback &result_callback,
                          const dataflow_use_case_test::InvalidCallback &invalid_callback,
                          dataflow_use_case_test::TimeDurationType max_ts_offset) =0;
        virtual void Start() = 0;
        virtual void Stop() = 0;
        virtual void NewData(int time_domain, const PortTsDataList& data) = 0;
    };
}



#endif //TRAACTMULTI_dataflow_use_case_test_DATAFLOWNETWORK_H
