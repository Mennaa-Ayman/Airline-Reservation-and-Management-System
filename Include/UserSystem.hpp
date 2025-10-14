#ifndef USERSYSTEM_HPP
#define USERSYSTEM_HPP

#include "User.hpp"
#include <iostream>
#include <fstream>
#include <memory>
#include <conio.h>
#include <string>


// ============================== UserSystem class ============================== //

class UserSystem{
private:
    std::vector<std::shared_ptr<User>> users;
    std::vector<std::shared_ptr<Passenger>> passengers;
    std::shared_ptr<User> inputUser;
    std::fstream Usersfile;
    Role role;

    bool isEmailUnique(const std::string& email) const; 

public: 
    UserSystem();
    ~UserSystem();

    std::shared_ptr<User> checkLogin(std::string& email, const std::string& password);
    bool login(const std::string& username, const std::string& password); 

    void addUser();
    void removeUser();
    void updateUser();
    std::shared_ptr<User> getCurrentUser(const std::string& email) ;

    void displayUsers() const; 
    std::shared_ptr<Passenger> getPassengerById(int id);

};

#endif