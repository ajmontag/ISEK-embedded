/**
 * Scanner.h
 */

#ifndef _CS309_A4_SCANNER_H_
#define _CS309_A4_SCANNER_H_

// fwd declare
class CalServo;

/**
 * A class for moving the servos.
 * This implementation does nothing,
 * it is ment to be inherited.
 * @see Traveller
 * @see Scanner
 */
class Scanner : public Mover
{

public:

    Scanner();

    virtual void init();

    virtual void stop();

protected:

    static const int MAX_MOVE = 3;

    virtual coord_t getNextPos();

    virtual long getDelayTime();

    byte maxSpeed_;

    Mover::coord_t pos_; 
    Mover::coord_t dest_;
    int pauseOnDestFrames_; 
    void findNewDest();
    static coord_t randomCoordInQuadrant(int q); 
};

inline Scanner::Scanner() : maxSpeed_(100), pauseOnDestFrames_(80) { /* do nothing */ }

void Scanner::stop()
{
    /* nothing to do here */
}

void Scanner::init()
{
    Serial.println("Scanner init");
    pos_ = coord_t(servoX_->calibratedRead(), servoY_->calibratedRead());
    findNewDest();
}

inline Mover::coord_t Scanner::getNextPos()
{
    if (pos_.closeTo(dest_, MAX_MOVE * 2)) {
        --pauseOnDestFrames_;
        if (pauseOnDestFrames_) {
            //Serial.println("waiting on dest"); 
            return Mover::coord_t(); // sit still and hang out 
        }
        pauseOnDestFrames_ = random(45, 120);
        Serial.print("pauseOnDestFrames = "); 
        Serial.println(pauseOnDestFrames_, DEC);  
        findNewDest(); 
    }

    coord_t toMove; 

    const int xDiff = dest_.x - pos_.x; 
    const int xDelta = abs(xDiff); 
    const int xSign = (xDiff < 0) ? -1 : 1; 
    const int yDiff = dest_.y - pos_.y;
    const int yDelta = abs(yDiff); 
    const int ySign = (yDiff < 0) ? -1 : 1; 

    if (xDelta > yDelta) {
        toMove.x = MAX_MOVE;
        toMove.y = (yDiff * MAX_MOVE) / xDiff; 
    } else {
        toMove.y = MAX_MOVE;
        toMove.x = (xDiff * MAX_MOVE) / yDiff; 
    } 

    const coord_t ret(pos_.x + toMove.x * xSign, pos_.y + toMove.y * ySign); 
    pos_ = ret; 

    Serial.print("Scanner next pos = (");
    Serial.print(ret.x, DEC);
    Serial.print(", ");
    Serial.print(ret.y, DEC); 
    Serial.println(")");

    return ret; 
}

inline long Scanner::getDelayTime()
{
    // determines the speed at which the robotic head moves
    // this value seems to work well 
    return 12;
}

inline void Scanner::findNewDest()
{
    const int nextQuad = random(1, 5);

    Serial.print("pos = ("); 
    Serial.print(pos_.x, DEC); 
    Serial.print(", "); 
    Serial.print(pos_.y, DEC); 
    Serial.print(") quad = "); 
    Serial.print(nextQuad, DEC); 
    dest_ = randomCoordInQuadrant(nextQuad);
    Serial.print(" New Dest = ("); 
    Serial.print(dest_.x, DEC); 
    Serial.print(", "); 
    Serial.print(dest_.y, DEC); 
    Serial.println(")"); 
}

inline Mover::coord_t Scanner::randomCoordInQuadrant(int q)
{
    // we add/sub from the center as needed per quadrant
    int deltaX = random(20, 49); 
    int deltaY = random(20, 49); 
    coord_t ret(50, 50); 
    switch (q) {
    case 1:
        ret.x += deltaX; 
        ret.y -= deltaY;
        break;
    case 2:
        ret.x -= deltaX; 
        ret.y -= deltaY;
        break;
    case 3:
        ret.x -= deltaX; 
        ret.y += deltaY;
        break;
    case 4:
        ret.x += deltaX; 
        ret.y += deltaY;
        break;
    }

    return ret; 
}

#endif // _CS309_A4_SCANNER_H_
