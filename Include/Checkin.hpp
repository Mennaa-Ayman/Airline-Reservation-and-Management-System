#ifndef CHECKIN_HPP
#define CHECKIN_HPP

#include <iostream>
#include <fstream>
#include <string>

#include "Reservation.hpp"
#include "json.hpp"
#include "Flight.hpp"
#include "User.hpp"

class CheckinSystem {
private:
    ReservationSystem& reservationSystem;
public:

    CheckinSystem(ReservationSystem& rs) : reservationSystem(rs) {}
    ~CheckinSystem();

    // Airport check-in process
    bool airportCheckIn(int reservationId);

    // Generate boarding pass
    void generateBoardingPass(int reservationId) const;

    // Display check-in status
    void displayCheckinStatus(int reservationId) const;
};

#endif