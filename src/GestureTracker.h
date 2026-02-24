#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"

// webcam blob tracking using ofxOpenCv
// Abhishek - feel free to change the internals here,
// just keep hasBlob/getBlobCenter/etc working so ofApp can use them
class GestureTracker {
public:
    void setup(int camWidth = 320, int camHeight = 240);
    void update();
    void draw(float x, float y, float w, float h);

    // blob queries (returns normalized 0-1 coords)
    bool      hasBlob() const;
    int       getNumBlobs() const;
    glm::vec2 getBlobCenter(int index = 0) const;
    float     getBlobArea(int index = 0) const;

    void setEnabled(bool val);
    bool isEnabled() const { return enabled; }
    void learnBackground();
    void setThreshold(int val);
    int  getThreshold() const { return threshold; }

private:
    ofVideoGrabber      vidGrabber;
    ofxCvColorImage      colorImg;
    ofxCvGrayscaleImage  grayImg;
    ofxCvGrayscaleImage  grayBg;
    ofxCvGrayscaleImage  grayDiff;
    ofxCvContourFinder   contourFinder;

    int  camWidth   = 320;
    int  camHeight  = 240;
    int  threshold  = 80;
    bool bLearnBg   = true;
    bool enabled    = false;
    bool camStarted = false;
    bool allocated  = false;
};
