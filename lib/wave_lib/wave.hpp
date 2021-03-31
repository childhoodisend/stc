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


struct wave_format{
public:
    wave_format()   = default;
    ~wave_format()  = default;

/*
 * -----Interface-----
 */
    void wav_reader();              // Read  WAVE file
    void print_info()        const; // Printing main info about WAVE file
    void print_header_info() const; // Printing header info of WAVE file
    void print_data();              // Printing data of WAVE file

//    void wav_saver(std::string &fout); // Save WAVE to file
 /*
 * ~~~~~Interface~~~~~
 */

private: // Private methods
    void get_header();
    void get_data();

    template<typename T>
    void get_one_channel(std::vector<T> &data);

    template<typename T>
    void get_two_channels(std::vector<LR<T>> &data_LR);

    template<typename T>
    void printer(const std::vector<T> &data) const;

    template<typename T>
    void printer(const std::vector<LR<T>> &data_LR) const;

private: // Work with files
    const std::string file_input  = "../source/Wave/TestWAV.wav";
    const std::string file_output = "../source/Wave/output.bin";
    std::fstream fin;
    std::ofstream fout;

private:
    size_t              HEADERSIZE{};
    wave_header         header{};

    std::vector<int8_t>      data_i8{};
    std::vector<LR<int8_t>>  data_LR_i8{};

    std::vector<int16_t>     data_i16{};
    std::vector<LR<int16_t>> data_LR_i16{};

    std::vector<int32_t>     data_i32{};
    std::vector<LR<int32_t>> data_LR_i32{};

    std::vector<float>       data_f{};
    std::vector<LR<float>>   data_LR_f{};
};


/*
 *  Реализация функций
 */


void wave_format::wav_reader() { // Read WAV file!
    get_header(); // Firstly, read the header
    get_data();   // Secondly, read the data after the header
}

/*
 * -----Getting WAV header-----
 */

void wave_format::get_header() {

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
void wave_format::get_one_channel(std::vector<T> &data) { // In case of 1 channel
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

template<typename T>
void wave_format::get_two_channels(std::vector<LR<T>> &data_LR) { // In case of 2 channels
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


void wave_format::get_data() {
    if (header.fmt.numChannels == 1) {
        if(header.fmt.audioFormat == 1) { // If PCM
            if (header.fmt.bitsPerSample == 8) { // If int8_t
                std::cout << "Getting int8_t data, 1 channel...\n\n";
                get_one_channel(data_i8);
            }

            if (header.fmt.bitsPerSample == 16) { // If int16_t
                std::cout << "Getting int16_t data, 1 channel...\n\n";
                get_one_channel(data_i16);
            }

            if (header.fmt.bitsPerSample == 32) { // If int32_t
                std::cout << "Getting int32_t data, 1 channel...\n\n";
                get_one_channel(data_i32);
            }
        }
        else {
            if (header.fmt.audioFormat == 3) { // If IEEE float
                std::cout << "Getting float data, 1 channel...\n\n";
                get_one_channel(data_f);
            }
            else {
                throw std::runtime_error("Error! One channel, but audioFormat is not PCM or IEEE float!\n");
            }
        }
    }
    else {
        if (header.fmt.numChannels == 2) {
            if(header.fmt.audioFormat == 1) { // If PCM
                if (header.fmt.bitsPerSample == 8) { // If int8_t
                    std::cout << "Getting int8_t data, 2 channels...\n\n";
                    get_two_channels(data_LR_i8);
                }

                if (header.fmt.bitsPerSample == 16) { // If int16_t
                    std::cout << "Getting int16_t data, 2 channels...\n\n";
                    get_two_channels(data_LR_i16);
                }

                if (header.fmt.bitsPerSample == 32) { // If int32_t
                    std::cout << "Getting int32_t data, 2 channels...\n\n";
                    get_two_channels(data_LR_i32);
                }
            }
            else {
                if (header.fmt.audioFormat == 3) { // If IEEE float
                    std::cout << "Getting float data, 2 channels...\n\n";
                    get_one_channel(data_LR_f);
                }
                else {
                    throw std::runtime_error("Error! Two channels, but audioFormat is not PCM or IEEE float!\n");
                }
            }
        }
        else {
            throw std::runtime_error("Error! More than 2 channels!\n");
        }
    }
}
/*
 * ~~~~~Getting WAV data~~~~~
 */


/*
 * ------Printing info about WAV file-----
 */

void wave_format::print_info() const {

    std::cout << "Size of the file  -> " << header.riff.chunkSize       << " bytes" << std::endl;
    std::cout << "Channels          -> " << header.fmt.numChannels      << std::endl;
    std::cout << "Sample rate       -> " << header.fmt.sampleRate       << " Hz"    << std::endl;
    std::cout << "Bits per sample   -> " << header.fmt.bitsPerSample    << " bit"   << std::endl;

    std::string name = (header.fmt.audioFormat == 1 ? "INT" : (header.fmt.audioFormat == 3 ? "FLOAT" : "BADTYPE"));
    std::cout << "Type of the data  -> " <<   name   << std::endl;

    float numSamples = (header.riff.chunkSize - HEADERSIZE) / (header.fmt.numChannels * header.fmt.bitsPerSample / 8);
    uint32_t SR = header.fmt.sampleRate;
    std::cout << "Num samples       -> " << numSamples                  << std::endl;
    std::cout << "Duration          -> " << numSamples / SR << " s"     << std::endl;
}


void wave_format::print_header_info() const {
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
void wave_format::printer(const std::vector<T> &data) const{
    for (const auto &i: data) {
        std::cout << i << std::endl;
    }
}

template<typename T>
void wave_format::printer(const std::vector<LR<T>> &data_LR) const{
    for (const auto &j: data_LR) {
        std::cout << "l: " << j.l << " r: " << j.r << std::endl;
    }
}

void wave_format::print_data(){
    if(!data_i8.empty())     printer(data_i8);
    if(!data_LR_i8.empty())  printer(data_LR_i8);
    if(!data_i16.empty())    printer(data_i16);
    if(!data_LR_i16.empty()) printer(data_LR_i16);
    if(!data_i32.empty())    printer(data_i32);
    if(!data_LR_i32.empty()) printer(data_LR_i32);
    if(!data_f.empty())      printer(data_f);
    if(!data_LR_f.empty())   printer(data_LR_f);
}

/*
 * ~~~~~Printing info about WAV file~~~~~
 */



#endif //STC_WAVE_HPP