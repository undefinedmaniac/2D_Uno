#include <iostream>
#include <random>
#include <chrono>

int main()
{
    // obtain a time-based seed:
    long long seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine engine(seed);

    std::uniform_int_distribution<> distr(25, 63); // define the range

    for(int n=0; n<40; ++n)
        std::cout << distr(engine) << ' '; // generate numbers

    return 0;
}
