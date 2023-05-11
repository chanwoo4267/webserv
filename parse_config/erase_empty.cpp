#include <iostream>
#include <string>

using namespace std;

int main(void){

    string st = " a b c d ";

    // 좌측 공백 제거
    st.erase(st.begin());
    cout << st << endl;

    // 우측 공백 제거
    st.erase(st.end());
    cout << st << endl;

    // 전체 공백 제거
    st.erase(remove(st.begin(), st.end(), ' '), st.end());
    cout << st << endl;
}

// std::erase std::remove both implemented in c++98