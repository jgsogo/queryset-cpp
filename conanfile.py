
from conans import ConanFile, CMake

VERSION = "0.5"


class QuerysetCPP(ConanFile):
    name = "queryset-cpp"
    version = VERSION

    description = """C++ implementation of an ORM inspired by Python/Django"""
    generators = "cmake"
    settings = "os", "compiler", "build_type", "arch"
    exports = "conanfile.py", "CMakeLists.txt", "queryset/*", "tests/*"
    url = "https://github.com/jgsogo/queryset-cpp"

    def requirements(self):
        self.requires.add("Boost/1.60.0@lasote/stable")
        self.requires.add("spdlog/0.9.0@memsharded/stable")
        self.requires.add("sqlite3cc/master@jgsogo/stable")

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin") # From bin to bin
        self.copy("*.dylib*", dst="bin", src="lib") # From lib to bin
      
    def build(self):
        cmake = CMake(self.settings)
        flag_build_tests = "-DBUILD_TEST:BOOL=ON" if self.scope.BUILD_TEST else ""
        if flag_build_tests:
            self.run('cmake "%s" %s %s' % (self.conanfile_directory, cmake.command_line, flag_build_tests))
            self.run("cmake --build . %s" % cmake.build_config)
            self.run("ctest -C {}".format(self.settings.build_type))

    def package(self):
        self.copy("*.h", dst="include")
        # self.copy("*.lib", dst="lib", src="lib")
        # self.copy("*.a", dst="lib", src="lib")

    def package_info(self):
        #self.cpp_info.libs = ["queryset-cpp"]  # Do not generates .lib
        pass


