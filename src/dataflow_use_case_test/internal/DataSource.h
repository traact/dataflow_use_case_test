#ifndef TRAACTMULTI_DATASOURCE_H
#define TRAACTMULTI_DATASOURCE_H

#include <thread>
#include <atomic>
#include <map>
#include "dataflow_use_case_test/TestUtils.h"

namespace dataflow_use_case_test {
class DataSource {
 public:
    DataSource(const DataList &data, DataCallback &dataCallback);

    void Start(TimestampType real_ts_start);
    void WaitForFinish();
    void Stop();

    SCTimestampType TimeStart;
    SCTimestampType TimeEnd;
    TimeDurationType TotalTime;
    TimeDurationType TimeSpendWaiting;
    TimeDurationType TimeSpendInCallback;
    std::map<TimestampType, SCTimestampType> DataTsToSendTs;
 private:
    const DataList &data_;
    DataCallback &data_callback_;
    TimestampType real_start_;
    std::shared_ptr<std::thread> thread_;
    std::atomic_bool running_{false};

    void threadLoop();
};
}

#endif //TRAACTMULTI_DATASOURCE_H
