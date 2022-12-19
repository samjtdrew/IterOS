#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint8_t BootJumpInstruction[3];
    uint8_t OemIdentifier[8];
    uint16_t BytesPerSector;
    uint8_t SectorsPerCluser;
    uint16_t ReservedSectors;
    uint8_t FatCount;
    uint16_t DirEntryCount;
    uint16_t TotalSectors;
    uint8_t MediaDescriptorType;
    uint16_t RSectorsPerFat;
    uint16_t SectorsPerTrack;
    uint16_t Heads;
    uint32_t HiddenSectors;
    uint32_t LargeSectorCount;

    //ext boot record
    uint32_t SectorsPerFat;
    uint16_t flags;
    uint16_t FatVersion;
    uint32_t RootClusterNumber;
    uint16_t FileSystemInformationSector;
    uint16_t BackupBootSector;
    uint8_t ReservedFatNull[12];
    uint8_t DriveNumber;
    uint8_t WindowsNTFlags;
    uint8_t Signature;
    uint32_t VolumeId;
    uint8_t VolumeLabel[11];
    uint8_t SystemId[8];
} __attribute__((packed)) BootSector;

typedef struct {
    uint8_t Name[11];
    uint8_t Attributes;
    uint8_t _Reserved;
    uint8_t CreatedTimeTenths;
    uint16_t CreatedTime;
    uint16_t CreatedDate;
    uint16_t AccessedDate;
    uint16_t FirstClusterHigh;
    uint16_t ModifiedTime;
    uint16_t ModifiedDate;
    uint16_t FirstClusterLow;
    uint32_t Size;
} __attribute__((packed)) DirectoryEntry;

BootSector gBootSector;
uint8_t* gFat = NULL;
DirectoryEntry* gRootDirectory = NULL;

int readBootSector(FILE *disk) {
    return fread(&gBootSector, sizeof(gBootSector), 1, disk) > 0;
}

int readSectors(FILE* disk, uint32_t lba, uint32_t count, void* bufferOut) {
    int ok = 0;
    ok = ok && (fseek(disk, lba * gBootSector.BytesPerSector, SEEK_SET) == 0);
    ok = ok && (fread(bufferOut, gBootSector.BytesPerSector, count, disk) == count);
    return ok;
}

int readFat(FILE* disk) {
    gFat = (uint8_t*) malloc(gBootSector.SectorsPerFat * gBootSector.BytesPerSector);
    return readSectors(disk, gBootSector.ReservedSectors, gBootSector.SectorsPerFat, gFat);
}

int readRootDirectory(FILE* disk) {
    uint32_t lba = gBootSector.ReservedSectors + gBootSector.SectorsPerFat * gBootSector.FatCount;
    uint32_t size = sizeof(DirectoryEntry) * gBootSector.DirEntryCount;
    uint32_t sectors = (size / gBootSector.BytesPerSector);
    if (size % gBootSector.BytesPerSector > 0) {
        sectors++;
    }
    gRootDirectory = (DirectoryEntry*) malloc(sectors * gBootSector.BytesPerSector);
    return readSectors(disk, lba, sectors, gRootDirectory);
}

DirectoryEntry* findFile(const char* name, ) {
    for (uint32_t i = 0; i < gBootSector.DirEntryCount; i++) {
        if (memcmp(name, gRootDirectory[i].Name, 11) == 0) {
            return &gRootDirectory[i];
        }
    }
    return NULL;
}

int fetch(char **argv) {

    FILE* disk = fopen(argv[1], "rb");
    if (!disk) {
        fprintf(stderr, "Cannot open disk image %s!", argv[1]);
        return -1;
    }

    if (!readBootSector(disk)) {
        fprintf(stderr, "Could not read boot sector!\n");
        return -2;
    }

    if (!readFat(disk)) {
        fprintf(stderr, "Could not read FAT!\n");
        free(gFat);
        return -3;
    }

    if(!readRootDirectory(disk)) {
        fprintf(stderr, "Could not read FAT!\n");
        free(gFat);
        free(gRootDirectory);
        return -4;
    }

    DirectoryEntry* fileEntry = findFile(argv[2]);
    if (!fileEntry) {
        fprintf(stderr, "Could not find file %s!\n", argv[2]);
        free(gFat);
        free(gRootDirectory);
        return -5;
    }

    free(gFat);
    free(gRootDirectory);

    return 0;
}