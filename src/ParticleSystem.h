#pragma once
#include "ofMain.h"
#include "Particle.h"
#include "Oscillator.h"
#include <mutex>
#include <vector>
#include <memory>

// manages all active particles + handles audio mixing
// uses a mutex because fillBuffer() runs on the audio thread
class ParticleSystem {
public:
    ParticleSystem();

    void spawn(glm::vec2 position, OscType type, float frequency,
               float amplitude = 0.5f, float lifetime = 3.0f);

    void update();
    void draw();

    // mixes all living particles into the output buffer (called from audio thread)
    void fillBuffer(float* output, int bufferSize, int nChannels, float sampleRate);

    int  getParticleCount() const;
    void clear();

private:
    std::vector<Particle> particles;
    std::vector<std::unique_ptr<Oscillator>> oscillators;  // one per waveform type
    mutable std::mutex mutex;

    static const int MAX_PARTICLES = 64;
};
