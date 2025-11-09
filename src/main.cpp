#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <filesystem>
#include <map>

namespace fs = std::filesystem;

bool initializeSDLAudio();
bool loadAudio(std::vector<Mix_Chunk*>& samples);
std::string textInput();
void textToAudio(const std::string textStr, const std::vector<Mix_Chunk*>& samples, std::vector<Mix_Chunk*>& textAudio);
void playMusic(const std::vector<Mix_Chunk*>& textAudio);

int main() {
    // Initialize SDL audio section
    if (!initializeSDLAudio()) {
        return 1;
    }

    // Load all the samples
    std::vector<Mix_Chunk*> samples; // Declare the vector of samples to use
    if (!loadAudio(samples)) {       // Load all the samples in the vector
        return 1;
    }

    Mix_Music* base = Mix_LoadMUS("base.mp3");
    if (!base) {
        std::cerr << "error: " << Mix_GetError() << std::endl;
    }
    Mix_PlayMusic(base, 10);

    // Declare and input the text to convert to audio
    std::string text = textInput();


    std::vector<Mix_Chunk*> textAudio;
    textToAudio(text, samples, textAudio);
    

    playMusic(textAudio);



    // Free all the sounds, close the audio device and quit SDL
    Mix_FreeMusic(base);
    for (auto* sample : samples) {
        Mix_FreeChunk(sample);
    }
    Mix_CloseAudio();
    SDL_Quit();
    return 0;
}




bool initializeSDLAudio() {
    // Initialize SDL audio section
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        std::cerr << "error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Open and setup the audio device
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        std::cerr << "error: " << Mix_GetError() << std::endl;
        return false;
    }

    return true;
}

bool loadAudio(std::vector<Mix_Chunk*>& samples) {
    const std::string samplesPath = "samples"; // Get the samples folder path

    // Iterate trough al files in the samples folder and push them in the samples vector if they exists
    for (const auto& file : fs::directory_iterator(samplesPath)) {
        const std::string pathStr = file.path().string();
        const char* samplePath = pathStr.c_str();         // Pointer to the path string as chars

        // Load the audio sample
        Mix_Chunk* sample = Mix_LoadWAV(samplePath); // Load the sample
        if (!sample) {
            std::cerr << "error: " << Mix_GetError() << std::endl;
            return false;
        }

        samples.push_back(sample);
    }

    return true; // All samples loaded successfully
}

std::string textInput() {
    std::cout << "Enter text: ";
    std::string input;
    std::getline(std::cin, input);
    return input;
}

// Transform the text input into
void textToAudio(const std::string textStr, const std::vector<Mix_Chunk*>& samples, std::vector<Mix_Chunk*>& textAudio) {
    std::map<char, int> alphabetMap = {
        {'a', 0}, {'b', 1}, {'c', 2}, {'d', 3}, {'e', 4},
        {'f', 5}, {'g', 6}, {'h', 7}, {'i', 8}, {'j', 9},
        {'k', 10}, {'l', 11}, {'m', 12}, {'n', 13}, {'o', 14},
        {'p', 15}, {'q', 16}, {'r', 17}, {'s', 18}, {'t', 19},
        {'u', 20}, {'v', 21}, {'w', 22}, {'x', 23}, {'y', 24}, {'z', 25}
    };

    std::vector<int> letterIndices;
    for (char c : textStr) {
        letterIndices.push_back(alphabetMap[c]);
    }

    for (int i : letterIndices) {
        textAudio.push_back(samples[i]);
    }

}

void playMusic(const std::vector<Mix_Chunk*>& textAudio) {
    for (auto* audio : textAudio) {
        Mix_PlayChannel(0, audio, 0); // play once

        // Wait until the music finishes
        while (Mix_Playing(0)) {
            SDL_Delay(10); // controlla ogni 100ms
        }
    }
}
