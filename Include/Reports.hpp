#ifndef REPORTS_HPP
#define REPORTS_HPP

#include <string>

class Reports {
public:
    void generateOperationalReport() const;
    void generateMaintenanceReport() const;
    void generateUserActivityReport() const;
};

#endif