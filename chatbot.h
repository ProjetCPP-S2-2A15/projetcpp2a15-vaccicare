#ifndef CHATBOT_H
#define CHATBOT_H

#include <string>
#include <vector>
#include <regex>

class SimpleChatbot {
public:
    SimpleChatbot();
    std::string getResponse(const std::string& userInput);
private:
    std::vector<std::pair<std::regex, std::string>> responses;
};

#endif // CHATBOT_H
