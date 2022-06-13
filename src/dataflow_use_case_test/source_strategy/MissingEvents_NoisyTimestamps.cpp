#include "MissingEvents_NoisyTimestamps.h"

namespace dataflow_use_case_test {
std::vector<ExpectedSource>
MissingEvents_NoisyTimestamps::CreateSources(UseCaseProblem problem, bool simulate_sensors,
                                             size_t num_events) {
    std::vector<ExpectedSource> result;
    auto time_delta = std::chrono::milliseconds(10);
    std::vector<std::uint32_t> pass_pattern;
    auto time_stddev = std::chrono::milliseconds(2);
    auto sin_offset = 0;
    auto sin_per_second = 0.1;
    TimestampType start_time(std::chrono::nanoseconds(3000000000000000ll));

    SourceConfiguration config
        (simulate_sensors, start_time, time_delta, time_stddev, pass_pattern, num_events, sin_offset, sin_per_second);

    switch (problem) {
        default:throw std::runtime_error("AllEvents_PerfectTimestamps unsupported UseCaseProblem");
        case UseCaseProblem::In0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer: {
            config.sin_offset = 3;
            config.start_time =
                TimestampType(std::chrono::nanoseconds(3000000000000000ll) + std::chrono::milliseconds(5));
            config.pass_pattern = {450};
            result.emplace_back(ExpectedSource(config));
            config.sin_offset = 2;
            config.start_time =
                TimestampType(std::chrono::nanoseconds(3000000000000000ll) + std::chrono::milliseconds(5));
            config.pass_pattern = {25, 47, 3};
            result.emplace_back(ExpectedSource(config));
            config.sin_offset = 1;
            config.start_time = TimestampType(std::chrono::nanoseconds(3000000000000000ll));
            config.pass_pattern = {4, 58, 4};
            result.emplace_back(ExpectedSource(config));
            config.start_time = TimestampType(std::chrono::nanoseconds(3000000000000000ll));
            config.sin_offset = 0;
            config.pass_pattern = {6, 5, 8};
            result.emplace_back(ExpectedSource(config));
            break;
        }
        case UseCaseProblem::In0In1_In2In3_Out0_Out1:
        case UseCaseProblem::In0In1_In2In3_Out0:
            config.start_time = TimestampType(std::chrono::nanoseconds(3000000000000000ll));
            config.sin_offset = 3;
            config.pass_pattern = {10, 45, 8, 89, 50, 4};
            result.emplace_back(ExpectedSource(config));
        case UseCaseProblem::In0In1_In2_Out0:config.sin_offset = 2;
            config.pass_pattern = {5, 69, 47, 12, 36};
            result.emplace_back(ExpectedSource(config));
        case UseCaseProblem::In0In1_Out0:config.sin_offset = 1;
            config.pass_pattern = {80, 45, 20, 7, 85};
            result.emplace_back(ExpectedSource(config));
        case UseCaseProblem::In0_Out0:config.sin_offset = 0;
            config.pass_pattern = {1, 2, 54, 49, 69, 30, 10, 4, 80};
            result.emplace_back(ExpectedSource(config));
    }
    std::reverse(result.begin(), result.end());

    return result;
}

BaseProblemList MissingEvents_NoisyTimestamps::CreateBaseProblems(UseCaseProblem problem, bool do_busy_work) {
    BaseProblemList result;
    std::vector<TimeDurationType> work_delays;
    TimeDurationType expected_frequency = std::chrono::milliseconds(10);
    switch (problem) {
        default:throw std::runtime_error("AllEvents_PerfectTimestamps unsupported UseCaseProblem");
        case UseCaseProblem::In0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer:
        case UseCaseProblem::In0In1_In2In3_Out0_Out1:
        case UseCaseProblem::In0In1_In2In3_Out0:
            work_delays = {std::chrono::milliseconds(150), std::chrono::milliseconds(4), std::chrono::milliseconds(15), std::chrono::milliseconds(20)};
            result.emplace_back(std::make_shared<BaseProblem>(do_busy_work, work_delays, expected_frequency));
        case UseCaseProblem::In0In1_In2_Out0:
            work_delays = {std::chrono::milliseconds(8), std::chrono::milliseconds(1), std::chrono::milliseconds(100), std::chrono::milliseconds(20)};
            result.emplace_back(std::make_shared<BaseProblem>(do_busy_work, work_delays, expected_frequency));
        case UseCaseProblem::In0In1_Out0:
            work_delays = {std::chrono::milliseconds(16), std::chrono::milliseconds(6), std::chrono::milliseconds(20), std::chrono::milliseconds(120)};
            result.emplace_back(std::make_shared<BaseProblem>(do_busy_work, work_delays, expected_frequency));
        case UseCaseProblem::In0_Out0:break;
    }

    std::reverse(result.begin(), result.end());

    return result;
}
}