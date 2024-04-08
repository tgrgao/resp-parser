#include "Protocol.h"

#include <iostream>

RedisExpression::RedisExpression() : array() {
    type = RedisType::Init;
    finished = false;
}

void RedisExpression::consume_input(std::string& s) {
    if (finished == true) {
        return;
    }

    if (s.empty()) {
        return;
    }

    if (type == RedisType::Init) {
        switch (s[0]) {
            case '+':
                type = RedisType::SimpleString;
                break;
            case '$':
                type = RedisType::BulkString;
                value = "";
                break;
            case ':':
                type = RedisType::Integer;
                break;
            case '*':
                type = RedisType::Array;
                break;
            default:
                type = RedisType::NoMatch;
                value = 0;
                return;
        }

        int at = 1;

        int next_rn = s.find("\r\n", at);
        if (next_rn == std::string::npos) {
            type = RedisType::Init;
            return;
        }

        if (type == RedisType::SimpleString || type == RedisType::Integer) {
            value = s.substr(at, next_rn - at);
            finished = true;
            length = std::get<std::string>(value).length();
            s = s.substr(next_rn + 2);
            return;
        }
        
        length = std::stoi(s.substr(at, next_rn - at));
        at = next_rn + 2;
        s = s.substr(at);

        if (type == RedisType::Array) {
            for (int i = 0; i < length; ++i) {
                array.push_back(std::make_unique<RedisExpression>(std::move(RedisExpression())));
            }
        }
    }

    int next_rn = s.find("\r\n");
    switch (type) {
        case RedisType::BulkString:
            if (next_rn == std::string::npos) {
                value = std::get<std::string>(value) + s;
                finished = false;
                s = "";
                return;
            }
            value = std::get<std::string>(value) + s.substr(0, next_rn);
            finished = true;
            s = s.substr(next_rn + 2);
            return;
            break;
        case RedisType::Array:
            for (int i = 0; i < length; ++i) {
                if (array[i]->finished == false) {
                    array[i]->consume_input(s);
                }
                if (array[i]->finished == false) {
                    break; // no need to keep parsing if the last one didn't complete
                }
            }
            if (array[length - 1]->finished == true) {
                finished = true;
            }
            return;
            break;
        default:
            std::cerr << "Not reachable";
            return;
    }
}

std::string RedisExpression::to_string() {
    if (type == RedisType::SimpleString || type == RedisType::BulkString || type == RedisType::Integer) {
        return std::get<std::string>(value);
    }
    if (type == RedisType::Array) {
        std::string ret = "[";
        for (int i = 0; i < length; ++i) {
            ret += array[i]->to_string();
            if (i != length - 1) {
                ret += ", ";
            }
        }
        ret += "]";
        return ret;
    }
    return "ERROR!";
}




int main() {
    std::string s;

    // s = "*2\r\n*3\r\n:1\r\n:2\r\n:3\r\n*2\r\n+Hello\r\n+World\r\n";
    // s = "*2\r\n$5\r\nhello\r\n$5\r\nworld\r\n";
    // s = "*1\r\n*2\r\n+Hello\r\n+World\r\n";
    // s = "*2\r\n*2\r\n+Hello\r\n+World\r\n*2\r\n+Hello\r\n+World\r\n";

    // RedisExpression exp;
    // exp.consume_input(s);
    // std::cout << exp.finished << " " << exp.to_string() << std::endl;


    s = "*2\r\n*3\r\n:1\r\n:2\r\n:3\r\n*2\r\n+Hel";
    RedisExpression exp;
    exp.consume_input(s);
    std::cout << exp.finished << " " << exp.to_string() << std::endl;
    std::cout << s << std::endl;

    s += "lo\r\n+World\r\n";
    exp.consume_input(s);
    std::cout << exp.finished << " " << exp.to_string() << std::endl;
    std::cout << s << std::endl;
}