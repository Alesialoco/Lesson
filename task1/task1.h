#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <sstream>

class User;
class Group;

class User{
    std::string user_id;
    std::string user_name;
    std::string email;
    Group* group;
public:
    User(const std::string& userid, const std::string& username, const std::string& useremail = "")
        : user_id(userid), user_name(username), email(useremail), group(nullptr) {
    }

    ~User();

    std::string getUserId(){ 
        return user_id; 
    }
    std::string getUserName(){ 
        return user_name; 
    }
    std::string getEmail(){ 
        return email; 
    }
    Group* getGroup(){ 
        return group; 
    }

    void setUsername(const std::string& username){ 
        user_name = username; 
    }
    void setEmail(const std::string& useremail){ 
        email = useremail; 
    }
    void setGroup(Group* usergroup){ 
        group = usergroup; 
    }

    void userInformation();
};

class Group{
    std::string group_id;
    std::vector<User*> users_list;
public:
    Group(const std::string& groupid) : group_id(groupid){
    }

    ~Group(){
        for (User* user : users_list) {
            if (user) {
                user->setGroup(nullptr);
            }
        }
        users_list.clear();
    }

    std::string getGroupId(){ 
        return group_id; 
    }
    std::vector<User*> getUsersList(){ 
        return users_list; 
    }

    void addUser(User* user){
        if (user){
            if (std::find(users_list.begin(), users_list.end(), user) == users_list.end()) {
                    users_list.push_back(user);
            }
        }
    }

    void deleteUser(const std::string& userid){
        for (auto i = users_list.begin(); i != users_list.end(); ++i) {
            if (*i != nullptr && (*i)->getUserId() == userid) {
                users_list.erase(i);
                break;
            }
        }  
    }

    void groupInformation(){
        std::cout << "Group ID: " << group_id << std::endl;
        if (!users_list.empty()) {
            std::cout << "Users List:" << std::endl;
            for (size_t i = 0; i < users_list.size(); ++i) {
                std::cout << "  " << (i + 1) << ") " << users_list[i]->getUserName() 
                         << "  ID: " << users_list[i]->getUserId() << std::endl;
            }
        } else {
            std::cout << "  No users" << std::endl;
        }
    }
};

class Management{
    std::map<std::string, std::unique_ptr<User>> all_users;
    std::map<std::string, std::unique_ptr<Group>> all_groups;
public:
    int createUser(const std::string& userid, const std::string& username, const std::string& useremail = ""){
        if (all_users.find(userid) != all_users.end()){
            std::cout << "Error: User " << userid << " exists" << std::endl;
            return 1;
        }
        all_users[userid] = std::make_unique<User>(userid, username, useremail);
        std::cout << "User created" << std::endl;
        return 0;
    }

    int deleteUser(const std::string& userid) {
        auto i = all_users.find(userid);
        if (i == all_users.end()) {
            std::cout << "Error: User " << userid << " not found" << std::endl;
            return 1;
        }
        std::cout << "User deleted" << std::endl;
        all_users.erase(i);
        return 0;
    } 
    
    void allUsers(){
        if (all_users.empty()) {
            std::cout << "No users registrated" << std::endl;
        }
        for (auto& pair : all_users) {
            std::cout << "\n";
            pair.second->userInformation();
        }
    }

    void getUser(const std::string& userid){
        auto i = all_users.find(userid);
        if (i == all_users.end()) {
            std::cout << "User " << userid <<" have not registrated" << std::endl;
        }
        else{
            i->second->userInformation();
        }
    }

    int createGroup(const std::string& groupid){
        if (all_groups.find(groupid) != all_groups.end()){
            std::cout << "Error: Group " << groupid << " exists" << std::endl;
            return 1;
        }
        all_groups[groupid] = std::make_unique<Group>(groupid);
        std::cout << "Grouo created" << std::endl;
        return 0;
    }

    int deleteGroup(const std::string& groupid){
        auto i = all_groups.find(groupid);
        if (i == all_groups.end()) {
            std::cout << "Error: Group " << groupid << " not found" << std::endl;
            return 1;
        }
        std::cout << "Group deleted" << std::endl;
        all_groups.erase(i);
        return 0;
    }

    void allGroups(){
        if (all_groups.empty()) {
            std::cout << "No groups registrated" << std::endl;
        }
        for (auto& pair : all_groups) {
            std::cout << "\n";
            pair.second->groupInformation();
        }
    }

    void getGroup(const std::string& groupid){
        auto i = all_groups.find(groupid);
        if (i == all_groups.end()) {
            std::cout << "Group " << groupid <<" have not registrated" << std::endl;
        }
        else{
            i->second->groupInformation();
        }
    }

    int addUserToGroup(const std::string& userid, const std::string& groupid){
        auto i_user = all_users.find(userid);
        auto i_group = all_groups.find(groupid);
        if (i_user == all_users.end() || !(i_user->second)){
            std::cout << "Error: User " << userid << " not found" << std::endl;
            return 1;
        }
        if (i_group == all_groups.end() || !(i_group->second)){
            std::cout << "Error: Group " << groupid << " not found" << std::endl;
            return 1;
        }
        User* user = i_user->second.get();
        Group* group = i_group->second.get();
        if (user->getGroup()){
            user->getGroup()->deleteUser(userid);
        }
        user->setGroup(group);
        group->addUser(user);
        std::cout << "User added to group" << std::endl;
        return 0;
    }

    int deleteUserFromGroup(const std::string& userid, const std::string& groupid){
        auto i_user = all_users.find(userid);
        auto i_group = all_groups.find(groupid);
        if (i_user == all_users.end() || !(i_user->second)){
            std::cout << "Error: User " << userid << " not found" << std::endl;
            return 1;
        }
        if (i_group == all_groups.end() || !(i_group->second)){
            std::cout << "Error: Group " << groupid << " not found" << std::endl;
            return 1;
        }
        User* user = i_user->second.get();
        Group* group = i_group->second.get();
        if (user->getGroup() != group) {
            std::cout << "User already not in this group" << std::endl;
            return 0;
        }
        group->deleteUser(userid);
        user->setGroup(nullptr);
        std::cout << "User removed from this group"<< std::endl;
        return 0;
    }
};