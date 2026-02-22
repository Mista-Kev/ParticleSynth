#pragma once
#include "ofMain.h"
#include "ParticleSystem.h"
#include <mutex>
#include <vector>

// handles the audio output side - gets samples from ParticleSystem
// and also keeps a copy of the waveform for drawing
class Synthesizer {
public:
    void setup(ParticleSystem* particleSystem,
               int sampleRate = 44100, int bufferSize = 512);
    void close();

    void audioOut(ofSoundBuffer& buffer);  // called from audio thread
    void drawWaveform(float x, float y, float w, float h);

    int getSampleRate() const { return sampleRate; }

private:
    ParticleSystem* particleSystem = nullptr;
    int sampleRate  = 44100;
    int bufferSize  = 512;

    // copy of last audio buffer for visualization
    std::vector<float> waveformDisplay;
    std::mutex         waveformMutex;
};
