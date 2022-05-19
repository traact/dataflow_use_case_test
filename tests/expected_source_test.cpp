#include <gtest/gtest.h>

#include <dataflow_use_case_test/ExpectedSource.h>
#include <spdlog/spdlog.h>
TEST(ExpectedSource, PerfectData) {
    using namespace dataflow_use_case_test;

    auto time_delta = std::chrono::milliseconds(10);
    std::vector<std::uint32_t> pass_pattern;
    auto time_stddev = std::chrono::milliseconds(0);
    auto num_events = 10;
    auto simulate_sensors = false;
    auto sin_offset = 0;
    auto sin_per_second = 0.1;
    auto start_time = now();

    SourceConfiguration config(simulate_sensors, start_time, time_delta, time_stddev, pass_pattern, num_events, sin_offset, sin_per_second );
    ExpectedSource source(config);

    EXPECT_EQ(source.all_timestamps.size(), num_events);
    EXPECT_EQ(source.timestamps.size(), num_events);

    auto ts = start_time;
    for (std::size_t i = 0; i < num_events; ++i) {

        EXPECT_TRUE(source.ValidTimestamp(ts));
        EXPECT_EQ(source.GetTimestamp(ts).value(), ts);
        EXPECT_NO_THROW(source.GetPose(ts));

        ts += time_delta;
    }


}

TEST(ExpectedSource, NoisyData) {
    using namespace dataflow_use_case_test;
    auto time_delta = std::chrono::milliseconds(10);
    std::vector<std::uint32_t> pass_pattern;
    auto time_stddev = std::chrono::milliseconds(4);
    auto num_events = 10;
    auto simulate_sensors = false;
    auto sin_offset = 0;
    auto sin_per_second = 0.1;
    auto start_time = now();

    auto max_offset = time_delta/2;

    SourceConfiguration config(simulate_sensors, start_time, time_delta, time_stddev, pass_pattern, num_events, sin_offset, sin_per_second );
    ExpectedSource source(config);

    EXPECT_EQ(source.all_timestamps.size(), num_events);
    EXPECT_EQ(source.timestamps.size(), num_events);

    auto ts = start_time;
    for (std::size_t i = 0; i < num_events; ++i) {
        EXPECT_TRUE(source.ValidTimestamp(ts));
        EXPECT_GE(source.GetTimestamp(ts).value(), ts-max_offset);
        EXPECT_LE(source.GetTimestamp(ts).value(), ts+max_offset);
        EXPECT_NO_THROW(source.GetPose(ts));

        ts += time_delta;
    }

}

TEST(ExpectedSource, TooMuchNoise) {
    using namespace dataflow_use_case_test;
    auto time_delta = std::chrono::milliseconds(10);
    std::vector<std::uint32_t> pass_pattern;
    auto time_stddev = std::chrono::milliseconds(5);
    auto num_events = 10;
    auto simulate_sensors = false;
    auto sin_offset = 0;
    auto sin_per_second = 0.1;
    auto start_time = now();

    auto max_offset = time_delta/2;

    SourceConfiguration config(simulate_sensors, start_time, time_delta, time_stddev, pass_pattern, num_events, sin_offset, sin_per_second );

    EXPECT_ANY_THROW(auto foo = ExpectedSource(config));


}

TEST(ExpectedSource, DropEverySecondData) {
    using namespace dataflow_use_case_test;
    auto time_delta = std::chrono::milliseconds(10);
    std::vector<std::uint32_t> pass_pattern{1};
    auto time_stddev = std::chrono::milliseconds(0);
    auto num_events = 10;
    auto simulate_sensors = false;
    auto sin_offset = 0;
    auto sin_per_second = 0.1;
    auto start_time = now();

    SourceConfiguration config(simulate_sensors, start_time, time_delta, time_stddev, pass_pattern, num_events, sin_offset, sin_per_second );
    ExpectedSource source(config);

    EXPECT_EQ(source.all_timestamps.size(), num_events);
    EXPECT_EQ(source.timestamps.size(), num_events/2);

    auto ts = start_time;
    for (std::size_t i = 0; i < num_events; ++i) {
        bool valid = (i+1) % 2;
        EXPECT_EQ(source.ValidTimestamp(ts), valid);
        if(valid)
            EXPECT_NO_THROW(source.GetPose(ts));

        ts += time_delta;
    }
}

TEST(ExpectedSource, DropEveryThirdData) {
    using namespace dataflow_use_case_test;
    auto time_delta = std::chrono::milliseconds(10);
    std::vector<std::uint32_t> pass_pattern{2};
    auto time_stddev = std::chrono::milliseconds(0);
    auto num_events = 10;
    auto simulate_sensors = false;
    auto sin_offset = 0;
    auto sin_per_second = 0.1;
    auto start_time = now();

    SourceConfiguration config(simulate_sensors, start_time, time_delta, time_stddev, pass_pattern, num_events, sin_offset, sin_per_second );
    ExpectedSource source(config);

    EXPECT_EQ(source.all_timestamps.size(), num_events);
    EXPECT_EQ(source.timestamps.size(), 7);

    auto ts = start_time;
    for (std::size_t i = 0; i < num_events; ++i) {
        bool valid = (i+1) % 3;
        EXPECT_EQ(source.ValidTimestamp(ts), valid);
        if(valid)
            EXPECT_NO_THROW(source.GetPose(ts));
        ts += time_delta;
    }
}

TEST(ExpectedSource, DropEverySecondThenThirdData) {
    using namespace dataflow_use_case_test;
    auto time_delta = std::chrono::milliseconds(10);
    std::vector<std::uint32_t> pass_pattern{1, 2};
    auto time_stddev = std::chrono::milliseconds(0);
    auto num_events = 10;
    auto simulate_sensors = false;
    auto sin_offset = 0;
    auto sin_per_second = 0.1;
    auto start_time = now();

    SourceConfiguration config(simulate_sensors, start_time, time_delta, time_stddev, pass_pattern, num_events, sin_offset, sin_per_second );
    ExpectedSource source(config);

    EXPECT_EQ(source.all_timestamps.size(), num_events);
    std::size_t expected_size = 6;
    EXPECT_EQ(source.timestamps.size(), expected_size);

    std::vector<bool> valid_pattern{true, false, true, true, false, true, false, true, true, false };
    auto ts = start_time;

    for (std::size_t i = 0; i < num_events; ++i) {
        EXPECT_EQ(source.ValidTimestamp(ts), valid_pattern[i]);
        if(valid_pattern[i])
            EXPECT_NO_THROW(source.GetPose(ts));
        ts += time_delta;
    }
}