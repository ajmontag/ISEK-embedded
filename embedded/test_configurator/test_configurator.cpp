#include "../configurator.h"
#include "../TwoAxisControl.h"
#include <iostream>

int main(int argc, char** argv)
{
    TwoAxisControl tac;
	Configurator config("configFile.xml", tac);


    config.saveLimits(1, 2, 3, 4);

	std::cout <<  config.get_invertX() << std::endl;
    std::cout << config.get_invertY() << std::endl;
    std::cout << config.get_imageWidth() << std::endl;
    std::cout << config.get_imageHeight() << std::endl;
    std::cout << config.get_tolerance() << std::endl;
    std::cout << config.get_prevAngle() << std::endl;
    std::cout << config.get_showGUI() << std::endl;
    std::cout << config.get_deadzone() << std::endl;
    std::cout << config.get_maxY() << std::endl;
    std::cout << config.get_minY() << std::endl;
    std::cout << config.get_maxX() << std::endl;
    std::cout << config.get_minX() << std::endl;
    std::cout << config.get_servo2() << std::endl;
    std::cout << config.get_servo1() << std::endl;

}