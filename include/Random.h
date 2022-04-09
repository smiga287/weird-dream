//
// Created by smiga287 on 4/9/22.
//

#ifndef FUTURAMA_RANDOM_H
#define FUTURAMA_RANDOM_H
#include <random>

class Random {
public:
    Random() : rng_(rand_dev_()) {};
    int get_random() { return dist_(rng_); }
private:
    std::random_device rand_dev_;
    std::mt19937 rng_;
    std::uniform_int_distribution<std::mt19937::result_type> dist_;
};

#endif //FUTURAMA_RANDOM_H
