queryset-cpp
============

C++ implementation of an ORM inspired by [Python/Django](https://www.djangoproject.com/). It allows to query
a database-like backend using compact and human readable code.

Supported backends:

 * filesystem (read)
 * sqlite3
 * postgresql


## Build status

All branches under active development (sources and dependencies).

| **master (linux/osx)** | **master (windows)** |
|------------------------|----------------------|
| [![Build travis](https://travis-ci.org/jgsogo/queryset-cpp.svg?branch=master)](https://travis-ci.org/jgsogo/queryset-cpp) | [![Build appveyor]( https://ci.appveyor.com/api/projects/status/vekvvo87f5dngklk/branch/master?svg=true)](https://ci.appveyor.com/project/jgsogo/queryset-cpp/branch/master)     |


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
 

