#include <iostream>
#include <string>
#include <map>
#include <regex>
using namespace std;

// Карта вопросов (ключи) и ответов (значения)
map<string, string> questions_answers = {
        {"Hello", "Hello to you human."},
        {"What is your.*name", "My name is SkillChatBot 3000."},
        {"How do you do", "I'm functioning withing normal parameters."},
        {"What are you doing", "Answering stupid questions."},
        {"Exit", "Never wanted talk to you in first place."}
};

void print_reply(string reply) {
    cout << "[bot]: " << reply << "\n";
}

// Функция bot_reply
// На вход: текст от пользователя (string user_text),
// На выход: ответ бота (string)
// return - результат работы функции
void bot_reply(string user_text) {
    // Цикл поиск и проверки ответа
    // Для каждой записи в карте вопросов(first) и ответов(second):
    bool found = false; // found { false = ответ не найден, true = ответ найден }
    for (auto item : questions_answers) {
        // Проверить похож ли вопрос на текст пользователя

        // Regular Expressions - сравнить строчку текста с шаблоном
        // .*hello.* - любой текст, который содержит слово hello
        // hello, hello!, HEdLLO=), Privet Hello Hola

        regex pattern(".*" + item.first + ".*", regex_constants::icase); // icase = ignore case
        if (regex_match(user_text, pattern)) {
            found = true;
            print_reply(item.second);
        }

    }
    // if not found - если не найдено
    if (!found) {  // ! - НЕ
        print_reply("I don't understand you, please correct yourself human.");
    }
}



int main()
{
    // cout - console output - выводить текст на экран
    // \n - новая строка
    cout << "Welcome to SkillChatBot 3000\n";
    // Чат бота
    // 1. Принимает сообщение от пользователя
    // // Нужен диалог!
    // 2. Код, который будет отвечать на сообщение
    // // Правильно понимать фразу! Hello! hello Hello=)

    string question;  // string - будем хранить строчку текста, назовем ее question

    // Выполнять до тех пор, пока в переменной не текст "exit"
    while (question != "exit") {
        cout << "[user]: ";
        getline(cin, question);  // getline = Прочитать строчку текста
        bot_reply(question);  // печатаем ответ бота
    }
}