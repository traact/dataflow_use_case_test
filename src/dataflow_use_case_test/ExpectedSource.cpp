#include "ExpectedSource.h"

#include <utility>

namespace dataflow_use_case_test {
ExpectedSource::ExpectedSource(SourceConfiguration config) : configuration(std::move(config)) {

    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(-1.0, 1.0);
    auto offset_factor = std::bind(distribution, generator);

    max_offset_for_valid = configuration.time_delta / 2;
    if (config.time_stddev >= max_offset_for_valid)
        throw std::runtime_error(
            "ExpectedSource: standard deviation bigger then half the frequency of the source. timestamps ambiguous");

    timestamps.reserve(configuration.num_events);
    all_timestamps.reserve(configuration.num_events);
    auto pass_pattern = configuration.pass_pattern;
    if (pass_pattern.empty())
        pass_pattern.push_back(std::numeric_limits<std::uint32_t>::max());

    TimestampType current_ts = configuration.start_time;
    size_t current_pass_pattern = 0;
    std::uint32_t current_pass_index = 0;
    for (int i = 0; i < configuration.num_events; ++i) {

        all_timestamps.push_back(current_ts);

        current_pass_index++;

        if (current_pass_index > pass_pattern[current_pass_pattern]) {
            current_pass_pattern++;
            current_pass_pattern = current_pass_pattern % pass_pattern.size();
            current_pass_index = 0;
        } else {
            auto offset = std::chrono::nanoseconds(static_cast<std::int64_t>(configuration.time_stddev.count()
                * offset_factor()));
            if (abs(offset) > max_offset_for_valid) {
                offset = max_offset_for_valid;
            }
            timestamps.push_back(current_ts + offset);
        }

        current_ts = current_ts + configuration.time_delta;
    }
}

bool ExpectedSource::ValidTimestamp(TimestampType clean_ts) const {
    auto result = FindTsWithOffset(timestamps.begin(), timestamps.end(), clean_ts, max_offset_for_valid);
    return result.has_value();
}

Eigen::Affine3d ExpectedSource::GetPose(TimestampType clean_ts) const {
    if (!ValidTimestamp(clean_ts))
        throw std::runtime_error(
            "ExpectedSource::getPose was called with ts that corresponded to no valid timestamp in its data");

    using nanoMilliseconds = std::chrono::duration<double, std::milli>;
    double seconds = nanoMilliseconds(clean_ts.time_since_epoch()).count();

    double p = seconds * configuration.sin_per_second;
    double pos = std::sin(configuration.sin_offset + p);

    Eigen::Affine3d pose = Eigen::Affine3d::Identity();
    pose.translation() = Eigen::Vector3d(0, 0, pos);

    return pose;

}

std::optional<TimestampType> ExpectedSource::GetTimestamp(TimestampType clean_ts) const {
    return FindTsWithOffset(timestamps.begin(), timestamps.end(), clean_ts, max_offset_for_valid);
}
}