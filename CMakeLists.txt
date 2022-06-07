cmake_minimum_required(VERSION 3.16)

include(GenerateExportHeader)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

# conan is required hard, library name, version and description are configured from conan
if (EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
else ()
    message(FATAL_ERROR "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif ()

#https://stackoverflow.com/questions/48212771/cmake-install-header-files-and-maintain-directory-heirarchy
macro(install_with_directory)
    set(optionsArgs "")
    set(oneValueArgs "DESTINATION")
    set(multiValueArgs "FILES")
    cmake_parse_arguments(CAS "${optionsArgs}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    foreach (FILE ${CAS_FILES})
        get_filename_component(DIR ${FILE} DIRECTORY)
        INSTALL(FILES ${FILE} DESTINATION ${CAS_DESTINATION}/${DIR})
    endforeach ()
endmacro(install_with_directory)

project(${CONAN_PACKAGE_NAME}_PROJECT VERSION ${CONAN_PACKAGE_VERSION})

conan_basic_setup(TARGETS)

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

generate_export_header(${CONAN_PACKAGE_NAME} EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/${CONAN_PACKAGE_NAME}/${CONAN_PACKAGE_NAME}_export.h)
set(PUBLIC_HEADER_FILES
        ${CMAKE_BINARY_DIR}/${CONAN_PACKAGE_NAME}/${CONAN_PACKAGE_NAME}_export.h
        src/dataflow_use_case_test/DataflowTest.h
        src/dataflow_use_case_test/DataflowNetwork.h
        src/dataflow_use_case_test/ExpectedSource.h
        src/dataflow_use_case_test/TestUtils.h
        src/dataflow_use_case_test/UseCase.h
        src/dataflow_use_case_test/UseCaseFactory.h
        src/dataflow_use_case_test/source_strategy/SourceStrategy.h
        src/dataflow_use_case_test/source_strategy/AllEvents_PerfectTimestamps.h
        src/dataflow_use_case_test/source_strategy/MissingEvents_NoisyTimestamps.h
        src/dataflow_use_case_test/use_case/UseCase_Behaviour.h
        src/dataflow_use_case_test/use_case/UseCase_Generic.h
        src/dataflow_use_case_test/use_case/UseCase_In0_Out0.h
        src/dataflow_use_case_test/use_case/UseCase_In0In1_Out0.cpp
        src/dataflow_use_case_test/use_case/UseCase_In0In1_In2_Out0.h
        src/dataflow_use_case_test/use_case/UseCase_In0In1_In2In3_Out0.h
        src/dataflow_use_case_test/use_case/UseCase_In0In1_In2In3_Out0_Out1.h
        src/dataflow_use_case_test/use_case/UseCase_In0In1TD0_In2In3TTD1_Out0TD0_Out1TD0_Buffer.h
        )
#set_target_properties(${CONAN_PACKAGE_NAME}  PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADER_FILES}")
#install_with_directory(   DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${CONAN_PACKAGE_NAME}/   FILES ${PUBLIC_HEADER_FILES})
install(DIRECTORY "./src/dataflow_use_case_test" DESTINATION "${CMAKE_INSTALL_PREFIX}/include" FILES_MATCHING
        PATTERN "*.h"
        PATTERN "internal" EXCLUDE
        )

target_include_directories(${CONAN_PACKAGE_NAME} PUBLIC
        ${CMAKE_BINARY_DIR}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

install(TARGETS ${CONAN_PACKAGE_NAME}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # This is for Windows
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )

#install(FILES ${PROJECT_BINARY_DIR}/dataflow_use_case_test_export.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

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