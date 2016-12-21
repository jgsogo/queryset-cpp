
import os
from conan.packager import ConanMultiPackager
from conanfile import QuerysetCPP


username = os.getenv("CONAN_USERNAME", "jgsogo")
reference = os.getenv("CONAN_REFERENCE", "{}/{}".format(QuerysetCPP.name, QuerysetCPP.version))


if __name__ == "__main__":
    builder = ConanMultiPackager(username=username,
                                 reference=reference,
                                 visual_versions=["14",],
                                 gcc_versions = ["5.2", "5.3", "5.4", "6.2",],
                                 apple_clang_versions = ["6.1", "7.0", "7.3", "8.0",],
                                 args = "--scope queryset-cpp:BUILD_TEST=1 --build=missing"
                                )
    builder.add_common_builds(pure_c=False)
    print("{} builds ahead!".format(len(builder.builds)))
    builder.run()
