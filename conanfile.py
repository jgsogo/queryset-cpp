from conans import ConanFile, CMake

class QuerysetCPP(ConanFile):
    name = "queryset-cpp"
    version = "0.4"
    requires = "Boost/1.60.0@lasote/stable", "spdlog/0.9.0@memsharded/stable"
    generators = "cmake"
    settings = "os", "compiler", "build_type", "arch"
    exports = "conanfile.py", "CMakeLists.txt", "queryset/*" , "tests/*"
    url = "https://github.com/jgsogo/queryset-cpp"

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin") # From bin to bin
        self.copy("*.dylib*", dst="bin", src="lib") # From lib to bin
      
    def build(self):
        cmake = CMake(self.settings)
        self.run('cmake %s %s' % (self.conanfile_directory, cmake.command_line))
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("*.h", dst="include")
        self.copy("*.lib", dst="lib", src="lib")
        self.copy("*.a", dst="lib", src="lib")

    def package_info(self):
        #self.cpp_info.libs = ["queryset-cpp"]  # Do not generates .lib
        pass
