#include "../include/Flight.hpp"
#include <algorithm>
#include <ctime>

// =========================================   SeatMap Class   ========================================= //
SeatMap::SeatMap(int seatNum) : totalSeats(seatNum) {
    for (int i = 1; i <= totalSeats; i++) {
        seatsAvailability.emplace(i, true);
    }
}

bool SeatMap::bookSeat(int num) {
    auto it = seatsAvailability.find(num);
    if (it != seatsAvailability.end() && it->second) {
        it->second = false;
        return true;
    }
    return false;
}

bool SeatMap::unbookSeat(int num) {
    auto it = seatsAvailability.find(num);
    if (it != seatsAvailability.end() && !it->second) {
        it->second = true;
        return true;
    }
    return false;
}

int SeatMap::seatsCount() {
    int count = 0;
    for (const auto& seat : seatsAvailability) {
        if (seat.second) count++;
    }
    return count;
}

void SeatMap::resetMap() {
    for (auto& seat : seatsAvailability) {
        seat.second = true;
    }
}

// ============================================   Flight Class   ============================================ //

Flight::Flight(int flightNum, const std::string& orig, const std::string& dest, FlightStatus s,
    std::shared_ptr<Aircraft> craft, const timeType& depTime, const timeType& arrTime)
    
    : flightNumber(flightNum), origin(orig), destination(dest),
      status(s), aircraft(craft), departureTime(depTime),
      arrivalTime(arrTime), seats(craft->getCapacity()) 
{
    if (!craft->isMaintained())
        throw std::runtime_error("Aircraft needs maintenance, Can't assign it to the flight");
}

// ------ Calculate flight duration in hours :
int Flight::getFlightHours() const {
    auto duration = std::chrono::duration_cast<std::chrono::hours>(arrivalTime - departureTime);
    return static_cast<int>(duration.count());
}

// ------ Assign crew to the flight :
void Flight::setCrew(std::shared_ptr<Crew> crew) {
    if (crew->isCrewAvailable()) {
        flightCrewMembers.push_back(crew);
        crew->assignToFlight(getFlightHours());
    } else {
        throw std::runtime_error("Crew member is not available for this flight");
    }
}

// ------ Assign aircraft to the flight :
void Flight::setAircraft(std::shared_ptr<Aircraft> craft) {
    if (craft->isMaintained()) {
        aircraft = craft;
    } else {
        throw std::runtime_error("Aircraft needs maintenance, Can't assign it to the flight");
    }
}

// ------ Check if flight is full :
bool Flight::isFlightFull() {
    return (seats.seatsCount() == 0);
}

// ------ Update Flight status :
void Flight::changeStatus(FlightStatus newStatus) {
    status = newStatus;
}

// ------ Display flight details :
std::string Flight::getFlightDetails() const {
    std::cout << flightNumber << " from " << origin << " to " << destination << std::endl;
    std::cout << "Departure time: " << formatDateTime(departureTime) << std::endl;
    std::cout << "Arrival time: " << formatDateTime(arrivalTime) << std::endl;
    return "";
}
// --- check if flight matches search criteria :
bool Flight::isFlightMatch(const std::string& orgi, const std::string& desti, const timeType& time) const {
    if (origin != orgi || destination != desti) return false;

    std::time_t dep_tt = std::chrono::system_clock::to_time_t(departureTime);
    std::time_t q_tt   = std::chrono::system_clock::to_time_t(time);

    std::tm dep_tm{};
    std::tm q_tm{};

    #if defined(_WIN32)
        localtime_s(&dep_tm, &dep_tt);
        localtime_s(&q_tm, &q_tt);
    #else
        localtime_r(&dep_tt, &dep_tm);
        localtime_r(&q_tt, &q_tm);
    #endif

    return dep_tm.tm_year == q_tm.tm_year &&
           dep_tm.tm_mon  == q_tm.tm_mon  &&
           dep_tm.tm_mday == q_tm.tm_mday;
}

// --- Get flight details in JSON format :
nlohmann::json Flight::getFlightJson() const {
    nlohmann::json j;
    j["flightNumber"] = flightNumber;
    j["origin"] = origin;
    j["destination"] = destination;
    j["status"] = (status == FlightStatus::scheduled) ? "scheduled" :
                  (status == FlightStatus::delayed) ? "delayed" :
                  (status == FlightStatus::canceled) ? "canceled" : "onTime";
    j["departureTime"] = formatDateTime(departureTime);
    j["arrivalTime"] = formatDateTime(arrivalTime);
    j["aircraftModel"] = aircraft ? aircraft->getModel() : "None";

    nlohmann::json crewArray = nlohmann::json::array();
    for (auto& member : flightCrewMembers) {
        if (member)
            crewArray.push_back(member->getId());
    }
    j["crewIDs"] = crewArray;

    return j;
}

