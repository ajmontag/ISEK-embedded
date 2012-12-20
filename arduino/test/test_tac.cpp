typedef unsigned char byte;
#define DELTA_MAX 100
#define M_PI 3.14159623

#include <cmath>
#include <cerrno>
#include <iostream>

class Traveller
{

public:

    Traveller();

    /**
     * 50 is the middle of the calibrated area.
     * 0 is left, 100 is right
     */
    void init(short degree);

//protected:

    byte maxSpeed_;

    byte speedX_;
    byte speedY_;

    bool xNeg_;
    bool yNeg_;

    int deltaX_;
    int deltaY_;

};

inline Traveller::Traveller() : maxSpeed_(100), speedX_(0), speedY_(0), deltaX_(0), deltaY_(0) { /* do nothing */ }

inline void Traveller::init(short degree)
{
    // initialize travel params

    double rad = 0;

    if (degree < 90) {
        std::cout << "(degree < 90)" << std::endl;
        rad = degree * (M_PI / 180.0);
        xNeg_ = false;
        yNeg_ = false;
    } else if (degree < 180) {
        std::cout << "(degree < 180)" << std::endl;
        // quadrant II
        rad = (180.0 - degree) * (M_PI / 180.0);
        xNeg_ = true;
        yNeg_ = false;
    } else if (degree < 270) {
        std::cout << "(degree < 270)" << std::endl;
        // quadrant III
        rad = (degree - 180.0) * (M_PI / 180.0);
        xNeg_ = true;
        yNeg_ = true;
    } else {
        std::cout << "(degree < 360)" << std::endl;
        // quadrant IV
        rad = (360.0 - degree) * (M_PI / 180.0);
        xNeg_ = false;
        yNeg_ = true;
    }

    deltaY_ = sin(rad)*maxSpeed_;
    deltaX_ = cos(rad)*maxSpeed_;

    if (errno == EDOM) {
        std::cerr << "Dominan Error!" << std::endl;
    }

    if (xNeg_) {
        deltaX_ *= -1;
    }

    if (yNeg_) {
        deltaY_ *= -1;
    }
}

int main ()
{

    Traveller traveller;

    for (double d = 0.0; d <= 360; d += 10.0) {
        std::cout << "\ndegree = " << d << std::endl;
        traveller.init(d);
        std::cout << "\tx = " << traveller.deltaX_ << "\ty = " << traveller.deltaY_ << std::endl;
    }

    return 0;
}
