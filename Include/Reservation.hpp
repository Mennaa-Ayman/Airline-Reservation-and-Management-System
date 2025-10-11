#ifndef RESERVATION_HPP
#define RESERVATION_HPP

#include <iostream>
#include <memory>
#include <fstream>
#include <optional>
#include <vector>
#include <string>
#include <utility>
#include "json.hpp"

class UserSystem;
class FlightSystem;
class Passenger;
class Flight;

// ============================ Payment Class ====================== //
class Payment{
friend class Reservation; 
friend class ReservationSystem;

private:
    std::string method;
    std::string details;
    int amount;

public: 
    Payment(std::string& m, std::string& d, int a)
    : method(m), details(d), amount(a) {}

};

// ============================ Reservation Class ====================== //
class Reservation{
    friend class ReservationSystem;
private:
    static int reservationCount;
    int reservationId; 

    std::shared_ptr<Passenger> passenger;
    std::shared_ptr<Flight> flight;
    Payment payment;
    int seatNum;

public:

    Reservation(int reservationId, const std::shared_ptr<Passenger>& p, const std::shared_ptr<Flight>& f,
        int s, std::string& method, std::string& details, int amount);


    void modifyReservation(int resid, int s) ;
    void confirmReservation() const;
    void cancelReservation();
    void displayReservation() const;
    nlohmann::json getReservationJson() const;

    // -------- Getters and Setters --------- //
    int getReservationId(){
        return this->reservationId;
    }
    void setReservationId(int r){
        this->reservationId = r;
    }
    int getCost() const{
        return payment.amount;
    }
};

// ============================ Reservation System class ====================== //
class ReservationSystem{
    private: 
    std::vector<std::shared_ptr<Passenger>> passengers;
    std::vector<std::shared_ptr<Flight>> flights;
    std::vector<std::shared_ptr<Reservation>> reservations;

    std::fstream reservationsFile;

    FlightSystem &flightSystem;
    UserSystem &userSystem;

    public:

    ReservationSystem(FlightSystem& fs, UserSystem& us);
    ~ReservationSystem() = default;

    std::shared_ptr<Reservation> getReservationById(int resId);

    void displayReservations(int p_id) const;
    void addReservation(const std::shared_ptr<Reservation>& r);
    void cancelReservation(int passengerId);
    std::optional<std::pair<std::string, std::string>> checkReservation(const int& p_id, const int& r_id);

    void BookByAgent();
    void removeBooking();
    void modifyBooking();
    
};

#endif