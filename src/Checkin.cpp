#include "Checkin.hpp"


bool CheckinSystem::airportCheckIn(int reservationId) {
 std::ifstream file("reservation.json");
    if (!file.is_open()) {
        std::cout << "Could not open reservation.json file.\n";
        return false;
    }

    nlohmann::json reservationsJson;
    file >> reservationsJson;
    file.close();

    bool updated = false;
    for (auto& res : reservationsJson) {
        if (res["reservationId"] == reservationId) {
            res["checkIn"] = "checked In";
            updated = true;
            break;
        }
    }

    if (updated) {
        std::ofstream outFile("reservation.json");
        outFile << reservationsJson.dump(4);
        outFile.close();
        std::cout << "Airport check-in completed for reservation ID: " << reservationId << std::endl;
        return true;
    } else {
        std::cout << "Reservation ID " << reservationId << " not found.\n";
        return false;
    }
}

void CheckinSystem::generateBoardingPass(int reservationId) const {
    std::ifstream file("reservation.json");
    if (!file.is_open()) {
        std::cout << "Could not open reservation.json file.\n";
        return;
    }

    nlohmann::json reservationsJson;
    file >> reservationsJson;
    file.close();

    for (const auto& res : reservationsJson) {
        if (res["reservationId"] == reservationId) {
            std::cout << "\n--- Boarding Pass ---\n";
            std::cout << "Passenger: " << res.value("passengerName", "N/A") << "\n";
            std::cout << "Flight Number: " << res.value("flightNumber", "N/A") << "\n";
            std::cout << "Gate: " << res.value("gate", "N/A") << "\n";
            std::cout << "Seat: " << res.value("seatNum", "N/A") << "\n";
            std::cout << "Boarding Time: " << res.value("boardingTime", "N/A") << "\n";
            std::cout << "---------------------\n";
            return;
        }
    }
    std::cout << "Reservation ID " << reservationId << " not found.\n";
}

void CheckinSystem::displayCheckinStatus(int reservationId) const {
    std::ifstream file("reservation.json");
    if (!file.is_open()) {
        std::cout << "Could not open reservation.json file.\n";
        return;
    }

    nlohmann::json reservationsJson;
    file >> reservationsJson;
    file.close();

    for (const auto& res : reservationsJson) {
        if (res["reservationId"] == reservationId) {
            std::cout << "Check-in status for reservation ID " << reservationId << ":\n";
            std::cout << "  Status: Checked In\n";
            std::cout << "  Gate: " << res.value("gate", "N/A") << "\n";
            std::cout << "  Seat: " << res.value("seatNum", "N/A") << "\n";
            std::cout << "  Boarding Time: " << res.value("boardingTime", "N/A") << "\n";
            std::cout << "  Flight Number: " << res.value("flightNumber", "N/A") << "\n";
            return;
        }
    }
    std::cout << "Reservation ID " << reservationId << " not found.\n";
}