// ============================================   FlightSystem Class   ============================================ //

FlightSystem::FlightSystem() {
    AircraftsSystem aircraftSystem;
    aircrafts = aircraftSystem.getAircrafts();
    Crew crew;
    crewMembers = crew.getCrewMembers();

    std::fstream flightsfile("database/Flights.json");
    if (!flightsfile.is_open()) {
        throw std::runtime_error("Could not open Flights.json");
    }

    nlohmann::json j;
    flightsfile >> j;

    for (const auto& item : j) {
        int flightNum              = item["flightNumber"];
        std::string orig           = item["origin"];
        std::string dest           = item["destination"];
        std::string statusStr      = item["status"];
        std::string depTimeStr     = item["departureTime"];
        std::string arrTimeStr     = item["arrivalTime"];
        std::string aircraftModel  = item["aircraftModel"];

        FlightStatus status;
        if (statusStr == "scheduled") status = FlightStatus::scheduled;
        else if (statusStr == "delayed") status = FlightStatus::delayed;
        else if (statusStr == "canceled") status = FlightStatus::canceled;
        else if (statusStr == "onTime") status = FlightStatus::onTime;
        else throw std::runtime_error("Invalid flight status in JSON");

        std::shared_ptr<Aircraft> aircraftPtr = nullptr;
        for (const auto& craft : aircrafts) {
            if (craft->getModel() == aircraftModel) {
                aircraftPtr = craft;
                break;
            }
        }
        if (!aircraftPtr)
            throw std::runtime_error("Aircraft model not found for flight " + aircraftModel);

        flights.push_back(std::make_shared<Flight>(
            flightNum, orig, dest, status, aircraftPtr,
            parseDate(depTimeStr), parseDate(arrTimeStr)
        ));

        auto newFlight = flights.back();

        // Load assigned crew IDs
        if (item.contains("crewIDs")) {
            for (int id : item["crewIDs"]) {
                for (auto& crew : crewMembers) {
                    if (crew && crew->getId() == id) {
                        newFlight->setCrew(crew);
                        break;
                    }
                }
            }
        }
    }
}

// -------------------- Destructor : Saves flight data to JSON file -------------------- //
// FlightSystem::~FlightSystem() {
//     nlohmann::json jArray = nlohmann::json::array();
//     for (auto& flight : flights) {
//         jArray.push_back(flight->getFlightJson());
//     }
//     std::ofstream outFile("database/Flights.json");
//     outFile << jArray.dump(4);
// }

// ---------------- Display all flights ----------------
void FlightSystem::displayFlights() const {
    std::cout << "Flights Available:\n";
    int i = 1;
    for (auto& flight : flights) {
        std::cout << i++ << ". ";
        flight->getFlightDetails();
    }
}

// ----------------------- Select a flight -----------------------
int FlightSystem::selectFlight() {
    std::cout << "Enter flight number to select: ";
    int choice;
    std::cin >> choice;
    return choice - 1;
}

