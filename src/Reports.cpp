#include "../Include/Reports.hpp"
#include <iostream>
#include <fstream>
#include "../Include/json.hpp"

void Reports::generateOperationalReport() const {
    std::cout << "\n--- Operational Report ---\n";
    std::ifstream flightsFile("database/Flights.json");
    std::ifstream reservationsFile("reservation.json");
    nlohmann::json flightsJson, reservationsJson;
    if (flightsFile) flightsFile >> flightsJson;
    if (reservationsFile) reservationsFile >> reservationsJson;
    std::cout << "Total Flights: " << flightsJson.size() << "\n";
    std::cout << "Total Reservations: " << reservationsJson.size() << "\n";
}

void Reports::generateMaintenanceReport() const {
    std::cout << "\n--- Maintenance Report ---\n";
    std::ifstream aircraftsFile("database/Aircrafts.json");
    nlohmann::json aircraftsJson;
    if (aircraftsFile) aircraftsFile >> aircraftsJson;
    for (const auto& aircraft : aircraftsJson) {
        std::cout << "Aircraft: " << aircraft.value("model", "N/A")
                  << " | Last Maintenance: " << aircraft.value("lastMaintenance", "N/A")
                  << " | Next Maintenance: " << aircraft.value("nextMaintenance", "N/A") << "\n";
    }
}

void Reports::generateUserActivityReport() const {
    std::cout << "\n--- User Activity Report ---\n";
    std::ifstream reservationsFile("reservation.json");
    nlohmann::json reservationsJson;
    if (reservationsFile) reservationsFile >> reservationsJson;
    for (const auto& res : reservationsJson) {
        std::cout << "Passenger: " << res.value("passengerName", "N/A")
                  << " | Flight: " << res.value("flightNumber", "N/A")
                  << " | Status: " << res.value("checkIn", "N/A") << "\n";
    }
}