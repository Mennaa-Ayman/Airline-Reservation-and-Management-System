#ifndef FLIGHT_HPP
#define FLIGHT_HPP
#include <map>
#include <iostream>
#include <memory>
#include <chrono>
#include <vector>
#include "Aircraft.hpp"
#include "Reservation.hpp"
#include "User.hpp"


// class Passenger;

enum class FlightStatus {scheduled, delayed, canceled, onTime};

// =====================================   SeatMap Class   ===================================== //

class SeatMap {
private:
    std::map<int, bool> seatsAvailability;
    const int totalSeats; 

public:
    SeatMap(int seatNum);
    bool bookSeat(int num);
    bool unbookSeat(int num);
    int seatsCount();
    void resetMap();
};

// =====================================   Flight Class   ===================================== //

class Flight {
private:
    int flightNumber;
    std::string origin;
    std::string destination;
    FlightStatus status;

    std::shared_ptr<Aircraft> aircraft;
    

    using timeType = std::chrono::system_clock::time_point;
    timeType departureTime;
    timeType arrivalTime;

    std::vector<std::shared_ptr<Crew>> flightCrewMembers;
    SeatMap seats;
public:
    Flight(int flightNum=0, const std::string& orig="", const std::string& dest="", FlightStatus s=FlightStatus::scheduled,
        std::shared_ptr<Aircraft> aircraft=nullptr, const timeType& depTime = timeType(), const timeType& arrTime = timeType());

    int getFlightHours() const;  

    // -- Assign crew and aircraft to the flight : 
    void setCrew(std::shared_ptr<Crew> crew);
    void setAircraft(std::shared_ptr<Aircraft> craft);
    
    bool isFlightFull();
    int getFlightNo(){
        return flightNumber;
    }
    std::string getOrigin(){
        return origin;
    }
    std::string getDestination(){
        return destination;
    }
    timeType getDepartureTime() {
        return departureTime;
    }

    bool isFlightMatch(const std::string& orgi, const std::string& desti, const timeType& time) const;
    void changeStatus(FlightStatus newStatus);
    std::string getFlightDetails() const;
    nlohmann::json getFlightJson() const;
};

// =====================================   FlightSystem Class   ===================================== //

class FlightSystem{
private: 
    std::vector<std::shared_ptr<Flight>> flights;
    std::vector<std::shared_ptr<Crew>> crewMembers;
    std::vector<std::shared_ptr<Aircraft>> aircrafts;
    std::fstream flightsfile;
    std::fstream crewfile;
    FlightStatus status;
    using timeType = std::chrono::system_clock::time_point;


public: 
    FlightSystem();
    ~FlightSystem();
    void displayFlights() const;
    int selectFlight();
    void addFlight();
    void removeFlight();
    std::shared_ptr<Flight> getFlightByNumber(int flightNum) const;

    void selectCrew(std::shared_ptr<Flight> myflight);
    void updateFlightStatus(int num);

    std::shared_ptr<Flight> updateFlight();
    void searchFlight() const;
    
    // std::shared_ptr<Reservation> bookFlight(const std::shared_ptr<Passenger>& p,bool agent = false);
};


#endif