//----------- Assign crew to flight ------------------
void FlightSystem::selectCrew(std::shared_ptr<Flight> myflight) {
    if (!myflight) throw std::runtime_error("Flight not found");

    const int maxFlightHours = 5; // avoid <cmath> ceil; use integer math
    const int flightHours = myflight->getFlightHours();
    int requiredPilots = std::max(1, (flightHours + maxFlightHours - 1) / maxFlightHours);

    std::vector<std::shared_ptr<Crew>> availablePilots;
    for (auto& c : crewMembers) {
        if (c && c->getRole() == "Pilot" && c->isCrewAvailable())
            availablePilots.push_back(c);
    }

    if (availablePilots.empty()) {
        std::cout << "No available pilots.\n";
        return;
    }

    int assigned = 0;
    while (assigned < requiredPilots && !availablePilots.empty()) {
        std::cout << "Available Pilots:\n";
        for (auto& p : availablePilots) p->displayCrewInfo();

        std::cout << "Select Pilot by ID (0 to finish): ";
        int id;
        if (!(std::cin >> id)) {
            std::cin.clear();
            std::string dummy;
            std::getline(std::cin, dummy);
            std::cout << "Invalid input.\n";
            continue;
        }
        if (id == 0) break;

        auto it = std::find_if(availablePilots.begin(), availablePilots.end(),
                               [id](const std::shared_ptr<Crew>& c) { return c->getId() == id; });

        if (it == availablePilots.end()) {
            std::cout << "Invalid ID or pilot not available.\n";
            continue;
        }

        try {
            myflight->setCrew(*it);
            availablePilots.erase(it);
            ++assigned;
            std::cout << "Pilot assigned.\n";
        } catch (const std::exception& e) {
            std::cout << "Failed to assign pilot: " << e.what() << "\n";
        }
    }

    if (assigned == 0) {
        std::cout << "No pilots assigned.\n";
        return;
    }
    if (assigned < requiredPilots) {
        std::cout << "Assigned " << assigned << " of " << requiredPilots << " required pilots.\n";
    } else {
        std::cout << "All required pilots assigned.\n";
    }

    // Persist changes to JSON
    nlohmann::json jArray = nlohmann::json::array();
    for (const auto& fl : flights) {
        if (fl) jArray.push_back(fl->getFlightJson());
    }
    std::ofstream outFile("database/Flights.json");
    if (!outFile.is_open())
        throw std::runtime_error("Could not open Flights.json for writing");
    outFile << jArray.dump(4);
}


// ----------- Change flight status ----------------- //
void FlightSystem::updateFlightStatus(int flightNum) {
    std::cout << "Enter new status (1: scheduled, 2: delayed, 3: canceled, 4: onTime): ";
    int num; std::cin >> num;
    FlightStatus newStatus;

    switch (num) {
        case 1: newStatus = FlightStatus::scheduled; break;
        case 2: newStatus = FlightStatus::delayed;   break;
        case 3: newStatus = FlightStatus::canceled;  break;
        case 4: newStatus = FlightStatus::onTime;    break;
        default: throw std::runtime_error("Invalid flight status");
    }

    for (const auto& f : flights) {
        if (f && f->getFlightNo() == flightNum) {
            f->changeStatus(newStatus);

            // Persist changes to JSON
            nlohmann::json jArray = nlohmann::json::array();
            for (const auto& fl : flights) {
                if (fl) jArray.push_back(fl->getFlightJson());
            }
            std::ofstream outFile("database/Flights.json");
            if (!outFile.is_open())
                throw std::runtime_error("Could not open Flights.json for writing");
            outFile << jArray.dump(4);

            std::cout << "Status updated.\n";
            return;
        }
    }
    throw std::runtime_error("Flight number incorrect");
}

// -------------------------------- Add a flight to the system ---------------------------------- //
void FlightSystem::addFlight() {
    int flightNum;
    std::string orig, dest, statusStr, depTimeStr, arrTimeStr, aircraftModel;

    // Ask for a unique, valid flight number 
    while (true) {
        std::cout << "Enter flight number: ";
        std::cin >> flightNum;

        if (getFlightByNumber(flightNum)) {
            std::cout << "Flight number already exists. Try a different number.\n";
            continue;
        }
        break;
    }
    std::cout << "Enter origin: ";              std::cin >> orig;
    std::cout << "Enter destination: ";         std::cin >> dest;

    // Input flight status
    FlightStatus status;
    while (true) {
        std::cout << "Enter status (1: scheduled, 2: delayed, 3: canceled, 4: onTime): ";
        std::cin >> statusStr;

        if (statusStr == "1")   { status = FlightStatus::scheduled; break; }
        if (statusStr == "2")   { status = FlightStatus::delayed;   break; }
        if (statusStr == "3")   { status = FlightStatus::canceled; break; }
        if (statusStr == "4")   { status = FlightStatus::onTime; break; }

        std::cout << "Invalid flight status. Please try again.\n";
    }

    // Get and validate times with retry on invalid input
    timeType dep, arr;
    while (true) {
        std::cout << "Enter departure time (YYYY-MM-DD HH:MM): ";
        std::cin >> std::ws;
        std::getline(std::cin, depTimeStr);
        try {
            dep = parseDate(depTimeStr);
            break;
        } catch (...) {
            std::cout << "Invalid departure time format. Please try again.\n";
        }
    }

    while (true) {
        std::cout << "Enter arrival time (YYYY-MM-DD HH:MM): ";
        std::getline(std::cin, arrTimeStr);
        try {
            arr = parseDate(arrTimeStr);
            if (arr <= dep) {
                std::cout << "Arrival time must be after departure time. Please try again.\n";
                continue;
            }
            break;
        } catch (...) {
            std::cout << "Invalid arrival time format. Please try again.\n";
        }
    }

    // Ask for aircraft model until a valid one is provided
    std::shared_ptr<Aircraft> aircraftPtr = nullptr;
    while (!aircraftPtr) {
        std::cout << "Enter aircraft model (type 'list' to see available models): ";
        std::getline(std::cin, aircraftModel);

        if (aircraftModel == "list") {
            std::cout << "Available aircraft models:\n";
            for (const auto& craft : aircrafts) {
                if (craft->isAvailable()) std::cout << "- " << craft->getModel() << "\n";
            }
            continue;
        }

        for (const auto& craft : aircrafts) {
            if (craft && craft->getModel() == aircraftModel) {
                aircraftPtr = craft;
                break;
            }
        }

        if (!aircraftPtr) 
            std::cout << "Aircraft model not found. Please try again(type 'list' to see available models).\n";
    }

    auto newFlight = std::make_shared<Flight>(flightNum, orig, dest, status, aircraftPtr, dep, arr);
    flights.push_back(newFlight);

    nlohmann::json jArray = nlohmann::json::array();
    for (auto& f : flights) {
        jArray.push_back(f->getFlightJson());
    }
    std::ofstream outFile("database/Flights.json");
    if (!outFile.is_open())
        throw std::runtime_error("Could not open Flights.json for writing");
    outFile << jArray.dump(4);
    std::cout << "Flight successfully added.\n";
}

