#include "Checkin.hpp"


bool CheckinSystem::airportCheckIn(int reservationId) {
 std::ifstream file("Database/Reservations.json");
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
        std::ofstream outFile("Database/Reservations.json");
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
    auto reservation = reservationSystem.getReservationById(reservationId);

    if (!reservation) {
        std::cout << "Reservation ID " << reservationId << " not found.\n";
        return;
    }

    auto passenger = reservation->getPassenger();
    auto flight = reservation->getFlight();

    if (!passenger || !flight) {
        std::cout << "Error: Passenger or Flight information missing.\n";
        return;
}

    std::cout << "\n--- Boarding Pass ---\n";
    std::cout << "Passenger: " << passenger->getUserName() << "\n";
    std::cout << "Flight Number: " << flight->getFlightNo() << "\n";
    std::cout << "From: " << flight->getOrigin() << "\n";
    std::cout << "To: " << flight->getDestination() << "\n";
    std::cout << "Seat: " << reservation->getSeatNo() << "\n";
    std::cout << "Boarding Time: " << formatDateTime(flight->getDepartureTime()) << "\n";
    std::cout << "---------------------\n";
}

void CheckinSystem::displayCheckinStatus(int reservationId) const {
    auto reservation = reservationSystem.getReservationById(reservationId);
    std::ifstream file("Database/Reservations.json");
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
            std::cout << "  Seat: " << reservation->getSeatNo() << "\n";
            std::cout << "  Flight Number: " << reservation->flight->getFlightNo() << "\n";
            std::cout << "  Boarding Time: " << formatDateTime(reservation->flight->getDepartureTime()) << "\n";
            return;
        }
    }
    std::cout << "Reservation ID " << reservationId << " not found.\n";
}