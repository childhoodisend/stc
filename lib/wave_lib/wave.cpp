//
// Created by artem on 19.03.2021.
//

#include "wave.h"
#include <iostream>
#include <fstream>
#include <types.hpp>
#include <vector>

static size_t HEADERSIZE = 0;

void wave_format::wav_reader() {
    get_header();
    get_data();
}

void wave_format::get_header() {

    fin.open(file_input, std::ios_base::binary | std::ios_base::in);

    if(fin.is_open()){

        char cur[4];

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
        std::cout <<"HEADER SIZE -> "<< HEADERSIZE << std::endl;
        fin.close();
    }
    else
        throw  std::runtime_error("Can't open " + file_input + "\n");

}
//TODO
void wave_format::get_one_channel() {
    fin.open(file_input, std::ios_base::binary | std::ios_base::in);
    if(fin.is_open()) {

        fin.seekg(HEADERSIZE, std::ios_base::beg);// Сдвигаемся на HEADERSIZE байт, отсюда начинаем читать

        uint32_t size = header.data.subchunk2Size;// Длина части в байтах, которую нужно считать

        fin.read((char*)data_f.data(), size);// Читаем, начиная с HEADERSIZE байта

    }
}

//TODO
void wave_format::get_two_channels(){
    fin.open(file_input, std::ios_base::binary | std::ios_base::in);
    if(fin.is_open()) {

        fin.seekg(HEADERSIZE, std::ios_base::beg);// Сдвигаемся на HEADERSIZE байт, отсюда начинаем читать

        uint32_t size = header.data.subchunk2Size;// Длина части в байтах, которую нужно считать

        std::vector<float> buf;
        buf.resize(size/sizeof (float));

        fin.read((char*)buf.data(), size);// Читаем, начиная с HEADERSIZE байта

    }
}

void wave_format::get_data() {
//    switch (header.fmt.numChannels) {
//        case 1:
//            get_one_channel();
//            break;
//        case 2:
//            get_two_channels();
//            break;
//
//        default:
//            throw std::runtime_error("Error! More than 2 channels!\n");
//    }
}

void wave_format::print_info() const {
    std::cout << "Size of the file -> " << header.riff.chunkSize << " bytes" << std::endl;
    std::cout << "Channels -> " << header.fmt.numChannels << std::endl;
    std::cout << "Sample rate -> " << header.fmt.sampleRate << " Hz" << std::endl;
    std::cout << "Bits per sample -> " << header.fmt.bitsPerSample << " bit" << std::endl;
    //TODO
    std::cout << "Type of the data -> ?" << std::endl;

    float numSamples =  (header.riff.chunkSize - HEADERSIZE) / (header.fmt.numChannels * header.fmt.bitsPerSample/8);
    std::cout << "Num samples " << numSamples << std::endl;
    std::cout << "Duration -> " << numSamples / header.fmt.sampleRate << " s" << std::endl;
}
std::string get_name(char A[4]){
    return reinterpret_cast<const char *>(A);
}
void wave_format::print_all_info() const {
    std::cout << "RIFF HEADER" << std::endl;

    std::cout << "chunkID -> " << header.riff.chunkID[0] + header.riff.chunkID[1] + header.riff.chunkID[2] + header.riff.chunkID[3] << std::endl;
    std::cout << "chunkSize -> " << header.riff.chunkSize << std::endl;
    std::cout << "format -> " << header.riff.format << std::endl;

    std::cout << std::endl;
    std::cout << "FMT SUBCHUNK" << std::endl;
    std::cout << "subchunk1ID -> " << header.fmt.subchunk1ID[0] + header.fmt.subchunk1ID[1] + header.fmt.subchunk1ID[2] + header.fmt.subchunk1ID[3] << std::endl;
    std::cout << "subchunk1Size -> " << header.fmt.subchunk1Size << std::endl;
    std::cout << "audioFormat -> " << header.fmt.audioFormat << std::endl;
    std::cout << "numChannels -> " << header.fmt.numChannels << std::endl;
    std::cout << "sampleRate -> " << header.fmt.sampleRate << std::endl;
    std::cout << "byteRate -> " << header.fmt.byteRate << std::endl;
    std::cout << "blockAlign -> " << header.fmt.blockAlign << std::endl;
    std::cout << "bitsPerSample -> " << header.fmt.bitsPerSample << std::endl;

    std::cout << std::endl;
    std::cout << "LIST SUBCHUNK" << std::endl;
    std::cout << "subchunk3ID -> " << header.list.subchunk3ID[0] + header.list.subchunk3ID[1] + header.list.subchunk3ID[2] + header.list.subchunk3ID[3] << std::endl;
    std::cout << "subchunk3Size -> " << header.list.subchunk3Size << std:: endl;

    std::cout << std::endl;
    std::cout << "DATA SUBCHUNK" << std::endl;
    std::cout << "chunk2ID -> " << header.data.subchunk2ID[0] + header.data.subchunk2ID[1] + header.data.subchunk2ID[2] + header.data.subchunk2ID[3] << std::endl;
    std::cout << "subchunk2Size -> " << header.data.subchunk2Size << std::endl;
    std::cout << std::endl;
}

