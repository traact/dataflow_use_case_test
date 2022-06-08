cmake_minimum_required(VERSION 3.16)

if (EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
else ()
    message(FATAL_ERROR "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif ()

list(APPEND CMAKE_MODULE_PATH ${CONAN_TRAACT_RUN_ENV_ROOT}/cmake)

project(${CONAN_PACKAGE_NAME}_PROJECT VERSION ${CONAN_PACKAGE_VERSION})

include(traact_default_library_setup)

add_library(${CONAN_PACKAGE_NAME} SHARED
        src/dataflow_use_case_test/DataflowTest.cpp src/dataflow_use_case_test/DataflowTest.h
        src/dataflow_use_case_test/UseCase.cpp src/dataflow_use_case_test/UseCase.h
        src/dataflow_use_case_test/DataflowNetwork.cpp src/dataflow_use_case_test/DataflowNetwork.h
        src/dataflow_use_case_test/TestUtils.h src/dataflow_use_case_test/TestUtils.cpp
        src/dataflow_use_case_test/internal/DataSource.cpp src/dataflow_use_case_test/internal/DataSource.h
        src/dataflow_use_case_test/internal/DataSink.cpp src/dataflow_use_case_test/internal/DataSink.h
        src/dataflow_use_case_test/ExpectedSource.cpp src/dataflow_use_case_test/ExpectedSource.h
        src/dataflow_use_case_test/UseCaseFactory.cpp src/dataflow_use_case_test/UseCaseFactory.h
        src/dataflow_use_case_test/use_case/UseCase_Generic.cpp
        src/dataflow_use_case_test/use_case/UseCase_Generic.h
        src/dataflow_use_case_test/use_case/UseCase_Behaviour.h
        src/dataflow_use_case_test/use_case/UseCase_In0_Out0.cpp
        src/dataflow_use_case_test/use_case/UseCase_In0_Out0.h
        src/dataflow_use_case_test/use_case/UseCase_In0In1_Out0.cpp
        src/dataflow_use_case_test/use_case/UseCase_In0In1_Out0.h
        src/dataflow_use_case_test/use_case/UseCase_In0In1_In2_Out0.cpp
        src/dataflow_use_case_test/use_case/UseCase_In0In1_In2_Out0.h
        src/dataflow_use_case_test/use_case/UseCase_In0In1_In2In3_Out0.cpp
        src/dataflow_use_case_test/use_case/UseCase_In0In1_In2In3_Out0.h
        src/dataflow_use_case_test/use_case/UseCase_In0In1_In2In3_Out0_Out1.cpp
        src/dataflow_use_case_test/use_case/UseCase_In0In1_In2In3_Out0_Out1.h
        src/dataflow_use_case_test/use_case/UseCase_In0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer.cpp
        src/dataflow_use_case_test/use_case/UseCase_In0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer.h
        src/dataflow_use_case_test/source_strategy/SourceStrategy.h
        src/dataflow_use_case_test/source_strategy/AllEvents_PerfectTimestamps.cpp
        src/dataflow_use_case_test/source_strategy/AllEvents_PerfectTimestamps.h
        src/dataflow_use_case_test/source_strategy/MissingEvents_NoisyTimestamps.cpp
        src/dataflow_use_case_test/source_strategy/MissingEvents_NoisyTimestamps.h)

conan_target_link_libraries(${CONAN_PACKAGE_NAME})

install(DIRECTORY "./src/dataflow_use_case_test" DESTINATION "${CMAKE_INSTALL_PREFIX}/include" FILES_MATCHING
        PATTERN "*.h"
        PATTERN "internal" EXCLUDE
        )

include(traact_default_library_final)

# tests --------------------------------------------------------

add_executable(dataflow_test_test
        tests/test_main.cpp
        tests/expected_source_test.cpp
        tests/dataflow_test.cpp
        tests/TestDataflowNetwork.cpp
        tests/TestDataflowNetwork.h
        tests/default_dataflow_tests.cpp tests/benchmark_test.cpp)
target_include_directories(dataflow_test_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
        ${CMAKE_CURRENT_BINARY_DIR}
        )

target_link_libraries(dataflow_test_test PRIVATE ${CONAN_PACKAGE_NAME})