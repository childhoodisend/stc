//
// Created by artem on 19.03.2021.
//

#ifndef STC_WAVE_HPP
#define STC_WAVE_HPP

#include <vector>
#include <fstream>
#include <string>

#include <types.hpp>

using namespace types;

struct riff_header { // First part
    riff_header() = default;
    ~riff_header() = default;

    uint8_t chunkID[4]{};            // "RIFF" = 0x46464952
    uint32_t chunkSize{};    // 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
    uint8_t format[4]{};             // "WAVE" = 0x45564157
};

struct fmt_subchunk{ // Second part
    fmt_subchunk() = default;
    ~fmt_subchunk() = default;

    uint8_t subchunk1ID[4]{};// "fmt " = 0x20746D66
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

struct list_subchunk{ // Fourth part, optional
    list_subchunk() = default;
    ~list_subchunk() = default;

    uint8_t subchunk3ID[4]{}; // "LIST"
    uint32_t subchunk3Size{};
    uint8_t subchunk3INFO[4]{}; // "INFO"
    uint8_t subchunk3IGNR[4]{};
};

struct data_subchunk{ // Third part
    data_subchunk() = default;
    ~data_subchunk() = default;

    uint8_t subchunk2ID[4]{};// "data" = 0x61746164
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

template<typename T>
struct wave_format{
public:
    wave_format()   = default;
    ~wave_format()  = default;


