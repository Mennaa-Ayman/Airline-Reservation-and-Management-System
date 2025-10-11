#include "AirlineSystem.hpp"

// Constructor
AirlineSystem::AirlineSystem() 
    :userSystem(), flightSystem(), reservationSystem(flightSystem, userSystem) {
    }

// ------------------------------- Hash Password ------------------------------- //
std::string getPasswordInput() {
    std::string password;
    char ch;
    std::cout << "Enter password: ";
    while ((ch = _getch()) != '\r') { // Enter key
        if (ch == '\b') { // Backspace
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b";
            }
        } else {
            password += ch;
            std::cout << '*';
        }
    }
    std::cout << std::endl;
    return password;
}

// ==================================== LOGIN ======================================== //
int AirlineSystem::loginMenu() {
    std::cout << " -- Welcome to the Airline Reservation System --" << std::endl;
    std::cout << " ------ Login Page -------" << std::endl;

    std::string email, password;

    while (true) {
        std::cout << " Enter your email: ";
        if (!(std::cin >> email)) return 0;
        std::string password = getPasswordInput();

        if (userSystem.login(email, password)) {
            currentUser = userSystem.getCurrentUser(email); 
            if (!currentUser) return 0;

            Role r = currentUser->getRole();
            if (r == Role::admin) return 1;
            if (r == Role::agent) return 2;
            if (r == Role::passenger) return 3;
            return 0;
        }
        std::cout << " Wrong email or password. Please try again." << std::endl;
    }
}

// ============================================== ADMIN OPERATIONS ================================= //

