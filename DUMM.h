#pragma once

#ifdef _WIN32


#else
#include <unistd.h>

#endif

// Program designed to return heap products
#include <fcntl.h> // Used for future memory manipulation
#include <thread> // Used for eventual multithreading
#include <cstdio> // General library
#include <iostream> // General library
#include <cstring>

    // Bytes that are read get allocated to a "sbyte", which is just a smart-byte
struct sbyte{
    long unsigned int size;
    long unsigned int offset;
    int* bytesRead;

    ~sbyte(){
        delete bytesRead;

    }
};

class Process{ // The loaded process is manipulated as a class

    int o = 0;

public:

    const ssize_t Handle;
    const char* procName;

    Process(ssize_t handle) : Handle(handle) {}

    ~Process(){ // Deconstructor for cleanup.
    close(Handle);
    }

    /* ALL FUNCTIONS DECLARED HERE ARE FOR READING FUNCTIONALITY */

    sbyte* readByte(long unsigned int offset, long unsigned int size) {

    if (offset != -1){
    lseek(this->Handle, offset, SEEK_SET);
    }

    unsigned char* bytes = new unsigned char[size];  // Corrected to use unsigned char*
    ssize_t bytes_read = read(this->Handle, bytes, size);  // Corrected type of bytesRead

    if (bytes_read == -1) {
        perror("read");
        delete[] bytes;

        std::cout << "ERROR: Cannot read";
        return nullptr;  // Return nullptr or handle error appropriately
    }

    sbyte* chunk = new sbyte;

    chunk->bytesRead = (int*)bytes;  // Store bytes read count (not bytes themselves)
    chunk->offset = offset;
    chunk->size = size;

    std::cout << bytes_read << " bytes read\n";  // Print the number of bytes read
    return chunk;
}

    /* ALL FUNCTIONS BELOW REPRESENT THE WRITING FUNCTIONALITY */

  // This function utilizes the "sbyte" struct as a method of modifying data
  // I wuold reccomend this method as it autonomously manages the offset itself.
    int pushBytes(sbyte* Byte){



        lseek(this->Handle,Byte->offset,SEEK_SET);
        std::cout << "\n" << Byte->offset << " " << Byte->bytesRead << " | ";
        int w = write(this->Handle,Byte->bytesRead,Byte->size);

    return w;
    }

    int changeByte(long unsigned int offset,int value){ // Changes a singular byte itself.

        int lsk = lseek(this->Handle,offset,SEEK_SET);
        int w = write(this->Handle,&value,(sizeof(value)));

        return w;
    }

    int streamBytes(long unsigned int startOffset, long unsigned int endOffset){
        if(o+startOffset != endOffset){
            std::cout << o << "\n";

            o++;
            return lseek(this->Handle,startOffset+o,sizeof(int));
        }
        o = 0;

        return -1;
    }
};


Process* MountProcess(const char* procName){ // mount is for getting the file itself to mount into memory
    // Exception checks
    if (procName[0] == 0 || procName == nullptr){
        std::cout << "Error! No path name specified!";
    }
    ssize_t f = 0;

    if (access(procName,R_OK) == -1){
        std::cout << "Note: Could not read to \"" << procName << "\", Do you have permission? \n";
            f = open(procName,O_WRONLY);
    }

    if (access(procName,W_OK) == -1){
        std::cout << "Note: Could not write to \"" << procName << "\", Do you have permission? \n";
            f = open(procName,O_RDONLY);
    }

    if (f == 0){
            f = open(procName,O_RDWR);

    }

    if(f == -1){ // Only returns -1 if it was unsucessful
        std::cout << "Error! Couldn't access process!";

        return nullptr;
    }

    // If all conditions are true

return new Process(f); // we return the process
}


