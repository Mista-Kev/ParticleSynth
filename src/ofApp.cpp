#include "ofApp.h"

// keyboard -> note frequency mapping (piano layout on QWERTY)
//
// white keys:  A  S  D  F  G  H  J  K  L
//              C4 D4 E4 F4 G4 A4 B4 C5 D5
//
// black keys:  W  E     T  Y  U     O  P
//              C# D#    F# G# A#    C# D#
//
static const std::map<int, float> noteMap = {
    { 'a', 261.63f }, // C4
    { 's', 293.66f }, // D4
    { 'd', 329.63f }, // E4
    { 'f', 349.23f }, // F4
    { 'g', 392.00f }, // G4
    { 'h', 440.00f }, // A4
    { 'j', 493.88f }, // B4
    { 'k', 523.25f }, // C5
    { 'l', 587.33f }, // D5
    { 'w', 277.18f }, // C#4
    { 'e', 311.13f }, // D#4
    { 't', 369.99f }, // F#4
    { 'y', 415.30f }, // G#4
    { 'u', 466.16f }, // A#4
    { 'o', 554.37f }, // C#5
    { 'p', 622.25f }, // D#5
};

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofBackground(10, 10, 20);
    ofSetCircleResolution(32);
    ofEnableAlphaBlending();

    // audio setup
    int sampleRate = 44100;
    int bufSize    = 512;
    synth.setup(&particleSystem, sampleRate, bufSize);

    ofSoundStreamSettings ss;
    ss.setOutListener(this);
    ss.sampleRate        = sampleRate;
    ss.numOutputChannels = 2;
    ss.numInputChannels  = 0;
    ss.bufferSize        = bufSize;
    ss.numBuffers        = 4;
    soundStream.setup(ss);

    // webcam off by default, use keyboard/mouse first
    gestureTracker.setup(320, 240);
    gestureTracker.setEnabled(false);
}

