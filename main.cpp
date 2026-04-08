#include "table.h"
#include <iostream>
#include <string>

int main(){
    Table table(10, 10);
    table.kiir();
    
    ExpCell* test = new ExpCell(table.parse("A10+A10*A10-A10/A10")); //10+10*10-10/10 = 109
    std::cout << test->getEval();
    delete test;
    return 0;
}

//TODO: make userinput, error handling, file system, text cell ops -> beta ver.
