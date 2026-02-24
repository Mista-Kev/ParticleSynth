#pragma once
#include "ofMain.h"
#include "ParticleSystem.h"
#include "Synthesizer.h"
#include "GestureTracker.h"
#include <map>

class ofApp : public ofBaseApp {
public:
    void setup()  override;
    void update() override;
    void draw()   override;
    void exit()   override;

    void keyPressed(int key)  override;
    void keyReleased(int key) override;
    void mousePressed(int x, int y, int button)  override;
    void mouseDragged(int x, int y, int button)  override;

    void audioOut(ofSoundBuffer& buffer);

private:
    ParticleSystem  particleSystem;
    Synthesizer     synth;
    GestureTracker  gestureTracker;
    ofSoundStream   soundStream;

    OscType currentOscType = OscType::SINE;

    void    spawnAtPosition(float x, float y);
    void    spawnAtPosition(float x, float y, OscType type);
    float   frequencyFromY(float y);      // screen Y -> Hz
    OscType oscTypeFromX(float x);        // screen zone -> waveform
    float   keyToFrequency(int key);      // keyboard key -> Hz (0 if not a note)

    void drawUI();
    void drawZoneGuides();
};
