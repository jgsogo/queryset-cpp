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
            <th>Windows</th>
            <th colspan="5">Unix</th>
        </tr>
    </thead>
    <tr>
        <td></td>
        <td>Visual Studio</td>
        <td>gcc 4.8</td>
        <td>gcc 4.9</td>
        <td>gcc 5.2</td>
        <td>gcc 5.3</td>
        <td>gcc 6.3</td>
    </tr>
    <tr>
        <td>master</td>
        <td>
            <a href="https://ci.appveyor.com/project/jgsogo/queryset-cpp">
                <img src="https://ci.appveyor.com/api/projects/status/vekvvo87f5dngklk/branch/master?svg=true" alt="Build status"/>
            </a>
        </td>        
        <td>
            <a href="https://travis-ci.org/jgsogo/queryset-cpp">
                <img src="https://travis-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branches/master/1" alt="Build status"/>
            </a>
        </td>
        <td>
            <a href="https://travis-ci.org/jgsogo/queryset-cpp">
                <img src="https://travis-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branches/master/2" alt="Build status"/>
            </a>
        </td>
        <td>
            <a href="https://travis-ci.org/jgsogo/queryset-cpp">
                <img src="https://travis-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branches/master/3" alt="Build status"/>
            </a>
        </td>
        <td>
            <a href="https://travis-ci.org/jgsogo/queryset-cpp">
                <img src="https://travis-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branches/master/4" alt="Build status"/>
            </a>
        </td>
        <td>
            <a href="https://travis-ci.org/jgsogo/queryset-cpp">
                <img src="https://travis-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branches/master/5" alt="Build status"/>
            </a>
        </td>
    </tr>
    <tr>
        <td>dev</td>
        <td>
            <a href="https://ci.appveyor.com/project/jgsogo/queryset-cpp">
                <img src="https://ci.appveyor.com/api/projects/status/vekvvo87f5dngklk/branch/dev?svg=true" alt="Build status"/>
            </a>
        </td>        
        <td>
            <a href="https://travis-ci.org/jgsogo/queryset-cpp">
                <img src="https://travis-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branches/dev/1" alt="Build status"/>
            </a>
        </td>
        <td>
            <a href="https://travis-ci.org/jgsogo/queryset-cpp">
                <img src="https://travis-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branches/dev/2" alt="Build status"/>
            </a>
        </td>
        <td>
            <a href="https://travis-ci.org/jgsogo/queryset-cpp">
                <img src="https://travis-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branches/dev/3" alt="Build status"/>
            </a>
        </td>
        <td>
            <a href="https://travis-ci.org/jgsogo/queryset-cpp">
                <img src="https://travis-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branches/dev/4" alt="Build status"/>
            </a>
        </td>
        <td>
            <a href="https://travis-ci.org/jgsogo/queryset-cpp">
                <img src="https://travis-matrix-badges.herokuapp.com/repos/jgsogo/queryset-cpp/branches/dev/5" alt="Build status"/>
            </a>
        </td>
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
    "Boost/1.60.0@lasote/stable" -> {"zlib/1.2.8@lasote/stable" "bzip2/1.0.6@lasote/stable"};
    "queryset-cpp/0.5@PROJECT" -> {"sqlite3cc/master@jgsogo/stable" "libpqxx/5.0.1@jgsogo/stable" "spdlog/0.9.0@memsharded/stable" "Boost/1.60.0@lasote/stable"};
    "sqlite3cc/master@jgsogo/stable" -> {"sqlite3/3.18.0@jgsogo/stable" "Boost/1.60.0@lasote/stable"};
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
 

