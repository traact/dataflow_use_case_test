#include "UseCase_Generic.h"

namespace dataflow_use_case_test {

    UseCase_Generic::UseCase_Generic(std::unique_ptr<UseCase_Behaviour>&& behaviour) : UseCase(), behaviour_(std::move(behaviour)) {

    }

    UseCaseProblem UseCase_Generic::GetProblem() {
        return behaviour_->GetProblem();
    }

    size_t UseCase_Generic::GetTimeDomainCount() {
        return behaviour_->GetTimeDomainCount();
    }

    void UseCase_Generic::Init(std::vector<ExpectedSource> expectedSources, BaseProblemList baseProblems) {

        behaviour_->CheckInputs(expectedSources, baseProblems);

        expected_sources_ = expectedSources;
        base_problems_ = baseProblems;

        behaviour_->AssignExpectedSources(source_idx_to_time_domain_and_port);

        GatherAllTimestamps();



        expected_results_.resize(GetTimeDomainCount());

        for(auto & ts_td : clean_timestamps_){
            auto current_ts = ts_td.first;

            std::map<bool, std::map<std::size_t , TimestampType >> event_ts;

            for(std::size_t source_idx = 0; source_idx < expected_sources_.size();++source_idx ){
                if(source_idx_to_time_domain_and_port[source_idx].first != ts_td.second)
                    continue;

                const auto& source = expected_sources_[source_idx];
                auto data_ts = source.GetTimestamp(current_ts);
                if(data_ts.has_value()){
                    event_ts[source.configuration.simulate_sensors][source_idx] = data_ts.value();
                }
            }

            CreateDataEvents(event_ts, ts_td);

            behaviour_->CreateResult(ts_td, expected_sources_, expected_results_);


        }
    }

    void
    UseCase_Generic::CreateDataEvents(const std::map<bool, std::map<std::size_t, TimestampType>> &source_data,
                                               const TS_TD &ts_td) {

        auto has_no_delay_data = source_data.find(false);
        if(has_no_delay_data != source_data.cend()){
            CreateData(ts_td, TimeDurationType(0), has_no_delay_data->second);
        }

        auto has_delay_data = source_data.find(true);
        if(has_delay_data != source_data.cend()){
            CreateData(ts_td, GetDelay(ts_td), has_delay_data->second);
        }
    }

    void UseCase_Generic::CreateData(const UseCase::TS_TD &ts_td, const TimeDurationType &delay,
                                              const std::map<size_t, TimestampType> &data) {
        DataSourceEvent event;
        event.TimeDomain = ts_td.second;
        event.Delay = delay;
        for(const auto& source_idx_ts : data){
            auto& source = expected_sources_[source_idx_ts.first];
            auto port = source_idx_to_time_domain_and_port[source_idx_ts.first].second;
            event.Data.emplace_back(std::make_tuple(port, source.GetTimestamp(ts_td.first).value(), source.GetPose(ts_td.first)));
        }
        assert(!event.Data.empty());
        source_data_.emplace_back(event);
    }

    TimeDurationType UseCase_Generic::GetMaxTimestampOffset(int time_domain, int port) {
        // should be the time_delta/2 of the source that limits the output frequency on the port
        // in all current problems source 0 drives all outputs
        return expected_sources_[0].configuration.time_delta/2;
    }


}