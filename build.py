
import os
from conan.packager import ConanMultiPackager


username = os.getenv("CONAN_USERNAME", "jgsogo")


if __name__ == "__main__":
    builder = ConanMultiPackager(username=username, 
                                 visual_versions=["14"],
                                 gcc_versions = ["5.2", "5.3", "5.4", "6.2"],
                                 apple_clang_versions = ["5.0", "5.1", "6.0", "6.1", "7.0", "7.3", "8.0"]
                                )
    builder.add_common_builds(pure_c=False)
    print("{} builds ahead!".format(len(builder.builds)))
    builder.run()
