#include "xorshift.hpp"
#include <cstdio>
#include <string>
#include <random>

int main() {
    printf("random::splitmix64\n");
    
    random::splitmix64 sm64(0);
    
    for (int i = 0; i < 10; i++) {
        printf("[%d] %llu\n", i, sm64());
    }
    
    printf("random::xoshiro256pp\n");
    
    random::xoshiro256pp gn(0);
    
    for (int i = 0; i < 10; i++) {
        printf("[%d] %llu\n", i, gn());
    }
    
    printf("random::xoshiro256pp -> std::uniform_int_distribution [-10, 10)\n");
    
    std::uniform_int_distribution<int32_t> dis(-10, 10);
    
    for (int i = 0; i < 10; i++) {
        printf("[%d] %d\n", i, dis(gn));
    }
    
    printf("random::xoshiro256pp -> std::uniform_real_distribution [-180.0, 180.0)\n");
    
    std::uniform_real_distribution<double> dis2(-180.0, 180.0);
    
    for (int i = 0; i < 10; i++) {
        printf("[%d] %f\n", i, dis2(gn));
    }
    
    return 0;
}

#include <Windows.h>

void testcp() {
    const char note[] = {
        // ♪
        char(0x81),
        char(0x37),
        char(0xAC),
        char(0x38),
        // end
        char(0x00),
    };
    
    int need = ::MultiByteToWideChar(/* CP_GB18030 */ 54936, 0,
                                                      note, 4,
                                                      nullptr, 0);
    std::wstring buf;
    buf.resize(need);
    int ccnt = ::MultiByteToWideChar(/* CP_GB18030 */ 54936, 0,
                                                      note, 4,
                                                      const_cast<LPWSTR>(buf.data()), need);
    
    int need2 = ::WideCharToMultiByte(CP_ACP, 0,
                                      buf.c_str(), buf.length(),
                                      nullptr, 0,
                                      nullptr, nullptr);
    std::string buf2;
    buf2.resize(need2);
    int ccnt2 = ::WideCharToMultiByte(CP_ACP, 0,
                                      buf.c_str(), buf.length(),
                                      const_cast<LPSTR>(buf2.data()), need2,
                                      nullptr, nullptr);
}
