#ifndef TRAACTMULTI_EXPECTEDSOURCE_H
#define TRAACTMULTI_EXPECTEDSOURCE_H

#include "TestUtils.h"
#include <random>

namespace dataflow_use_case_test {

    struct DATAFLOW_USE_CASE_TEST_EXPORT ExpectedSource {
        ExpectedSource() = default;
        explicit ExpectedSource(SourceConfiguration config);

        bool ValidTimestamp(TimestampType clean_ts) const;
        Eigen::Affine3d GetPose(TimestampType clean_ts) const;
        std::optional<TimestampType> GetTimestamp(TimestampType clean_ts) const;

        SourceConfiguration configuration;

        std::vector<TimestampType> timestamps;
        std::vector<TimestampType> all_timestamps;
        TimeDurationType max_offset_for_valid;
    };
}



#endif //TRAACTMULTI_EXPECTEDSOURCE_H
