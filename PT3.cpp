#include <FEHLCD.h>
#include <FEHUtility.h>
#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHServo.h>
#include <FEHRPS.h>
#include <FEHSD.h>

const double driveVoltage = 9.0;
const double turn180Counts = 450; //Set this after checking theoretical counts for 180 degrees
const double rightPercent = 25.09;
const double leftPercent = 25.0;
const double turnPercent = 20;
const double turnConstant = 0.14;
const double countsPerInch = 40.5; //Input theoretical counts here
const int initialDegree1 = 130; //GOOD
const int initialDegree2 = 4; //GOOD
const int halfDegree2 = 180;
const int endDegree1 = 34; //GOOD
const int creamEndDegree1 = 70; //GOOD
const int endDegree2 = 158; //GOOD
const int burgerInitialDegree2 = 16; //FIRE
const int burgerFinalDegree2 = 120; //ITS SO GOOD

//Initializing all motors and electronics, such as CdS cell, bump switches, etc.
FEHMotor rightDrive(FEHMotor::Motor0, driveVoltage);  //Right motor
FEHMotor leftDrive(FEHMotor::Motor3, driveVoltage);   //Left motor
AnalogInputPin cell(FEHIO::P3_7);                     //CdS Initialization
DigitalInputPin frontLeftBump(FEHIO::P1_7);           //Front left bump switch
DigitalInputPin frontRightBump(FEHIO::P2_1);          //Front right bump switch
DigitalEncoder rightEncoder(FEHIO::P1_0);             //Right shaft encoder
DigitalEncoder leftEncoder(FEHIO::P0_0);              //Left Shaft encoder
FEHServo servo1(FEHServo::Servo7);                    //Tray arm servo
FEHServo servo2(FEHServo::Servo0);                    //Rack and Pinion servo

//This function initializes the arms and sets up RPS
void setupBot(){

    servo1.SetMin(500);
    servo2.SetMin(500);

    servo1.SetMax(2365);
    servo2.SetMax(2375);

    servo1.SetDegree(initialDegree1);
    servo2.SetDegree(initialDegree2);

    RPS.InitializeTouchMenu();

}

//This function waits for the startlight before activating the robot
void startLight(){

    while(cell.Value() > 0.8){}

    LCD.WriteLine("Light Detected. Drew is a legend.");

}

