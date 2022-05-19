# /usr/bin/python3
import os
from conans import ConanFile, CMake, tools


class DataflowUseCaseTest(ConanFile):
    name = "dataflow_use_case_test"
    version = "0.0.1"    

    description = "Test framework for a realtime generic dataflow network"
    url = ""
    license = "MIT"
    author = "Frieder Pankratz"

    short_paths = True

    generators = "cmake"
    settings = "os", "compiler", "build_type", "arch"
    compiler = "cppstd"
    options = {
        "shared": [True, False],
    }

    default_options = {
        "shared": True,
    }

    exports_sources = "CMakeLists.txt", "src/*", "tests/*"

    def requirements(self):
        self.requires("gtest/[>=1.10.0]")
        self.requires("eigen/[>=3.3.9]@camposs/stable")
        self.requires("spdlog/1.8.2")

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.verbose = True

        def add_cmake_option(option, value):
            var_name = "{}".format(option).upper()
            value_str = "{}".format(value)
            var_value = "ON" if value_str == 'True' else "OFF" if value_str == 'False' else value_str
            cmake.definitions[var_name] = var_value

        for option, value in self.options.items():
            add_cmake_option(option, value)

        cmake.configure()
        return cmake

    def configure(self):
        self.options['gtest'].shared = self.options.shared
        self.options['spdlog'].shared = self.options.shared

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = [self.name]
