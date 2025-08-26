#include "encoder.h"


Encoder::Encoder(double ppr, double alpha)
    : ppr_(ppr), reference_pulse_(0), pulses_(0),
      last_time_(std::chrono::steady_clock::now()), last_pulses_(0),
      alpha_(alpha)
{
    if(ppr <= 0.0)
        throw std::invalid_argument("Error: encoder pulse per revolution cannot be  less than 0!\n");  
    if(alpha < 0.0)
        throw std::invalid_argument("Error: smoothing factor for velociy calculation cannot be less than 0! Choose a value between 0 and 1.\n");
    if(alpha > 1.0)
        throw std::invalid_argument("Error: smoothing factor for velociy calculation cannot be greater than 1! Choose a value between 0 and 1.\n");
}

void Encoder::setReferencePulseCount(long pulses)
{
    reference_pulse_ = pulses;
}

double Encoder::computeVelocity(long pulses, std::chrono::duration<double> dt) const
{
    if (dt.count() <= 0) 
        throw std::runtime_error("Error: Non-positive time difference in velocity calculation.\n");

    long dpulse = pulses - last_pulses_;
    double drev = static_cast<double>(dpulse) / ppr_;
    double omega = (drev * 2.0 * M_PI) / dt.count(); // rad/s
    return omega;
}

void Encoder::setPulseCount(long pulses)
{
    pulses_ = pulses;
    // Skip first velocity calculation to initialize variables
    auto now = std::chrono::steady_clock::now();
    if (fisrt_reading_)
    {
        last_time_ = now;
        last_pulses_ = pulses_;
        fisrt_reading_ = false;
        return;
    }
    std::chrono::duration<double> dt = now - last_time_;
    double omega = computeVelocity(pulses_, dt);

    // Apply filter iir
    filtered_velocity_ = alpha_ * omega + (1.0 - alpha_) * filtered_velocity_;

    // Store for next iteration
    last_time_ = now;
    last_pulses_ = pulses_;
}

long Encoder::getPulseCount() const
{
    return pulses_ - reference_pulse_;
}

double Encoder::getRevolutions() const
{
    return double(getPulseCount()) / ppr_;
}

double Encoder::getRadiants() const
{
    return getRevolutions() * 2 * M_PI;
}

double Encoder::getDegrees() const
{
    return getRevolutions() * 360.0;
}

int Encoder::getPPR() const
{
    return static_cast<int>(ppr_);
}


double Encoder::getVelocity() 
{
    return filtered_velocity_;
}