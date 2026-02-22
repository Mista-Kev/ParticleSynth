#include "Oscillator.h"

float SineOscillator::getSample(float phase) {
    return sin(phase * TWO_PI);
}

float SquareOscillator::getSample(float phase) {
    return phase < 0.5f ? 1.0f : -1.0f;
}

float SawOscillator::getSample(float phase) {
    return 2.0f * phase - 1.0f;
}

float NoiseOscillator::getSample(float phase) {
    return ofRandom(-1.0f, 1.0f);
}
