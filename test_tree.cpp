#include "rbtree.h"
#include <iostream>
int main( int argc, char **argv) {

    tree<int> intTree;    
    intTree.insert(15);
    intTree.insert(6);
    intTree.insert(18);
    intTree.insert(3);
    intTree.insert(7);
    intTree.insert(17);
    intTree.insert(20);
    intTree.insert(2);
    intTree.insert(4);
    intTree.insert(13);
    intTree.insert(9);

    for ( tree<int>::iterator itr = intTree.begin();
        itr != intTree.end(); ++itr) {
            std::cout << *itr << "  " ;
    }
    std::cout << std::endl;
    
    intTree.remove(13);

    for ( tree<int>::iterator itr = intTree.begin();
        itr != intTree.end(); ++itr) {
            std::cout << *itr << "  " ;
    }

    if ( intTree.find(17) != intTree.end()) {
        std::cout << "Found " << std::endl;
    }
    if ( intTree.find(29) == intTree.end()) {
        std::cout << "Not Found " << std::endl;
    }

    return 0;
}