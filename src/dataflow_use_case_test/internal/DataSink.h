#ifndef TRAACTMULTI_DATASINK_H
#define TRAACTMULTI_DATASINK_H
#include <map>
#include <set>
#include "dataflow_use_case_test/TestUtils.h"
#include <mutex>
namespace dataflow_use_case_test {
    class DataSink {
    public:
        DataSink(std::size_t time_domain_count, std::size_t expected_result_count);
        void Receive(int time_domain, PortTsData data);
        void ReceiveInvalid(int time_domain, int port, TimestampType timestamp);

        std::vector< std::map< TimestampType, std::map<int, TestDataType > > > GetDataEvents();
        std::vector< std::map< TimestampType, std::set<int > > > GetInvalidEvents();
        std::map<std::tuple<int, int, TimestampType>, SCTimestampType > DataTsToReceiveTs;
    private:
        std::vector<std::vector<PortTsData>> valid_events_;
        std::vector<std::vector<std::pair<int, TimestampType>>> invalid_events_;
        std::mutex valid_events_lock_;
        std::mutex invalid_events_lock_;
    };
}




#endif //TRAACTMULTI_DATASINK_H
