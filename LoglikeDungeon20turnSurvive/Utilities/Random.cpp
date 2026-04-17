#include "Random.h"

// static 멤버 변수 초기화
std::random_device Random::s_Rd;
std::mt19937 Random::s_Gen(Random::s_Rd());

int Random::Range(int p_Min, int p_Max)
{
    // uniform_int_distribution은 [min, max] 범위를 포함합니다.
    std::uniform_int_distribution<int> dist(p_Min, p_Max);
    return dist(s_Gen);
}

float Random::Range(float p_Min, float p_Max)
{
    std::uniform_real_distribution<float> dist(p_Min, p_Max);
    return dist(s_Gen);
}