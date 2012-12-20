
#include "two_axis_control_cmd.h"
#include <Servo.h>
#include "CalServo.h"
#include "Mover.h"
#include "Traveller.h"
#include "Scanner.h"

//////
// global variables.
//     This is an accepted practice in Arduino programming.
//////

CalServo servos[3];

Mover mover;
Traveller traveller;
Scanner scanner;

Mover* activeMover;

/**
 * This code runs at power on.
 */
void setup()
{
    mover.setServos((&servos[SERVO_X_INDEX]), (&servos[SERVO_Y_INDEX]));
    traveller.setServos((&servos[SERVO_X_INDEX]), (&servos[SERVO_Y_INDEX]));
    scanner.setServos((&servos[SERVO_X_INDEX]), (&servos[SERVO_Y_INDEX]));

    activeMover = &mover;
    Serial.begin(9600);
    randomSeed(analogRead(0));
    Serial.println("setup bottom");
}

/**
 * This code runs in a loop during runtime.
 */
void loop()
{
    checkAndHandleSerial();
    activeMover->step();
}

/**
 * If Serial is available, read from the stream,
 * parse, and act on the command.
 */
void checkAndHandleSerial()
{
    if (Serial.available() >=3 ) {

        Serial.println("serial is available");

        const byte cmd   = Serial.read();
        const byte index = Serial.read();
        const byte param = Serial.read();

        CalServo& servo = servos[index];

        switch (cmd)
        {
        case CMD_SET_MAX_POS:
            Serial.println("setMax");
            servo.setMax(param);
            break;
        case CMD_SET_MIN_POS:
            Serial.println("setMin");
            servo.setMin(param);
            break;
        case CMD_GO_TO_POS:
            Serial.println("write");
            servo.write(param);
            break;
        case CMD_GO_TO_RELATIVE:
            Serial.println("calibratedWrite");
            servo.calibratedWrite(param);
            break;
        case CMD_ADD_SERVO:
            Serial.println("attach");
            servo.attach(param);
            break;
        case CMD_BEGIN_TRAVEL:
            Serial.println("beginTravel");
            /*
             * Params:
             *  1 byte: the degree of the direction of travel, LSB
             *  1 byte: the degree of the direction of travel, MSB
             */
            traveller.init((param << 8) | index);
            activeMover = &traveller;
            break;
        case CMD_END_TRAVEL:
            Serial.println("endTravel");
            traveller.stop();
            activeMover = &mover;
            break;
        case CMD_SET_MAX_SPEED:
            Serial.println("setMaxSpeed");
            traveller.setMaxSpeed(param);
            scanner.setMaxSpeed(param);
            break;
        case CMD_SCAN_START:
            Serial.println("scanStart");
            scanner.init();
            activeMover = &scanner;
            break;
        case CMD_SCAN_STOP:
            Serial.println("scanStop");
            scanner.stop();
            activeMover = &mover;
            break;
        default:
            Serial.println("unknown command!");
            break;
        }
    }
}
