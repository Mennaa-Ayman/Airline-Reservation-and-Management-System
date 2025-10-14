#ifndef AIRLINESYSTEM_HPP
#define AIRLINESYSTEM_HPP

#include <iostream> 
#include <memory>
#include "User.hpp"
#include "UserSystem.hpp"
#include "Flight.hpp"
#include "Aircraft.hpp"
#include "Reservation.hpp"
#include "UserSystem.hpp"
#include "Checkin.hpp"
#include "Reports.hpp"

class AirlineSystem {
private:
    UserSystem userSystem;
    FlightSystem flightSystem;
    AircraftsSystem aircraftSystem;
    ReservationSystem reservationSystem;
    CheckinSystem checkinSystem;
    
    std::shared_ptr<User> currentUser=nullptr;


    std::vector<std::shared_ptr<Reservation>> reservations;
    Reports logSystem;

public:
    AirlineSystem();
    int loginMenu();
    ~AirlineSystem();

//==================== Admin Operations ===================== //
    void adminLoop();
    void admin_ManageFligts();
    void admin_ManageAircrafts();
    void admin_ManageUsers();
    void admin_GenerateReports();

// ================= Passenger Operations ================= //
    void passengerLoop();
    void passenger_SearchFlights();
    void passenger_DisplayReservations() const;
    void passenger_Checkin();
    void passenger_CancelReservation();

// ===================== Agent Operations ======================= //
    void agentLoop();
    void agent_BookFlight();
    void agent_RemoveBooking();
    void agent_ModifyBooking();

public:
    void userLoop();
    
};

#endif