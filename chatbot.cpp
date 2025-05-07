#include "chatbot.h"
#include "qgraphicseffect.h"
#include "ui_chatbot.h"
#include <cstdlib>
#include <ctime>
#include <regex>
#include "Design.h"

Chatbot::Chatbot(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Chatbot)
{
    ui->setupUi(this);
    ui->Reply->isReadOnly();
    showBotImage();
    connect(ui->BTR,&QPushButton::clicked,this,&Chatbot::ExitApp);
    connect(ui->BTC, &QPushButton::clicked, this, [this]() {
        // Get the user input and pass it to SimpleChatbot for processing
        SimpleChatbot(ui->Prompt, ui->Reply);

    setupDesign();

    });
}

Chatbot::~Chatbot()
{
    delete ui;
}

void Chatbot::showBotImage() {
    QPixmap pixmap("D:/OpenGlChess/OpenGlFramework/projetcpp2a15-vaccicare/images/Vaccino.png");
    if (!pixmap.isNull()) {
        // Set the frame to be transparent
        ui->frame->setStyleSheet("QFrame { background: transparent; border: none; }");

        // Create a QLabel inside the frame to display the image
        QLabel* imageLabel = new QLabel(ui->frame);
        imageLabel->setPixmap(pixmap.scaled(ui->frame->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        imageLabel->setAlignment(Qt::AlignCenter);
        imageLabel->setGeometry(ui->frame->rect()); // Fill the entire frame
        imageLabel->show();

        // Set full opacity for the image
        QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(imageLabel);
        effect->setOpacity(1.0); // 100% opacity
        imageLabel->setGraphicsEffect(effect);
    } else {
        qDebug() << "Image not found!";
    }
}

void Chatbot::SimpleChatbot(QTextEdit* prompt, QTextEdit* reply) {
    // Define the possible responses using regular expressions
    responses = {
        {std::regex("bonjour|salut|coucou|cc", std::regex_constants::icase), "Bonjour ! Comment puis-je vous aider ?"},
        {std::regex("comment ça va|comment vas-tu|ça va", std::regex_constants::icase), "Je suis juste un bot, mais je vais bien, merci de demander !"},
        {std::regex("ton nom|nom|comment dois-je t'appeler|qui es-tu", std::regex_constants::icase), "Je suis Vaccino, votre assistant dans l'application VacciCare."},
        {std::regex("comment peux-tu m'aider|comment peux-tu m'assister|j'ai besoin d'aide|aide", std::regex_constants::icase), "Je suis Vaccino, je peux être votre guide dans VacciCare. Je raconte même des blagues si vous me demandez un fait amusant."},
        {std::regex("qu'est-ce que bioShield|c'est quoi bioShield|bioShield|à propos", std::regex_constants::icase), "BIOShield, un groupe d'étudiants en informatique ambitieux et passionnés, a créé cette application de bureau. Leur vision est de révolutionner la recherche biologique et de rendre la vaccination plus intelligente, tout en utilisant la technologie pour améliorer la santé publique."},
        {std::regex("au revoir|quitter|bye", std::regex_constants::icase), "Au revoir ! Passez une excellente journée !"}
    };


    std::srand(std::time(nullptr)); // Initialize random seed

    // Get user input from the prompt area (QTextEdit)
    QString userInput = prompt->toPlainText();  // Read the user's input from the prompt QTextEdit

    // Display user input in red in the reply QTextEdit
    QString userMessage = "<p style='color:red;'>User: " + userInput + "</p>";
    reply->append(userMessage);

    // Initialize a flag to check if a match is found
    bool foundResponse = false;

    // Loop through the responses to check if the user input matches any of them
    for (const auto& pair : responses) {
        if (std::regex_search(userInput.toStdString(), pair.first)) {
            // If a match is found, display the chatbot response in blue
            QString botReply = "<p style='color:blue;'>Vaccino: " + QString::fromStdString(pair.second) + "</p>";
            reply->append(botReply);
            foundResponse = true;
            break; // Exit the loop after finding the first match
        }
    }

    // If no match was found, display a default response
    if (!foundResponse) {
        QString botReply = "<p style='color:blue;'>Vaccino: Je suis désolé, je ne comprends pas. Pouvez-vous reformuler cela ?</p>";
        reply->append(botReply);
    }

    // Optionally clear the prompt text after processing (if you want to reset the input area)
    prompt->clear();
}

void Chatbot::onUserInputReceived(const QString& userInput, QTextEdit* prompt, QTextEdit* reply) {
    if (userInput.isEmpty()) {
        return;
    }

    // Display user input in red in the prompt QTextEdit
    QString userMessage = "<p style='color:red;'>User: " + userInput + "</p>";
    prompt->append(userMessage);

    // Get chatbot response based on the user input
    std::string response = getResponse(userInput.toStdString());

    // Display chatbot reply in blue in the Reply QTextEdit
    QString botReply = "<p style='color:blue;'>Vaccino: " + QString::fromStdString(response) + "</p>";
    reply->append(botReply);
}

std::string Chatbot::getResponse(const std::string& userInput) {
    if (std::regex_search(userInput, std::regex("qu'est-ce que vaccicare|à propos de vaccicare", std::regex_constants::icase))) {
        return "VacciCare, développé par l'équipe BIOshield, améliore la recherche biologique et optimise la vaccination en la rendant intelligente. Vous pouvez trouver cette section dans la barre de navigation en haut, dans la section Accueil.";
    } else if (std::regex_search(userInput, std::regex("tableau de bord", std::regex_constants::icase))) {
        return "Vous pouvez trouver le Tableau de bord dans le menu d'Acceuil.";
    } else if (std::regex_search(userInput, std::regex("docteur", std::regex_constants::icase))) {
        return "La section Docteur est accessible depuis le menu d'Acceuil.";
    } else if (std::regex_search(userInput, std::regex("vaccin|vaccination", std::regex_constants::icase))) {
        return "Pour gérer ou rechercher des vaccinations, allez dans la section Vaccin dans le menu d'Acceuil.";
    } else if (std::regex_search(userInput, std::regex("patients", std::regex_constants::icase))) {
        return "La section Patients se trouve dans le menu de d'Acceuil.";
    } else if (std::regex_search(userInput, std::regex("ressources", std::regex_constants::icase))) {
        return "Vous pouvez trouver la section Ressources dans le menu d'Acceuil.";
    }else{
        return"Je suis désolé, je ne comprends pas. Pouvez-vous reformuler cela ?";
}
}

void Chatbot::updateChat(const QString& userInput, QTextEdit* prompt, QTextEdit* reply) {
    // Set the Reply QTextEdit to read-only
    reply->setReadOnly(true);

    // Display user input in red in the prompt QTextEdit
    QString userMessage = "<p style='color:red;'>User: " + userInput + "</p>";
    prompt->append(userMessage);

    // Get chatbot response
    std::string response = getResponse(userInput.toStdString());

    // Display Vaccino's reply in blue in the Reply QTextEdit
    QString botReply = "<p style='color:blue;'>Vaccino: " + QString::fromStdString(response) + "</p>";
    reply->append(botReply);
}

void Chatbot::ExitApp(){
    close();
}

void Chatbot::setupDesign() {

    StyleButtonGreen(ui->BTC);
    StyleButtonGreen(ui->BTR);

    StyleTextEdit(ui->Prompt);
    StyleTextEdit(ui->Reply);

}