    void wav_reader();
    void print_info()     const;
    void print_all_info() const;
    void print_data()     const;

private: // Methods
    void get_header();
    void get_data();
    void get_one_channel();
    void get_two_channels();

private: // Work with files
    const std::string file_input  = "../source/Wave/brat2_96.wav";
    const std::string file_output = "../source/Wave/output.bin";
    std::fstream fin;
    std::ofstream fout;

private:
    size_t              HEADERSIZE{};
    wave_header         header{};
    std::vector<T>      data{};
    std::vector<LR<T>>  data_LR{};
};


/*
 *  Реализация функций
 */

template<typename T>
void wave_format<T>::wav_reader() { // Read WAV file!
    get_header(); // Firstly, read the header
    get_data();   // Secondly, read the data after the header
}

/*
 * -----Getting WAV header-----
 */
template<typename T>
void wave_format<T>::get_header() {

    fin.open(file_input, std::ios_base::binary | std::ios_base::in);

    if (fin.is_open()) {

        uint8_t cur[4]{};

        while (cur[0] != 'd') {
            fin.read((char *) (&cur), sizeof(cur));
            if (cur[0] == 'R') { // in case "RIFF"
                fin.seekg(-sizeof(cur), std::ios_base::cur);
                fin.read((char *) (&header.riff), sizeof(header.riff));
            }
            if (cur[0] == 'f') { // in case "fmt "
                fin.seekg(-sizeof(cur), std::ios_base::cur);
                fin.read((char *) (&header.fmt), sizeof(header.fmt));
            }
            if (cur[0] == 'L') { // in case "LIST"
                fin.seekg(-sizeof(cur), std::ios_base::cur);
                fin.read((char *) (&header.list), sizeof(header.list));
            }
            if (cur[0] == 'd') { // in case "data"
                fin.seekg(-sizeof(cur), std::ios_base::cur);
                fin.read((char *) (&header.data), sizeof(header.data));
            }
        }

        HEADERSIZE = fin.tellg();
        std::cout << "HEADER SIZE       -> " << HEADERSIZE << " bytes" << std::endl;
        fin.close();
    } else
        throw std::runtime_error("Can't open " + file_input + "\n");

}
/*
 * ~~~~~Getting WAV header~~~~~
 */


/*
 * -----Getting WAV data-----
 */

template<typename T>
void wave_format<T>::get_one_channel() { // In case of 1 channel
    fin.open(file_input, std::ios_base::binary | std::ios_base::in);
    if (fin.is_open()) {
        if (HEADERSIZE) {
            fin.seekg(HEADERSIZE, std::ios_base::beg);      // Сдвигаемся на HEADERSIZE байт относительно начала файла,
                                                            // отсюда начинаем читать
            uint32_t size = header.data.subchunk2Size;      // Длина части в байтах, которую нужно считать
            data.resize(size / sizeof(T));
            fin.read((char *) data.data(), size);        // Читаем, начиная с HEADERSIZE байта
        }
    }
}

//TODO
template<typename T>
void wave_format<T>::get_two_channels() { // In case of 2 channels
    fin.open(file_input, std::ios_base::binary | std::ios_base::in);
    if (fin.is_open()) {

        fin.seekg(HEADERSIZE, std::ios_base::beg);  // Сдвигаемся на HEADERSIZE байт относительно начала файла,
                                                    // отсюда начинаем читать
        uint32_t size = header.data.subchunk2Size;  // Длина части в байтах, которую нужно считать

        std::vector<T> buf;
        buf.resize(size / sizeof(T));

        fin.read((char *) buf.data(), size);          // Читаем, начиная с HEADERSIZE байта
        data_LR.reserve(buf.size() / 2);

        LR<T> comp;
        for (size_t i = 0; i < buf.size(); i+=2) {
            comp.l = buf[i];
            comp.r = buf[i + 1];
            data_LR.push_back(comp);
        }
    }
}

template<typename T>
void wave_format<T>::get_data() {
    switch (header.fmt.numChannels) {
        case 1:
            get_one_channel(); // In case of 1 channel
            break;
        case 2:
            get_two_channels(); // In case of 2 channels
            break;

        default:
            throw std::runtime_error("Error! More than 2 channels!\n");
    }
}
/*
 * ~~~~~Getting WAV data~~~~~
 */


/*
 * ------Printing info about WAV file-----
 */
template<typename T>
void wave_format<T>::print_info() const {
    std::cout << "Size of the file  -> " << header.riff.chunkSize       << " bytes" << std::endl;
    std::cout << "Channels          -> " << header.fmt.numChannels      << std::endl;
    std::cout << "Sample rate       -> " << header.fmt.sampleRate       << " Hz"    << std::endl;
    std::cout << "Bits per sample   -> " << header.fmt.bitsPerSample    << " bit"   << std::endl;
    //TODO
    std::cout << "Type of the data  -> " << typeid(T).name()            << std::endl;

    float numSamples = (header.riff.chunkSize - HEADERSIZE) / (header.fmt.numChannels * header.fmt.bitsPerSample / 8);
    uint32_t SR = header.fmt.sampleRate;
    std::cout << "Num samples       -> " << numSamples                  << std::endl;
    std::cout << "Duration          -> " << numSamples / SR << " s"     << std::endl;
}

template<typename T>
void wave_format<T>::print_all_info() const {
    std::cout << "RIFF HEADER" << std::endl; // RIFF info
    std::cout << "chunkID       -> " << header.riff.chunkID[0]
              << header.riff.chunkID[1]
              << header.riff.chunkID[2]
              << header.riff.chunkID[3]
              << std::endl;

    std::cout << "chunkSize     -> " << header.riff.chunkSize << std::endl;

    std::cout << "format        -> " << header.riff.format[0]
              << header.riff.format[1]
              << header.riff.format[2]
              << header.riff.format[3]
              << std::endl;

    std::cout << std::endl;
    std::cout << "FMT SUBCHUNK" << std::endl; // fmt info

    std::cout << "subchunk1ID   -> " << header.fmt.subchunk1ID[0]
              << header.fmt.subchunk1ID[1]
              << header.fmt.subchunk1ID[2]
              << header.fmt.subchunk1ID[3]
              << std::endl;

    std::cout << "subchunk1Size -> " << header.fmt.subchunk1Size << std::endl;
    std::cout << "audioFormat   -> " << header.fmt.audioFormat << std::endl;
    std::cout << "numChannels   -> " << header.fmt.numChannels << std::endl;
    std::cout << "sampleRate    -> " << header.fmt.sampleRate << std::endl;
    std::cout << "byteRate      -> " << header.fmt.byteRate << std::endl;
    std::cout << "blockAlign    -> " << header.fmt.blockAlign << std::endl;
    std::cout << "bitsPerSample -> " << header.fmt.bitsPerSample << std::endl;

    if (header.list.subchunk3ID[0] == 'L') { // LIST info, optional
        std::cout << std::endl;
        std::cout << "LIST SUBCHUNK" << std::endl;

        std::cout << "subchunk3ID   -> " << header.list.subchunk3ID[0]
                  << header.list.subchunk3ID[1]
                  << header.list.subchunk3ID[2]
                  << header.list.subchunk3ID[3]
                  << std::endl;

        std::cout << "subchunk3Size -> " << header.list.subchunk3Size << std::endl;

        std::cout << "subchunk3INFO -> " << header.list.subchunk3INFO[0]
                  << header.list.subchunk3INFO[1]
                  << header.list.subchunk3INFO[2]
                  << header.list.subchunk3INFO[3]
                  << std::endl;

        std::cout << "subchunk3IGNR -> " << header.list.subchunk3IGNR[0]
                  << header.list.subchunk3IGNR[1]
                  << header.list.subchunk3IGNR[2]
                  << header.list.subchunk3IGNR[3]
                  << std::endl;
    }

    std::cout << std::endl;
    std::cout << "DATA SUBCHUNK" << std::endl; // Data info

    std::cout << "chunk2ID      -> " << header.data.subchunk2ID[0]
              << header.data.subchunk2ID[1]
              << header.data.subchunk2ID[2]
              << header.data.subchunk2ID[3]
              << std::endl;

    std::cout << "subchunk2Size -> " << header.data.subchunk2Size << std::endl;
    std::cout << std::endl;
}

template<typename T>
void wave_format<T>::print_data() const {
    if(!data.empty() && !data_LR.empty()) // Вдруг, оба вектора не пустые 0_о
        throw std::runtime_error("!data.empty() && !data_LR.empty()\n");

    if(!data.empty()) {
        if (std::is_same<T, int32_t>::value || std::is_same<T, float>::value) {
            for (const auto &i: data) {
                std::cout << i << std::endl;
            }
        }
        else {
            throw std::runtime_error("Error! Bad type T!\n");
        }
    } else {

        if (!data_LR.empty()) {
            for (const auto &j: data_LR) {
                std::cout << "l: " << j.l << " r: " << j.r << std::endl;
            }
        } else
            throw std::runtime_error("Error! Data is empty!\n");
    }
}


/*
 * ~~~~~Printing info about WAV file~~~~~
 */



#endif //STC_WAVE_HPP