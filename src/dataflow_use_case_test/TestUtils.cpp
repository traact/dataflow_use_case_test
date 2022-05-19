#include "TestUtils.h"

namespace dataflow_use_case_test {
    SourceConfiguration::SourceConfiguration() {}

    SourceConfiguration::SourceConfiguration(bool simulateSensors, TimestampType startTime, TimeDurationType timeDelta,
                                             TimeDurationType timeStdDev,
                                             std::vector<std::uint32_t> passPattern, size_t numEvents,
                                             double sin_offset,
                                             double sin_per_second)
            : simulate_sensors(simulateSensors), start_time(startTime), time_delta(timeDelta), time_stddev(timeStdDev),
               pass_pattern(std::move(passPattern)), num_events(numEvents),
              sin_offset(sin_offset), sin_per_second(sin_per_second) {}
}