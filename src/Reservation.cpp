#include "Reservation.hpp"
#include "User.hpp"
#include "UserSystem.hpp"
#include "Flight.hpp"

// ================================== Reservation Class =================================== //

// Initialize static member++
int Reservation::reservationCount = 0; 

// -------------------- Constructor -------------------- //
Reservation::Reservation(int reservationId, const std::shared_ptr<Passenger>& p, const std::shared_ptr<Flight>& f, int s, 
    std::string& method, std::string& details, int amount)
    : reservationId(reservationId),passenger(p), flight(f), payment(method, details, amount), seatNum(s){
         ++reservationCount;
    }

// --------------------- Confirm Reservation --------------------- //
void Reservation::confirmReservation() const {
    std::cout << "\nBooking successful!\n";
    std::cout << "Reservation ID: " << reservationId << std::endl;
    std::cout << "Flight: " << flight->getFlightDetails() << std::endl;
    std::cout << "Seat: " << seatNum << std::endl;
    std::cout << "Total Cost: " << payment.amount << std::endl;
    std::cout << "Payment Method: " << payment.method << std::endl;
}

// --------------------- Display Reservation --------------------- //
void Reservation::displayReservation() const {
    // std::cout << "Passenger: " << passenger->getUserName() << std::endl;
    std::cout << "Reservation ID: " << reservationId << std::endl;
    std::cout << "Flight: " << flight->getFlightDetails() << std::endl;
    std::cout << "Seat: " << seatNum << std::endl;
    std::cout << "Total Cost: " << payment.amount << std::endl;
    std::cout << "Payment Method: " << payment.method << std::endl;
}

// --------------------- Modify Reservation --------------------- //
void Reservation::modifyReservation(int resid, int s){
    if (reservationId == resid) {
        seatNum = s;
        std::cout<<"Reservation ID: "<<reservationId;
        std::cout<<" is modified with new seat number: "<<seatNum<<std::endl;
    }
}
// ---------------------- Cancel Reservation -------------------- //
void Reservation::cancelReservation(){
    std::cout<<"Reservation ID: "<<reservationId;
    std::cout<<" is canceled\n";
    std::cout << "Total Cost: " << payment.amount << " is refunded through ";
    std::cout << "Payment Method: " << payment.method << std::endl;
}

// ================================== Reservation System Class ================================== // 

// ---------------------------------- Default constructor ---------------------------------- //
ReservationSystem::ReservationSystem(FlightSystem& fs, UserSystem& us)
    : flightSystem(fs), userSystem(us)
{
    std::ifstream reservationsFile("Database/Reservations.json");
    if (!reservationsFile.is_open())
        throw std::runtime_error("Could not open Reservations.json");

    nlohmann::json j;
    reservationsFile >> j;

    for (const auto& item : j) {
        int resId               = item["reservationId"];
        int pId                 = item["passengerid"];
        int flightNum           = item["flightNumber"];
        int seatNum             = item["seatNumber"];
        std::string method      = item["payment"]["method"];
        std::string details     = item["payment"]["details"];
        int amount              = item["payment"]["amount"];

        auto passenger = userSystem.getPassengerById(pId);
        auto flight = flightSystem.getFlightByNumber(flightNum);

        reservations.push_back(std::make_shared<Reservation>(
            resId, passenger, flight, seatNum, method, details, amount));
    }
}


// ------------------ Get Reservation by ID -------------------- //
std::shared_ptr<Reservation> ReservationSystem::getReservationById(int resId) {
    for (const auto& reservation : reservations) {
        if (reservation->getReservationId() == resId) {
            return reservation;
        }
    }
    return nullptr; // Return nullptr if not found
}


// ---------------------- display Reservations of a passenger --------------------- //
void ReservationSystem::displayReservations(int p_id) const {
    bool found = false;
    int count =1;

    std::cout<< "You Reservations: " << std::endl;
    for (const auto& reservation : reservations) {
        if (reservation->getPassenger()->getId() == p_id) {
            found = true;
            std::cout << count++ << ". ";
            reservation->displayReservation();
            std::cout << "-----------------------\n";
        }
    }
    if (!found) {
        std::cout << "No reservations found for Passenger ID: " << p_id << std::endl;
    }
}

// ----------------------- add Reservation -------------------------- //
void ReservationSystem::addReservation(const std::shared_ptr<Reservation>& r){
    // In‑memory list
    reservations.push_back(r);
}

// ------------------------ Remove Reservation --------------------- //
void ReservationSystem::cancelReservation(int passengerId){
  std::cout << "Enter your Reservation ID: ";
    int resId; std::cin >> resId;
    for (auto it = reservations.begin(); it != reservations.end(); ++it) {
        if ((*it)->getReservationId() == resId && (*it)->getPassenger()->getId() == passengerId) {
            (*it)->cancelReservation();
            reservations.erase(it);
            std::cout << "Cancellation successful for Reservation ID: " << resId << std::endl;

    // ---- Update JSON file ----
    std::ifstream inFile("Database/Reservations.json");
    nlohmann::json j; 
    inFile >> j;
    inFile.close();

    // Remove the reservation from JSON
    auto &arr = j;
    for (auto jsonIt = arr.begin(); jsonIt != arr.end(); ++jsonIt) {
        if ((*jsonIt)["reservationId"] == resId) {
            arr.erase(jsonIt);
            break;
        }
    }
     std::ofstream outFile("Database/Reservations.json");
            if (!outFile.is_open()) {
                std::cerr << "Error writing to Reservations.json\n";
                return;
            }
            outFile << std::setw(4) << j << std::endl;
            outFile.close();

            return;
        }
    }
    std::cout << "Reservation ID: " << resId << " not found or does not belong to you.\n";

}

