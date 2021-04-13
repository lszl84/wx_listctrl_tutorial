#pragma once
#include <string>
#include <istream>
#include <sstream>

struct ItemData
{
    std::string date;
    double low, high, open, close, volume;
    int ivr;

    // Simple and not error-proof. Do not use in production.
    static ItemData fromCsvLine(std::istream &stream)
    {
        constexpr int count = 7;

        std::string line;
        std::stringstream lineStream;

        if (std::getline(stream, line))
        {
            std::string cells[count];
            lineStream << line;

            for (int i = 0; i < count && std::getline(lineStream, cells[i], ','); i++)
                ;

            auto doubleOrZero = [](const std::string &str) { return str.empty() ? 0.0 : std::stod(str); };
            auto intOrZero = [](const std::string &str) { return str.empty() ? 0 : std::stoi(str); };

            ItemData result{
                cells[0],
                doubleOrZero(cells[1]),
                doubleOrZero(cells[2]),
                doubleOrZero(cells[3]),
                doubleOrZero(cells[4]),
                doubleOrZero(cells[5]),
                intOrZero(cells[6])};

            return result;
        }
        else
        {
            return {};
        }
    }
};
