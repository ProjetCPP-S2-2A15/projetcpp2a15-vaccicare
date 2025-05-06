#include "chatbot.h"
#include "ui_chatbot.h"
#include <cstdlib>
#include <ctime>
#include <regex>

Chatbot::Chatbot(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Chatbot)
{
    ui->setupUi(this);
}

Chatbot::~Chatbot()
{
    delete ui;
}

void Chatbot::SimpleChatbot() {
    responses = {
        {std::regex("hello|hi|what's up", std::regex_constants::icase), "Hello! How can I help you?"},
        {std::regex("how are you|how do you do", std::regex_constants::icase), "I'm just a bot, but I'm doing well, thanks for asking!"},
        {std::regex("your name|name|how should i call you|who are you", std::regex_constants::icase), "I'm Vaccino, your assistant in the VacciCare app."},
        {std::regex("how can you assist me|how can you help|i need help|help", std::regex_constants::icase), "I'm Vaccino, I can be your tour guide in VacciCare. I even tell jokes if you ask me about a fun fact."},
        {std::regex("what is vaccicare|what's vaccicare|vaccicare|about", std::regex_constants::icase), "VacciCare, developed by the BIOshield team, enhances biological research and improves vaccination by making it smart."},
        {std::regex("bye|exit", std::regex_constants::icase), "Goodbye! Have a great day!"}
    };

    std::srand(std::time(nullptr)); // Initialize random seed
}
std::string Chatbot::getResponse(const std::string& userInput) {
    if (std::regex_search(userInput, std::regex("what is VacciCare|about VacciCare", std::regex_constants::icase))) {
        return "VacciCare, developed by the BIOshield team, enhances biological research and improves vaccination by making it smart.";
    } else if (std::regex_search(userInput, std::regex("dashboard", std::regex_constants::icase))) {
        return "You can find the Dashboard in the left menu.";
    } else if (std::regex_search(userInput, std::regex("doctor", std::regex_constants::icase))) {
        return "The Doctor section is accessible from the left menu.";
    } else if (std::regex_search(userInput, std::regex("vaccin|vaccine", std::regex_constants::icase))) {
        return "To manage or look up vaccinations, go to the Vaccin section in the left menu.";
    } else if (std::regex_search(userInput, std::regex("patients", std::regex_constants::icase))) {
        return "The Patients section is located in the left menu.";
    } else if (std::regex_search(userInput, std::regex("resources|ressources", std::regex_constants::icase))) {
        return "You can find the Ressources section in the left menu.";
    }

    // Fallback to default patterns
    for (const auto& pair : responses) {
        if (std::regex_search(userInput, pair.first)) {
            return pair.second;
        }
    }

    return "I'm sorry, I don't understand. Can you rephrase that?";
}

void Chatbot::ExitApp(){
    close();
}
