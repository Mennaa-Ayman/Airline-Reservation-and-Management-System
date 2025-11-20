# Airline Reservation and Management System ✈️

## Overview

The Airline Reservation and Management System is a comprehensive console-based C++ application simulating real-world airline operations.
The system supports multiple user roles—Administrators, Booking Agents, and Passengers—each with dedicated, secure functionalities.
Built with modern C++ features and OOP principles, it emphasizes modularity, data integrity, and realistic airline workflows.

## Features

**1. User Roles and Authentication:**

- Implementing multiple user roles including Administrators, Booking Agents, and Passengers.
- Secure login system with role-based access control.

**2. Flight Management:**

- Flight Scheduling: Add, update, and remove flights with details like flight number, origin, destination, departure and arrival times, aircraft type, and status.
- Aircraft Management: Manage fleet information, including aircraft specifications, maintenance schedules, and availability.
- Crew Assignment: Assign pilots and flight attendants to specific flights, ensuring compliance with maximum flight hours regulations.

**3. Booking System:**

- Search Flights: Allow passengers and booking agents to search for flights based on criteria (date & destination).
- Seat Selection: Implement seat maps for flights, enabling passengers to select preferred seats.
- Booking Management: Create, modify, and cancel reservations. Handle booking confirmations and waitlists.
- Payment Processing: Simulate payment transactions for bookings, including handling refunds for cancellations.

**4. Passenge Management:**

- Passenger Profiles: Store and manage passenger information, including contact details, preferences, and travel history.
- Loyalty Programs: Implement a basic loyalty system where passengers earn points for bookings and can redeem them for discounts.

**5. Check-In System:**

- Online Check-In: Allow passengers to check in online, select seats, and obtain boarding passes.
- Airport Check-In: Simulate airport check-in counter operations for booking agents to assist passengers.

**6. Flight Operations:**

- Real-Time Updates: Update flight statuses (e.g., on time, delayed, canceled) and notify relevant users.
- Boarding Process: Manage boarding procedures, including scanning boarding passes and verifying passenger identities.

**7. Maintenance Tracking:**

- Scheduled Maintenance: Track maintenance schedules for each aircraft, ensuring timely servicing and compliance with safety regulations.
- Maintenance Logs: Maintain detailed logs of maintenance performed, parts replaced, and any issues encountered.

**8. Reporting and Analytics:**

- Operational Reports: Generate reports on flight performance, reservation statistics, and financial summaries.
- Maintenance Reports: Provide insights into maintenance activities and aircraft utilization.
- User Activity Reports: Track user interactions and booking patterns for analysis.

**9. Data Persistence:**

- Database Integration: Use file-based databases (e.g., JSON, CSV)

## Project Structure:

```
│  MakeFile
│
├───database
    ├── Aircrafts.json
    ├── Crew.json
    ├── Flights.json
    ├── Reservations.json
    ├── Users.json
│
└── include/
    ├── Aircraft.hpp
    ├── AirlineSystem.hpp
    ├── Checkin.hpp
    ├── Flight.hpp
    ├── json.hpp
    ├── reports.hpp
    ├── Reservation.hpp
    ├── Users.hpp
    ├── UserSystem.hpp
│
└───src/
    ├── Aircraft.cpp
    ├── AirlineSystem.cpp
    ├── Checkin.cpp
    ├── Flight.cpp
    ├── main.cpp
    ├── reports.cpp
    ├── Reservation.cpp
    ├── Users.cpp
    ├── UserSystem.cpp

```

## Build

```
make build
make run

```

## Requirements

- C++ 17
- GNU Make 4.4
