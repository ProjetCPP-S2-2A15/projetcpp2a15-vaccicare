#ifndef CHATBOT_H
#define CHATBOT_H

#include <QDialog>
#include <regex>

namespace Ui {
class Chatbot;
}

class Chatbot : public QDialog
{
    Q_OBJECT

public:
    explicit Chatbot(QWidget *parent = nullptr);
    ~Chatbot();
    void SimpleChatbot();
    std::string getResponse(const std::string& userInput);
private:
    Ui::Chatbot *ui;
    //std::string responses ;
    std::vector<std::pair<std::regex, std::string>> responses;

};

#endif // CHATBOT_H
