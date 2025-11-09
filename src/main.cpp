#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <filesystem>
#include <map>

namespace fs = std::filesystem;

bool initializeSDLAudio();
bool loadAudio(std::vector<Mix_Music*>& samples);
std::string textInput();
void textToAudio(std::string textStr, std::vector<Mix_Music*>& samples, std::vector<Mix_Music*>& textAudio);
void playMusic(std::vector<Mix_Music*>& textAudio);

int main() {
    // Initialize SDL audio section
    if (!initializeSDLAudio()) {
        return 1;
    }

    // Load all the samples
    std::vector<Mix_Music*> samples; // Declare the vector of samples to use
    if (!loadAudio(samples)) {       // Load all the samples in the vector
        return 1;
    }

    // Declare and input the text to convert to audio
    std::string text = textInput();


    std::vector<Mix_Music*> textAudio;
    textToAudio(text, samples, textAudio);
    
    playMusic(textAudio);




    // Free all the sounds, close the audio device and quit SDL
    for (auto* sample : samples) {
        Mix_FreeMusic(sample);
    }
    for (auto* audio : textAudio) {
        Mix_FreeMusic(audio);
    }
    Mix_CloseAudio();
    SDL_Quit();
    return 0;
}




bool initializeSDLAudio() {
    // Initialize SDL audio section
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        std::cout << "error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Open and setup the audio device
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        std::cout << "error: " << Mix_GetError() << std::endl;
        return false;
    }

    return true;
}

bool loadAudio(std::vector<Mix_Music*>& samples) {
    const std::string samplesPath = "samples"; // Get the samples folder path

    // Iterate trough al files in the samples folder and push them in the samples vector if they exists
    for (const auto& file : fs::directory_iterator(samplesPath)) {
        const std::string pathStr = file.path().string();
        const char* samplePath = pathStr.c_str();         // Pointer to the path string as chars

        // Load the audio sample
        Mix_Music* sample = Mix_LoadMUS(samplePath); // Load the sample
        if (!sample) {
            std::cout << "error: " << Mix_GetError() << std::endl;
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
void textToAudio(std::string textStr, std::vector<Mix_Music*>& samples, std::vector<Mix_Music*>& textAudio) {
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

void playMusic(std::vector<Mix_Music*>& textAudio) {
    for (auto* audio : textAudio) {
        Mix_PlayMusic(audio, 0);

        // Let the sound play for x seconds
        SDL_Delay(1000);
    }
    
}
