
/**
 * serail comm code borrowed from manorios at:
 *     http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1288819881
 */

#include <string>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <locale>

#include "../TwoAxisControl.h"

void _wait(int sec)
{
    time_t start = time(NULL);
    while (difftime(time(NULL), start) < sec) ;
}

int main (int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "usage: [serial port file path]." << std::endl;
        return -1;
    }

    std::cout << "init" << std::endl;

    TwoAxisControl tac;
    tac.init(argv[1], 11, 10);

    tac.setMaxPos(TwoAxisControl::Y_AXIS, 140);
    tac.setMinPos(TwoAxisControl::Y_AXIS, 40);

    std::locale loc;
    std::string in;
    while (1) {

        std::cin >> in;

        if (in == "r") {
            tac.travelStop();
            tac.goToAbsolutePos(TwoAxisControl::X_AXIS, 90);
            tac.goToAbsolutePos(TwoAxisControl::Y_AXIS, 90);
        } else if (in == "return") {
            tac.goToAbsolutePos(TwoAxisControl::X_AXIS, 90);
            tac.goToAbsolutePos(TwoAxisControl::Y_AXIS, 90);
        } else if (in == "stop") {
            tac.travelStop();
        } else if (in == "quit") {
            return 0;
        } else if (std::isdigit(in.at(0), loc)) {
            int i = 0;
            i = atoi(in.c_str());
            if (!i) std::cerr << "warning, malformed input" << std::endl;
            tac.travelStart(static_cast<double>(i));
        } else {
            std::cerr << "please input a number or command." << std::endl;
        }
    }

    return 0;
}

