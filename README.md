# dataflow_use_case_test

A test framework to test a real-time dataflow network designed to handle multiple synchronous and asynchronous inputs.

## How to use

An example is in the “tests” directory with the implementation of the TestDataflowNetwork and the gtests
benchmark_test.cpp and default_dataflow_tests.cpp

Create an extension the DataflowNetwork class to manage your dataflow network.

 ```
 class DataflowNetwork {
 public:
 virtual ~DataflowNetwork() = default;

 virtual void init(dataflow_use_case_test::UseCaseProblem problem, const dataflow_use_case_test::BaseProblemList &problem_list,
 const dataflow_use_case_test::ResultCallback &result_callback,
 const dataflow_use_case_test::InvalidCallback &invalid_callback,
 dataflow_use_case_test::TimeDuration max_ts_offset) =0;
 virtual void start() = 0;
 virtual void stop() = 0;
 virtual void NewData(int time_domain, const PortTsDataList& data) = 0;
 };

 ```

The framework will call the “init” function before the test starts.

Depending on the enum parameter “ problem” your dataflow network has to solve a different task:

*All input sources are synchronous, time domain 0, result time domain 0*

**In0_Out0**: pass input 0 directly to output 0

**In0In1_Out0**: when the inputs 0 and 1 have synchronous data then solve0(0,1) and pass the result to output 0

**In0In1_In2_Out0**, when the inputs 0,1 and 2 have synchronous data then r0 = solve0(0,1) result = solve(r0,2) and pass
the result to output 0

**In0In1_In2In3_Out0**: when the inputs 0,1,2 and 4 have synchronous data then r0 = solve0(0,1) r0 = solve1(2,3) result
= solve2(r0,r1) and pass the result to output 0

**In0In1_In2In3_Out0_Out1**: when the inputs 0 and 1 have synchronous data then r0 = solve0(0,1) and pass to r0 output
0; when the inputs 2,3 and 0,1(or r0) have synchronous data then r1 = solve1(2,3) final = solve2(r0,r1) and pass to
output 1

*Inputs 0 and 1 are synchronous, time domain 0. Inputs 2 and 3 are synchronous, time domain 1. result time domain 0*

**In0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer**: when the inputs 2 and 3 have synchronous data then r1 = solve1(2,3) and
buffer it;when the inputs 0 and 1 have synchronous data then r0 = solve1(0,1) and pass r0 to output 0, when a buffered
r1 is available then final = solve2(r0,r1) and pass to output 1

To “solve” a problem the init call passes a list of “BaseProblem”. The index of “solveX” corresponds to the index in the
list. Call the “Execute” function with the timestamp of the dataset and its values.

 ```
struct BaseProblem {
 TestDataType Execute(Timestamp ts, const TestDataType &input1, const TestDataType &input2);
 };
 ```

To pass the result back to the test framework use the “ResultCallback”.
First parameter is the result time domain, currently all use cases use 0.
“PortTsData” is a tuple<int,Timestamp,TestDatatype>  representing the port, timestamp and result.

In case your dataflow network can notice when a time step has passed and no valid result can be computed (e.g. missing
input data) then you can report it using the “InvalidCallback” with the parameters timedomain, port and timestamp

 ```
using InvalidCallback = std::function<void(int, int, Timestamp)>;
using ResultCallback = std::function<void(int, PortTsData)>;
 ```

Next the test framework will call the “start” method.
After that the source thread will start sending data to your framework using the “NewData” method. Every call is only
for one timedomain but can have multiple entries in “data” parameter for different input ports.

All left to do is to define your tests. A simple way is to copy the “default_dataflow_test.cpp” and replace the the
TestDataflowNetwork with your implementation.