#ifndef AIRCRAFT_HPP
#define AIRCRAFT_HPP
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <memory>
#include "../include/json.hpp"

// ===================================== Time functions ===================================== //
using timeType = std::chrono::system_clock::time_point;
const std::string formatDateTime(const timeType& tp); 
timeType parseDate(const std::string& str);

// ===================================== Aircraft Class ===================================== //

class Aircraft {
private:
    std::string model;
    int capacity;
    bool available;

// --- Managing maintenance time of the flight: 
using timeType = std::chrono::system_clock::time_point;
    timeType lastMaintenance;
    timeType nextMaintenance;

public:
    Aircraft(const std::string& model, int capacity, bool av = true, 
        const timeType& last = std::chrono::system_clock::now(),
        const timeType& next = std::chrono::system_clock::now()+std::chrono::minutes(2900));
 
    bool isMaintained() const;
    bool isAvailable() const;
    void updateMaintenanceSchedule(const timeType& newLast, const timeType& newNext);
        
    const std::string& getModel() const;
    nlohmann::json getAircraftJson() const;
    void displayAircraftInfo() const;
    int getCapacity() const;
};

// ===================================== Aircrafts System Class ===================================== //

class AircraftsSystem {
private:
    std::vector<std::shared_ptr<Aircraft>> aircrafts;
    std::fstream aircraftsFile;
    using timeType = std::chrono::system_clock::time_point;

    int selectAircraft() const;

public:
    AircraftsSystem();
    ~AircraftsSystem();

    void addAircraft();
    void removeAircraft();
    void displayAircrafts() const;
    std::vector<std::shared_ptr<Aircraft>> getAircrafts() const { return aircrafts; }

    void logMaintenance();
};

// =====================================   Crew class   ===================================== //

class Crew {

private:
    int crewID;
    std::string name;
    std::string role; 
    double totalFlightHours;
    std::vector<std::shared_ptr<Crew>> crewmembers;
    const double maxFlightHours = 5; //max no of hours per day

public:
    Crew();
    Crew(int id, const std::string& name="", const std::string& role="", double totalFlightHours=0.0);
    bool isCrewAvailable() const;
    void displayCrewInfo() const;
    void assignToFlight(int flightHours);
    std::string getRole(){ return role; }
    int getId() { return crewID;}
    std::vector<std::shared_ptr<Crew>> getCrewMembers() const { return crewmembers; }

};

#endif