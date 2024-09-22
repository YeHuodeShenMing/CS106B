#include <iostream>
#include <vector>

int main() {
    // 初始化映射表，大小为128，以覆盖所有可能的ASCII字符
    std::vector<char> mapping(128, '-'); // 用 '-' 表示未定义的字符

    // 填充映射表
    std::string letters = "AEIOUHWY";
    for (char c : letters) mapping[c] = '0';
    
    letters = "BFVP";
    for (char c : letters) mapping[c] = '1';
    
    letters = "CGJKQSXZ";
    for (char c : letters) mapping[c] = '2';
    
    letters = "DT";
    for (char c : letters) mapping[c] = '3';
    
    letters = "L";
    for (char c : letters) mapping[c] = '4';
    
    letters = "MN";
    for (char c : letters) mapping[c] = '5';
    
    letters = "R";
    for (char c : letters) mapping[c] = '6';
    
    std::string input = "HELLO";
    for (char c : input) {
        std::cout << mapping[c];
    }
    std::cout << std::endl;

    return 0;
}