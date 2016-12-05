from conan.packager import ConanMultiPackager
import os


username = os.getenv("CONAN_USERNAME", "jgsogo")


if __name__ == "__main__":
    builder = ConanMultiPackager(username=username)
    builder.add_common_builds()
    builder.run()
