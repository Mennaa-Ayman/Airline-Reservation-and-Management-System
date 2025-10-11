#ifndef CHECKIN_HPP
#define CHECKIN_HPP

#include <iostream>
#include <fstream>
#include <string>

#include "Reservation.hpp"
#include "json.hpp"

class CheckinSystem {
public:

    // Airport check-in process
    bool airportCheckIn(int reservationId);

    // Generate boarding pass
    void generateBoardingPass(int reservationId) const;

    // Display check-in status
    void displayCheckinStatus(int reservationId) const;
};

#endif