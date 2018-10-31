#include <TXLib.h>

#include "../../AKTools/akio.h"
#include <cstdlib>
#include <dirent.h>

const char* const TempFName = "processorUTTempFile.txt";
const size_t TempFNameLen = 25;

int main(){
    chdir("./processorTests/");

    DIR* dir = opendir(".");

    for(dirent* curFile = readdir(dir); curFile != NULL; curFile = readdir(dir)){
        char* dotPointer = strrchr(curFile -> d_name, '.');
        if(!dotPointer) continue;
        if(strcmp(dotPointer, ".akprc") != 0) continue;

        char* resFName = (char*) calloc(dotPointer - curFile->d_name + 4 + 1, 1);
        strncpy(resFName, curFile->d_name, dotPointer - curFile->d_name);
        strcpy(resFName + (dotPointer - curFile->d_name), ".txt");

        FILE* resF = fopen(resFName, "rb");
        char* resFIn = (char*) writeFileToBuf(resF).first;

        char* inpFName = (char*) calloc(dotPointer - curFile->d_name + 7 + 1, 1);
        strncpy(inpFName, curFile->d_name, dotPointer - curFile->d_name);
        strcpy(inpFName + (dotPointer - curFile->d_name), ".inpprc");

//        printf("Before inpF\n");

        FILE* inpF = fopen(inpFName, "r");
        char* inpFStr = 0;
        if(inpF){
            inpFStr = (char*) calloc(dotPointer - curFile->d_name + 7 + 1 + 3, 1);
            strcpy(inpFStr, " < ");
            strcpy(inpFStr + 3, inpFName);
        }

//        printf("After inpF: inpF = 0x%p\nnext: calloc(%d, 1);\n", inpF, dotPointer - curFile->d_name + 6 + TempFNameLen + 3);

        char* cmd = (char*) calloc( (dotPointer - curFile->d_name)*2 + 6 + 7 + TempFNameLen + 6 + 1, 1 );

//        printf("1\n");

        sprintf(cmd, "%s %s %s", curFile->d_name, TempFName, inpF? inpFStr : "");

//        printf("2: cmd = '%s'\n", cmd);

        system(cmd);

//        printf("After system\n");

        FILE* outF = fopen(TempFName, "rb");
        char* outFIn = (char*) writeFileToBuf(outF).first;

        if(strcmp(outFIn, resFIn) != 0){ $e printf("\"%s\" failed\n",    curFile->d_name); }
        else                           { $o printf("\"%s\" succeeded\n", curFile->d_name); }
        $d

//        printf("Before fclose's\n");

        fclose(resF);
        fclose(outF);
        if(inpF) fclose(inpF);
    }

    closedir(dir);
    chdir("../");
}
