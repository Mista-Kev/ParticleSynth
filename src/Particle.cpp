#include "Particle.h"

Particle::Particle(glm::vec2 pos, OscType type, float freq, float amp, float life)
    : position(pos)
    , velocity(ofRandom(-60, 60), ofRandom(-120, -20))
    , radius(ofMap(freq, 110, 880, 22, 5, true))   // lower freq = bigger circle
    , oscType(type)
    , frequency(freq)
    , amplitude(amp)
    , phase(0.0f)
    , lifetime(life)
    , age(0.0f)
{
    // color depends on waveform type
    switch (type) {
        case OscType::SINE:   color = ofColor(100, 200, 255); break;
        case OscType::SQUARE: color = ofColor(255, 100, 100); break;
        case OscType::SAW:    color = ofColor(255, 200,  50); break;
        case OscType::NOISE:  color = ofColor(200, 100, 255); break;
        default:              color = ofColor(255);            break;
    }
}

void Particle::update(float dt) {
    age += dt;
    position += velocity * dt;

    // gravity
    velocity.y += 40.0f * dt;
    // friction
    velocity *= 0.999f;

    // bounce off edges
    float w = ofGetWidth();
    float h = ofGetHeight();
    if (position.x < radius) {
        velocity.x = fabs(velocity.x) * 0.8f;
        position.x = radius;
    } else if (position.x > w - radius) {
        velocity.x = -fabs(velocity.x) * 0.8f;
        position.x = w - radius;
    }
    if (position.y < radius) {
        velocity.y = fabs(velocity.y) * 0.8f;
        position.y = radius;
    } else if (position.y > h - radius) {
        velocity.y = -fabs(velocity.y) * 0.8f;
        position.y = h - radius;
    }

    // shrink over time
    radius = ofMax(1.0f, radius - dt * 1.5f);
}

void Particle::draw() {
    float alpha = getCurrentAmplitude() * 255;

    // glow
    ofSetColor(color, (int)(alpha * 0.25f));
    ofDrawCircle(position, radius * 2.5f);

    // main circle
    ofSetColor(color, (int)alpha);
    ofDrawCircle(position, radius);

    // bright center
    ofSetColor(255, (int)(alpha * 0.6f));
    ofDrawCircle(position, radius * 0.35f);
}

bool Particle::isDead() const {
    return age >= lifetime;
}

float Particle::getNextSample(Oscillator* osc, float sampleRate) {
    float sample = osc->getSample(phase) * getCurrentAmplitude();
    phase += frequency / sampleRate;
    if (phase >= 1.0f) phase -= 1.0f;
    return sample;
}

float Particle::getCurrentAmplitude() const {
    float env = 1.0f - (age / lifetime);    // fade out
    env = ofClamp(env, 0.0f, 1.0f);
    float attack = ofClamp(age / 0.01f, 0.0f, 1.0f);  // 10ms attack
    return amplitude * env * attack;
}
