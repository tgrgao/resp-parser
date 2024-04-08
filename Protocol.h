#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <memory>
#include <variant>
#include <vector>
#include <map>

class RedisExpression;

class RedisExpression {
    public:
        enum RedisType {
            Init,
            NoMatch,
            SimpleString,
            BulkString,
            Integer,
            Array
        };

        RedisType type;
        std::variant<int, std::string> value;
        std::vector<std::unique_ptr<RedisExpression>> array;
        bool finished;
        int length;

        RedisExpression();
        void consume_input(std::string& s);

        std::string to_string();
};

#endif