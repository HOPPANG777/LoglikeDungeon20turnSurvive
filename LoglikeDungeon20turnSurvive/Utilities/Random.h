#pragma once
#include <random>

class Random
{
public:
    // 어디서든 Random::Range(...)로 호출 가능
    static int Range(int p_Min, int p_Max);
    static float Range(float p_Min, float p_Max);

private:
    // 공유해서 사용할 난수 엔진
    static std::mt19937 s_Gen;
    static std::random_device s_Rd;
};

