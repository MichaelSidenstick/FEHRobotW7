#include <FEHLCD.h>
#include <FEHUtility.h>
#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHServo.h>
const double driveVoltage = 9.0;
const double turn180Counts = 450; //Set this after checking theoretical counts for 180 degrees
const double rightPercent = 25.11;
const double leftPercent = 25.0;
const double turnPercent = 20;
const double countsPerInch = 40.5; //Input theoretical counts here
const int initialDegree1 = 110; //GOOD
const int initialDegree2 = 90; //GOOD
const int halfDegree2 = 180;
const int endDegree1 = 34; //GOOD
const int endDegree2 = 158; //GOOD
const int burgerInitialDegree1 = 90; //GOOD
const int burgerInitialDegree2 = 160; //GOOD

FEHMotor rightDrive(FEHMotor::Motor0, driveVoltage);
FEHMotor leftDrive(FEHMotor::Motor3, driveVoltage);
AnalogInputPin cell(FEHIO::P3_7);
DigitalInputPin frontLeftBump(FEHIO::P1_7);
DigitalInputPin frontRightBump(FEHIO::P2_1);
DigitalEncoder rightEncoder(FEHIO::P1_0);
DigitalEncoder leftEncoder(FEHIO::P0_0);
FEHServo servo1(FEHServo::Servo7);
FEHServo servo2(FEHServo::Servo0);

void setupBot(){

    servo1.SetMin(500);
    servo2.SetMin(500);

    servo1.SetMax(2365);
    servo2.SetMax(2375);

    servo1.SetDegree(initialDegree1);
    servo2.SetDegree(initialDegree2);

}

void startLight(){

    while(cell.Value() > 0.8){}

    LCD.WriteLine("Light Detected. Drew is legendary.");

}

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

void rampDrive(double length) //using encoders
{
    //Reset encoder counts
    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();

    //Set both motors to desired percent
    rightDrive.SetPercent(rightPercent+0.2);
    leftDrive.SetPercent(leftPercent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((leftEncoder.Counts() + rightEncoder.Counts()) / 2.0 < (length*countsPerInch));

    //Turn off motors
    rightDrive.Stop();
    leftDrive.Stop();

    Sleep(0.1);

}

void leftPowerForward(double length) //using encoders
{
    //Reset encoder counts
    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();

    //Set both motors to desired percent
    rightDrive.SetPercent(rightPercent*(2.0/3.0));
    leftDrive.SetPercent(leftPercent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((leftEncoder.Counts() + rightEncoder.Counts()) / 2.0 < (length*countsPerInch));

    //Turn off motors
    rightDrive.Stop();
    leftDrive.Stop();

    Sleep(0.1);

}

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

void turnRight(int angle){

    //Reset encoder counts
    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();

    //Set both motors to desired percent
    rightDrive.SetPercent(-(rightPercent+0.16));
    leftDrive.SetPercent(leftPercent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((leftEncoder.Counts() + rightEncoder.Counts()) / 2.0 < (turn180Counts*angle/180.0));

    //Turn off motors
    rightDrive.Stop();
    leftDrive.Stop();

    Sleep(0.1);

}

void turnLeft(int angle){

    //Reset encoder counts
    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();

    //Set both motors to desired percent
    rightDrive.SetPercent((rightPercent+0.16));
    leftDrive.SetPercent(-leftPercent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((leftEncoder.Counts() + rightEncoder.Counts()) / 2.0 < (turn180Counts*angle/180.0));

    //Turn off motors
    rightDrive.Stop();
    leftDrive.Stop();

    Sleep(0.1);

}

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

void wallPushEither(int time){

    //Set both motors to desired percent
    rightDrive.SetPercent(rightPercent);
    leftDrive.SetPercent(leftPercent);

    float initialTime = TimeNow();

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while(frontLeftBump.Value() && frontRightBump.Value() && (TimeNow() - initialTime < time));

    goBackward(5);

    //Turn off motors
    rightDrive.Stop();
    leftDrive.Stop();

    Sleep(0.1);

}

void dropTray(){

    servo1.SetDegree(endDegree1);
    Sleep(2.0);
    servo1.SetDegree(initialDegree1);

}

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

void ticketArm(){

    servo2.SetDegree(halfDegree2);

    Sleep(1.0);

    servo2.SetDegree(endDegree2);

    Sleep(1.0);

    shimmy(10);

    turnLeft(4);

    leftPowerForward(7);

    Sleep(3.0);

    servo2.SetDegree(initialDegree2);

}

int main(void)
{
    float x, y; //for touch screen

    //Initialize the screen
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);

    setupBot();

    LCD.WriteLine("Sick AF Code");
    LCD.WriteLine("Waiting to Start");

    startLight(); //Waiting for Start Light
    goBackward(1.2); //Moving at start
    turnRight(180); //Flipping robot around
    goForward(11); //Diagonal from Start Pad
    turnRight(45); //Facing ramp
    rampDrive(27.1); //Driving up ramp
    turnLeft(90); // Facing Tray to standardize with
    wallPushEither(3); //Go until bump wall
    turnRight(180); //Face tray troy
    goBackward(1);
    dropTray();
    turnLeft(90);
    goForward(1.4); //Drive to line up with ticket holder
    turnRight(90);
    wallPush();
    goForward(1);
    turnLeft(180);
    ticketArm();
    goForward(2);
    turnRight(90);
    goForward(5);
    turnRight(90);
    wallPush();
    turnLeft(90);
    goForward(20);


//    while(true){

//        LCD.WriteLine(frontLeftBump.Value());
//        LCD.WriteLine(frontRightBump.Value());

//    }


//            arm.SetDegree(54.5454 * cell.Value());

    //Print out data
    LCD.WriteLine("Run Completed.");

}
