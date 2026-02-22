#pragma once
#include "ofMain.h"
#include "Oscillator.h"

class Particle {
public:
    // visual stuff
    glm::vec2 position;
    glm::vec2 velocity;
    float     radius;
    ofColor   color;

    float lifetime;   // seconds until it dies
    float age;

    // sound stuff
    OscType oscType;
    float   frequency;
    float   amplitude;
    float   phase;     // [0, 1), advances per audio sample

    Particle(glm::vec2 pos, OscType type, float freq,
             float amp = 0.5f, float life = 3.0f);

    void  update(float dt);
    void  draw();
    bool  isDead() const;

    float getNextSample(Oscillator* osc, float sampleRate);
    float getCurrentAmplitude() const;
};
