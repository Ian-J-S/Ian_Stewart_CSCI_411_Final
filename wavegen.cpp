#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <sndfile.h>
#include <string>
#include <algorithm>

// Sample rate in Hz
const int sampleRate = 44100;
// Frequencies of notes in the 4th octave
const std::map<std::string, float> noteFrequencies = {
    {"C4", 261.63f}, {"C#4", 277.18f}, {"D4", 293.66f}, {"D#4", 311.13f}, {"E4", 329.63f},
    {"F4", 349.23f}, {"F#4", 369.99f}, {"G4", 392.00f}, {"G#4", 415.30f}, {"A4", 440.00f},
    {"A#4", 466.16f}, {"B4", 493.88f}
};

// Function to generate a sine wave for a specific frequency and duration
std::vector<float> generateSineWave(float frequency, float duration) {
    int totalSamples = static_cast<int>(sampleRate * duration);
    std::vector<float> samples(totalSamples);
    for (int i = 0; i < totalSamples; ++i) {
        float t = static_cast<float>(i) / sampleRate; // Time in seconds
        samples[i] = 0.5f * std::sin(2.0f * M_PI * frequency * t); // Amplitude scaled to 0.5
    }
    return samples;
}

// Function to combine multiple notes into a single waveform
std::vector<float> combineChords(const std::vector<std::string>& notes, float duration) {
    int totalSamples = static_cast<int>(sampleRate * duration);
    // Initialize with zeros
    std::vector<float> combinedSamples(totalSamples, 0.0f); 

    for (const std::string& note : notes) {
        auto it = noteFrequencies.find(note);
        if (it == noteFrequencies.end()) {
            throw std::invalid_argument("Note " + note + " is not recognized.");
        }
        float frequency = it->second;

        // Generate samples for this note
        std::vector<float> noteSamples = generateSineWave(frequency, duration);
        for (int i = 0; i < totalSamples; ++i) {
            combinedSamples[i] += noteSamples[i];
        }
    }

    // Normalize the combined samples
    float maxAmplitude = *std::max_element(combinedSamples.begin(), combinedSamples.end(),
        [](float a, float b) { return std::abs(a) < std::abs(b); });
        
    if (maxAmplitude > 1.0f) {
        for (float& sample : combinedSamples) {
            sample /= maxAmplitude;
        }
    }

    return combinedSamples;
}

// Function to save the generated notes to a .wav file
void saveToWav(const std::string& filename, const std::vector<float>& samples) {
    SF_INFO sfInfo;
    sfInfo.samplerate = sampleRate;
    sfInfo.frames = static_cast<int>(samples.size());
    sfInfo.channels = 1;
    sfInfo.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;

    SNDFILE* outFile = sf_open(filename.c_str(), SFM_WRITE, &sfInfo);
    if (!outFile) {
        std::cerr << "Error opening file for writing: " << sf_strerror(outFile) << std::endl;
        return;
    }

    sf_write_float(outFile, samples.data(), samples.size());
    sf_close(outFile);
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <output.wav> <duration> <note1> [note2] ..." << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    float duration = std::stof(argv[2]);

    // Get notes from command line arguments
    std::vector<std::string> notes;
    for (int i = 3; i < argc; ++i) {
        notes.push_back(argv[i]);
    }

    try {
        std::vector<float> combinedSamples = combineChords(notes, duration);
        saveToWav(filename, combinedSamples);
        std::cout << "Generated " << filename << " with specified notes." << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}