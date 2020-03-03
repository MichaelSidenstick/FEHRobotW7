#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#include <FEHSD.h>

//Declarations for encoders & motors
DigitalEncoder right_encoder(FEHIO::P0_1);
DigitalEncoder left_encoder(FEHIO::P0_0);
FEHMotor right_motor(FEHMotor::Motor1, 9.0);
FEHMotor left_motor(FEHMotor::Motor0, 9.0);

void move_forward(int percent, int counts) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder are less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void turn_left(int percent, int counts) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(-percent);

    //While the average of the left and right encoder are less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void check_x_minus(float x_coordinate) //using RPS while robot is in the -x direction
{
    //check if receiving proper RPS coordinates and whether the robot is within an acceptable range
    while(RPS.X() != -2 && (RPS.X() < x_coordinate - 1 || RPS.X() > x_coordinate + 1))
    {
        if(RPS.X() > x_coordinate)
        {
            //pulse the motors for a short duration in the correct direction

            left_motor.SetPercent(15);
            right_motor.SetPercent(15);

            Sleep(0.1);

            left_motor.SetPercent(0);
            right_motor.SetPercent(0);

            Sleep(0.1);

        }
        else if(RPS.X() < x_coordinate)
        {

            left_motor.SetPercent(-15);
            right_motor.SetPercent(-15);

            Sleep(0.1);

            left_motor.SetPercent(0);
            right_motor.SetPercent(0);

            Sleep(0.1);

        }
    }
}

void check_y_minus(float y_coordinate) //using RPS while robot is in the -y direction
{
    //check if receiving proper RPS coordinates and whether the robot is within an acceptable range
    while(RPS.Y() != -1 && RPS.Y() != -2 && (RPS.Y() < y_coordinate - 1 || RPS.Y() > y_coordinate + 1))
    {
        if(RPS.Y() > y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction

            left_motor.SetPercent(15);
            right_motor.SetPercent(15);

            Sleep(0.1);

            left_motor.SetPercent(0);
            right_motor.SetPercent(0);

            Sleep(0.1);

        }
        else if(RPS.Y() < y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction

            left_motor.SetPercent(-25);
            right_motor.SetPercent(-25);

            Sleep(0.1);

            left_motor.SetPercent(0);
            right_motor.SetPercent(0);

            Sleep(0.1);

        }
    }
}

void check_y_plus(float y_coordinate) //using RPS while robot is in the +y direction
{
    //check if receiving proper RPS coordinates and whether the robot is within an acceptable range
    while(RPS.Y() != -1 && RPS.Y() != -2 && (RPS.Y() < y_coordinate - 1 || RPS.Y() > y_coordinate + 1))
    {
        if(RPS.Y() > y_coordinate)
        {
            left_motor.SetPercent(-15);
            right_motor.SetPercent(-15);

            Sleep(0.1);

            left_motor.SetPercent(0);
            right_motor.SetPercent(0);

            Sleep(0.1);

        }
        else if(RPS.Y() < y_coordinate)
        {

            left_motor.SetPercent(15);
            right_motor.SetPercent(15);

            Sleep(0.1);

            left_motor.SetPercent(0);
            right_motor.SetPercent(0);

            Sleep(0.1);

        }
    }
}

void check_heading(float heading) //using RPS
{
    //you will need to fill out this one yourself and take into account
    //checking for proper RPS data and the edge conditions
    //(when you want the robot to go to 0 degrees or close to 0 degrees)

    if((heading > 0 || heading < 0)){

    //check if receiving proper RPS coordinates and whether the robot is within an acceptable range
    while(RPS.Heading() != -1 && RPS.Heading() != -2 && (RPS.Heading() < heading - 0.8 || RPS.Heading() > heading + 0.8))
    {
        if(RPS.Heading() > heading)
        {
            left_motor.SetPercent(15);
            right_motor.SetPercent(-15);

            Sleep(0.08);

            left_motor.SetPercent(0);
            right_motor.SetPercent(0);

            Sleep(0.1);

        }
        else if(RPS.Heading() < heading)
        {

            left_motor.SetPercent(-15);
            right_motor.SetPercent(15);

            Sleep(0.08);

            left_motor.SetPercent(0);
            right_motor.SetPercent(0);

            Sleep(0.1);

        }
    }

    } else {

        //check if receiving proper RPS coordinates and whether the robot is within an acceptable range
        while(RPS.Heading() != -1 && RPS.Heading() != -2 && (RPS.Heading() < 359.2) && (RPS.Heading() > 0.8))
        {
            if(RPS.Heading() > 180)
            {
                left_motor.SetPercent(-15);
                right_motor.SetPercent(15);

                Sleep(0.08);

                left_motor.SetPercent(0);
                right_motor.SetPercent(0);

                Sleep(0.1);

            }
            else if(RPS.Heading() < 180)
            {

                left_motor.SetPercent(15);
                right_motor.SetPercent(-15);

                Sleep(0.08);

                left_motor.SetPercent(0);
                right_motor.SetPercent(0);

                Sleep(0.1);

            }
        }

    }

}

int main(void)
{
    float touch_x,touch_y;
    float A_x, A_y, B_x, B_y, C_x, C_y, D_x, D_y;
    float A_heading, B_heading, C_heading, D_heading;
    int power, B_C_counts, C_D_counts, turn_90_counts;

    RPS.InitializeTouchMenu();

    LCD.WriteLine("RPS & Data Logging Test");
    LCD.WriteLine("Press Screen To Start");
    while(!LCD.Touch(&touch_x,&touch_y));
    while(LCD.Touch(&touch_x,&touch_y));

    //WRITE CODE HERE TO READ SD CARD FOR LOGGED X AND Y DATA POINTS
    FEHFile *fptr = SD.FOpen("RPS_TEST.TXT","r");
    SD.FScanf(fptr,"%f %f", &A_x, &A_y);
    SD.FScanf(fptr,"%f %f", &B_x, &B_y);
    SD.FScanf(fptr,"%f %f", &C_x, &C_y);
    SD.FScanf(fptr,"%f %f", &D_x, &D_y);


    SD.FClose(fptr);

    //WRITE CODE HERE TO SET THE HEADING DEGREES, MOTOR POWER, AND COUNTS VALUES
    A_heading = 90;
    B_heading = 180;
    C_heading = 270;
    D_heading = 0;
    power = 15;
    B_C_counts = 648;
    C_D_counts = 405;
    turn_90_counts = 210;

    LCD.WriteLine(A_x);
    LCD.WriteLine(A_y);
    LCD.WriteLine(B_x);
    LCD.WriteLine(B_y);
    LCD.WriteLine(C_x);
    LCD.WriteLine(C_y);
    LCD.WriteLine(D_x);
    LCD.WriteLine(D_y);


    //A --> B
    check_y_plus(B_y);
    check_heading(B_heading);

    //B --> C
    move_forward(power, B_C_counts);
    check_x_minus(C_x);
    turn_left(power, turn_90_counts);
    check_heading(C_heading);

    //C --> D
    move_forward(power, C_D_counts);
    check_y_minus(D_y);
    turn_left(power, turn_90_counts);
    check_heading(D_heading);

    return 0;
}
