#ifndef USER_HPP
#define USER_HPP
#include <memory>
#include <string>
#include <iostream>
#include <functional>
#include <fstream>

#include "json.hpp"

// ============================= Role enum class ============================= //
enum class Role{admin, passenger, agent, none};
inline std::string roleToString(Role role){
    switch (role) {
        case Role::admin: return "admin";
        case Role::passenger: return "passenger";
        case Role::agent: return "agent";
        default: return "none";
    }
}
inline Role stringToRole(const std::string& str){
    // std::transform(str.begin(), str.end(), str.begin(), ::tolower);

    if (str == "admin") return Role::admin;
    if (str == "passenger") return Role::passenger;
    if (str == "agent") return Role::agent;
    throw std::runtime_error("Invalid role string: " + str);
}

// ============================= User base abstract class ============================= //
class User{
    friend class UserSystem;
private: 
    int id;
    std::string userName;
    std::string email;
    std::string password;

protected:
    Role role;
   
public:
    User(const std::string& name, const std::string& email ="", const std::string& password ="", int id=0, Role r=Role::none)
        : id(id), userName(name), email(email), password(password), role(r) {}
    
// --------- Getters --------- //
    int getId() const { return id; }
    std::string getUserName() const { return userName; }
    std::string getEmail() const { return email; }
    Role getRole() const { return role; }
    std::string getpassword() const { return password; }

    nlohmann::json getUserJson() const; 
    bool checkPassword(const std::string& input) const;

    // Pure virtual function overriden in subclasses
    virtual bool checkAccessLevel(Role role) const = 0;

    void displayUser() const;
};

// ================================= Administrator Class ================================= //

class Administrator : public User{
    public:
    Administrator(const std::string& name ="", const std::string& email="", const std::string& password="", int id=0)
        : User(name, email, password, id, Role::admin) {}

    bool checkAccessLevel(Role role) const override {
        return role == Role::admin;
    }
};

// ================================= Booking agent Class ================================= //
class BookingAgent : public User{
    public:
    BookingAgent(const std::string& name, const std::string& email="", const std::string& password="", int id=0)
        : User(name, email, password, id, Role::agent) {}

    bool checkAccessLevel(Role role) const override {
        return role == Role::agent;
    }
};

// ===================================== Loyalty Program Class ===================================== //
enum class LoyaltyTier {Basic, Silver, Gold, Platinum};

// ================================= Passenger Class ================================= //
class Passenger : public User{
private: 
    int loyaltyPoints = 0;
    LoyaltyTier loyaltyTier = LoyaltyTier::Basic;

public:
    Passenger() = default;
    Passenger(const std::string& name, const std::string& email="", const std::string& password="", int id=0)
        : User(name, email, password, id, Role::passenger) {}
    bool checkAccessLevel(Role role) const override {
        return role == Role::passenger;
    }
    void addLoyaltyPoints(int points){
        loyaltyPoints += points;
    }
    int getLoyaltyPoints() const { return loyaltyPoints; }
    
    LoyaltyTier getLoyaltyTier() const { return loyaltyTier; }
    void updateLoyaltyTier();
    
};




#endif