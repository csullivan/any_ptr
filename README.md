any_ptr
--------------
A simple header-only smart pointer class that employs type erasure and safe run-time casts of void*.

__any_ptrs__ enable the storage of heterogeneous types within a single container, e.g.

```C++
    std::vector<any_ptr> ptrs; 
    ptrs.emplace_back(new std::vector<int>({4,5,6})); 
    ptrs.push_back(make_any<std::vector<double>>({7.7,8.8,9.9}));
```

and safe run-time casting to their progenitor types,

```C++

    for (auto& ptr : ptrs) {
        if (auto runtime_casted_ptr = std::shared_ptr<std::vector<double>>(ptr)) {
            std::cout << "Runtime cast to std::vector<double> successful!\n";
            std::cout << runtime_casted_ptr[0] << endl;
        }
    }
```

 Adapted from Cassio Neri's implementation to be Boost-free with C++11 idioms.