//--------------------------------------------------------------
void ofApp::update() {
    particleSystem.update();
    gestureTracker.update();

    // if webcam is on, spawn particles where blobs are detected
    if (gestureTracker.isEnabled() && gestureTracker.hasBlob()) {
        for (int i = 0; i < gestureTracker.getNumBlobs(); i++) {
            glm::vec2 norm = gestureTracker.getBlobCenter(i);
            float x = norm.x * ofGetWidth();
            float y = norm.y * ofGetHeight();
            // don't spawn every single frame or it gets crazy
            if (ofGetFrameNum() % 6 == 0) {
                spawnAtPosition(x, y);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    drawZoneGuides();

    particleSystem.draw();

    // waveform at the bottom
    float scopeH = 100;
    synth.drawWaveform(0, ofGetHeight() - scopeH, ofGetWidth(), scopeH);

    // small webcam preview top-right
    if (gestureTracker.isEnabled()) {
        float pw = 160, ph = 120;
        gestureTracker.draw(ofGetWidth() - pw - 10, 10, pw, ph);
    }

    drawUI();
}

//--------------------------------------------------------------
void ofApp::exit() {
    soundStream.close();
    synth.close();
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer& buffer) {
    synth.audioOut(buffer);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    // switch waveform
    if (key == '1') { currentOscType = OscType::SINE;   return; }
    if (key == '2') { currentOscType = OscType::SQUARE; return; }
    if (key == '3') { currentOscType = OscType::SAW;    return; }
    if (key == '4') { currentOscType = OscType::NOISE;  return; }

    if (key == ' ') { particleSystem.clear(); return; }

    // webcam controls
    if (key == 'c') {
        gestureTracker.setEnabled(!gestureTracker.isEnabled());
        return;
    }
    if (key == 'b') { gestureTracker.learnBackground(); return; }

    if (key == '=' || key == '+') {
        gestureTracker.setThreshold(
            ofClamp(gestureTracker.getThreshold() + 5, 0, 255));
        return;
    }
    if (key == '-') {
        gestureTracker.setThreshold(
            ofClamp(gestureTracker.getThreshold() - 5, 0, 255));
        return;
    }

    // play a note
    float freq = keyToFrequency(key);
    if (freq > 0) {
        float cx = ofGetWidth()  * 0.5f + ofRandom(-80, 80);
        float cy = ofGetHeight() * 0.4f + ofRandom(-40, 40);
        particleSystem.spawn(glm::vec2(cx, cy), currentOscType, freq);
    }
}

void ofApp::keyReleased(int key) {
    // TODO: could do sustained notes here later
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    spawnAtPosition((float)x, (float)y);
}

void ofApp::mouseDragged(int x, int y, int button) {
    if (ofGetFrameNum() % 3 == 0) {
        spawnAtPosition((float)x, (float)y);
    }
}

//--------------------------------------------------------------
void ofApp::spawnAtPosition(float x, float y) {
    OscType type = oscTypeFromX(x);
    spawnAtPosition(x, y, type);
}

void ofApp::spawnAtPosition(float x, float y, OscType type) {
    float freq = frequencyFromY(y);
    particleSystem.spawn(glm::vec2(x, y), type, freq);
}

float ofApp::frequencyFromY(float y) {
    // top = high pitch, bottom = low (3 octave range)
    return ofMap(y, 0, ofGetHeight(), 880.0f, 110.0f, true);
}

OscType ofApp::oscTypeFromX(float x) {
    // screen split into 4 zones left to right
    float frac = x / ofGetWidth();
    if (frac < 0.25f) return OscType::SINE;
    if (frac < 0.50f) return OscType::SQUARE;
    if (frac < 0.75f) return OscType::SAW;
    return OscType::NOISE;
}

float ofApp::keyToFrequency(int key) {
    auto it = noteMap.find(key);
    return (it != noteMap.end()) ? it->second : 0.0f;
}

//--------------------------------------------------------------
void ofApp::drawZoneGuides() {
    float w = ofGetWidth();
    float h = ofGetHeight();

    // faint color strips for the 4 waveform zones
    ofSetColor(100, 200, 255, 12);  ofDrawRectangle(0,          0, w * 0.25f, h);
    ofSetColor(255, 100, 100, 12);  ofDrawRectangle(w * 0.25f,  0, w * 0.25f, h);
    ofSetColor(255, 200,  50, 12);  ofDrawRectangle(w * 0.50f,  0, w * 0.25f, h);
    ofSetColor(200, 100, 255, 12);  ofDrawRectangle(w * 0.75f,  0, w * 0.25f, h);

    ofSetColor(255, 50);
    ofDrawBitmapString("SINE",   w * 0.125f - 12, 20);
    ofDrawBitmapString("SQUARE", w * 0.375f - 18, 20);
    ofDrawBitmapString("SAW",    w * 0.625f - 10, 20);
    ofDrawBitmapString("NOISE",  w * 0.875f - 15, 20);
}

void ofApp::drawUI() {
    int y = 40;
    ofSetColor(255);

    const char* oscNames[] = { "SINE", "SQUARE", "SAW", "NOISE" };
    ofDrawBitmapString("Osc: "
        + std::string(oscNames[static_cast<int>(currentOscType)])
        + "  [1-4 to switch]", 10, y);
    y += 18;

    ofDrawBitmapString("Particles: "
        + ofToString(particleSystem.getParticleCount()) + " / 64", 10, y);
    y += 18;

    ofDrawBitmapString("Webcam: "
        + std::string(gestureTracker.isEnabled() ? "ON" : "OFF")
        + "  [C to toggle]", 10, y);
    y += 18;

    if (gestureTracker.isEnabled()) {
        ofDrawBitmapString("Threshold: "
            + ofToString(gestureTracker.getThreshold())
            + "  [+/- adjust]  [B = re-learn bg]", 10, y);
        y += 18;
    }

    ofSetColor(255, 80);
    int bottom = ofGetHeight() - 115;
    ofDrawBitmapString(
        "KEYS: A-L = notes | W E T Y U O P = sharps | 1-4 = waveform | SPACE = clear",
        10, bottom);
    ofDrawBitmapString(
        "MOUSE: click/drag = spawn | X-zone = waveform | Y = pitch",
        10, bottom + 14);

    ofDrawBitmapString("FPS: " + ofToString((int)ofGetFrameRate()),
                       ofGetWidth() - 70, bottom);
}
