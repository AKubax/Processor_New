#include <TXLib.h>

#include "../Compiler.h"

#include <algorithm>


using myPair = std::pair<const char*, size_t>;

std::map<const char*, size_t> doTest(char* inp);
std::map<const char*, size_t> doTest(char* inp){ return makeLabels(tokenize(inp)); }

bool comparePairKeys(myPair p1, myPair p2);
bool comparePairKeys(myPair p1, myPair p2){ return (strcmp(p1.first, p2.first) < 0); }

bool comparePairs(myPair p1, myPair p2);
bool comparePairs(myPair p1, myPair p2){ return ( strcmp(p1.first, p2.first) == 0 && p1.second == p2.second); }


bool compareMaps(std::map<const char*, size_t> m1, std::map<const char*, size_t> m2);
bool compareMaps(std::map<const char*, size_t> m1, std::map<const char*, size_t> m2){
    if(m1.size() != m2.size()){
        printd("Sizes are not equal\n");
        return 0;
    }

    myPair arr1[m1.size()], arr2[m2.size()];

    std::copy(m1.begin(), m1.end(), arr1);
    std::copy(m2.begin(), m2.end(), arr2);

    std::sort(arr1, arr1 + m1.size(), comparePairKeys);
    std::sort(arr2, arr2 + m2.size(), comparePairKeys);

    for(size_t i = 0; i < m1.size(); i++){
        if( !comparePairs(arr1[i], arr2[i]) ) return false;
    }

    return true;
}


char* unConst(const char* inp);    //const char* -> char* by copying
char* unConst(const char* inp){
    char* res = (char*) calloc(strlen(inp), 1);
    strcpy(res, inp);

    return res;
}

myPair mkP(const char* f, size_t s);
myPair mkP(const char* f, size_t s) { return myPair(f, s); }

std::map<const char*, size_t> mkM(int n, ...);
std::map<const char*, size_t> mkM(int n, ...){
    va_list args;
    va_start(args, n);

    std::map<const char*, size_t> res;

    while(n--){
        myPair arg = va_arg(args, myPair);

        res[arg.first] = arg.second;
    }
    va_end(args);

    return res;
}

#define makeTest(num, str, ...) { tests[num - 1] = unConst(str); answers[num - 1] = mkM(__VA_ARGS__); }

//makeTest(3, "INPUT", 2, mkP("l1", 2), mkP("l2", 13));


int main(){
    const int numTests = 3;
    char* tests[numTests] = {};
    std::map<const char*, size_t> answers[numTests] = {};

//---TESTS---     all the UTs are described here ||| always remember to change numTests

    makeTest(1, "DEBUG_INSERT_BYTES 5 :LABEL EXIT", 1, mkP(":LABEL", 5));
    makeTest(2, ":l1 DEBUG_INSERT_BYTES 0 :l2", 2, mkP(":l1", 0), mkP(":l2", 0));
    makeTest(3, ":l1 DEBUG_INSERT_BYTES 8 :l2 DEBUG_INSERT_BYTES 13 :l3", 3, mkP(":l1", 0), mkP(":l2", 8), mkP(":l3", 21));

//-----------
    for(int i = 0; i < numTests; i++){
        $y printf("Test #%d: input = '%s'\n", i, tests[i]); $d;

        auto res = doTest(tests[i]);
        if( !compareMaps(res, answers[i]) ) { $r printf("UNIT TEST FAILED\n"); }
        else                                { $g printf("Unit test passed\n"); }

        printf("output =\n{");
        for(auto it = res.begin(); it != res.end(); it++){
            printf("\t'%s' == %u\n", (*it).first, (*it).second);
        }
        printf("}\n");
        $d
        printf("-=-=-=-\n");
    }
}
