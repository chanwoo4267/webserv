#include <iostream>
#include <string>

void removeWhiteSpace(std::string &content)
{
	size_t i = 0;
    size_t j = 0;

	while (content[i] && isspace(content[i]))
		i++;
	j = content.length() - 1;
	while (j > 0 && isspace(content[j]))
		j--;
	content = content.substr(i, j + 1 - i);
}

void test(void)
{
    system("leaks a.out");
}

int main()
{
    std::string str = "            asdf1234      ";
    removeWhiteSpace(str);
    std::cout << "-" << str << "-" << std::endl;
    atexit(test);
}