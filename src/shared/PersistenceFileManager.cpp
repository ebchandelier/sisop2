#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

#include "datagram.h"

class PersistenceFileManager {

public:
    
    std::vector<datagram> read(std::string fileName) {

        char memblock[sizeOfFile(fileName)];
        std::vector<datagram> datagramVector;
        std::streampos size = readToBuffer (fileName, memblock);

        if(size < 0) {
            
            std::cout << "\nError: could not read file " << fileName << "\n";
            return datagramVector;

        } else {

            int indexToStartReading = 0;

            while(size > DATA_BUFFER_SIZE) {

                datagram dataGram = createSingleDatagram(DATA_BUFFER_SIZE, memblock, indexToStartReading++);

                datagramVector.push_back(dataGram);

                size -= DATA_BUFFER_SIZE;
            }

            if(size > 0) {

                datagram dataGram = createSingleDatagram(size, memblock, indexToStartReading++);

                datagramVector.push_back(dataGram);
            }

        }

        if(size == 0) {

            datagram theOnlyOne = createEmptyDatagram();
            datagramVector.push_back(theOnlyOne);
        }
        
        datagramVector.back().data.is_last = true;

        return datagramVector;
    }
    
    void write(std::string fileName, std::vector<datagram> data) {
        
        int size = 0;

        for (auto datagram : data) {
            
            size += datagram.data.data_length;
        }

        char buffer[size];

        for( int i=0; i<data.size(); i++) {
            
            int dataSize = data.at(i).data.data_length;
            memcpy(buffer + i*DATA_BUFFER_SIZE, data.at(i).data.buffer, dataSize);
        }

        std::ofstream file (fileName, std::ios::out| std::ios::ate);
        
        if (file.is_open()) {

            file.write(buffer, size);
        }

        file.close();
    }

private:

    std::streampos readToBuffer(std::string fileName, char* buffer) {
        
        std::streampos size = -1;
        std::ifstream file (fileName, std::ios::in| std::ios::ate);
        
        if (file.is_open()) {

            size = file.tellg();
            file.seekg (0, std::ios::beg);
            file.read (buffer, size);
            file.close();
        }

        return size;
    }

    datagram createSingleDatagram(std::streampos size, char* memblock, int indexToStartReading) {
        
        data_datagram data;
        data.data_length = size;

        data.package_id = indexToStartReading;
        std::memcpy(data.buffer, memblock + indexToStartReading*DATA_BUFFER_SIZE, size);

        datagram dataGram;
        dataGram.type = datagram_type::data;
        dataGram.data = data;
        dataGram.data.is_last = false;

        return dataGram;
    }

    datagram createEmptyDatagram() {
        
        data_datagram data;
        data.data_length = 0;

        datagram dataGram;
        dataGram.type = datagram_type::data;
        dataGram.data = data;
        dataGram.data.is_last = false;

        return dataGram;
    }

    int sizeOfFile(std::string fileName) {
        
        std::streampos size = 0;
        std::ifstream file (fileName, std::ios::in| std::ios::ate);
        
        if (file.is_open()) {

            size = file.tellg();
            file.close();
        }

        return size;
    }
};
