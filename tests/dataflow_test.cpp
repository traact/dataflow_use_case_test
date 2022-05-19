#include <gtest/gtest.h>

#include <dataflow_use_case_test/use_case/UseCase_Generic.h>
#include <dataflow_use_case_test/use_case/UseCase_In0_Out0.h>
#include <dataflow_use_case_test/use_case/UseCase_In0In1_Out0.h>
#include <spdlog/spdlog.h>
#include <dataflow_use_case_test/DataflowTest.h>
#include "TestDataflowNetwork.h"
TEST(DataflowTest, UseCase_In0_Out0_PerfectData_NoDelay) {
    using namespace dataflow_use_case_test;


    std::vector<ExpectedSource> sources;
    BaseProblemList base_problems;

    auto time_delta = std::chrono::milliseconds(10);
    std::vector<std::uint32_t> pass_pattern;
    auto time_stddev = std::chrono::milliseconds(0);
    auto num_events = 1000;
    auto simulate_sensors = false;
    auto sin_offset = 0;
    auto sin_per_second = 0.1;
    TimestampType start_time(std::chrono::nanoseconds (3000000000000000ll));

    SourceConfiguration config(simulate_sensors, start_time, time_delta, time_stddev, pass_pattern, num_events, sin_offset, sin_per_second );
    sources.emplace_back(ExpectedSource(config));

    auto use_case = std::make_shared<UseCase_Generic>(std::make_unique<UseCase_In0_Out0>());

    use_case->Init(sources, base_problems);

    auto network = std::make_shared<TestDataflowNetwork>();

    DataflowTest test(network, use_case);

    EXPECT_TRUE(test.Test());
}

TEST(DataflowTest, UseCase_In0_Out0_PerfectData_SimulateSensors) {
    using namespace dataflow_use_case_test;

    std::vector<ExpectedSource> sources;
    BaseProblemList base_problems;

    auto time_delta = std::chrono::milliseconds(10);
    std::vector<std::uint32_t> pass_pattern;
    auto time_stddev = std::chrono::milliseconds(0);
    
    auto num_events = 10;
    auto simulate_sensors = true;
    auto sin_offset = 0;
    auto sin_per_second = 0.1;

    TimestampType start_time(std::chrono::nanoseconds (3000000000000000ll));

    SourceConfiguration config(simulate_sensors, start_time, time_delta, time_stddev, pass_pattern, num_events, sin_offset, sin_per_second );
    sources.emplace_back(ExpectedSource(config));

    auto use_case = std::make_shared<UseCase_Generic>(std::make_unique<UseCase_In0_Out0>());
    use_case->Init(sources, base_problems);

    auto network = std::make_shared<TestDataflowNetwork>();

    DataflowTest test(network, use_case);

    EXPECT_TRUE(test.Test());
}

TEST(DataflowTest, UseCase_In0_Out0_DropHalfData_NoDelay) {
    using namespace dataflow_use_case_test;


    std::vector<ExpectedSource> sources;
    BaseProblemList base_problems;

    auto time_delta = std::chrono::milliseconds(10);
    std::vector<std::uint32_t> pass_pattern{1};
    auto time_stddev = std::chrono::milliseconds(0);
    
    auto num_events = 1000;
    auto simulate_sensors = false;
    auto sin_offset = 0;
    auto sin_per_second = 0.1;
    TimestampType start_time(std::chrono::nanoseconds (3000000000000000ll));

    SourceConfiguration config(simulate_sensors, start_time, time_delta, time_stddev, pass_pattern, num_events, sin_offset, sin_per_second );
    sources.emplace_back(ExpectedSource(config));

    auto use_case = std::make_shared<UseCase_Generic>(std::make_unique<UseCase_In0_Out0>());

    use_case->Init(sources, base_problems);

    auto network = std::make_shared<TestDataflowNetwork>();

    DataflowTest test(network, use_case);

    EXPECT_TRUE(test.Test());
}

