/*
 * Проблема с подключениес библиотек при помощи vcpkg
 *
 */


#include <iostream>
#include <uwebsockets/App.h>
#include <nlohmann/json.hpp>


using namespace std;
using json = nlohmann::json;


struct UserData {
    int id;
    string name;
};

int main()
{  // каод взят с  https://github.com/uNetworking/uWebSockets

    int latest_eser_id = 10;
    uWS::App().get("/hello", [](auto* response, auto* request) {
        /*
           You can efficiently stream huge files too
           response - запрос на веб сервер
           request - ответ сервера
        */
        response->writeHeader("Content-Type", "text/html; charset=utf-8")->end("Welcome CharServer");

    }).ws<UserData>("/*", {
            /* вызывается при открытие соединние когда подключается новый пользователь */
            .open = [&latest_eser_id](auto* ws) {
                UserData* user_data = ws->getUserData();
                // Выдать пользователю ID
                user_data->id = latest_eser_id++;
                // По-умолчанию имя нового пользователя "unnamed user"
                user_data->name = "unnamed user";

                cout << "User connected: " << user_data->id << endl;

                ws->subscribe("public_channel"); // Вссех кто подключается подписываем на канал public_channel
                ws->subscribe("user" + to_string(user_data->id)); // И еще на ЛИЧНЫЙ канал // user11
            },
            // Вызывается при отправление сообщения пользователем (байтики)
            .message = [](auto* ws, string_view message, uWS::OpCode opCode) {
                // https://github.com/nlohmann/json - библиотек для json

                UserData* user_data = ws->getUserData();
                json message_data = json::parse(message);// Данные от пользователя


                if (message_data["command"] == "public_msg") { // Усли пользователь отправит public_msg
                    json payload; // готовим посылку
                    // Отправляем публичое сообщение
                    payload["command"] = "public_msg";
                    payload["text"] = message_data["text"];
                    payload["user_id"] = user_data->id;
                    // payload.dump - выводит JSON
                    ws->publish("public_channel", payload.dump());
                    cout << "User sent public message: " << user_data->id << endl;
                }
                if (message_data["command"] == "private_msg") {
                    json payload;
                    payload["command"] = "private_msg";
                    payload["text"] = message_data["text"];
                    payload["user_from"] = user_data->id;
                    ws->publish("user" + to_string(message_data["user_to"]), payload.dump()); // Отправляем лично получателю
                    cout << "User sent private message: " << user_data->id << " to: " << message_data["user_to"] << endl;
                }
                // Протокол на основе JSON
                // Планируем дать пользователю след. возможности:
                // 1. Отправлять публичные сообщение всем на сервер
                //  // "СЕРВЕР ЛОХ" => обработать ошибку, из-за неверного формата
                // Пользователь серверу: {"command": "public_msg", "text": "Всем приветики и чмоки"}
                // Сервер пользователям: {"command": "public_msg", "text": "Всем приветики и чмоки", "user_id": 11}
                // 2. Отправлять приватные сообщения кому-то конкретному
                // Пользователь серверу: {"command": "private_msg", "text": "Привет тебе 11ый!", "user_to": 11}
                // Сервер пользователю : {"command": "private_msg", "text": "Привет тебе 11ый!", "user_from": 13}
                // 3. Указать свое имя
                // ws = new WebSocket("ws://127.0.0.1:9001/"); ws.onmessge = ({data}) => console.log("FROM SERVER: ", data);
                // payload = {"command": "public_msg", "text": "Всем привет"}
                // ws.send(JSON.stringify(payload))
            }

    }).listen(9001, [](auto* listenSocket) { // запускает наш сервер по адресу http://127.0.0.1:9001

        if (listenSocket) {
            cout << "Listening on port " << 9001 << endl;
        }

    }).run();
}
