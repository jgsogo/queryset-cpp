
#include <iostream>

#include "queryset/models/model.h"


int main(int argc, char** argv){
    std::cout << "== QuerysetCPP Test Package ==\n";
    
    typedef qs::Model<int, float> MyModel;
    std::cout << "Count is: " << MyModel::objects().all().count() << std::endl;
}