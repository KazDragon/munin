from conans import ConanFile, CMake


class MuninConan(ConanFile):
    name = "munin"
    license = "MIT"
    author = "KazDragon"
    url = "https://github.com/KazDragon/munin"
    description = "A text-based gui component library build on Terminal++"
    topics = ("ansi-escape-codes", "text-ui")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "coverage": [True, False], "sanitize" : ["off", "address"]}
    default_options = {"shared": False, "coverage": False, "sanitize": "off"}
    exports = "*.hpp", "*.in", "*.cpp", "CMakeLists.txt", "*.md", "LICENSE", "*.cmake"
    requires = ("terminalpp/[>=2.3.0]",
                "nlohmann_json/[>=3.3.0]",
                "boost/[>=1.69]")
    build_requires = ("gtest/[>=1.8.1]")
    generators = "cmake"

    def imports(self):
        # If Munin is built as shared, then running the tests will
        # rely on the shared object for terminalpp being available
        # in the same directory.
        self.copy("*.so*", dst="", src="", keep_path=False, root_package="terminalpp")

    def configure(self):
        self.options["terminalpp"].shared = self.options.shared

    def build(self):
        cmake = CMake(self)
        cmake.definitions["BUILD_SHARED_LIBS"] = self.options.shared
        cmake.definitions["MUNIN_COVERAGE"] = self.options.coverage
        cmake.definitions["MUNIN_SANITIZE"] = self.options.sanitize
        cmake.definitions["MUNIN_VERSION"] = self.version
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("*.hpp", dst="include", src="include")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.so.*", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        if self.settings.build_type == "Debug":
            self.cpp_info.libs = ["munind"]
        else:
            self.cpp_info.libs = ["munin"]

