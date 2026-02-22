#include "ParticleSystem.h"
#include <algorithm>

ParticleSystem::ParticleSystem() {
    // create one oscillator per waveform type
    oscillators.resize(static_cast<int>(OscType::COUNT));
    oscillators[static_cast<int>(OscType::SINE)]   = std::make_unique<SineOscillator>();
    oscillators[static_cast<int>(OscType::SQUARE)] = std::make_unique<SquareOscillator>();
    oscillators[static_cast<int>(OscType::SAW)]    = std::make_unique<SawOscillator>();
    oscillators[static_cast<int>(OscType::NOISE)]  = std::make_unique<NoiseOscillator>();
}

void ParticleSystem::spawn(glm::vec2 position, OscType type,
                           float frequency, float amplitude, float lifetime) {
    std::lock_guard<std::mutex> lock(mutex);

    if ((int)particles.size() >= MAX_PARTICLES) {
        particles.erase(particles.begin()); // drop oldest
    }
    particles.emplace_back(position, type, frequency, amplitude, lifetime);
}

void ParticleSystem::update() {
    std::lock_guard<std::mutex> lock(mutex);
    float dt = ofGetLastFrameTime();

    for (auto& p : particles) {
        p.update(dt);
    }

    // remove dead ones
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
                       [](const Particle& p) { return p.isDead(); }),
        particles.end());
}

void ParticleSystem::draw() {
    std::lock_guard<std::mutex> lock(mutex);
    ofEnableAlphaBlending();
    for (auto& p : particles) {
        p.draw();
    }
}

void ParticleSystem::fillBuffer(float* output, int bufferSize,
                                int nChannels, float sampleRate) {
    std::lock_guard<std::mutex> lock(mutex);

    // clear
    for (int i = 0; i < bufferSize * nChannels; i++) {
        output[i] = 0.0f;
    }

    if (particles.empty()) return;

    // mix all particles together
    for (auto& p : particles) {
        Oscillator* osc = oscillators[static_cast<int>(p.oscType)].get();
        for (int i = 0; i < bufferSize; i++) {
            float sample = p.getNextSample(osc, sampleRate);
            for (int ch = 0; ch < nChannels; ch++) {
                output[i * nChannels + ch] += sample;
            }
        }
    }

    // normalize + clip so it doesn't blow out the speakers
    float scale = 1.0f / ofMax(1.0f, (float)particles.size() * 0.5f);
    float masterVol = 0.4f;
    for (int i = 0; i < bufferSize * nChannels; i++) {
        output[i] *= scale * masterVol;
        output[i] = ofClamp(output[i], -1.0f, 1.0f);
    }
}

int ParticleSystem::getParticleCount() const {
    std::lock_guard<std::mutex> lock(mutex);
    return (int)particles.size();
}

void ParticleSystem::clear() {
    std::lock_guard<std::mutex> lock(mutex);
    particles.clear();
}
