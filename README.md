# Airline Reservation and Management System

## Overview
A console-based C++ application simulating real-world airline operations. The system supports multiple user roles — Administrators, Booking Agents, and Passengers — each with dedicated, secure functionalities. Built with modern C++17 features and OOP principles, it emphasizes modularity, data integrity, and realistic airline workflows.

## Features
### 1. User Roles and Authentication
- Three distinct user roles: **Administrator**, **Booking Agent**, and **Passenger**
- Secure login system with role-based access control 
### 2. Flight Management (Admin)
- **Flight Scheduling** — Add, update, and remove flights with details including flight number, origin, destination, departure/arrival times, aircraft type, and operational status
- **Aircraft Management** — Manage fleet information such as aircraft specifications, maintenance schedules, and availability
- **Crew Assignment** — Assign pilots and flight attendants to specific flights with compliance checks against maximum flight hour regulations
### 3. Booking System 
- **Search Flights** — Passengers and agents can search flights by date and destination
- **Seat Selection** — Interactive seat maps for choosing preferred seats
- **Booking Management** — Create, modify, and cancel reservations; handle confirmations and waitlists
- **Payment Processing** — Simulated payment transactions with refund handling for cancellations
### 4. Passenger Management (Admin)
- **Passenger Profiles** — Store and manage contact details, preferences, and travel history
- **Loyalty Program** — Points-based loyalty system; earn points on bookings and redeem them for discounts
### 5. Check-In System 
- **Online Check-In** — Passengers can check in, select seats, and generate boarding passes
- **Airport Check-In** — Booking agents can assist passengers via simulated counter operations
### 6. Flight Operations 
- **Real-Time Status Updates** — Update flight statuses (On Time, Delayed, Canceled) and notify relevant users
- **Boarding Process** — Manage boarding procedures including boarding pass scanning and passenger identity verification
### 7. Maintenance Tracking
- **Scheduled Maintenance** — Track maintenance schedules per aircraft to ensure timely servicing and safety compliance
- **Maintenance Logs** — Detailed logs of work performed, parts replaced, and issues encountered
### 8. Reporting and Analytics
- **Operational Reports** — Flight performance, reservation statistics, and financial summaries
- **Maintenance Reports** — Aircraft utilization and maintenance activity insights
- **User Activity Reports** — Track user interactions and booking patterns

### 9. Data Persistence
All application data is stored in JSON files under the `database/` directory.
 
| File | Contents |
|------|----------|
| `Aircrafts.json` | Fleet details and maintenance schedules |
| `Crew.json` | Pilot and attendant records and assignments |
| `Flights.json` | Flight schedules and status information |
| `Reservations.json` | Booking records, seat assignments, and payment history |
| `Users.json` | User accounts, roles, profiles, and loyalty points |

## Project Structure:
```
AirlineSystem/
│
├── Makefile
│
├── database/
│   ├── Aircrafts.json
│   ├── Crew.json
│   ├── Flights.json
│   ├── Reservations.json
│   └── Users.json
│
├── include/
│   ├── Aircraft.hpp
│   ├── AirlineSystem.hpp
│   ├── Checkin.hpp
│   ├── Flight.hpp
│   ├── json.hpp
│   ├── reports.hpp
│   ├── Reservation.hpp
│   ├── Users.hpp
│   └── UserSystem.hpp
│
└── src/
    ├── Aircraft.cpp
    ├── AirlineSystem.cpp
    ├── Checkin.cpp
    ├── Flight.cpp
    ├── main.cpp
    ├── reports.cpp
    ├── Reservation.cpp
    ├── Users.cpp
    └── UserSystem.cpp
```

## Getting Started
 
### Requirements
- C++ 17
- GNU Make ≥ 4.4 
 
### Build
```bash
make build
```
 
### Run
```bash
make run
```
