#include "task1.h"

User::~User(){
    if (group){
        group->deleteUser(user_id);
    }
}

void User::userInformation(){
    std::cout << "User ID: " << user_id << std::endl;
    std::cout << "Username: " << user_name << std::endl;
    if (!email.empty()) {
        std::cout << "Email: " << email << std::endl;
    }
    if (group) {
        std::cout << "Group: " << group->getGroupId() << std::endl;
    } else {
        std::cout << "Group: None" << std::endl;
    }
}


void processCommand(Management& system, const std::string& command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == "createUser") {
        std::string userId, username, email = "";
        iss >> userId >> username;
        iss >> email;

        if (userId.empty() || username.empty()) {
            std::cout << "Usage: createUser userId username (+email)" << std::endl;
            return;
        }
        system.createUser(userId, username, email);

    } else if (cmd == "deleteUser") {
        std::string userId;
        iss >> userId;
        if (userId.empty()) {
            std::cout << "Usage: deleteUser userId" << std::endl;
            return;
        }
        system.deleteUser(userId);

    } else if (cmd == "allUsers") {
        system.allUsers();

    } else if (cmd == "getUser") {
        std::string userId;
        iss >> userId;
        if (userId.empty()) {
            std::cout << "Usage: getUser userId" << std::endl;
            return;
        }
        system.getUser(userId);

    } else if (cmd == "createGroup") {
        std::string groupId;
        iss >> groupId;
        
        if (groupId.empty()) {
            std::cout << "Usage: createGroup groupId" << std::endl;
            return;
        }
        system.createGroup(groupId);

    } else if (cmd == "deleteGroup") {
        std::string groupId;
        iss >> groupId;
        if (groupId.empty()) {
            std::cout << "Usage: deleteGroup groupId" << std::endl;
            return;
        }
        system.deleteGroup(groupId);

    } else if (cmd == "allGroups") {
        system.allGroups();

    } else if (cmd == "getGroup") {
        std::string groupId;
        iss >> groupId;
        if (groupId.empty()) {
            std::cout << "Usage: getGroup groupId" << std::endl;
            return;
        }
        system.getGroup(groupId);

    } else if (cmd == "addUserToGroup") {
        std::string userId, groupId;
        iss >> userId >> groupId;
        if (userId.empty() || groupId.empty()) {
            std::cout << "Usage: addUserToGroup userId groupId" << std::endl;
            return;
        }
        system.addUserToGroup(userId, groupId);

    } else if (cmd == "removeUserFromGroup") {
        std::string userId, groupId;
        iss >> userId >> groupId;
        if (userId.empty() || groupId.empty()) {
            std::cout << "Usage: removeUserFromGroup userId groupId" << std::endl;
            return;
        }
        system.deleteUserFromGroup(userId, groupId);

    } else if (cmd != "exit") {
        std::cout << "Unknown command" << std::endl;
    }
}

int main() {
    Management system;
    std::string command;

    std::cout << "User Management System" << std::endl;
    std::cout << "Type 'exit' to quit" << std::endl;

    while (true) {
        std::cout << "\n> ";
        std::getline(std::cin, command);

        if (command == "exit") {
            std::cout << "Bye!" << std::endl;
            break;
        }

        if (!command.empty()) {
            processCommand(system, command);
        }
    }

    return 0;
}