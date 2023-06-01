#include <iostream>
#include <vector>

class TestTwo
{
    public:
        std::string str;
};

class TestOne
{
    public:
        std::vector<TestTwo> vec;
        void testFunc();
};

void TestOne::testFunc()
{
    TestTwo a;

    a.str = "test string";
    this->vec.push_back(a);
}

int main()
{
    TestOne t;

    t.testFunc();

    std::cout << t.vec[0].str << std::endl;
}