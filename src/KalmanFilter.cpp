#include "KalmanFilter.h"
#include <cmath>

KalmanFilter::KalmanFilter(float process_noise, float sensor_noise, float estimated_error, float initial_value) {
    q = process_noise;
    r = sensor_noise;
    p = estimated_error;
    x = initial_value;
    k = 0;
}

float KalmanFilter::update(float measurement) {
    // Adjust measurement for wrap-around effect
    float delta = measurement - x;
    if (delta > 180) {
        delta -= 360;
    }
    else if (delta < -180)
        delta += 360;
    measurement = x + delta;
    // Prediction update
    p = p + q;

    // Measurement update
    k = p / (p + r);
    x = x + k * (measurement - x);
    p = (1 - k) * p;

    // Handling angle wrap-around for 0-360 degree range
    if (x < 0) x += 360;
    if (x >= 360) x -= 360;

    return x;
}