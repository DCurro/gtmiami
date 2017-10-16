#include "RandomIdentifierProvider.hpp"

#include <random>
#include <limits>

RandomIdentifierProvider::RandomIdentifierProvider() {
    
}

RandomIdentifierProvider::~RandomIdentifierProvider() {
    
}

int RandomIdentifierProvider::randomIdentifer() {
    std::random_device rd;
    
    std::default_random_engine e1(rd());
    std::uniform_int_distribution<int> uniform_dist(1, INT_MAX);
    int randomInt = uniform_dist(e1);
    
    return randomInt;
}