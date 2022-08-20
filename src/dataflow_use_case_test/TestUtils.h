
#ifndef TRAACTMULTI_dataflow_use_case_test_TESTUTILS_H
#define TRAACTMULTI_dataflow_use_case_test_TESTUTILS_H

#include <chrono>
#include <memory>
#include <utility>
#include <vector>
#include <optional>
#include <Eigen/Core>
#include <Eigen/Geometry>
//#include "dataflow_use_case_test/dataflow_use_case_test_export.h"

#define DATAFLOW_USE_CASE_TEST_EXPORT

namespace dataflow_use_case_test {
using TimeDurationType = std::chrono::duration<int64_t, std::nano>;
using SCTimestampType = std::chrono::time_point<std::chrono::steady_clock, TimeDurationType>;
using TimestampType = std::chrono::time_point<std::chrono::system_clock, TimeDurationType>;
using TestDataType = Eigen::Affine3f;
using PortTsData = std::tuple<int, TimestampType, TestDataType>;
using PortTsDataList = std::vector<PortTsData>;

inline static TimestampType now() {
    return std::chrono::system_clock::now();
}

inline static auto steady_now() {
    return std::chrono::steady_clock::now();
}

using DataCallback = std::function<void(int, const PortTsDataList &)>;
using InvalidCallback = std::function<void(int, int, TimestampType)>;
using ResultCallback = std::function<void(int, PortTsData)>;

enum class UseCaseProblem {
    In0_Out0 = 0, // pass input 0 directly to output 0
    In0In1_Out0,  // when sync input 0 and 1 then  solve(0,1) and pass to output 0
    In0In1_In2_Out0,  // when sync input 0,1 and 2 then r0 = solve(0,1) final = solve(r0,2) and pass to output 0
    In0In1_In2In3_Out0, // when sync input 0,1,2 and 3 then r0 = solve(0,1) r1 = solve(2,3) final = solve(r0,r1) and pass to output 0
    In0In1_In2In3_Out0_Out1, // when sync input 0,1 then r0 = solve(0,1) and pass to output 0; when sync input 2,3 and 0,1(or r0) then r1 = solve(2,3) final = solve(r0,r1) and pass to output 1
    In0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer, // first test with async sources. when sync input 2,3 then r1 = solve(2,3) ;when sync input 0,1 then r0 = solve(0,1) and pass to output 0, when a buffered r1 is available then final = solve(r0,r1) and pass to output 1
};
constexpr const char *UseCaseProblemToString(UseCaseProblem p) {
    switch (p) {
        case UseCaseProblem::In0_Out0: return "In0_Out0";
        case UseCaseProblem::In0In1_Out0: return "In0In1_Out0";
        case UseCaseProblem::In0In1_In2_Out0: return "In0In1_In2_Out0";
        case UseCaseProblem::In0In1_In2In3_Out0: return "In0In1_In2In3_Out0";
        case UseCaseProblem::In0In1_In2In3_Out0_Out1: return "In0In1_In2In3_Out0_Out1";
        case UseCaseProblem::In0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer: return "In0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer";
        default: return "invalid";
            //default: throw std::invalid_argument("Unimplemented item");
    }
}

struct BaseProblem {
    BaseProblem() = default;
    BaseProblem(bool doBusyWork, const std::vector<TimeDurationType> &busyTime,
                const TimeDurationType &expectedInputTimeDelta) : DoBusyWork(doBusyWork), busy_time(busyTime),
                                                                  expected_input_time_delta(
                                                                      expectedInputTimeDelta) {}

    bool DoBusyWork{false};
    std::vector<TimeDurationType> busy_time;
    TimeDurationType expected_input_time_delta;

    TimeDurationType GetTimeDeltaForTs(TimestampType ts) {
        size_t deltaIndex = ts.time_since_epoch().count() / expected_input_time_delta.count();
        deltaIndex = deltaIndex % busy_time.size();

        return busy_time[deltaIndex];
    }

    TestDataType Execute(TimestampType ts, const TestDataType &input1, const TestDataType &input2) {

        if (DoBusyWork) {
            TimestampType real_ts = now();
            TimestampType real_ts_until = now() + GetTimeDeltaForTs(ts);

            while (real_ts_until > real_ts)
                real_ts = now();
        }
        return input1 * input2;
    }
};

using BaseProblemList = std::vector<std::shared_ptr<BaseProblem>>;

struct DataSourceEvent {
    TimeDurationType Delay;
    int TimeDomain;
    PortTsDataList Data;

};

using DataList = std::vector<DataSourceEvent>;

struct DATAFLOW_USE_CASE_TEST_EXPORT SourceConfiguration {
    SourceConfiguration();

    SourceConfiguration(bool simulateSensors, TimestampType startTime, TimeDurationType timeDelta,
                        TimeDurationType timeStdDev,
                        std::vector<std::uint32_t> passPattern, size_t numEvents, float sin_offset,
                        float sin_per_second);

    bool simulate_sensors;
    TimestampType start_time;
    TimeDurationType time_delta;
    TimeDurationType time_stddev;
    std::vector<std::uint32_t> pass_pattern;
    size_t num_events;
    float sin_offset;
    float sin_per_second;

};

static bool TimestampsMatch(TimestampType ts1, TimestampType ts2, TimeDurationType max_offset) {
    auto diff = ts1 - ts2;
    return -max_offset <= diff && diff < max_offset;
}

template<typename _InputIterator>
std::optional<TimestampType> FindTsWithOffset(const _InputIterator &begin,
                                              const _InputIterator &end,
                                              TimestampType ts,
                                              TimeDurationType max_offset) {
    auto result_ts = std::find_if(begin, end, [ts, max_offset](const auto &value) {
        return TimestampsMatch(ts, value, max_offset);
    });

    if (result_ts == end) {
        return {};
    } else {
        return {*result_ts};
    }
}

}

#endif //TRAACTMULTI_dataflow_use_case_test_TESTUTILS_H
