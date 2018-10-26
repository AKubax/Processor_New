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

        char* resFName = (char*) calloc(dotPointer - curFile->d_name + 4, 1);
        strncpy(resFName, curFile->d_name, dotPointer - curFile->d_name);
        strcpy(resFName + (dotPointer - curFile->d_name), ".txt");

        FILE* resF = fopen(resFName, "rb");
        char* resFIn = (char*) writeFileToBuf(resF).first;

        char* cmd = (char*) calloc( dotPointer - curFile->d_name + 6 + TempFNameLen + 3, 1 );
        sprintf(cmd, "%s %s", curFile->d_name, TempFName);
        system(cmd);

        FILE* outF = fopen(TempFName, "rb");
        char* outFIn = (char*) writeFileToBuf(outF).first;

        if(strcmp(outFIn, resFIn) != 0){ $e printf("\"%s\" failed\n",    curFile->d_name); }
        else                           { $o printf("\"%s\" succeeded\n", curFile->d_name); }
        $d

        fclose(resF);
        fclose(outF);
    }

    closedir(dir);
    chdir("../");
}
