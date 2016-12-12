
import os
from conan.packager import ConanMultiPackager


username = os.getenv("CONAN_USERNAME", "jgsogo")


if __name__ == "__main__":
    builder = ConanMultiPackager(username=username, 
                                 visual_versions=["14"],
                                 gcc_versions = ["5.2", "5.3", "5.4", "6.2"],
                                 apple_clang_versions = ["6.1", "7.0", "7.3", "8.0"],
                                 args = "--scope queryset-cpp:BUILD_TEST=1 --build=missing"
                                )
    builder.add_common_builds()
    print("{} builds ahead!".format(len(builder.builds)))
    builder.run()
