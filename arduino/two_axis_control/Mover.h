/**
 * Mover.h
 */

#ifndef _CS309_A4_MOVER_H_
#define _CS309_A4_MOVER_H_

// fwd declare
class CalServo;

/**
 * A class for moving the servos.
 * This implementation does nothing,
 * it is ment to be inherited.
 * @see Traveller
 * @see Scanner
 */
class Mover
{

public:

    Mover();

    void step() { doStep(); }

    virtual void init() { /* do nothing */ }

    virtual void stop() { /* do nothing */ }

    void setServos(CalServo* x, CalServo* y);

    void setMaxSpeed(byte maxSpeed);

protected:

    class coord_t {
    public:
        coord_t() : x(-1), y(-1) { }
        coord_t(int x_, int y_) : x(x_), y(y_) { }
        bool operator !() const { return !(x == -1 || y == -1); }
        bool hasX() const { return x != -1; }
        bool hasY() const { return y != -1; }
        int quadrant() const; 
        bool closeTo(const coord_t&, int tol) const;

        int x;
        int y;
    };

    void doStep();

    virtual coord_t getNextPos() { return coord_t(); }

    virtual long getDelayTime() { return 0; }

    byte maxSpeed_;

    CalServo* servoX_;
    CalServo* servoY_;

};

inline Mover::Mover() : maxSpeed_(100) { /* do nothing */ }

inline void Mover::doStep()
{
    const coord_t coord = this->getNextPos();

    if (coord.hasX())
        servoX_->calibratedWrite(coord.x);

    if (coord.hasY())
        servoY_->calibratedWrite(coord.y);

    delay(getDelayTime()); 
}

inline void Mover::setServos(CalServo* x, CalServo* y)
{
    servoX_ = x;
    servoY_ = y;
}

inline void Mover::setMaxSpeed(byte maxSpeed)
{
    maxSpeed_ = maxSpeed;
}

/**
 * 0,0_________
 * |     |     |
 * |  2  |  1  |
 * |-----------|
 * |  3  |  4  |
 * |_____|_____| 100,100
 */
inline int Mover::coord_t::quadrant() const
{
    if (!(*this))
        return 0; 

    if (x >= 50 && y < 50)
        return 1; 
    else if (x < 50 && y < 50)
        return 2; 
    else if (x < 50 && y >= 50)
        return 3; 
    else if (x >= 50 && y >= 50)
        return 4; 

    /* unreachable */
    return 0; 
}

inline bool Mover::coord_t::closeTo(const coord_t& rhs, int tol) const
{
    return abs(rhs.x - x) <= tol && abs(rhs.y - y) <= tol; 
}

#endif // _CS309_A4_MOVER_H_
