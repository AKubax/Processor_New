#include "../AKTools/akio.h"

int main(){
    char c = 1;
    FILE* f = fopen("./noCode/proc.txt", "wb");
    writeBufToFile(&c, 1, f);
}
