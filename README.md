# queryset-cpp

C++ implementation of an ORM inspired by [Python/Django](https://www.djangoproject.com/). It allows to query
a database-like backend using compact and human readable code.

Supported backends:

 * filesystem (read)
 * sqlite3
 * postgresql

## Index

 * [Build status](#build-status)
 * [Dependencies](#dependencies)
 * [Basic usage](#basic-usage)
 * [License](#license)
 

## Build status

<table>
    <thead>
        <tr>
            <th></th>
            <th colspan="3">Windows</th>
            <th colspan="4">Unix</th>
            <th>Macos</th>
        </tr>
    </thead>
    <tr>
        <td></td>
        <td>msvc 12</td>
        <td>msvc 14</td>
        <td>msvc 15</td>
        <td>gcc 4.9</td>
        <td>gcc 5.4</td>
        <td>gcc 6.3</td>
        <td>clang 4.0</td>
        <td>apple-clang 8.1</td>
    </tr>
    <tr>
        <td>master</td>
        <td><a href="https://ci.appveyor.com/project/jgsogo/queryset-cpp"><img src="https://appveyor-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branch/master/1" alt="Build status"/></a></td>        
        <td><a href="https://ci.appveyor.com/project/jgsogo/queryset-cpp"><img src="https://appveyor-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branch/master/2" alt="Build status"/></a></td>        
        <td><a href="https://ci.appveyor.com/project/jgsogo/queryset-cpp"><img src="https://appveyor-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branch/master/3" alt="Build status"/></a></td>        
        <td><a href="https://travis-ci.org/jgsogo/queryset-cpp"><img src="https://travis-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branches/master/1" alt="Build status"/></a></td>
        <td><a href="https://travis-ci.org/jgsogo/queryset-cpp"><img src="https://travis-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branches/master/2" alt="Build status"/></a></td>
        <td><a href="https://travis-ci.org/jgsogo/queryset-cpp"><img src="https://travis-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branches/master/3" alt="Build status"/></a></td>
        <td><a href="https://travis-ci.org/jgsogo/queryset-cpp"><img src="https://travis-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branches/master/4" alt="Build status"/></a></td>
        <td><a href="https://travis-ci.org/jgsogo/queryset-cpp"><img src="https://travis-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branches/master/5" alt="Build status"/></a></td>
    </tr>
    <tr>
        <td>dev</td>
        <td><a href="https://ci.appveyor.com/project/jgsogo/queryset-cpp"><img src="https://appveyor-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branch/dev/1" alt="Build status"/></a></td>        
        <td><a href="https://ci.appveyor.com/project/jgsogo/queryset-cpp"><img src="https://appveyor-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branch/dev/2" alt="Build status"/></a></td>        
        <td><a href="https://ci.appveyor.com/project/jgsogo/queryset-cpp"><img src="https://appveyor-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branch/dev/3" alt="Build status"/></a></td>        
        <td><a href="https://travis-ci.org/jgsogo/queryset-cpp"><img src="https://travis-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branches/dev/1" alt="Build status"/></a></td>
        <td><a href="https://travis-ci.org/jgsogo/queryset-cpp"><img src="https://travis-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branches/dev/2" alt="Build status"/></a></td>
        <td><a href="https://travis-ci.org/jgsogo/queryset-cpp"><img src="https://travis-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branches/dev/3" alt="Build status"/></a></td>
        <td><a href="https://travis-ci.org/jgsogo/queryset-cpp"><img src="https://travis-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branches/dev/4" alt="Build status"/></a></td>
        <td><a href="https://travis-ci.org/jgsogo/queryset-cpp"><img src="https://travis-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branches/dev/5" alt="Build status"/></a></td>
    </tr>
</table>

All branches under active development (sources and dependencies).


## Dependencies

This is the dependency graph for the project automatically generated with
`conan info`.

![Dependency graph](https://g.gravizo.com/source/dependencies?https%3A%2F%2Fraw.githubusercontent.com%2Fjgsogo%2Fqueryset-cpp%2Fmaster%2FREADME.md)

<details> 
<summary></summary>
dependencies
digraph G {
    rankdir=LR;
    "sqlite3cc/master@jgsogo/stable" -> {"Boost/1.64.0@conan/stable" "sqlite3/3.18.0@jgsogo/stable"};
    "queryset-cpp/0.5@PROJECT" -> {"libpqxx/5.0.1@jgsogo/stable" "Boost/1.64.0@conan/stable" "spdlog/0.9.0@memsharded/stable" "sqlite3cc/master@jgsogo/stable"};
    "Boost/1.64.0@conan/stable" -> {"zlib/1.2.11@conan/stable" "bzip2/1.0.6@conan/stable"};
    "libpqxx/5.0.1@jgsogo/stable" -> {"postgresql/v9.6.5@jgsogo/stable"};
    } 
dependencies
</details> 

Every library can be found in the official Conan repositories or in my own
[bintray remote](https://bintray.com/jgsogo/conan-packages). In order to be able to
compile the project you have to add it to your conan remotes:

```bash
$> conan remote add jgsogo-conan-packages https://api.bintray.com/conan/jgsogo/conan-packages
```


## Basic usage

Iterate a table 

```cpp
    std::cout << "== List of informers ==" << std::endl;
    auto informers = Informer::objects().all();
    for (auto& region : informers.groupBy<Region>(false)) {
        std::cout << region.first << ":" << std::endl;
        for (auto& informer : region.second) {
            std::cout << "\t- " << informer << std::endl;
        }
    }
```

Check a full usage example at https://github.com/jgsogo/neutron


## License

[MIT LICENSE](./LICENSE)
 

