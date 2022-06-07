#include "DataSink.h"

namespace dataflow_use_case_test {

DataSink::DataSink(size_t time_domain_count, size_t expected_result_count) {
    valid_events_.resize(time_domain_count);
    invalid_events_.resize(time_domain_count);
    for (size_t i = 0; i < time_domain_count; ++i) {
        valid_events_[i].reserve(expected_result_count);
        invalid_events_[i].reserve(expected_result_count);
    }
}

void DataSink::Receive(int time_domain, PortTsData data) {
    const std::lock_guard<std::mutex> lock(valid_events_lock_);
    valid_events_[time_domain].emplace_back(data);
    DataTsToReceiveTs.emplace(std::make_pair(std::make_tuple(time_domain, std::get<0>(data), std::get<1>(data)),
                                             steady_now()));
}

void DataSink::ReceiveInvalid(int time_domain, int port, TimestampType timestamp) {
    const std::lock_guard<std::mutex> lock(invalid_events_lock_);
    invalid_events_[time_domain].emplace_back(std::make_pair(port, timestamp));
    DataTsToReceiveTs.emplace(std::make_pair(std::make_tuple(time_domain, port, timestamp), steady_now()));
}

std::vector<std::map<TimestampType, std::map<int, TestDataType> > > DataSink::GetDataEvents() {
    std::vector<std::map<TimestampType, std::map<int, TestDataType>>> result;
    result.resize(valid_events_.size());
    for (size_t time_domain = 0; time_domain < valid_events_.size(); ++time_domain) {
        for (size_t index = 0; index < valid_events_[time_domain].size(); ++index) {
            const auto &port_ts_data = valid_events_[time_domain][index];
            result[time_domain][std::get<1>(port_ts_data)][std::get<0>(port_ts_data)] = std::get<2>(port_ts_data);
        }
    }

    return result;
}

std::vector<std::map<TimestampType, std::set<int> > > DataSink::GetInvalidEvents() {
    std::vector<std::map<TimestampType, std::set<int>>> result;
    result.resize(invalid_events_.size());
    for (size_t time_domain = 0; time_domain < invalid_events_.size(); ++time_domain) {
        for (size_t index = 0; index < invalid_events_[time_domain].size(); ++index) {
            const auto &port_ts = invalid_events_[time_domain][index];
            result[time_domain][std::get<1>(port_ts)].emplace(std::get<0>(port_ts));
        }
    }
    return result;
}
}