TEST(DataflowTest, UseCase_In0In1_Out0_PerfectData_NoDelay) {
    using namespace dataflow_use_case_test;


    std::vector<ExpectedSource> sources;
    BaseProblemList base_problems;

    {
        auto time_delta = std::chrono::milliseconds(10);
        std::vector<std::uint32_t> pass_pattern;
        auto time_stddev = std::chrono::milliseconds(0);
        
        auto num_events = 1000;
        auto simulate_sensors = false;
        auto sin_offset = 0;
        auto sin_per_second = 0.1;
        TimestampType start_time(std::chrono::nanoseconds (3000000000000000ll));

        SourceConfiguration config(simulate_sensors, start_time, time_delta, time_stddev, pass_pattern, num_events, sin_offset, sin_per_second );
        sources.emplace_back(ExpectedSource(config));
    }
    {
        auto time_delta = std::chrono::milliseconds(10);
        std::vector<std::uint32_t> pass_pattern;
        auto time_stddev = std::chrono::milliseconds(0);
        
        auto num_events = 1000;
        auto simulate_sensors = false;
        auto sin_offset = 0;
        auto sin_per_second = 0.1;
        TimestampType start_time(std::chrono::nanoseconds (3000000000000000ll));

        SourceConfiguration config(simulate_sensors, start_time, time_delta, time_stddev, pass_pattern, num_events, sin_offset, sin_per_second );
        sources.emplace_back(ExpectedSource(config));
    }
    {
        auto base_problem = std::make_shared<BaseProblem>();
        base_problem->DoBusyWork = false;
        base_problems.push_back(base_problem);
    }

    auto use_case = std::make_shared<UseCase_Generic>(std::make_unique<UseCase_In0In1_Out0>());

    use_case->Init(sources, base_problems);

    auto network = std::make_shared<TestDataflowNetwork>();

    DataflowTest test(network, use_case);

    EXPECT_TRUE(test.Test());
}

TEST(DataflowTest, UseCase_In0In1_Out0_DropHalfOfSecondData_NoDelay) {
    using namespace dataflow_use_case_test;


    std::vector<ExpectedSource> sources;
    BaseProblemList base_problems;

    {
        auto time_delta = std::chrono::milliseconds(10);
        std::vector<std::uint32_t> pass_pattern;
        auto time_stddev = std::chrono::milliseconds(0);
        
        auto num_events = 1000;
        auto simulate_sensors = false;
        auto sin_offset = 0;
        auto sin_per_second = 0.1;
        TimestampType start_time(std::chrono::nanoseconds (3000000000000000ll));

        SourceConfiguration config(simulate_sensors, start_time, time_delta, time_stddev, pass_pattern, num_events, sin_offset, sin_per_second );
        sources.emplace_back(ExpectedSource(config));
    }
    {
        auto time_delta = std::chrono::milliseconds(10);
        std::vector<std::uint32_t> pass_pattern{1};
        auto time_stddev = std::chrono::milliseconds(0);
        
        auto num_events = 1000;
        auto simulate_sensors = false;
        auto sin_offset = 0;
        auto sin_per_second = 0.1;
        TimestampType start_time(std::chrono::nanoseconds (3000000000000000ll));

        SourceConfiguration config(simulate_sensors, start_time, time_delta, time_stddev, pass_pattern, num_events, sin_offset, sin_per_second );
        sources.emplace_back(ExpectedSource(config));
    }
    {
        auto base_problem = std::make_shared<BaseProblem>();
        base_problem->DoBusyWork = false;
        base_problems.push_back(base_problem);
    }

    auto use_case = std::make_shared<UseCase_Generic>(std::make_unique<UseCase_In0In1_Out0>());

    use_case->Init(sources, base_problems);

    auto network = std::make_shared<TestDataflowNetwork>();

    DataflowTest test(network, use_case);

    EXPECT_TRUE(test.Test());
}