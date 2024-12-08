#include <iostream>
#include <sndfile.h> // For manipulating/reading wav files
#include <vector>
#include <complex>   // For operations on complex numbers 
#include <map>       // To hold note values and labels
#include <set>       // Could I use vector here instead?

// #include <cmath>
// #include <algorithm>
// #include <limits>

// Approximation of pi for convenience
// const double PI = 3.14159265358979323846;

// Define note frequencies for notes in the 4th octave
const std::map<std::string, double> noteFrequencies = {
    {"C4", 261.626},
    {"C#4 / Db4", 277.183},
    {"D4", 293.665},
    {"D#4 / Eb4", 311.127},
    {"E4", 329.628},
    {"F4", 349.228},
    {"F#4 / Gb4", 369.994},
    {"G4", 391.995},
    {"G#4 / Ab4", 415.305},
    {"A4", 440.000},
    {"A#4 / Bb4", 466.164},
    {"B4", 493.883},
};

// Function to read WAV file
std::vector<double> readWavFile(const std::string &filepath, int &sampleRate) {
    SF_INFO sfInfo;
    SNDFILE* file = sf_open(filepath.c_str(), SFM_READ, &sfInfo);
    if (!file) {
        std::cerr << "Error opening file: " << sf_strerror(file) << std::endl;
        exit(1);
    }

    sampleRate = sfInfo.samplerate;
    std::vector<double> samples(sfInfo.frames);
    sf_read_double(file, samples.data(), sfInfo.frames);
    sf_close(file);

    return samples;
}

// Recursive function to perform FFT
void fft(std::vector<std::complex<double>> &data) {
    int N = data.size();

    // Base case
    if (N <= 1)
        return;

    // Divide data into even indices and odd indices for divide and conquer
    std::vector<std::complex<double>> even(N / 2);
    std::vector<std::complex<double>> odd(N / 2);
    for (int i = 0; i < N / 2; i++) {
        even[i] = data[i * 2];
        odd[i] = data[i * 2 + 1];
    }

    // Recursive calls on both halves of the data
    fft(even);
    fft(odd);

    // Combine the data back together using twiddle factor
    for (int k = 0; k < N / 2; k++) { // why is it N / 2 ?
        // Twiddle factor t = e ^ (2 * pi * k) / N
        std::complex<double> t = exp(std::complex<double>(0, -2 * M_PI * k / N)) * odd[k];
        data[k] = even[k] + t;
        data[k + N / 2] = even[k] - t;
    }
}

// Function to identify prominent notes from FFT output
std::vector<std::string> identifyDominantNotes(std::vector<std::complex<double>> &fftData, double sampleRate) {
    int N = fftData.size();
    std::vector<double> magnitudes(N / 2);

    // Calculate magnitudes of the first half of FFT (since it's symmetric)
    double maxMagnitude = 0.0;
    for (int i = 0; i < N / 2; ++i) {
        magnitudes[i] = std::abs(fftData[i]);
        if (magnitudes[i] > maxMagnitude) {
            maxMagnitude = magnitudes[i];
        }
    }

    // Set a dynamic threshold as a fraction of the max magnitude to filter noise
    double threshold = maxMagnitude * 0.3;
    std::vector<int> peakIndices;

    // Find peaks above threshold
    for (int i = 1; i < N / 2 - 1; ++i) {
        if (magnitudes[i] > magnitudes[i - 1] && magnitudes[i] > magnitudes[i + 1] && magnitudes[i] > threshold) {
            peakIndices.push_back(i);
        }
    }

    // Convert peak indices to frequencies and match to notes
    std::set<std::string> identifiedNotes;
    for (int index : peakIndices) {
        double frequency = index * sampleRate / N;

        // Find the closest note to this frequency within a small tolerance
        std::string closestNote;
        double tolerance = 5.0; // Tolerance in Hz for note matching
        for (const auto& [note, freq] : noteFrequencies) {
            double diff = std::abs(frequency - freq);
            if (diff < tolerance) {
                tolerance = diff;
                closestNote = note;
            }
        }

        // Only add if a matching note was found within tolerance
        if (!closestNote.empty()) {
            identifiedNotes.insert(closestNote);
        }
    }

    // Return a sorted list of detected notes
    return std::vector<std::string>(identifiedNotes.begin(), identifiedNotes.end());
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_wav_file>" << std::endl;
        return 1;
    }

    // Read the WAV file
    int sampleRate;
    std::vector<double> audioData = readWavFile(argv[1], sampleRate);

    // Prepare data for FFT (convert to complex numbers)
    std::vector<std::complex<double>> complexData(audioData.begin(), audioData.end());

    // Apply FFT
    fft(complexData);

    // Identify the dominant notes from the FFT output
    std::vector<std::string> detectedNotes = identifyDominantNotes(complexData, sampleRate);

    // Print the detected notes
    for (const std::string& note : detectedNotes)
        std::cout << "Detected note: " << note << std::endl;

    return 0;
}