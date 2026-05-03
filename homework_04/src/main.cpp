#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

const int ticks_per_revolution = 1024;
const float wheel_radius_m = 0.3;
const float wheelbase_m = 1.0;


int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: ugv_odometry <input_path>\n";
        return 1;
    }
    
    std::string input_path = argv[1];
    std::ifstream file(input_path);

    double x = 0.0, y = 0.0, theta = 0.0;

    long timestamp = 0; 
    long fl_ticks = 0, fr_ticks = 0;
    long bl_ticks = 0, br_ticks = 0;

    long last_timestamp = 0; 
    long last_fl_ticks = 0, last_fr_ticks = 0;
    long last_bl_ticks = 0, last_br_ticks = 0;
    bool first_read = true;

    float pi = M_PI;

    while (file >> timestamp >> fl_ticks >> fr_ticks >> bl_ticks >> br_ticks) {

        
        if (first_read){
            last_timestamp = timestamp;
            last_bl_ticks = bl_ticks;
            last_br_ticks = br_ticks;
            last_fl_ticks = fl_ticks;
            last_fr_ticks = fr_ticks;
            first_read = false;
            continue;
        } 

        // diffrent of last and current
        long d_fl = fl_ticks - last_fl_ticks;
        long d_fr = fr_ticks - last_fr_ticks;
        long d_bl = bl_ticks - last_bl_ticks;
        long d_br = br_ticks - last_br_ticks;

        long d_left = (d_fl + d_bl) / 2.0;
        long d_right = (d_fr + d_br) / 2.0;
        
        //impulse to meter
        double distance_per_tick = 2 * pi * wheel_radius_m / ticks_per_revolution;
        double dL = d_left * distance_per_tick;
        double dR = d_right * distance_per_tick;

        //path and angle of robot
        double D = (dL + dR) /2;
        double dtheta = (dR - dL)/wheelbase_m;

        //update position
        x += D * std::cos(theta + dtheta/2);
        y += D * std::sin(theta + dtheta/2);
        theta += dtheta;

        last_fl_ticks = fl_ticks;
        last_fr_ticks = fr_ticks;
        last_bl_ticks = bl_ticks;
        last_br_ticks = br_ticks;

    }
    

    
    

    // TODO: implement wheel odometry for a 4-wheel differential-drive UGV.
    //
    // Parameters:
    //   ticks_per_revolution = 1024
    //   wheel_radius_m       = 0.3
    //   wheelbase_m          = 1.0
    //
    // Input:  text file with 5 whitespace-separated numbers per line:
    //         timestamp_ms fl_ticks fr_ticks bl_ticks br_ticks
    // Output: same tabular format on stdout, starting from the second sample:
    //         timestamp_ms x y theta

    return 0;
}
