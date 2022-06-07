#ifndef TRAACTMULTI_TESTDATAFLOWNETWORK_H
#define TRAACTMULTI_TESTDATAFLOWNETWORK_H

#include <dataflow_use_case_test/DataflowNetwork.h>

class TestDataflowNetwork : public dataflow_use_case_test::DataflowNetwork {
 public:
    void Init(dataflow_use_case_test::UseCaseProblem problem,
              const dataflow_use_case_test::BaseProblemList &problem_list,
              const dataflow_use_case_test::ResultCallback &result_callback,
              const dataflow_use_case_test::InvalidCallback &invalid_callback,
              dataflow_use_case_test::TimeDurationType max_ts_offset) override;

    void Start() override;

    void Stop() override;

    void NewData(int time_domain, const dataflow_use_case_test::PortTsDataList &data) override;

 private:
    dataflow_use_case_test::UseCaseProblem problem_;
    dataflow_use_case_test::BaseProblemList problem_list_;
    dataflow_use_case_test::ResultCallback result_callback_;
    dataflow_use_case_test::InvalidCallback invalid_callback_;
    std::vector<std::map<int, std::pair<dataflow_use_case_test::TimestampType, dataflow_use_case_test::TestDataType>>>
        buffer_;
    std::optional<dataflow_use_case_test::TestDataType> get_data(int td, int port);
    std::optional<dataflow_use_case_test::TimestampType> get_ts(int td, int port);
    std::optional<dataflow_use_case_test::TestDataType> td1_buffer_;
    dataflow_use_case_test::TimeDurationType max_ts_offset_;
    void ProcessIn0_Out0(int time_domain);
    void ProcessIn0In1_Out0(int time_domain);
    void ProcessIn0In1_In2_Out0(int time_domain);
    void ProcessIn0In1_In2In3_Out0(int time_domain);
    void ProcessIn0In1_In2In3_Out0_Out1(int time_domain);
    void ProcessIn0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer(int time_domain);

};

#endif //TRAACTMULTI_TESTDATAFLOWNETWORK_H
