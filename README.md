# queryset-cpp

C++ implementation of an ORM inspired by [Python/Django](https://www.djangoproject.com/). It allows to query
a database-like backend using compact and human readable code.

Supported backends:

 * filesystem (read)
 * sqlite3
 * postgresql


## Build status

All branches under active development (sources and dependencies).

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
                <img src="https://ci.appveyor.com/api/projects/status/vekvvo87f5dngklk?svg=true" alt="Build status"/>
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
</table>


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
 

