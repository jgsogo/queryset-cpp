
import os
from conan.packager import ConanMultiPackager
from conanfile import QuerysetCPP


username = os.getenv("CONAN_USERNAME", "jgsogo")
reference = os.getenv("CONAN_REFERENCE", "{}/{}".format(QuerysetCPP.name, QuerysetCPP.version))


if __name__ == "__main__":
    builder = ConanMultiPackager(username=username,
                                 reference=reference,
                                 visual_versions=["14",],
                                 gcc_versions = ["5.4", "6.3",],
                                 apple_clang_versions = ["7.3", "8.1",],
                                 args = "--scope queryset-cpp:BUILD_TEST=1 --build=missing"
                                )
    builder.add_common_builds(shared_option_name="queryset-cpp:shared")
    filtered_builds = []
    for settings, options, env_vars, build_requires in builder.builds:
        if settings["build_type"] == "Release":
            filtered_builds.append([settings, options, env_vars, build_requires])
    builder.builds = filtered_builds
    print("{} builds ahead!".format(len(builder.builds)))
    builder.run()

