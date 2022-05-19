#include "TestDataflowNetwork.h"

using namespace dataflow_use_case_test;

void
TestDataflowNetwork::Init(dataflow_use_case_test::UseCaseProblem problem, const dataflow_use_case_test::BaseProblemList &problem_list,
                          const dataflow_use_case_test::ResultCallback &result_callback,
                          const dataflow_use_case_test::InvalidCallback &invalid_callback,
                          dataflow_use_case_test::TimeDurationType max_ts_offset) {
    
    
    
    problem_ = problem;
    problem_list_ = problem_list;
    result_callback_ = result_callback;
    invalid_callback_ = invalid_callback;
    max_ts_offset_ = max_ts_offset;

    switch (problem_) {
        case UseCaseProblem::In0_Out0:
        case UseCaseProblem::In0In1_Out0:
        case UseCaseProblem::In0In1_In2_Out0:
        case UseCaseProblem::In0In1_In2In3_Out0:
        case UseCaseProblem::In0In1_In2In3_Out0_Out1:
            buffer_.resize(1);
            break;
        case UseCaseProblem::In0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer:
            buffer_.resize(2);
            break;
        
    }

}

void TestDataflowNetwork::Start() {

}

void TestDataflowNetwork::Stop() {

}
std::optional<TestDataType> TestDataflowNetwork::get_data(int td, int port)  {

    auto result = buffer_[td].find(port);

    if(result == buffer_[td].end())
        return {};
    else
        return {std::get<1>(result->second)};
};
std::optional<dataflow_use_case_test::TimestampType> TestDataflowNetwork::get_ts(int td, int port)  {

    auto result = buffer_[td].find(port);

    if(result == buffer_[td].end())
        return {};
    else
        return {std::get<0>(result->second)};
};

void TestDataflowNetwork::NewData(int time_domain, const PortTsDataList &data) {
    for(const auto& port_data : data ){
        buffer_[time_domain][std::get<0>(port_data)] = std::make_pair(std::get<1>(port_data),std::get<2>(port_data));
    }


    switch (problem_) {
        case UseCaseProblem::In0_Out0:{
            ProcessIn0_Out0(time_domain);
            break;
        }
        case UseCaseProblem::In0In1_Out0: {
            ProcessIn0In1_Out0(time_domain);
            break;
        }
        case UseCaseProblem::In0In1_In2_Out0:{
            ProcessIn0In1_In2_Out0(time_domain);
            break;
            }
        case UseCaseProblem::In0In1_In2In3_Out0:{
            ProcessIn0In1_In2In3_Out0(time_domain);
            break;
        }
        case UseCaseProblem::In0In1_In2In3_Out0_Out1:{
            ProcessIn0In1_In2In3_Out0_Out1(time_domain);
            break;
        }
        case UseCaseProblem::In0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer:
        {
            ProcessIn0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer(time_domain);
            break;
        }


    }
}

void TestDataflowNetwork::ProcessIn0_Out0(int time_domain) {

    result_callback_(0, std::make_tuple(0,std::get<0>(buffer_[0][0]), std::get<1>(buffer_[0][0])));
    buffer_[0].clear();
}

void TestDataflowNetwork::ProcessIn0In1_Out0(int time_domain) {
    auto p0 = get_data(0,0);
    auto p1 = get_data(0,1);
    auto ts = get_ts(0,0);
    if(!ts.has_value())
        ts = get_ts(0,1);

    if(p0.has_value() && p1.has_value()) {
        auto result = problem_list_[0]->Execute(ts.value(), p0.value() , p1.value());
        result_callback_(0, std::make_tuple(0,ts.value(), result));
    } else {

        invalid_callback_(0,0, ts.value());
    }
    buffer_[0].clear();
}

