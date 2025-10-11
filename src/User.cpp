#include "../Include/User.hpp"
// ================================= User Class Base Methods ================================= //

//------------- Return user details as a JSON object ------------- //

nlohmann::json User::getUserJson() const {
    nlohmann::json j;
    j["id"]       = id;
    j["name"]     = userName;
    j["email"]    = email;
    j["password"] = password;
    j["role"]     = roleToString(role);
    return j;
}

// ------------- Check if input password matches stored password ------------- //
bool User::checkPassword(const std::string& inputPassword) const {
    return inputPassword == password;
}

// ------------------ Get user details -----------------------------------//
void User::displayUser() const{
    std::cout<< "User Role: " << roleToString(role) << std::endl;
    std::cout << "user name: " << userName << " User id: " << id 
    << "User email: " << email << std::endl;
}

// ================================= Passenger Class Methods ================================= //
void Passenger::updateLoyaltyTier() {
    if (loyaltyPoints >= 20) {
        loyaltyTier = LoyaltyTier::Platinum;
    } else if (loyaltyPoints >= 10) {
        loyaltyTier = LoyaltyTier::Gold;
    } else if (loyaltyPoints >= 5) {
        loyaltyTier = LoyaltyTier::Silver;
    } else {
        loyaltyTier = LoyaltyTier::Basic;
    }
}