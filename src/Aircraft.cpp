#include "../Include/Aircraft.hpp"
#include "../Include/Flight.hpp"
#include <iostream>
#include <chrono>
#include <fstream>
#include <iomanip>
#include "../include/json.hpp"

// ===================================== Time Functions ===================================== //

// --- Convert time_point to string :
const std::string formatDateTime(const timeType& tp) {
    std::time_t timeT = std::chrono::system_clock::to_time_t(tp);
    std::tm* tm = std::localtime(&timeT);
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d");
    return oss.str();
}

timeType parseDate(const std::string& str) {
    std::tm tm = {};
    std::istringstream ss(str);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    if (ss.fail()) throw std::runtime_error("Invalid date: " + str);
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

// =====================================   Aircraft Class functions   ===================================== //

Aircraft::Aircraft(const std::string& model, int capacity, bool av, 
    const timeType& last, const timeType& next):
    model(model), capacity(capacity), available(av), lastMaintenance(last), nextMaintenance(next){}

// --- Checking maintenance & availabilty of the aircraft:

bool Aircraft::isMaintained() const{
     if (std::chrono::system_clock::now() > nextMaintenance) {
        std::cout << "Air plane needs maintenance, Please attend to it "
        << "or update maintenance schedule if already maintained" << std::endl;
        return false;
    }
    return true;
}

bool Aircraft::isAvailable() const{
    return (isMaintained() && available);
}

// --- Update maintenance scheduele: 

void Aircraft::updateMaintenanceSchedule(const timeType& newLast, const timeType& newNext){
    try {
        if (newLast < lastMaintenance || newLast > newNext)
            throw std::runtime_error("Incorrect new maintenance schedule");
        lastMaintenance = newLast;
        nextMaintenance = newNext;
    }
    catch (const std::exception& e)
    {
        std::cerr << "exception: " << e.what() << '\n';
    }
}

// ---- Getters  :
const std::string& Aircraft::getModel() const{
    return model;
}

int Aircraft::getCapacity() const{
    return capacity;
}

void Aircraft::displayAircraftInfo() const{
    std::cout<< "Aircraft details: " << std::endl;
    std::cout<< "Aircraft model: " << model << std::endl;
    std::cout<< "Aircraft capacity: " << capacity << std::endl;
    std::cout<< "Aircraft availability: " << model << std::endl;
    std::cout << "Last maintenance time: " << formatDateTime(lastMaintenance) << std::endl;
    std::cout << "Next maintenance time: " << formatDateTime(nextMaintenance) << std::endl;
}

nlohmann::json Aircraft::getAircraftJson() const {
    return {
        {"model", model},
        {"capacity", capacity},
        {"available", available},
        {"lastMaintenance", formatDateTime(lastMaintenance)},
        {"nextMaintenance", formatDateTime(nextMaintenance)}
    };
}

// =====================================   AircraftsSystem class functions   ===================================== //

// ------------ Constructor: Loads aircraft data from JSON file ----------- //
AircraftsSystem::AircraftsSystem() {
    std::ifstream aircraftsFile("database/Aircrafts.json");
    if (!aircraftsFile.is_open()) {
        throw std::runtime_error("Could not open Aircrafts.json");
    }

    nlohmann::json j;
    aircraftsFile >> j; // Read the full JSON array from file

    // Iterate over each aircraft entry in the JSON array
    for (const auto& item : j) {
        std::string type        = item["model"];
        int numPass             = item["capacity"];
        bool av                 = item["available"];
        std::string lastMain    = item["lastMaintenance"];
        std::string nextMain    = item["nextMaintenance"];

        // Create Aircraft object and add to aircrafts vector
        aircrafts.push_back(std::make_shared<Aircraft>(type, numPass, av, parseDate(lastMain), parseDate(nextMain)));
    }
}

// --------- Destructor: Saves aircraft data back to JSON file --------- //
AircraftsSystem::~AircraftsSystem() {
    nlohmann::json jArray = nlohmann::json::array();
    for (auto& aircraft : aircrafts) {
        jArray.push_back(aircraft->getAircraftJson());
    }
    std::ofstream outFile("database/Aircrafts.json");
    outFile << jArray.dump(4);
}

// -------------- Display all aircrafts in the system ----------------------- //
void AircraftsSystem::displayAircrafts() const {
    std::cout << "Aircrafts:\n";
    int i = 1;
    for (auto& aircraft : aircrafts)
    {
        std::cout << i++ << ". ";
        aircraft->displayAircraftInfo();
    }
}

// -------------- Select an aircraft from the list ----------------------- //
int AircraftsSystem::selectAircraft() const{
    displayAircrafts();
    std::cout << "Please enter the number of the aircraft you want to select: " << std::endl;
    int choice;
    std::cin >> choice;

    return choice - 1;
}

// -------------- Add an aircraft to the system and save it to the Aircrafts.json file :
void AircraftsSystem::addAircraft(){
    std::string model;
    int capacity;

        std::cout << "Enter aircraft model: ";
        std::cin >> model;
        std::cout << "Enter aircraft capacity: ";
        std::cin >> capacity;

        auto aircraft = std::make_shared<Aircraft>(model, capacity);
        aircrafts.push_back(aircraft);

        nlohmann::json jArray = nlohmann::json::array();
        for (auto& craft : aircrafts) { 
            jArray.push_back(craft->getAircraftJson());
    }

    // --- Save to JSON file after adding
    std::ofstream outFile("database/Aircrafts.json");
    outFile << jArray.dump(4);
    std::cout << "Aircraft added successfully.\n";
}

// ---- Remove an aircraft from the system :

void AircraftsSystem::removeAircraft(){
    int choice = selectAircraft(); 

    aircrafts.erase(aircrafts.begin() + choice);
    // --- Rebuild JSON array from remaining aircrafts
    nlohmann::json jArray = nlohmann::json::array();
    for (auto& craft : aircrafts) {
        jArray.push_back(craft->getAircraftJson());
    }
    // Save back to file
    std::ofstream outFile("database/Aircrafts.json");
    outFile << jArray.dump(4);  
    std::cout << "Aircraft removed successfully.\n";
}

// ----------------------- Log maintenance of an aircraft ------------------------- //

void AircraftsSystem::logMaintenance() {
    int choice = selectAircraft();
    auto aircraft = aircrafts[choice];

    std::string lastStr, nextStr;
    std::cout << "Enter last maintenance date (YYYY-MM-DD): ";
    std::cin >> lastStr;
    std::cout << "Enter next maintenance date (YYYY-MM-DD): ";
    std::cin >> nextStr;

    try {
        timeType newLast = parseDate(lastStr);
        timeType newNext = parseDate(nextStr);
        aircraft->updateMaintenanceSchedule(newLast, newNext);

        // --- Update JSON file after logging maintenance
        nlohmann::json jArray = nlohmann::json::array();
        for (auto& craft : aircrafts) {
            jArray.push_back(craft->getAircraftJson());
        }
        std::ofstream outFile("database/Aircrafts.json");
        outFile << jArray.dump(4);
        std::cout << "Maintenance logged successfully.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error logging maintenance: " << e.what() << std::endl;
    }
}


// =========================================   Crew Class functions   ======================================= //

Crew::Crew(int id, const std::string& name, const std::string& role, double totalFlightHours): 
    crewID(id), name(name), role(role), totalFlightHours(totalFlightHours){}

// Default constructor loading crew data
Crew::Crew(){
    std::ifstream crewfile("database/Crew.json");
    if (!crewfile.is_open()) {
        throw std::runtime_error("Could not open Crew.json");
    }

    nlohmann::json j;
    crewfile >> j; // Read the full JSON array from file

    // Iterate over each aircraft entry in the JSON array
    for (const auto& item : j) {
        int crewID                   = item["crewID"];
        std::string name             = item["name"];
        std::string role             = item["role"];
        double totalFlightHours      = item["totalFlightHours"];

        // Create Aircraft object and add to aircrafts vector
        crewmembers.push_back(std::make_shared<Crew>(crewID,name, role, totalFlightHours));
    }
}

// ---  Crew members availability :

bool Crew::isCrewAvailable() const{
    if(totalFlightHours < maxFlightHours)   return true;
        else return false; 
}

// --- Assign crew to flights : 

void Crew::assignToFlight(int flightHours){

    if(isCrewAvailable()){
        totalFlightHours +=flightHours;
    }
}

void Crew::displayCrewInfo() const{
    std::cout<< "---- Crew Info ----" <<std::endl; 
    std::cout<< "Crew member name: " << name <<std::endl;
    std::cout<< "Crew member ID: " << crewID <<std::endl;
    std::cout<< "Crew member role: " << role <<std::endl;
    std::cout<< "Crew member total flight hours" << totalFlightHours << std::endl;
}


