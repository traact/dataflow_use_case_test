#include "DataflowTest.h"

#include "dataflow_use_case_test/internal/DataSink.h"
#include "dataflow_use_case_test/internal/DataSource.h"
#include <spdlog/spdlog.h>
#include <numeric>
namespace dataflow_use_case_test {

DataflowTest::DataflowTest(std::shared_ptr<DataflowNetwork> network, std::shared_ptr<UseCase> useCase)
    : network_(std::move(network)), useCase_(std::move(useCase)) {}

bool DataflowTest::Test() {

    // init test framework and connect
    // data source
    DataCallback
        data_callback = std::bind(&DataflowNetwork::NewData, network_, std::placeholders::_1, std::placeholders::_2);
    DataSource data_source(useCase_->GetSourceData(), data_callback);
    // data sink
    DataSink data_sink(useCase_->GetTimeDomainCount(), useCase_->GetExpectedResultCount());
    auto result_callback = std::bind(&DataSink::Receive, &data_sink, std::placeholders::_1, std::placeholders::_2);
    auto invalid_callback = std::bind(&DataSink::ReceiveInvalid,
                                      &data_sink,
                                      std::placeholders::_1,
                                      std::placeholders::_2,
                                      std::placeholders::_3);
    // TODO other init for dataflow network to get the max ts offset, it depends on the data sources and use case, use case return list of offset and td port of result ports?
    network_->Init(useCase_->GetProblem(), useCase_->GetBaseProblems(), result_callback, invalid_callback,
                   useCase_->GetMaxTimestampOffset(0, 0));

    // start everything
    auto time_start = steady_now();
    network_->Start();
    auto time_network_started = steady_now();
    data_source.Start(now());
    // wait for source to finish sending
    data_source.WaitForFinish();
    auto time_source_finished = steady_now();

    // wait for network to stop
    network_->Stop();
    auto time_network_stopped = steady_now();

    auto FindWithOffset = [](const auto &data, TimestampType ts, int port, TimeDurationType max_offset) {
        auto result_ts = std::find_if(data.cbegin(), data.cend(), [ts, port, max_offset](const auto &value) {
            auto port_result = std::get<1>(value).find(port);
            if (port_result == std::get<1>(value).cend())
                return false;

            TimestampType min_ts = ts - max_offset;
            TimestampType max_ts = ts + max_offset;

            return min_ts <= std::get<0>(value) && std::get<0>(value) < max_ts;
        });

        return result_ts;
    };

    const auto &expected_results = useCase_->GetExpectedResults();
    auto result_events = data_sink.GetDataEvents();
    auto invalid_events = data_sink.GetInvalidEvents();

    bool all_ok = true;
    std::vector<size_t> expected_valid_count;
    std::vector<size_t> expected_invalid_count;
    expected_valid_count.resize(useCase_->GetTimeDomainCount());
    expected_invalid_count.resize(useCase_->GetTimeDomainCount());
    size_t valid_results = 0;
    size_t invalid_results = 0;
    size_t total_valid_results = 0;
    size_t total_invalid_results = 0;
    for (size_t time_domain_idx = 0; time_domain_idx < expected_results.size(); ++time_domain_idx) {
        expected_valid_count[time_domain_idx] = 0;
        expected_invalid_count[time_domain_idx] = 0;
        const auto &expected_time_domain = expected_results[time_domain_idx];
        const auto &result_time_domain = result_events[time_domain_idx];
        const auto &invalid_time_domain = invalid_events[time_domain_idx];

        for (const auto &expected : expected_time_domain) {
            auto expected_valid = std::get<0>(expected);
            auto expected_port = std::get<1>(expected);
            auto expected_ts = std::get<2>(expected);

            auto in_result = FindWithOffset(result_time_domain, expected_ts, expected_port,
                                            useCase_->GetMaxTimestampOffset(time_domain_idx, expected_port));
            auto in_invalid = FindWithOffset(invalid_time_domain, expected_ts, expected_port,
                                             useCase_->GetMaxTimestampOffset(time_domain_idx, expected_port));

            if (expected_valid) {
                expected_valid_count[time_domain_idx]++;
                total_valid_results++;

                if (in_result == result_time_domain.end()) {
                    SPDLOG_ERROR("Expected valid result for time domain {0}, port {1}, ts {2}, no found",
                                  time_domain_idx,
                                  expected_port,
                                  expected_ts.time_since_epoch().count());
                    all_ok = false;
                    continue;
                }
                if (in_invalid != invalid_time_domain.end()) {
                    SPDLOG_ERROR(
                        "Expected valid result for time domain {0}, port {1}, ts {2}, but found in invalid results",
                        time_domain_idx,
                        expected_port,
                        expected_ts.time_since_epoch().count());
                    all_ok = false;
                    continue;
                }

                auto port_result = in_result->second.find(expected_port);
                if (port_result == in_result->second.end()) {
                    SPDLOG_ERROR(
                        "Expected valid result for time domain {0}, port {1}, ts {2}, but port not found in valid results",
                        time_domain_idx,
                        expected_port,
                        expected_ts.time_since_epoch().count());
                    all_ok = false;
                    continue;
                }
                auto expected_value = std::get<3>(expected);
                if (!port_result->second.isApprox(expected_value)) {
                    Eigen::IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");
                    std::stringstream ss_expected, ss_result;
                    ss_expected << expected_value.matrix().format(CleanFmt);
                    ss_result << port_result->second.matrix().format(CleanFmt);
                    SPDLOG_ERROR(
                        "Expected valid result for time domain {0}, port {1}, ts {2}, values differ, expected:\n{3}\nresult:\n{4}",
                        time_domain_idx,
                        expected_port,
                        expected_ts.time_since_epoch().count(),
                        ss_expected.str(),
                        ss_result.str());
                    all_ok = false;
                    continue;
                }
                valid_results++;
            } else {
                expected_invalid_count[time_domain_idx]++;
                total_invalid_results++;
                if (in_result != result_time_domain.end()) {
                    SPDLOG_ERROR(
                        "Expected invalid result for time domain {0}, port {1}, ts {2}, but found in valid results",
                        time_domain_idx,
                        expected_port,
                        expected_ts.time_since_epoch().count());
                    all_ok = false;
                    continue;
                }
                if (in_invalid == invalid_time_domain.end()) {
                    //SPDLOG_WARN("Expected invalid result for time domain {0}, port {1}, ts {2}, but no found", time_domain_idx, expected_port, expected_ts.time_since_epoch().count());
                    continue;
                }

                auto port_result = in_invalid->second.find(expected_port);
                if (port_result == in_invalid->second.end()) {
                    SPDLOG_ERROR(
                        "Expected invalid result for time domain {0}, port {1}, ts {2}, but port not found in invalid results",
                        time_domain_idx,
                        expected_port,
                        expected_ts.time_since_epoch().count());
                    all_ok = false;
                    continue;
                }
                invalid_results++;
            }
        }
    }

    auto network_start_time = time_network_started - time_start;
    auto source_time = time_source_finished - time_network_started;
    auto network_stop_time = time_network_stopped - time_source_finished;
    auto total_time = time_network_stopped - time_start;
    auto total_events = valid_results + invalid_results;
    using nanoToMicro = std::chrono::duration<float, std::micro>;
    using nanoToMilli = std::chrono::duration<float, std::milli>;
    using nanoToSeconds = std::chrono::duration<float, std::ratio<1, 1> >;
    double events_per_second = total_events / (nanoToMilli(total_time).count() / 1000.0);

    std::stringstream ss;
    ss << "Dataflow Statistic\n";
    ss << fmt::format("Total Result Events: {4} : Valid Results {0}/{1} ; Invalid Results {2}/{3}\n",
                      valid_results,
                      total_valid_results,
                      invalid_results,
                      total_invalid_results,
                      total_events);
    ss << "Total Time: " << nanoToSeconds(total_time).count() << "s  Events/Second: " << events_per_second << "\n";
    ss << "Network start: " << nanoToMicro(network_start_time).count() << "us\n";
    ss << "Source Sending: " << nanoToSeconds(source_time).count() << "s\n";
    ss << "\tTotal loop:\t" << nanoToSeconds(data_source.TotalTime).count() << "s\n";
    ss << "\t\twaiting:\t\t" << nanoToSeconds(data_source.TimeSpendWaiting).count() << "s\n";
    ss << "\t\tin callback:\t" << nanoToSeconds(data_source.TimeSpendInCallback).count() << "s\n";
    ss << "Network stop: " << nanoToMicro(network_stop_time).count() << "us\n";

    std::map<std::pair<int, int>, std::vector<TimeDurationType> > delays;
    for (const auto &receive_event : data_sink.DataTsToReceiveTs) {
        int td;
        int port;
        TimestampType event_ts;
        std::tie(td, port, event_ts) = receive_event.first;
        SCTimestampType receive_ts = receive_event.second;

        auto send_ts = data_source.DataTsToSendTs.find(event_ts);
        if (send_ts == data_source.DataTsToSendTs.end()) {
            SPDLOG_WARN("no sending event found for result event {0}, test broken", event_ts.time_since_epoch().count());
            continue;
        }
        auto delay = receive_ts - send_ts->second;
        delays[std::make_pair(td, port)].emplace_back(delay);
    }
    ss << "Delay on result ports:\n";
    for (const auto &td_port_delay : delays) {
        int td, port;
        std::tie(td, port) = td_port_delay.first;
        const auto &delay_data = td_port_delay.second;

        auto total_delay = std::accumulate(delay_data.cbegin(), delay_data.cend(), TimeDurationType(0));
        auto mean_delay = nanoToMicro(total_delay) / delay_data.size();

        std::vector<double> diff_delay(delay_data.size());
        std::transform(delay_data.begin(),
                       delay_data.end(),
                       diff_delay.begin(),
                       [mean_delay](TimeDurationType x) { return nanoToMicro(x).count() - mean_delay.count(); });

        auto sq_sum_delay = std::inner_product(diff_delay.begin(), diff_delay.end(), diff_delay.begin(), 0.0f);
        double stddev_delay = std::sqrt(sq_sum_delay / delay_data.size());

        ss << fmt::format("TD: {0} Port: {1} Mean: {2:4f}us StdDev {3:4f}us Events: {4}\n",
                          td,
                          port,
                          mean_delay.count(),
                          stddev_delay,
                          delay_data.size());

    }

    SPDLOG_INFO(ss.str());

    return all_ok;
}
}