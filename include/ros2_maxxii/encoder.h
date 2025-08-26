#ifndef ENCODER_H
#define ENCODER_H

#include <vector>
#include <iostream>
#include <memory>
#include <cmath>
#include <stdexcept> // exception handling
#include <chrono>
#include <deque>


class Encoder
{
private:
    double ppr_;
    long reference_pulse_;
    long pulses_;

    // Used for velocity calculation
    std::chrono::steady_clock::time_point last_time_;
    long last_pulses_;

    double filtered_velocity_ = 0.0;
    double alpha_ = 0.1; // Smoothing factor for exponential smoothing
    bool fisrt_reading_ = true;
    
    double computeVelocity(long pulses, std::chrono::duration<double> dt) const;

public:
    Encoder(double ppr, double alpha = 0.1);
    
    void setReferencePulseCount(long pulse_count);
    void setPulseCount(long pulses);

    long getPulseCount() const;
    double getRevolutions() const;
    double getRadiants() const;
    double getDegrees() const;
    double getVelocity();
    int getPPR() const;
};

#endif