#include "Protocol.h"

#include <iostream>


int main() {
    std::string s;

    // s = "*2\r\n*3\r\n:1\r\n:2\r\n:3\r\n*2\r\n+Hello\r\n+World\r\n";
    // s = "*2\r\n$5\r\nhello\r\n$5\r\nworld\r\n";
    // s = "*1\r\n*2\r\n+Hello\r\n+World\r\n";
    // s = "*2\r\n*2\r\n+Hello\r\n+World\r\n*2\r\n+Hello\r\n+World\r\n";

    // RedisExpression exp;
    // exp.consume_input(s);
    // std::cout << exp.finished << " " << exp.to_string() << std::endl;


    // s = "*2\r\n*3\r\n:1\r\n:2\r\n:3\r\n*2\r\n+Hel";
    // RedisExpression exp;
    // exp.consume_input(s);
    // std::cout << exp.finished << " " << exp.to_string() << std::endl;
    // std::cout << s << std::endl;

    // s += "lo\r\n+World\r\n";
    // exp.consume_input(s);
    // std::cout << exp.finished << " " << exp.to_string() << std::endl;
    // std::cout << s << std::endl;


    // s = "$5\r\nHello\r";
    // RedisExpression exp1;
    // exp1.consume_input(s);
    // std::cout << exp1.finished << " " << exp1.to_string() << std::endl;

    // s += "\n$5\r\nWorld\r\n";
    // RedisExpression exp2;
    // exp2.consume_input(s);
    // std::cout << exp2.finished << " " << exp2.to_string() << std::endl;

    s = "$5\r\nHello$5\r\nWorld";
    RedisExpression exp;
    exp.consume_input(s);
    std::cout << exp.to_string() << std::endl;
    std::cout << s << std::endl;
}