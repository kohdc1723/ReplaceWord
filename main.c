#include <stdio.h>
#include <string.h>
#include <conio.h>
#define BUFFER_SIZE 512
#define WORD_SIZE 50

void replaceWordAndUpdateFile(int argCount, char* inputFileName);
FILE* readNextLineAndReturnTempFile(FILE* inputFile_ptr, FILE* tempFile_ptr, char* oldWord, char* newWord);
void removeAndRename(char* tempFileName, char* inputFileName);
int readOldAndNewWord(char* oldWord, char* newWord);
int validateFiles(FILE* inputFile_ptr, FILE* tempFile_ptr);
void closeFiles(FILE* inputFile_ptr, FILE* tempFile_ptr);
int replaceWordAndWriteFile(char* buffer, char* oldWord, char* newWord, FILE* tempFile_ptr);

int main(int argc, char* argv[]) {
    int argCount = argc;
    char* inputFileName = argv[1];
    replaceWordAndUpdateFile(argCount, inputFileName);
    getch();
    return 0;
}

// replace old word to new word and update file
void replaceWordAndUpdateFile(int argCount, char* inputFileName) {
    char oldWord[WORD_SIZE];
    char newWord[WORD_SIZE];
    if (argCount == 2) {
        char* tempFileName = "temp.txt";
        FILE* inputFile_ptr = fopen(inputFileName, "r");
        FILE* tempFile_ptr = fopen(tempFileName, "w");
        if (!validateFiles(inputFile_ptr, tempFile_ptr)) {
            printf("Could not open files!\n");
        } else {
            if (!readOldAndNewWord(oldWord, newWord)) {
                printf("Old word is the same as new word!\n");
                closeFiles(inputFile_ptr, tempFile_ptr);
            } else {
                tempFile_ptr = readNextLineAndReturnTempFile(inputFile_ptr, tempFile_ptr, oldWord, newWord);
                closeFiles(inputFile_ptr, tempFile_ptr);
                removeAndRename(tempFileName, inputFileName);
            }
        }
    } else {
        printf("Invalid CommandLine Input!\n");
    }
}

// read each line and then return updated temp file
FILE* readNextLineAndReturnTempFile(FILE* inputFile_ptr, FILE* tempFile_ptr, char* oldWord, char* newWord) {
    char buffer[BUFFER_SIZE];
    int count = 0;
    while (fgets(buffer, BUFFER_SIZE, inputFile_ptr) != NULL) {
        count += replaceWordAndWriteFile(buffer, oldWord, newWord, tempFile_ptr);
    }
    if (count == 0) {
        printf("Could not find \'%s\'!\n", oldWord);
    } else {
        printf("Successfully replaced all %d occurrences of \'%s\' with \'%s\'!\n", count, oldWord, newWord);
    }
    return tempFile_ptr;
}

// remove original file and rename temporary file
void removeAndRename(char* tempFileName, char* inputFileName) {
    remove(inputFileName);
    rename(tempFileName, inputFileName);
}

// read old word and new word
int readOldAndNewWord(char* oldWord, char* newWord) {
    printf("Enter word to replace:");
    scanf("%s", oldWord);
    getchar();
    printf("Replace \'%s\' with:", oldWord);
    scanf("%s", newWord);
    if (strcmp(oldWord, newWord) == 0) {
        return 0;
    } else {
        return 1;
    }
}

// validate files => print error message if NULL
int validateFiles(FILE* inputFile_ptr, FILE* tempFile_ptr) {
    if (inputFile_ptr == NULL || tempFile_ptr == NULL) {
        return 0;
    } else {
        return 1;
    }
}

// close input file and temp file
void closeFiles(FILE* inputFile_ptr, FILE* tempFile_ptr) {
    fclose(inputFile_ptr);
    fclose(tempFile_ptr);
}

// replace word and return the number of replacement
int replaceWordAndWriteFile(char* buffer, char* oldWord, char* newWord, FILE* tempFile_ptr) {
    char tempBuffer[BUFFER_SIZE] = {'\0'};
    char* nextOldWord_ptr;
    int i = 0;
    int count = 0;
    while ((nextOldWord_ptr = strstr(&buffer[i], oldWord)) != NULL) {
        count++;
        while (&buffer[i] != nextOldWord_ptr) {
            strncat(tempBuffer, &buffer[i], 1);
            i++;
        }
        strcat(tempBuffer, newWord);
        i += (int) strlen(oldWord);
    }
    strncat(tempBuffer, &buffer[i], strlen(buffer) - i);
    fputs(tempBuffer, tempFile_ptr);
    return count;
}
