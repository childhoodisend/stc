//
// Created by artem on 19.03.2021.
//

#ifndef STC_WAVE_H
#define STC_WAVE_H

#include <vector>
#include <fstream>
#include <string>
#include <types.hpp>

struct riff_header { // First part
    riff_header() = default;
    ~riff_header() = default;

    char chunkID[4] = {'n','o','n','e'};            // "RIFF" = 0x46464952
    uint32_t chunkSize{};    // 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
    uint8_t format[4]{};             // "WAVE" = 0x45564157
};

struct fmt_subchunk{ // Second part
    fmt_subchunk() = default;
    ~fmt_subchunk() = default;

    char subchunk1ID[4] = {'n','o','n','e'};// "fmt " = 0x20746D66
    uint32_t subchunk1Size{};// 16 for PCM.
    uint16_t audioFormat{};  // PCM = 1 (i.e. Linear quantization)
    uint16_t numChannels{};  // Mono = 1, Stereo = 2, etc.
    uint32_t sampleRate{};   // 8000, 44100, etc.
    uint32_t byteRate{};     // == SampleRate * NumChannels * BitsPerSample/8
    uint16_t blockAlign{};   // == NumChannels * BitsPerSample/8
    uint16_t bitsPerSample{};

    //  ExtraParamSize   if PCM, then doesn't exist
    //  ExtraParams      space for extra parameters
};
struct list_subchunk{
    list_subchunk() = default;
    ~list_subchunk() = default;

    uint8_t subchunk3ID[4] = {'n','o','n','e'}; // "LIST"
    uint32_t subchunk3Size{};
};

struct data_subchunk{ // Third part
    data_subchunk() = default;
    ~data_subchunk() = default;

    uint8_t subchunk2ID[4] = {'n','o','n','e'};// "data" = 0x61746164
    uint32_t subchunk2Size{};//  == NumSamples * NumChannels * BitsPerSample/8
};


struct wave_header{ // Main header, containing all of the parts above
    wave_header() = default;
    ~wave_header() = default;
    riff_header riff{};
    fmt_subchunk fmt{};
    list_subchunk list{};
    data_subchunk data{};
};

struct wave_format{
public:
    wave_format() = default;
    ~wave_format() = default;

    void wav_reader();
    void print_info() const;
    void print_all_info() const;

private: // Methods
    void get_header();
    void get_data();
    void get_one_channel();
    void get_two_channels();

private: // Work with files
    const std::string file_input = "../source/Wave/brat2_96.wav";
    const std::string file_output = "../source/Wave/output.bin";
    std::fstream fin;
    std::ofstream fout;

private:
    wave_header header{};
    std::vector<float>  data_f{};
    std::vector<IQF>    data_IQF{};
};
#endif //STC_WAVE_H