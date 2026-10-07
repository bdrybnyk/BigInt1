#include <iostream>
#include <chrono>
#include "BigInt.h"

void runTests() {
    std::cout << "Тести" << std::endl;
    BigInt a("0x94D5A66");
    BigInt b("0x8A5B6C7D01500155DD");
    BigInt c("0x5807359104760186AB937");
    //дистрибутивність
    BigInt d1 = (a + b) * c;
    BigInt d2 = (a * c) + (b * c);
    std::cout << "Дистрибутивність: ";
    if (d1 == d2) {
        std::cout << "УСПІШНО" << std::endl;
    } else {
        std::cout << "ПОМИЛКА" << std::endl;
    }
    //множення як сума
    uint32_t n = 105;
    BigInt BIGn(n);
    BigInt m1 = a * BIGn; 
    BigInt m2(0);
    for (uint32_t i = 0; i < n; ++i) {
        m2 = m2 + a; 
    }
    std::cout << "Множення як сума: ";
    if (m1 == m2) {
        std::cout << "УСПІШНО" << std::endl;
    } else {
        std::cout << "ПОМИЛКА" << std::endl;
    }

    //Перевірка ділення
    BigInt q = a / b;
    BigInt r = a % b;
    BigInt rec = (q * b) + r;
    std::cout << "Перевірка ділення: ";
    if (a == rec) {
        std::cout << "УСПІШНО" << std::endl;
    } else {
        std::cout << "ПОМИЛКА" << std::endl;
    }
std::cout << "\nЧас" << std::endl;
    
    int iterations = 1000;
    BigInt d(0);
    auto startMul = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        BigInt C = c + BigInt(i);
        d = a * C;
    }
    auto endMul = std::chrono::high_resolution_clock::now();
    auto durMul = std::chrono::duration_cast<std::chrono::microseconds>(endMul - startMul).count() / iterations;
    std::cout << "Множення: " << durMul << " мікросекунд" << std::endl;

    auto startDiv = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        BigInt A = a + BigInt(i);
        d = b / A;
    }
    auto endDiv = std::chrono::high_resolution_clock::now();
    auto durDiv = std::chrono::duration_cast<std::chrono::microseconds>(endDiv - startDiv).count() / iterations;
    std::cout << "Ділення: " << durDiv << " мікросекунд" << std::endl;

    BigInt smallExp("0xFF"); 
    int pow = 11; 
    auto startPow = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < pow; ++i) {
        BigInt dynamicExp = smallExp + BigInt(i);
        d = a.power(dynamicExp);
    }
    auto endPow = std::chrono::high_resolution_clock::now();
    auto durPow = std::chrono::duration_cast<std::chrono::microseconds>(endPow - startPow).count() / pow;
    std::cout << "Піднесення до степені: " << durPow << " мікросекунд" << std::endl;
    if (d == BigInt(0)) std::cout << " ";
}

int main() {
    try {
        runTests();
    } catch (const std::exception& e) {
        std::cout << "Помилка: " << e.what() << std::endl;
    }
    return 0;
}