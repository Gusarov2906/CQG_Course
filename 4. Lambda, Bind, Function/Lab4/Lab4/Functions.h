#pragma once
class Decrement {
public:
    Decrement() {};
    int operator()(int val) { val--; return val; }
};

class Functions
{
public:
    static int square(int x, int y);
};
