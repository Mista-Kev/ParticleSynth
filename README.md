# ParticleSynth

An interactive audio-visual synthesizer built with openFrameworks that generates sound through particle-based synthesis. Create mesmerizing visuals and soundscapes using your mouse, keyboard, or webcam gestures.

## Overview

ParticleSynth combines particle physics simulation with real-time audio synthesis. Each particle you create emits a sound based on its position and properties, creating an intuitive and engaging way to make music. The vertical position determines pitch, while horizontal position selects the waveform type.

## Features

- **Multiple Waveforms**: Sine, Square, Sawtooth, and Noise oscillators
- **Interactive Controls**: Mouse, keyboard, and webcam gesture support
- **Real-time Audio**: Each particle generates audio based on its properties
- **Visual Feedback**: See your sounds as animated particles
- **Piano Keyboard Mapping**: Play notes using QWERTY keyboard keys
- **Waveform Visualization**: Real-time display of the audio output

## Requirements

- openFrameworks (0.11.0 or later)
- C++14 compatible compiler
- Audio output device

## Building

### macOS/Linux
```bash
make
make run
```

### Windows (Visual Studio)
Open the `.sln` file and build the project in Visual Studio.

## Controls

### Mouse
- **Click/Drag**: Spawn particles that emit sounds
  - **Vertical position**: Controls frequency (pitch) - higher = higher pitch
  - **Horizontal position**: Determines waveform type (divided into zones)

### Keyboard

#### Musical Notes
Play notes using piano-style keyboard layout:

**White keys** (ASDFGHJKL):
- `A` = C4 (261.63 Hz)
- `S` = D4 (293.66 Hz)
- `D` = E4 (329.63 Hz)
- `F` = F4 (349.23 Hz)
- `G` = G4 (392.00 Hz)
- `H` = A4 (440.00 Hz)
- `J` = B4 (493.88 Hz)
- `K` = C5 (523.25 Hz)
- `L` = D5 (587.33 Hz)

**Black keys** (WETYUOP):
- `W` = C#4 (277.18 Hz)
- `E` = D#4 (311.13 Hz)
- `T` = F#4 (369.99 Hz)
- `Y` = G#4 (415.30 Hz)
- `U` = A#4 (466.16 Hz)
- `O` = C#5 (554.37 Hz)
- `P` = D#5 (622.25 Hz)

#### Waveform Selection
- `1` = Sine wave
- `2` = Square wave
- `3` = Sawtooth wave
- `4` = Noise

#### Other Controls
- `Space` = Clear all particles
- `C` = Toggle webcam gesture control
- `B` = Learn background (when webcam is enabled)
- `+/=` = Increase webcam threshold
- `-` = Decrease webcam threshold

### Webcam Gestures
When enabled with `C`, the webcam tracks movement and automatically spawns particles based on detected blobs.

## Project Structure

```
src/
├── main.cpp              - Application entry point
├── ofApp.h/cpp           - Main application and UI
├── Particle.h/cpp        - Individual particle with audio properties
├── ParticleSystem.h/cpp  - Manages all particles and audio mixing
├── Oscillator.h/cpp      - Waveform generation (sine, square, saw, noise)
├── Synthesizer.h/cpp     - Audio output and waveform visualization
└── GestureTracker.h/cpp  - Webcam-based gesture detection
```

## How It Works

1. **Particle Spawning**: When you interact with the application, particles are spawned with visual and audio properties
2. **Audio Synthesis**: Each particle has an oscillator that generates sound at a specific frequency
3. **Mixing**: All active particles are mixed together in real-time on the audio thread
4. **Visualization**: Particles are drawn on screen and fade out as they age
5. **Lifetime**: Particles have a limited lifetime (default 3 seconds) and automatically fade away

## Audio Details

- **Sample Rate**: 44,100 Hz
- **Buffer Size**: 512 samples
- **Channels**: Stereo (2 channels)
- **Max Particles**: 64 simultaneous particles
- **Frequency Range**: Determined by screen height (lower = higher pitch)

## Tips

- Try dragging your mouse to create flowing musical phrases
- Combine different waveforms using the number keys
- Use the webcam mode for hands-free, dance-based sound creation
- Press Space to clear and start fresh
- The waveform display at the bottom shows the combined audio output

## License

This project is created as part of a Coding Camp II project.

## Credits

Built with [openFrameworks](https://openframeworks.cc/)
