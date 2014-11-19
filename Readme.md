Currently, this only runs Memoized Fibonicci.
Other fun functions coming, as I'm also trying to learn templates + lambdas in C++

Compile

    make

Run

    ./memo 40

Observe Results!

    fibonocci(30) -> 102334155
    Duration: 827
    fibonocci_with_memoization(30) -> 102334155
    Duration: 0
    memoized(30) -> 102334155
    Duration: 6229


I think I'm doing something wrong if the memoize template (last result) is taking longer than the exponential version of fibonocci (the first one)