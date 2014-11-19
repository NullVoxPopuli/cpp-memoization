#include <map>
#include <stdio.h>
#include <string>
#include <iostream>
#include <functional>
#include <chrono>

// Most of the fibonocci code and memoize template are influenced from:
// http://programminggenin.blogspot.com/2013/01/memoization-in-c.html

template<class InType, class OutType>
std::function<OutType(InType)> memoize(std::function<OutType(InType)> foo)
{
    // return a lambda, this is a function
    return [foo](InType n)
    {
        static std::map<InType, OutType> memo;

        OutType ret;
        if (memo.count(n) > 0)
        {
            ret = memo[n];
            return ret;
        }
        ret = foo(n);
        memo[n] = ret;
        return ret;
    };
}

// http://stackoverflow.com/questions/2808398/easily-measure-elapsed-time
template<typename TimeT = std::chrono::milliseconds>
struct measure
{
    template<typename F, typename ...Args>
    static typename TimeT::rep execution(F func, Args &&... args)
    {
        auto start = std::chrono::system_clock::now();

        // Now call the function with all the parameters you need.
        func(std::forward<Args>(args)...);

        auto duration = std::chrono::duration_cast< TimeT>
                        (std::chrono::system_clock::now() - start);

        return duration.count();
    }
};



// standard fibonocci
// - exponential run time, as we calculate fibonicci twice per call
unsigned fibonocci(unsigned n)
{
    if (n <= 1)
    {
        return n;

    }
    return fibonocci(n - 1) + fibonocci(n - 2);
}


// lambda version of the above, yet with the memoize template, has the
// performance of the function below this one
std::function<unsigned(unsigned)> fibonocci_lambda = [](unsigned n)
{
    if (n <= 1)
    {
        return n;
    }
    return fibonocci_lambda(n - 1) + fibonocci_lambda(n - 2);
};



// fibonocci with memoization
// - uses a map to cache values
// - runs in n * log(n)
unsigned fibonocci_with_memoization(unsigned n)
{

    // stores all of the key-value pairs of n to value of the fibonocci sequence
    // - dramatically speeds up calculation as we don't have to re-calculate
    // everything each time this method is called
    static std::map<unsigned, unsigned> memo;

    if (n <= 1)
    {
        return n;
    }


    // check our cache if we have calculated for this n before
    if (memo.count(n) > 0)
    {
        return memo[n];
    }


    // we haven't yet calculated for this n
    unsigned result = fibonocci_with_memoization(n - 1) +
                      fibonocci_with_memoization(n - 2);

    // store this result in the cache
    memo[n] = result;
    return result;
}

int main(int argc, char *argv[])
{
    int n = atoi(argv[1]);

    // tell our lambda fibonacci to use our memoize template
    auto memoized = memoize(fibonocci_lambda);

    std::cout << "Duration: " << measure<>::execution([&]()
    {
        std::cout << "fibonocci(" << n << ") -> " << fibonocci(n) << std::endl;
    }) << std::endl;

    std::cout << "Duration: " << measure<>::execution([&]()
    {

        std::cout << "fibonocci_with_memoization(" << n << ") -> " << fibonocci_with_memoization(n) << std::endl;
    }) << std::endl;

    std::cout << "Duration: " << measure<>::execution([&]()
    {
        std::cout << "memoized(" << n << ") -> " << memoized(n) << std::endl;
    }) << std::endl;
}