//This function turns both engines on, bringing the robot forward
void goForward(double length) //using encoders
{
    //Reset encoder counts
    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();

    //Set both motors to desired percent
    rightDrive.SetPercent(rightPercent);
    leftDrive.SetPercent(leftPercent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((leftEncoder.Counts() + rightEncoder.Counts()) / 2.0 < (length*countsPerInch));

    //Turn off motors
    rightDrive.Stop();
    leftDrive.Stop();

    Sleep(0.1);

}

//This function turns the engines on at a stronger percent to drive up the ramp.
void rampDrive(double length) //using encoders
{
    //Reset encoder counts
    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();

    //Set both motors to desired percent
    //rightDrive.SetPercent(rightPercent+0.2);
    rightDrive.SetPercent((rightPercent*2));
    leftDrive.SetPercent((leftPercent*2)+3);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((leftEncoder.Counts() + rightEncoder.Counts()) / 2.0 < (length*countsPerInch));

    //Turn off motors
    rightDrive.Stop();
    leftDrive.Stop();

    Sleep(0.1);

}

//This function turns the motors on in reverse to drive backwards
void goBackward(double length) //using encoders
{
    //Reset encoder counts
    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();

    //Set both motors to desired percent
    rightDrive.SetPercent(-rightPercent);
    leftDrive.SetPercent(-leftPercent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((leftEncoder.Counts() + rightEncoder.Counts()) / 2.0 < (length*countsPerInch));

    //Turn off motors
    rightDrive.Stop();
    leftDrive.Stop();

    Sleep(0.1);

}

//This function turns the robot right for a specified amount of degrees
void turnRight(int angle){

    //Reset encoder counts
    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();

    //Set both motors to desired percent
    rightDrive.SetPercent(-(rightPercent+turnConstant));
    leftDrive.SetPercent(leftPercent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((leftEncoder.Counts() + rightEncoder.Counts()) / 2.0 < (turn180Counts*angle/180.0));

    //Turn off motors
    rightDrive.Stop();
    leftDrive.Stop();

    Sleep(0.1);

}

//This function turns the robot left for a specified amount of degrees
void turnLeft(int angle){

    //Reset encoder counts
    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();

    //Set both motors to desired percent
    rightDrive.SetPercent((rightPercent+turnConstant));
    leftDrive.SetPercent(-leftPercent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((leftEncoder.Counts() + rightEncoder.Counts()) / 2.0 < (turn180Counts*angle/180.0));

    //Turn off motors
    rightDrive.Stop();
    leftDrive.Stop();

    Sleep(0.1);

}

//This function is used for the jukebox, as it determines whether the light on the ground is blue, red, or not detected
char colorChecker(char color){


    Sleep(0.1);

    if(cell.Value() < 0.6){

        color = 'r';
        LCD.WriteLine("Red Found");

    } else if (cell.Value() < 1.6 && cell.Value() > 1.1){

        color = 'b';
        LCD.WriteLine("Blue Found");

    }

    return color;

}

//This function swerves forward to cover more ground while looking for the jukebox light on the ground
char lightSwerve(){

    char color = 'n';
    int counter = 0;

    while(color == 'n'){

        color = colorChecker(color);

        if(color == 'n'){

            turnLeft(30);
            counter++;
            LCD.WriteLine("Counter: ");
            LCD.WriteLine(counter);
            color = colorChecker(color);

        }

        if(color == 'n'){

            goForward(0.5);
            color = colorChecker(color);

        }

        if(color == 'n'){

            turnRight(30);
            counter++;
            LCD.WriteLine("Counter: ");
            LCD.WriteLine(counter);
            color = colorChecker(color);

        }

        if(color == 'n'){

            goForward(0.5);
            color = colorChecker(color);

        }

        if(color == 'n'){

            turnRight(30);
            counter++;
            LCD.WriteLine("Counter: ");
            LCD.WriteLine(counter);
            color = colorChecker(color);

         }

        if(color == 'n'){

            goForward(0.5);
            color = colorChecker(color);

        }

        if(color == 'n'){

            turnLeft(30);
            counter++;
            LCD.WriteLine("Counter: ");
            LCD.WriteLine(counter);
            color = colorChecker(color);

        }

    }

    //counter % 4 == 0 || 2 - STAY STRAIGHT
    //counter % 4 == 1 - TURN 30 DEG RIGHT
    //counter % 4 == 3 - TURN 30 LEFT

    if((counter % 4 == 1)){

        turnRight(30);

    } else if (counter % 4 == 3){

        turnLeft(30);

    }

    return color;

}

//This function drives to a certain button depending on the color identified.
void colorPath(char color){

    if(color == 'b'){

        goBackward(7.75);
        turnLeft(90);
        goForward(8);

    } else if(color == 'r') {

        goBackward(5);
        turnLeft(90);
        goForward(8);

    }

}

//This function uses the bump switches to push against a wall and line up
void wallPush(){

    //Set both motors to desired percent
    rightDrive.SetPercent(rightPercent);
    leftDrive.SetPercent(leftPercent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while(frontLeftBump.Value() || frontRightBump.Value());

    goBackward(5);

    //Turn off motors
    rightDrive.Stop();
    leftDrive.Stop();

    Sleep(0.1);

}

//This function is used to press against the wall with bumpswitches until just 1 bumpswitch is pressed.
//It is used when the wall does not extend long enough to use bumpswitches, like for the sink at the top of the ramp.
void wallPushEither(int time){

    //Set both motors to desired percent
    rightDrive.SetPercent(rightPercent);
    leftDrive.SetPercent(leftPercent);

    float initialTime = TimeNow();

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    //TimeNow() checks whether the robot has tried to press for too long.
    while(frontLeftBump.Value() && frontRightBump.Value() && (TimeNow() - initialTime < time));

    goBackward(5);

    //Turn off motors
    rightDrive.Stop();
    leftDrive.Stop();

    Sleep(0.1);

}

//This function activates the tray arm to release the tray
void dropTray(){

    servo1.SetDegree(endDegree1);
    Sleep(2.0);
    servo1.SetDegree(initialDegree1);

}

//This function shimmies the robot to get the arm into the ticket slot
//Try: Right wheel right turn only, drive a bit, both wheel left turn, drive back a bit
void shimmy(int time){

    float initialTime = TimeNow();

    while(TimeNow() - initialTime < time){

        rightDrive.SetPercent(0);
        leftDrive.SetPercent(-10);

        Sleep(0.3);

        rightDrive.SetPercent(-10);
        leftDrive.SetPercent(10);

        Sleep(0.3);

    }

}

//This function used to be used for the ticket slider. Unfortunately, because we
//didn't get to do the final competition, we were going to repair this method but didn't get the chance.
//void ticketArm(){

//    servo2.SetDegree(halfDegree2);

//    Sleep(1.0);

//    servo2.SetDegree(endDegree2);

//    Sleep(1.0);

//    shimmy(10);

//    turnLeft(4);

//    leftPowerForward(7);

//    Sleep(3.0);

//    servo2.SetDegree(initialDegree2);

//}

//This function activates the rack and pinion to flip the burger
void flipBurger(){

    servo2.SetDegree(burgerInitialDegree2);
    Sleep(2.0);
    servo2.SetDegree(burgerFinalDegree2);
    Sleep(2.0);
    servo2.SetDegree(burgerInitialDegree2);
    Sleep(2.0);

}

//This function uses RPS to line up with the given x coordinate
void xFrontRPS(float x_coordinate) //using RPS while robot is in the +y direction
{

    Sleep(0.5);

    //check if receiving proper RPS coordinates and whether the robot is within an acceptable range
    while(RPS.X() != -1 && RPS.X() != -2 && (RPS.X() < x_coordinate - 1 || RPS.X() > x_coordinate + 1))
    {
        if(RPS.X() > x_coordinate)
        {
            leftDrive.SetPercent(-leftPercent);
            rightDrive.SetPercent(-rightPercent);

            Sleep(0.1);

            leftDrive.SetPercent(0);
            rightDrive.SetPercent(0);

            Sleep(0.1);

        }
        else if(RPS.X() < x_coordinate)
        {

            leftDrive.SetPercent(leftPercent);
            rightDrive.SetPercent(rightPercent);

            Sleep(0.1);

            leftDrive.SetPercent(0);
            rightDrive.SetPercent(0);

            Sleep(0.1);

        }
    }

    LCD.WriteLine("Reached x standardization");
}

//This function uses RPS to line up with the given y coordinate
void yBackRPS(float y_coordinate) //using RPS while robot is in the +y direction
{

    Sleep(0.5);

    //check if receiving proper RPS coordinates and whether the robot is within an acceptable range
    while(RPS.Y() != -1 && RPS.Y() != -2 && (RPS.Y() < y_coordinate - 1 || RPS.Y() > y_coordinate + 1))
    {
        if(RPS.Y() > y_coordinate)
        {
            leftDrive.SetPercent(leftPercent);
            rightDrive.SetPercent(rightPercent);

            Sleep(0.1);

            leftDrive.SetPercent(0);
            rightDrive.SetPercent(0);

            Sleep(0.1);

        }
        else if(RPS.Y() < y_coordinate)
        {

            leftDrive.SetPercent(-leftPercent);
            rightDrive.SetPercent(-rightPercent);

            Sleep(0.1);

            leftDrive.SetPercent(0);
            rightDrive.SetPercent(0);

            Sleep(0.1);

        }
    }

    LCD.WriteLine("Reached y standardization");
}

//This function attempts to flip the ice cream lever twice, as it occasionally missed with 1 flip.
void iceCreamFlip(){

    servo1.SetDegree(creamEndDegree1);
    Sleep(2.0);

    servo1.SetDegree(initialDegree1);
    Sleep(2.0);

    turnLeft(10);
    Sleep(0.5);

    servo1.SetDegree(creamEndDegree1);
    Sleep(2.0);

    servo1.SetDegree(initialDegree1);
    Sleep(2.0);

}

//This function adjusts the robot's angle according to the reading from RPS on the QR code
void checkHeading(float heading) //using RPS
{

    bool checker = true;

    //you will need to fill out this one yourself and take into account
    //checking for proper RPS data and the edge conditions
    //(when you want the robot to go to 0 degrees or close to 0 degrees)

    if((heading > 0 || heading < 0)){

    //check if receiving proper RPS coordinates and whether the robot is within an acceptable range
    while(checker && RPS.Heading() != -1 && RPS.Heading() != -2 && (RPS.Heading() < heading - 0.4 || RPS.Heading() > heading + 0.4))
    {
        if(RPS.Heading() > heading)
        {
            leftDrive.SetPercent(10);
            rightDrive.SetPercent(-10);

            Sleep(0.15);

            leftDrive.SetPercent(0);
            rightDrive.SetPercent(0);

            Sleep(0.15);

        }
        else if(RPS.Heading() < heading)
        {

            leftDrive.SetPercent(-11);
            rightDrive.SetPercent(11);

            Sleep(0.15);

            leftDrive.SetPercent(0);
            rightDrive.SetPercent(0);

            Sleep(0.15);

        }

        float initialTime = TimeNow();
        float time = 5;

        if(TimeNow() - initialTime < time){

            checker = false;

        }
    }

    } else {

        //check if receiving proper RPS coordinates and whether the robot is within an acceptable range
        while(RPS.Heading() != -1 && RPS.Heading() != -2 && (RPS.Heading() < 359.2) && (RPS.Heading() > 0.8))
        {
            if(RPS.Heading() > 180)
            {
                leftDrive.SetPercent(-15);
                rightDrive.SetPercent(15);

                Sleep(0.08);

                leftDrive.SetPercent(0);
                rightDrive.SetPercent(0);

                Sleep(0.1);

            }
            else if(RPS.Heading() < 180)
            {

                leftDrive.SetPercent(15);
                rightDrive.SetPercent(-15);

                Sleep(0.08);

                leftDrive.SetPercent(0);
                rightDrive.SetPercent(0);

                Sleep(0.1);

            }

            float initialTime = TimeNow();
            float time = 5;

            if(TimeNow() - initialTime < time){

                checker = false;

            }

        }

    }

    LCD.WriteLine("Reached angle standardization");

}

//Main body
int main(void)
{
    float x, y; //for touch screen

    //Initialize the screen
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);

    char color = 'n';

    setupBot();

    LCD.WriteLine("Sick Code");
    LCD.WriteLine("Waiting to Start");

    startLight(); //Waiting for Start Light
    goForward(11.2); //Diagonal from Start Pad
    turnRight(45); //Facing ramp
    goForward(6);
    rampDrive(19); //Driving up ramp
    goForward(12);
    turnRight(90); //Turn towards burger wall
    wallPush(); //Bump against wall to standardize
    xFrontRPS(25.935); //Lining up with x coordinate
    turnRight(90); //Making back face burger
    yBackRPS(54.3);
    checkHeading(90); //Lining robot up perfectly
    goBackward(6.57);
    checkHeading(90); //Lining robot up perfectly
    goBackward(0.84); //1 worked well sometimes, 7.57 overall
    flipBurger(); //Triggering servo to flip burger
    goForward(3); //Driving out of slow
    turnLeft(90); //Turning towards ice cream
    goBackward(12); //Driving up to ice cream
    turnRight(25);
    goBackward(1);
    iceCreamFlip(); //Flipping lever

    //Print out data
    LCD.WriteLine("Run Completed.");

}
