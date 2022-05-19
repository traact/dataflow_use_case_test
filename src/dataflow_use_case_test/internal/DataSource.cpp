#include "DataSource.h"

namespace dataflow_use_case_test {

    DataSource::DataSource(const DataList &data, DataCallback &dataCallback) : data_(data),
                                                                                     data_callback_(dataCallback) {}

    void DataSource::Start(TimestampType real_ts_start) {
        running_ = true;
        real_start_ = real_ts_start;
        thread_.reset(new std::thread(std::bind(&DataSource::threadLoop, this)));
    }

    void DataSource::Stop() {
        if (running_) {
            running_ = false;
            thread_->join();
        }
    }

    void DataSource::threadLoop() {

        TimeSpendWaiting = TimeDurationType (0);
        TimeSpendInCallback = TimeDurationType (0);
        TimeStart = steady_now();

        TimestampType next_real_ts = now();
        TimestampType current_real_ts = now();


        for (size_t output_count = 0; running_ && output_count < data_.size();++output_count) {

            const auto& next_event = data_[output_count];
            next_real_ts = next_real_ts + next_event.Delay;

            auto time_wait_start = steady_now();
            while(next_real_ts > current_real_ts){
                std::chrono::milliseconds time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(next_real_ts - current_real_ts);
                if(time_diff.count() > 1) {
                    std::this_thread::sleep_for( time_diff - std::chrono::milliseconds(1));
                } else {
                    std::this_thread::yield();
                }
                current_real_ts = now();
            }
            TimeSpendWaiting += steady_now() - time_wait_start;

            auto time_callback_start = steady_now();
            data_callback_(next_event.TimeDomain, next_event.Data);

            TimeSpendInCallback += steady_now() - time_callback_start;
            // get ts of result generating events, in all use cases currently timedomain 0 port 0 drives all outputs
            // store all events of ts 0 for missing events
            if(next_event.TimeDomain == 0){
                for(const auto& data : next_event.Data) {
                    DataTsToSendTs[std::get<1>(data)] = time_callback_start;
                }
            }

        }
        running_ = false;
        TimeEnd = steady_now();
        TotalTime = TimeEnd - TimeStart;
    }

    void DataSource::WaitForFinish() {
        thread_->join();
    }
}