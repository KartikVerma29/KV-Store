#include <iostream>
#include <sstream>
#include <string>
#include "../factory/StorageEngineFactory.h"
#include "../factory/EngineConfig.h"

using namespace std;

int main() {
    EngineConfig configs;
    configs.enableLogging = true;
    configs.walPath = "/home/zerotwo/Desktop/Code/Project/Project-4/KV_Store/data/wal.log";
    configs.logtype = LoggerType::CONSOLE;

    StorageEngineFactory factory;
    auto engine = factory.createEngine(configs);

    cout << "\n--- KV Store REPL ---\n";
    cout << "Commands: PUT <key> <value> | GET <key> | DEL <key> | EXIT\n\n";

    string line;
    while (true) {
        cout << "> ";
        if (!getline(cin, line)) break;

        istringstream iss(line);
        string cmd, key;
        iss >> cmd;

        if (cmd == "EXIT") {
            break;
        } else if (cmd == "PUT") {
            iss >> key;
            string value;
            // Get the rest of the line as the value (handles spaces)
            getline(iss >> ws, value); 
            if (key.empty() || value.empty()) {
                cout << "Error: PUT requires a key and a value.\n";
            } else {
                engine->put(key, value);
            }
        } else if (cmd == "GET") {
            iss >> key;
            if (key.empty()) {
                cout << "Error: GET requires a key.\n";
            } else {
                auto result = engine->get(key);
                if (result) {
                    cout << *result << "\n";
                } else {
                    cout << "(nil)\n";
                }
            }
        } else if (cmd == "DEL") {
            iss >> key;
            if (key.empty()) {
                cout << "Error: DEL requires a key.\n";
            } else {
                engine->remove(key);
            }
        } else if (!cmd.empty()) {
            cout << "Unknown command: " << cmd << "\n";
        }
    }

    engine->flush();
    return 0;
}