// ---------------- Remove a flight from the system ---------------- //
void FlightSystem::removeFlight() {
    int flightNum; 
    std::cout << "Enter flight number: ";
    std::cin >> flightNum;
    
    auto it = std::remove_if(flights.begin(), flights.end(),
        [flightNum](const std::shared_ptr<Flight>& f) {
            return f && f->getFlightNo() == flightNum;
        });
    if (it == flights.end()) {
        throw std::runtime_error("Flight number not found");
    }
    flights.erase(it, flights.end());
    std::cout << "Flight removed successfully.\n";

    nlohmann::json jArray = nlohmann::json::array();
    for (auto& flight : flights) {
        if (flight) {
            jArray.push_back(flight->getFlightJson());
        }
    }

    std::ofstream outFile("database/Flights.json");
    if (!outFile.is_open()) {
        throw std::runtime_error("Could not open Flights.json for writing");
    }
    outFile << jArray.dump(4);
}

// --------------- Update flight details --------------- //
std::shared_ptr<Flight> FlightSystem::updateFlight() {
    int flightNum;
    std::cout << "Enter flight number to update: ";
    std::cin >> flightNum;
    auto flight = getFlightByNumber(flightNum);
    if (!flight)
        throw std::runtime_error("Flight not found");

    std::cout << "Select update option:\n"
              << "1. Flight Details\n"
              << "2. Crew Assignments\n"
              << "3. Status\n"
              << "4. Back\n"
              << "Enter choice: ";

    int choice; std::cin >> choice;
    switch (choice) {
        case 1: getFlightByNumber(flightNum)->getFlightDetails(); break;
        case 2: selectCrew(flight); break;
        case 3: updateFlightStatus(flightNum); break;
        case 4: return nullptr; break;
    }
    return flight;
}

// ------- Search for flights -----------
void FlightSystem::searchFlight() const {
    std::string orig, dest, dateStr;
    std::cout << "Enter origin: "; std::cin >> orig;
    std::cout << "Enter destination: "; std::cin >> dest;
    std::cout << "Enter date (YYYY-MM-DD): "; std::cin >> dateStr;

    timeType dateFormatted;
    try {
        dateFormatted = parseDate(dateStr + " 00:00");
    } catch (...) {
        std::cout << "Invalid date format. Use YYYY-MM-DD.\n";
        return;
    }

    std::cout << "Available Flights:\n";
    int i = 0;
    for (const auto& flight : flights) {
        if (flight && flight->isFlightMatch(orig, dest, dateFormatted)) {
            std::cout << ++i << ". ";
            flight->getFlightDetails();
        }
    }
    if (i == 0) {
        std::cout << "No Flights available.\n";
    }
}

// ----------------- Get flight by number ------------------ //
std::shared_ptr<Flight> FlightSystem::getFlightByNumber(int flightNum) const {
    for (const auto& flight : flights) {
        if (flight->getFlightNo() == flightNum) {
            return flight;
        }
    }
    return nullptr;
}
