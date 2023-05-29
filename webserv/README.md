## Coding Convention
### Class
1. Class 이름은 PascalCase
ex) class ConfigParser

2. Class 정의는 private, protected, public 순으로 작성

3. Class 멤버변수는 _로 시작하는 SnakeCase
ex) std::string _server_name

3-1. Class 멤버변수 선언부는 전부 tab으로 줄간격을 일치시킨다.

4. Class 멤버함수는 CamelCase
ex) void setServerName(std::string name)

4-1. Class 멤버변수를 반환하는 모든 get Methods는 return by const reference로 작성
ex) const size_t& getServerClientMaxBodySize()

4-2. 이외의 모든 함수는 return by Value로 작성
ex) std::string readFile()
-> return by value는 값의 복사가 일어나서 비효율적
-> 그러나 return by reference는 invalid한 메모리 주소를 반환하거나, memory leak 문제가 발생할 가능성이 있음
-> 따라서 함수 내부에서 stack영역에 변수를 할당하고 그걸 반환하는 함수는 return by value로 작성해야 함
-> 그러나 class 멤버 변수는 class 객체 선언과 동시에 할당되고, 해당 클래스 소멸시 자동으로 메모리 해제됨

4-3. Class 멤버함수에서 멤버 변수에 접근할 경우 반드시 this 포인터를 사용

4-4. Class 멤버함수의 선언부 주석은 함수들의 기능에 따라 한줄 주석으로 구분한다.
ex) /* get methods */

4-5. Class 멤버함수의 구현부 주석은 구현부 함수 위에만 작성하고, 주석의 시작과 끝은 반드시 줄바꿈으로 구분하며, 주석 내용은 탭으로 들여쓰기를 수행한다. 첫줄에는 함수의 기능을 간단하게 한줄로 설명하고, 한줄을 비운 뒤 다음줄부터 세부적인 설명을 작성한다.
ex)
/*
    this function is sample function.
    
    in this line, specific explanation should be located.
*/
void Class::Function()

4-6. get/set method를 제외한 모든 멤버함수들은 주석을 달아 코드를 읽지 않아도 기능을 이해할 수 있도록 한다.