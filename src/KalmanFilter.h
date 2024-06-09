#ifndef KALMANFILTER_H
#define KALMANFILTER_H

class KalmanFilter {
public:
    KalmanFilter(float process_noise, float sensor_noise, float estimated_error, float initial_value);
    float update(float measurement);

private:
    float q; // Process noise covariance
    float r; // Measurement noise covariance
    float p; // Estimation error covariance
    float k; // Kalman gain
    float x; // Value
};

#endif // KALMANFILTER_H