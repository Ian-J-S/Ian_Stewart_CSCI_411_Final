#!/bin/bash

# Set up directories and generate .wav files
setup() {
    make
    mkdir -p notes
    echo "Generating test .wav files..."

    ./wavegen notes/a.wav 2 A4
    ./wavegen notes/c.wav 2 C4
    ./wavegen notes/d.wav 2 D4
    ./wavegen notes/cmaj.wav 2 C4 E4 G4
    ./wavegen notes/csus2.wav 2 C4 D4 G4

    echo "Note generation complete."
}

# Run FFT tests
test() {
    echo -e "\nTesting generated notes"

    echo -e "\nExpecting A4"
    ./fft notes/a.wav

    echo -e "\nExpecting C4"
    ./fft notes/c.wav

    echo -e "\nExpecting D4"
    ./fft notes/d.wav

    echo -e "\nExpecting C4 E4 G4"
    ./fft notes/cmaj.wav

    echo -e "\nExpecting C4 D4 G4"
    ./fft notes/csus2.wav

    echo -e "\nTesting .wav files from freesound.com"
    
    echo -e "Pitch pipe - Expecting C4"
    ./fft notes/pitch-pipe-c4.wav

    echo -e "Piano - Expecting C4"
    ./fft notes/piano-c4.wav

    echo -e "Oboe - Expecting A4"
    ./fft notes/oboe-a4.wav

    echo "Tests complete."
}

# Execute setup and test functions
setup
test