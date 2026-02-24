#include "GestureTracker.h"

void GestureTracker::setup(int w, int h) {
    camWidth  = w;
    camHeight = h;
}

void GestureTracker::update() {
    if (!enabled) return;

    // lazy init: start the camera the first time webcam is enabled
    // retries each frame in case macOS permission dialog blocked the first attempt
    if (!vidGrabber.isInitialized()) {
        vidGrabber.setDeviceID(0);
        vidGrabber.setup(camWidth, camHeight);
        return;
    }

    vidGrabber.update();

    if (vidGrabber.isFrameNew()) {
        // allocate opencv images once we get the actual camera resolution
        if (!allocated) {
            int w = vidGrabber.getWidth();
            int h = vidGrabber.getHeight();
            if (w > 0 && h > 0) {
                camWidth  = w;
                camHeight = h;
                colorImg.allocate(camWidth, camHeight);
                grayImg.allocate(camWidth, camHeight);
                grayBg.allocate(camWidth, camHeight);
                grayDiff.allocate(camWidth, camHeight);
                bLearnBg  = true;
                allocated = true;
            }
        }

        if (allocated) {
            ofPixels pix = vidGrabber.getPixels();
            pix.setImageType(OF_IMAGE_COLOR);
            colorImg.setFromPixels(pix);
            grayImg = colorImg;

            if (bLearnBg) {
                grayBg   = grayImg;
                bLearnBg = false;
            }

            grayDiff.absDiff(grayBg, grayImg);
            grayDiff.threshold(threshold);

            contourFinder.findContours(grayDiff, 200,
                                       (camWidth * camHeight) / 3,
                                       10, true);
        }
    }
}

void GestureTracker::draw(float x, float y, float w, float h) {
    if (!enabled || !allocated) return;

    ofSetColor(255);
    colorImg.draw(x, y, w, h);

    // overlay contours
    float sx = w / camWidth;
    float sy = h / camHeight;
    ofPushMatrix();
    ofTranslate(x, y);
    ofScale(sx, sy);
    contourFinder.draw();
    ofPopMatrix();
}

bool GestureTracker::hasBlob() const {
    return enabled && allocated && contourFinder.nBlobs > 0;
}

int GestureTracker::getNumBlobs() const {
    if (!enabled || !allocated) return 0;
    return contourFinder.nBlobs;
}

glm::vec2 GestureTracker::getBlobCenter(int index) const {
    if (!enabled || !allocated || index >= contourFinder.nBlobs)
        return glm::vec2(0.5f, 0.5f);

    auto& blob = contourFinder.blobs[index];
    return glm::vec2(blob.centroid.x / camWidth,
                     blob.centroid.y / camHeight);
}

float GestureTracker::getBlobArea(int index) const {
    if (!enabled || !allocated || index >= contourFinder.nBlobs) return 0.0f;
    return contourFinder.blobs[index].area / (float)(camWidth * camHeight);
}

void GestureTracker::setEnabled(bool val) {
    enabled = val;
}

void GestureTracker::learnBackground() {
    bLearnBg = true;
}

void GestureTracker::setThreshold(int val) {
    threshold = val;
}
