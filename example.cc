#include "any_ptr.hh"
#include <vector>
#include <iostream>

int main() {

    std::vector<double>* DoubleVecPtr = new std::vector<double>;
    DoubleVecPtr->push_back(1.1);
    DoubleVecPtr->push_back(2.2);
    DoubleVecPtr->push_back(3.3);

    std::vector<any_ptr> ptrs;

    ptrs.emplace_back(DoubleVecPtr);
    ptrs.emplace_back(new std::vector<int>({4,5,6}));
    ptrs.push_back(make_any<std::vector<double>>({7.7,8.8,9.9}));


    for (auto& ptr : ptrs)
    {
        if (auto runtime_casted_ptr = std::shared_ptr<std::vector<double>>(ptr))
        {
            std::cout << "Runtime cast to std::vector<double> successful!\n";
            for (auto const& dbl : *runtime_casted_ptr)
            {
                std::cout << dbl << std::endl;
            }
        }
        else if (auto runtime_casted_ptr = std::shared_ptr<std::vector<int>>(ptr))
        {
            std::cout << "Runtime cast to std::vector<int> successful!\n";
            for (auto const& integer : *runtime_casted_ptr)
            {
                std::cout << integer << std::endl;
            }
        } else {  std::cout << "Unsuccessful runtime cast from any_ptr\n";  }
    }

    return 0;
}
