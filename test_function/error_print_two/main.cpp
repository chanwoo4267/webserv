#include <iostream>
#include "error.hpp"

void throwError()
{
    //throw std::runtime_error("test runtime");
    ThrowExceptionWithInfo(); // 에러 발생시키기
}

int main()
{
    try {
        throwError();
    } catch(const ExceptionWithInfo& e) {
        std::cerr << e.what() << std::endl;
    } catch(const std::runtime_error& e) {
        std::cerr << "runtime error!!\n";
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
