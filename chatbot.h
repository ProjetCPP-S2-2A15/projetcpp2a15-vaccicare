#ifndef CHATBOT_H
#define CHATBOT_H

#include <QTextEdit>
#include <QString>
#include <QColor>
#include "qlabel.h"
#include "qtextedit.h"
#include <QDialog>
#include <regex>
#include <QDebug>

namespace Ui {
class Chatbot;
}

class Chatbot : public QDialog
{
    Q_OBJECT

public:
    explicit Chatbot(QWidget *parent = nullptr);
    ~Chatbot();
    void SimpleChatbot(QTextEdit* prompt, QTextEdit* reply);
    std::string getResponse(const std::string& userInput);
    void updateChat(const QString& userInput, QTextEdit* prompt, QTextEdit* reply);
    void onUserInputReceived(const QString& userInput, QTextEdit* prompt, QTextEdit* reply);
    void showBotImage();
private:
    Ui::Chatbot *ui;
    //std::string responses ;
    std::vector<std::pair<std::regex, std::string>> responses;
    void ExitApp();

    void setupDesign();

};

#endif // CHATBOT_H
