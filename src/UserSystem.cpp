#include "../Include/UserSystem.hpp"

// ================================= UserSystem Class Methods ================================= //

// -------- No argument constructor to load users from JSON file --------- //

UserSystem::UserSystem(){
    std::ifstream Usersfile("database/Users.json");
    if (!Usersfile.is_open()) 
        throw std::runtime_error("Could not open users.json");
    nlohmann::json j;
    Usersfile >> j; // Read the full JSON array from file

    // Iterate over each user entry in the JSON array
    for (const auto& item : j) {
        int id              = item["id"];
        std::string name    = item["name"]; 
        std::string email   = item["email"];
        std::string pass    = item["password"];
        Role role           = stringToRole(item["role"]);  
        std::shared_ptr<User> user = nullptr;

        if(role == Role::admin) user = std::make_shared<Administrator>(name, email, pass, id); 
        else if(role == Role::agent) user = std::make_shared<BookingAgent>(name, email, pass, id);
        else if(role == Role::passenger) user = std::make_shared<Passenger>(name, email, pass, id);

        else throw std::runtime_error("Invalid role in JSON" + roleToString(role));
        users.push_back(user);

    }
}

// --------------------------- Display all users ------------------------// 
void UserSystem::displayUsers() const{
    std::cout << "Users on the system: " <<std::endl;
    int i=1;
    for (auto user:users){
        std::cout << i++ << ".";
        user->displayUser();
    }
}

// ------------------------------- Current user ------------------------------- //
std::shared_ptr <User> UserSystem::getCurrentUser(const std::string& email){
    for(const auto& user: users){
        if(user->getEmail() == email)          
         return user;
    }
    return nullptr; // Return nullptr if no matching user is found
}

// --------------------------- Check email is unique ----------------------- // 
bool UserSystem::isEmailUnique(const std::string& email) const{
    for(const auto& user: users){
        if(user->getEmail() == email) return false;
    }
    return true;
}
// --------------------------- Create a new user --------------------------- //
void UserSystem::addUser(){
    int newId = users.empty() ? 1 : users.back()->getId() + 1;
    std::string name, email, password;
    Role role;
    while(true){
        std::cout << "Enter name: ";            std::cin >> name;
        std::cout << "Enter email: ";           std::cin >> email;
        if(!isEmailUnique(email)){
            std::cout << "Email already exists. Please use a different email.";
            continue;
        }
    std::cout << "Enter password: ";        std::cin >> password;
    std::cout << "Enter role (1) admin/ (2) agent/ (3) passenger): ";
    int roleChoice;                          std::cin >> roleChoice;
    role = static_cast<Role>(roleChoice - 1);
    break;
    }

    if(role == Role::admin)
        inputUser = std::make_shared<Administrator>(name, email, password, newId);
    else if(role == Role::agent)
        inputUser = std::make_shared<BookingAgent>(name, email, password, newId);
    else if(role == Role::passenger)
        inputUser = std::make_shared<Passenger>(name, email, password, newId);
    else throw std::runtime_error("Invalid role selected.");

    users.push_back(inputUser);

    // Save to JSON file after adding
    nlohmann::json jArray = nlohmann::json::array();

   for (auto& user : users) {
    // parse the JSON string returned by getUserJson into a json object
        jArray.push_back(user->getUserJson());
    }

    std::ofstream outFile("database/Users.json");
    if(!outFile.is_open())
        throw std::runtime_error("Could not open Flights.json for writing");
    outFile << jArray.dump(4); 
    std::cout << "User added successfully.\n";
}

// --------------------------------------- Remove user --------------------------------------- //
void UserSystem::removeUser() {
    std::cout << "Enter User ID to remove: ";
    int userId; std::cin>>userId;
    auto it = std::remove_if(users.begin(), users.end(),
        [userId](const std::shared_ptr<User>& user) { return user->getId() == userId;
        });

    if (it != users.end()) {
        users.erase(it, users.end());

        // Save to JSON file after removing
        nlohmann::json jArray = nlohmann::json::array();
        for (const auto& user : users) {
            jArray.push_back(user->getUserJson());
        }

        std::ofstream outFile("database/Users.json");
        outFile << jArray.dump(4);
    } 
    else std::cout << "User ID not found.";
}

// --------------------------------------- Update user --------------------------------------- //
void UserSystem::updateUser() {
    std::cout << "Enter User ID: ";
    int userId; std::cin>>userId;
    std::string newName, newEmail, newPassword;
    std::cout << "Enter new name (or press Enter to skip): ";
    std::cin.ignore(); 
    std::getline(std::cin, newName);
    std::cout << "Enter new email (or press Enter to skip): ";
    std::getline(std::cin, newEmail);
    std::cout << "Enter new password (or press Enter to skip): ";
    std::getline(std::cin, newPassword);

    for (auto& user : users) {
        if (user->getId() == userId) {
            if (!newName.empty()) user->userName = newName;
            if (!newEmail.empty()) {
                if (!isEmailUnique(newEmail)) {
                    throw std::runtime_error("Email already exists. Please use a different email.");
                }
                user->email = newEmail;
            }
            if (!newPassword.empty()) user->password = newPassword;

            // Save to JSON file after updating
            nlohmann::json jArray = nlohmann::json::array();
            for (const auto& u : users) {
                jArray.push_back(u->getUserJson());
            }

            std::ofstream outFile("database/Users.json");
            outFile << jArray.dump(4);
            std::cout << "User updated successfully.\n";
            return;
        }
    }
    throw std::runtime_error("User ID not found.");
}

// ------------- check login Info ---------------------- //
bool UserSystem::login(const std::string& inputemail, const std::string& password) {
    for (const auto& user : users) {
        if (user->getEmail() == inputemail && user->checkPassword(password)) {
            Role r = user->getRole();
        if (r == Role::admin)
            inputUser = std::make_shared<Administrator>(user->getUserName(), user->getEmail(), user->getpassword(), user->getId());
        else if (r == Role::agent)
            inputUser = std::make_shared<BookingAgent>(user->getUserName(), user->getEmail(), user->getpassword(), user->getId());
        else if (r == Role::passenger)
            inputUser = std::make_shared<Passenger>(user->getUserName(), user->getEmail(), user->getpassword(), user->getId());
    std::cout << "Login successful as " << roleToString(r) << "!\n";
    return true;
}
        }
        std::cout << "Invalid email or password.\n";
        return false;
    }

// ----------------------- Get passenger by id -------------------------- //

std::shared_ptr<Passenger> UserSystem::getPassengerById(int id) {
    for (const auto& user : users) {
        if (user->getId() == id) {
            return std::dynamic_pointer_cast<Passenger>(user);
        }
    }
    return nullptr; // No matching passenger found
}

