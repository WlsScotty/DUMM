#pragma once
// Program designed to return heap products
#include <fcntl.h> // Used for future memory manipulation
#include <thread> // Used for eventual multithreading
#include <unistd.h> // Used for future libraries in advancing this header
#include <cstdio> // General library
#include <iostream> // General library

    // Bytes that are read get allocated to a "chunkByte", which is just a way to store offsets and size easily.
struct chunkByte{ 
    long unsigned int size;
    long unsigned int offset;
    int* bytesRead;
}; 

class Process{ // The loaded process is manipulated as a class
int o = 0;
public:
    
    ssize_t Handle;

    ~Process(){ // Deconstructor for cleanup.
    close(Handle);
    }
    
    /* ALL FUNCTIONS DECLARED HERE ARE FOR READING FUNCTIONALITY */
    
    chunkByte* readBytes(long unsigned int size,long unsigned int Offset){
        lseek(this->Handle,Offset,SEEK_SET);
        
        int* bytesRead = new int[size]; 
        read(this->Handle,bytesRead,size * sizeof(int)); 
        
        if(*bytesRead == -1){
            perror("read");
            delete[] bytesRead;
        }
        
        chunkByte *chunk = new chunkByte; //Assinging to a "chunkByte", which gives better memory manipulation when it comes to writing to memory.
        chunk->bytesRead = bytesRead;
        chunk->offset = Offset;
        chunk->size = size;
        
        delete[] bytesRead;
        return *chunk; // return the streamed, read bytes
    }
    
    /* ALL FUNCTIONS BELOW REPRESENT THE WRITING FUNCTIONALITY */
    
    
  // This function utilizes the "chunkByte" struct as a method of modifying data
  // I wuold reccomend this method as it autonomously manages the offset itself.
    int pushBytes(chunkByte *Byte){
        lseek(this->Handle,Byte->offset,SEEK_SET);
        int w = write(this->Handle,Byte->bytesRead,Byte->size);
        
        delete[] Byte;
    return w;
    }
    
    int changeByte(long unsigned int offset,int value){ // Changes a singular byte itself.
        int lsk = lseek(this->Handle,offset,SEEK_SET);

        int w = write(this->Handle,&value,(sizeof(value))); 
        return w;
    }
    
    ssize_t streamBytes(long unsigned int startOffset, long unsigned int endOffset){
        if(o+startOffset <= endOffset){
            
            std::cout << o << "\n";
            o++;
            return lseek(this->Handle,startOffset+o,sizeof(int));
        } 
        o = 0;
        return -1;
    }
};


Process* MountProcess(const char* procName){ // mount is for getting the file itself to mount into memory
    ssize_t f = open(procName,O_RDWR);
    
    if(f == -1){ // Only returns -1 if it was unsucessful
        std::cout << "Error! Couldn't find process!"; 
        close(f);
        return nullptr;
    }
Process *proc = new Process(); // We create a new heap process, allowing for manual lifetime management
proc->Handle = f; // assigning the handle to the one that was seeked

return proc; // we return the process
}

 
