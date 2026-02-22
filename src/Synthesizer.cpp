#include "Synthesizer.h"

void Synthesizer::setup(ParticleSystem* ps, int sr, int bs) {
    particleSystem = ps;
    sampleRate     = sr;
    bufferSize     = bs;
    waveformDisplay.resize(bufferSize, 0.0f);
}

void Synthesizer::close() {
    // ofApp owns the sound stream so nothing to clean up here
}

void Synthesizer::audioOut(ofSoundBuffer& buffer) {
    if (!particleSystem) return;

    particleSystem->fillBuffer(buffer.getBuffer().data(),
                               buffer.getNumFrames(),
                               buffer.getNumChannels(),
                               sampleRate);

    // grab left channel for the scope
    {
        std::lock_guard<std::mutex> lock(waveformMutex);
        int frames = buffer.getNumFrames();
        int chans  = buffer.getNumChannels();
        waveformDisplay.resize(frames);
        for (int i = 0; i < frames; i++) {
            waveformDisplay[i] = buffer.getBuffer()[i * chans];
        }
    }
}

void Synthesizer::drawWaveform(float x, float y, float w, float h) {
    std::lock_guard<std::mutex> lock(waveformMutex);
    if (waveformDisplay.empty()) return;

    ofSetColor(20, 20, 30);
    ofDrawRectangle(x, y, w, h);

    // center line
    ofSetColor(50);
    ofDrawLine(x, y + h * 0.5f, x + w, y + h * 0.5f);

    // draw the waveform
    ofSetColor(0, 255, 128);
    ofNoFill();
    ofBeginShape();
    for (size_t i = 0; i < waveformDisplay.size(); i++) {
        float px = x + (float)i / waveformDisplay.size() * w;
        float py = y + h * 0.5f - waveformDisplay[i] * h * 0.45f;
        ofVertex(px, py);
    }
    ofEndShape(false);
    ofFill();
}
