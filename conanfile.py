from conans import ConanFile, CMake, tools


class MuninConan(ConanFile):
    name = "munin"
    version = "0.3.6"
    license = "MIT"
    author = "KazDragon"
    url = "https://github.com/KazDragon/munin"
    description = "A text-based gui component library build on Terminal++"
    topics = ("ansi-escape-codes", "text-ui")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "coverage": [True, False], "sanitize" : ["off", "address"]}
    default_options = {"shared": False, "coverage": False, "sanitize": "off"}    
    exports = "*.hpp", "*.in", "*.cpp", "CMakeLists.txt", "*.md", "LICENSE"
    requires = ("terminalpp/[>=1.3.3]@kazdragon/conan-public",
                "jsonformoderncpp/[>=3.3.0]@vthiery/stable",
                "boost_any/[>=1.69]@bincrafters/stable",
                "boost_optional/[>=1.69]@bincrafters/stable",
                "boost_scope_exit/[>=1.69]@bincrafters/stable",
                "boost_signals2/[>=1.69]@bincrafters/stable")
    build_requires = ("gtest/[>=1.8.1]@bincrafters/stable")
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
        self.cpp_info.libs = ["munin"]