// ---------------------- Check Reservation --------------------- //
std::optional<std::pair<std::string, std::string>> ReservationSystem::
    checkReservation(const int& p_id, const int& r_id){
    for(const auto& reservation : reservations){
        if(reservation->getReservationId() == r_id && reservation->getPassenger()->getId() == p_id){
            return std::make_optional(std::make_pair(reservation->getFlight()->getFlightDetails(), std::to_string(reservation->getSeatNo())));
        }
    }
    return std::nullopt;
}

// ----------------------------- Book by Agent  ---------------------------------- //
void ReservationSystem::BookByAgent(){
    int passengerId, flightNum, seatNum, amount;
    std::string method, details, input;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter Passenger ID: ";
    std::getline(std::cin, input);
    try {
        passengerId = std::stoi(input);
    } catch (...) {
        std::cout << "Invalid Passenger ID.\n";
        return;
    }
    auto passenger = userSystem.getPassengerById(passengerId);
    if (!passenger) {
        std::cout << "Passenger not found.\n";
        return;
    }

    std::cout << "Enter Flight Number: ";
    std::getline(std::cin, input);
        flightNum = std::stoi(input);

    auto flight = flightSystem.getFlightByNumber(flightNum);
    if (!flight) {
        std::cout << "Flight not found.\n";
        return;
    }

    std::cout << "Enter Seat Number: ";
    std::getline(std::cin, input);
    seatNum = std::stoi(input);

    std::cout << "Enter Payment Method: ";
    std::getline(std::cin, method);

    std::cout << "Enter Payment Details: ";
    std::getline(std::cin, details);

    std::cout << "Enter Amount: ";
    std::getline(std::cin, input);
    amount = std::stoi(input);

    int newReservationId = ++Reservation::reservationCount;
    auto reservation = std::make_shared<Reservation>(newReservationId, passenger, flight, seatNum, method, details, amount);
    reservations.push_back(reservation);
    reservation->confirmReservation();
    std::cout << "Booking completed.\n";

    std::ifstream inFile("Database/Reservations.json");
    nlohmann::json j;
if (inFile.is_open()) {
    inFile >> j;
    inFile.close();
} else {
    std::cout << "Failed to open Reservations.json for reading.\n";
    return;
}

// 2. Add the new reservation
nlohmann::json newRes = {
    {"reservationId", newReservationId},
    {"passengerid", passenger->getId()},
    {"flightNumber", flight->getFlightNo()},
    {"seatNumber", seatNum},
    {"checkIn", "not yet"},
     {"payment", {
        {"method", method},
        {"details", details},
        {"amount", amount}
    }}
};

j.push_back(newRes);

// 3. Write back to the file
std::ofstream outFile("Database/Reservations.json");
if (outFile.is_open()) {
    outFile << j.dump(4);
    outFile.close();
    std::cout << "Reservation saved to file.\n";
} else {
    std::cout << "Failed to open Reservations.json for writing.\n";
}
}

// ----------------------------- Remove booking  ---------------------------------- //
// Cancel reservation as booking agent (can cancel any reservation)
void ReservationSystem::removeBooking() {
    std::cout << "Enter Reservation ID to cancel: ";
    int resId;
    std::cin >> resId;

    for (auto it = reservations.begin(); it != reservations.end(); ++it) {
        if ((*it)->getReservationId() == resId) {
            // Cancel and remove from memory
            (*it)->cancelReservation();
            reservations.erase(it);
            std::cout << "Cancellation successful for Reservation ID: " << resId << std::endl;

            // Now update the JSON file
            std::ifstream inFile("Database/Reservations.json");

            nlohmann::json j;
            inFile >> j;
            inFile.close();

            // Remove the reservation from JSON
            auto &arr = j;
            for (auto jsonIt = arr.begin(); jsonIt != arr.end(); ++jsonIt) {
                if ((*jsonIt)["reservationId"] == resId) {
                    arr.erase(jsonIt);
                    break;
                }
            }
            // Write back to file
            std::ofstream outFile("Database/Reservations.json");
            if (!outFile.is_open()) {
                std::cerr << "Error writing to Reservations.json\n";
                return;
            }
            outFile << std::setw(4) << j << std::endl;
            outFile.close();
            return;
        }
    }
    std::cout << "Reservation ID: " << resId << " not found.\n";
}

// ----------------------------- Modify booking  ---------------------------------- //
void ReservationSystem::modifyBooking() {
    std::cout << "Enter Reservation ID to modify: ";
    int resId;
    std::cin >> resId;

    std::cout << "Enter new Seat Number: ";
    int newSeat;
    std::cin >> newSeat;

    for (const auto& reservation : reservations) {
        if (reservation->getReservationId() == resId) {
            // Modify the reservation in memory
            reservation->modifyReservation(resId, newSeat);
            std::cout << "Modification successful for Reservation ID: " << resId << std::endl;

            // ---- Update JSON file ----
            std::ifstream inFile("Database/Reservations.json");

            nlohmann::json j;
            inFile >> j;
            inFile.close();

            // Find and update the matching reservation
            for (auto& item : j) {
                if (item["reservationId"] == resId) {
                    item["seatNumber"] = newSeat;
                    break;
                }
            }

            // Write updated JSON back to file
            std::ofstream outFile("Database/Reservations.json");
            if (!outFile.is_open()) {
                std::cerr << "Error writing to Reservations.json\n";
                return;
            }

            outFile << std::setw(4) << j << std::endl;
            outFile.close();

            return;
        }
    }

    std::cout << "Reservation ID: " << resId << " not found.\n";
}