void TestDataflowNetwork::ProcessIn0In1_In2_Out0(int time_domain) {
    auto p0 = get_data(0,0);
    auto p1 = get_data(0,1);
    auto p2 = get_data(0,2);
    auto ts = get_ts(0,0);
    if(!ts.has_value())
        ts = get_ts(0,1);
    if(!ts.has_value())
        ts = get_ts(0,2);

    if(p0.has_value() && p1.has_value() && p2.has_value()) {
        auto result = problem_list_[0]->Execute(ts.value(), p0.value(), p1.value());
        result = problem_list_[1]->Execute(ts.value(), result, p2.value());
        result_callback_(0, std::make_tuple(0,ts.value(), result));
    } else {

        invalid_callback_(0,0, ts.value());
    }
    buffer_[0].clear();
}

void TestDataflowNetwork::ProcessIn0In1_In2In3_Out0(int time_domain) {
    auto p0 = get_data(0,0);
    auto p1 = get_data(0,1);
    auto p2 = get_data(0,2);
    auto p3 = get_data(0,3);
    auto ts = get_ts(0,0);
    if(!ts.has_value())
        ts = get_ts(0,1);
    if(!ts.has_value())
        ts = get_ts(0,2);
    if(!ts.has_value())
        ts = get_ts(0,3);
    if(p0.has_value() && p1.has_value() && p2.has_value()&& p3.has_value()) {
        auto result1 = problem_list_[0]->Execute(ts.value(), p0.value(), p1.value());
        auto result2 = problem_list_[1]->Execute(ts.value(), p2.value(), p3.value());
        auto result = problem_list_[2]->Execute(ts.value(), result1, result2);
        result_callback_(0, std::make_tuple(0,ts.value(), result));
    } else {

        invalid_callback_(0,0, ts.value());
    }
    buffer_[0].clear();
}

void TestDataflowNetwork::ProcessIn0In1_In2In3_Out0_Out1(int time_domain) {
    auto p0 = get_data(0,0);
    auto p1 = get_data(0,1);
    auto p2 = get_data(0,2);
    auto p3 = get_data(0,3);

    auto ts = get_ts(0,0);
    if(!ts.has_value())
        ts = get_ts(0,1);
    if(!ts.has_value())
        ts = get_ts(0,2);
    if(!ts.has_value())
        ts = get_ts(0,3);

    if(p0.has_value() && p1.has_value()) {
        auto result1 = problem_list_[0]->Execute(ts.value(), p0.value(), p1.value());
        result_callback_(0, std::make_tuple(0,ts.value(), result1));

        if( p2.has_value()&& p3.has_value()) {
            auto result2 = problem_list_[1]->Execute(ts.value(), p2.value(), p3.value());
            auto result = problem_list_[2]->Execute(ts.value(), result1, result2);
            result_callback_(0, std::make_tuple(1,ts.value(), result));
        } else {
            invalid_callback_(0,1, ts.value());
        }
    } else {
        invalid_callback_(0,0, ts.value());
    }
    buffer_[0].clear();
}

void TestDataflowNetwork::ProcessIn0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer(int time_domain) {
    if(time_domain == 1) {
        auto ts2 = get_ts(1,0);
        auto ts3 = get_ts(1,1);
        if(ts2.has_value() && ts3.has_value() && TimestampsMatch(ts2.value(),ts3.value(), max_ts_offset_)){
            auto p2 = get_data(1,0);
            auto p3 = get_data(1,1);
            td1_buffer_ = problem_list_[1]->Execute(ts2.value(), p2.value(), p3.value());
        }
        return;
    }


    auto ts0 = get_ts(0,0);
    auto ts1 = get_ts(0,1);

    if(!ts0.has_value())
        return;

    TimestampType ts = ts0.value();

    if(ts1.has_value() && TimestampsMatch(ts0.value(),ts1.value(), max_ts_offset_)){

        auto p0 = get_data(0,0);
        auto p1 = get_data(0,1);
        auto result1 = problem_list_[0]->Execute(ts, p0.value(), p1.value());
        result_callback_(0, std::make_tuple(0,ts, result1));

        if( td1_buffer_.has_value() ) {

            auto result = problem_list_[2]->Execute(ts, result1, td1_buffer_.value());
            result_callback_(0, std::make_tuple(1,ts, result));
        } else {
            invalid_callback_(0,1, ts);
        }
    } else {
        invalid_callback_(0,0, ts);
    }
    buffer_[0].clear();
}