// ----------- Admin Manage Flights ------------ //
void AirlineSystem::admin_ManageFligts() {
    while (true) {
        std::cout << "\n--- Manage Flights ---\n"
                  << "1. Add Flight\n"
                  << "2. Remove Flight\n"
                  << "3. Update existing Flight\n"
                  << "4. View All Flights\n"
                  << "5. Back to Admin Menu\n"
                  << "Enter your choice: ";
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: flightSystem.addFlight(); break;
            case 2: flightSystem.removeFlight(); break;
            case 3: flightSystem.updateFlight(); break;
            case 4: flightSystem.displayFlights(); break;
            case 5: return;
            default: std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

// ----------- Admin Manage Aircrafts ------------ //
void AirlineSystem::admin_ManageAircrafts() {
    while (true) {
        std::cout << "\n--- Manage Aircrafts ---\n"
                <<"1. Add New Aircraft\n"
                <<"2. Remove Aircraft\n"
                <<"3. Log Aircraft Maintenance\n"
                <<"4. Display all aircrafts\n"
                <<"5. back to Admin Menu\n"
                <<"Enter choice: ";
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: aircraftSystem.addAircraft(); break;
            case 2: aircraftSystem.removeAircraft(); break;
            case 3: aircraftSystem.logMaintenance(); break;
            case 4: aircraftSystem.displayAircrafts(); break;
            case 5: return;
            default: std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

// ----------- Admin Manage Users ------------ //
void AirlineSystem::admin_ManageUsers() {
    while (true) {
        std::cout << "\n--- Manage Users ---\n"
                  << "1. Add User\n"
                  << "2. Remove User\n"
                  << "3. Update User\n"
                  << "4. View All Users\n"
                  << "5. Back to Admin Menu\n"
                  << "Enter your choice: ";
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: userSystem.addUser(); break;
            case 2: userSystem.removeUser(); break;
            case 3: userSystem.updateUser(); break;
            case 4: userSystem.displayUsers(); break;
            case 5: return;
            default: std::cout << "Invalid choice. Please try again.\n";
        }
    }
}
// ----------- Admin Generate reports ------------ //
void AirlineSystem::admin_GenerateReports() {
    while(true){
        std::cout << "\n--- Generate Reports ---\n"
                  << "1. Operational Report\n"
                  << "2. Maintenance Report\n"
                  << "3. User Activity Report\n"
                  << "4. Back to Admin Menu\n"
                  << "Enter your choice: ";
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: logSystem.generateOperationalReport(); break;   //total flights, total reservations
            case 2: logSystem.generateMaintenanceReport(); break;   // aircraft status, maintenance schedules
            case 3: logSystem.generateUserActivityReport(); break;  // user flight bookings
            case 4: return;
            default: std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

// ========================== ADMIN LOOP ========================== //
void AirlineSystem::adminLoop() {
    int choice;
    do {
        std::cout << "\n--- Admin Menu ---\n"
                  << "1. Manage Flights\n"
                  << "2. Manage Aircrafts\n"
                  << "3. Manage Users\n"    
                  << "4. Generate Reports\n"
                  << "5. Logout\n"
                  << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: admin_ManageFligts(); break;
            case 2: admin_ManageAircrafts(); break;
            case 3: admin_ManageUsers(); break;
            case 4: admin_GenerateReports(); break;
            case 5: std::cout << "Logging out...\n"; break;
            default: std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    userLoop();
}

// ============================================== AGENT OPERATIONS ============================================== //

// ----------- Agent Book Flight for a passenger ------------ //
void AirlineSystem::agent_BookFlight() {
    reservationSystem.BookByAgent();
}

// ----------- Agent Remove Booking of a passenger ------------ //
void AirlineSystem::agent_RemoveBooking() {
    reservationSystem.removeBooking();
}

// ----------- Agent Modify Booking of a passenger ------------ //
void AirlineSystem::agent_ModifyBooking() {
    reservationSystem.modifyBooking();
}

// ========================== AGENT LOOP ========================== //
void AirlineSystem::agentLoop() {
    int choice;
    do {
        std::cout << "\n--- Booking Agent Menu ---\n"
                  << "1. Book Flight\n"
                  << "2. Remove Booking\n"
                  << "3. Modify Booking\n"
                  << "4. Logout\n"
                  << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: /* agent_BookFlight(); */ break;
            case 2: /* agent_RemoveBooking(); */ break;
            case 3: /* agent_ModifyBooking(); */ break;
            case 4: std::cout << "Logging out...\n"; break;
            default: std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);

    userLoop();
}

// ============================================== PASSENGER OPERATIONS ================================= //

// ---------- Passenger Search Flights by destination and date ---------- //
void AirlineSystem::passenger_SearchFlights() {
    flightSystem.searchFlight();
}

// ---------- Passenger Display their reservations ---------- //
void AirlineSystem::passenger_DisplayReservations() const {
    auto passenger = std::dynamic_pointer_cast<Passenger>(currentUser);
    reservationSystem.displayReservations(passenger->getId());
}

// ---------- Passenger Check-in for their flight ---------- //
void AirlineSystem::passenger_Checkin() {
    int reservationId;
    std::cout << "Enter your Reservation ID for check-in: ";
    std::cin >> reservationId;

    if (checkinSystem.airportCheckIn(reservationId)) {
        checkinSystem.generateBoardingPass(reservationId);
    }
}

// ---------- Passenger Cancel their reservation ---------- //
void AirlineSystem::passenger_CancelReservation() {
    auto passenger = std::dynamic_pointer_cast<Passenger>(currentUser);
    reservationSystem.cancelReservation(passenger->getId());
}

// ========================== PASSENGER LOOP ========================== //
void AirlineSystem::passengerLoop() {
    int choice;
    do {
        std::cout << "\n--- Passenger Menu ---\n"
                  << "1. Search Flights\n"
                  << "2. Display Reservations\n"
                  << "3. Check-in\n"
                  << "4. Cancel Reservation\n"
                  << "5. Logout\n"
                  << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: passenger_SearchFlights(); break;
            case 2: passenger_DisplayReservations(); break;
            case 3: passenger_Checkin(); break;
            case 4: passenger_CancelReservation(); break;
            case 5: std::cout << "Logging out...\n"; break;
            default: std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    userLoop();
}

// ============================================== ALL USERS LOOP ================================= //
void AirlineSystem::userLoop() {
    int role = loginMenu();
    switch (role) {
        case 1: adminLoop(); break;
        case 2: agentLoop(); break;
        case 3: passengerLoop(); break;
        default: break;
    }
}
