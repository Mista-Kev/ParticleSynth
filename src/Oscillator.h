#pragma once
#include "ofMain.h"

enum class OscType { SINE = 0, SQUARE, SAW, NOISE, COUNT };

// base class - each subclass implements its own waveform shape
class Oscillator {
public:
    virtual ~Oscillator() = default;
    virtual float getSample(float phase) = 0;  // phase in [0,1), returns [-1,1]
    virtual std::string getName() = 0;
};

class SineOscillator : public Oscillator {
public:
    float getSample(float phase) override;
    std::string getName() override { return "Sine"; }
};

class SquareOscillator : public Oscillator {
public:
    float getSample(float phase) override;
    std::string getName() override { return "Square"; }
};

class SawOscillator : public Oscillator {
public:
    float getSample(float phase) override;
    std::string getName() override { return "Saw"; }
};

class NoiseOscillator : public Oscillator {
public:
    float getSample(float phase) override;
    std::string getName() override { return "Noise"; }
};
