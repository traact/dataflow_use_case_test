# /usr/bin/python3
import os
from conans import ConanFile, CMake, tools


class TraactPackage(ConanFile):
    python_requires = "traact_run_env/1.0.0@traact/latest"
    python_requires_extend = "traact_run_env.TraactPackageCmake"

    name = "dataflow_use_case_test"
    description = "Test framework for a generic realtime dataflow network"
    url = "https://github.com/traact/traact_dataflow_test.git"
    license = "MIT"
    author = "Frieder Pankratz"

    settings = "os", "compiler", "build_type", "arch"
    compiler = "cppstd"

    exports_sources = "CMakeLists.txt", "src/*", "tests/*"

    def requirements(self):
        self.traact_requires("traact_run_env", "latest")
        self.requires("gtest/cci.20210126")
        self.requires("eigen/[>=3.4.0]")
        self.requires("spdlog/1.11.0")

    def configure(self):
        self.options['gtest'].shared = self.options.shared
        self.options['spdlog'].shared = self.options.shared
