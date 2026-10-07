#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdint>
#include <utility>
#include <stdexcept>

class BigInt {
private:
    std::vector<uint32_t> myNumbers;

    void remZero() {
        while (myNumbers.size() > 1 && myNumbers.back() == 0) 
        {
            myNumbers.pop_back();
        }
    }

    int comparison(const BigInt& other) const {
        if (myNumbers.size() != other.myNumbers.size()) 
        {
            return myNumbers.size() > other.myNumbers.size() ? 1 : -1;
        }
        
        for (int i = myNumbers.size() - 1; i >= 0; --i) 
        {
            if (myNumbers[i] > other.myNumbers[i]) return 1;
            if (myNumbers[i] < other.myNumbers[i]) return -1;
        }
        
        return 0;
    }

    BigInt simpleMultiply(uint32_t digit) const {
        BigInt res;
        res.myNumbers.clear();
        uint32_t carry = 0;

        for (size_t i = 0; i < myNumbers.size(); ++i) 
        {
            uint64_t temp = static_cast<uint64_t>(myNumbers[i]) * digit + carry;
            res.myNumbers.push_back(static_cast<uint32_t>(temp & 0xFFFFFFFF));
            carry = static_cast<uint32_t>(temp >> 32);
        }
        if (carry > 0)
        {
            res.myNumbers.push_back(carry);
        }
        return res;
    }

    void leftshiftNumber(size_t s) {
        if (s == 0 || (myNumbers.size() == 1 && myNumbers[0] == 0)) return;
        myNumbers.insert(myNumbers.begin(), s, 0);
    }

    int bitLength() const {
        if (myNumbers.empty() || (myNumbers.size() == 1 && myNumbers[0] == 0)) return 0;
        int bits = (myNumbers.size() - 1) * 32;
        uint32_t last = myNumbers.back();
        while (last > 0) 
        {
            bits++;
            last >>= 1;
        }
        return bits;
    }

    BigInt leftshiftBits(int s) const {
        if (s == 0) return *this;
        BigInt res;
        res.myNumbers.clear();
        int shiftBlock = s / 32;
        int ShiftBit = s % 32;
        res.myNumbers.insert(res.myNumbers.end(), shiftBlock, 0);

        uint32_t carry = 0;
        for (size_t i = 0; i < myNumbers.size(); ++i) 
        {
            uint32_t c = myNumbers[i];
            uint32_t newBlock = (c << ShiftBit) | carry;
            res.myNumbers.push_back(newBlock);
            if (ShiftBit > 0) 
            {
                carry = c >> (32 - ShiftBit);
            } else 
            {
                carry = 0;
            }
        }
        if (carry > 0) 
        {
            res.myNumbers.push_back(carry);
        }
        return res;
    }

    std::pair<BigInt, BigInt> divide(const BigInt& other) const {
        if (other == BigInt(0)) {
            throw std::invalid_argument("Помилка. дільник рівний 0");
        }

        int k = other.bitLength();
        BigInt R = *this;
        BigInt Q(0);

        while (R >= other) 
        {
            int t = R.bitLength();    
            BigInt C = other.leftshiftBits(t - k);
            if (R < C) {
                t = t - 1;
                C = other.leftshiftBits(t - k);
            }
            R = R - C;
            BigInt bit(1);
            bit = bit.leftshiftBits(t - k);
            Q = Q + bit;
        }

        return {Q, R};
    }

    int getBit(int i) const {
        int blockIndex = i / 32;
        int bitIndex = i % 32;
        if (blockIndex >= myNumbers.size()) return 0;
        return (myNumbers[blockIndex] >> bitIndex) & 1;
    }

public:
    BigInt() {
        myNumbers.push_back(0);
    }

    BigInt(uint32_t number) {
        myNumbers.push_back(number);
    }

    BigInt(std::string hex) {
        if (hex.length() >= 2 && hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')) 
        {
            hex = hex.substr(2);
        }
        if (hex.empty()) 
        {
            myNumbers.push_back(0);
            return;
        }
        for (int i = hex.length(); i > 0; i -= 8) {
            int length = 8;
            int start = i - length;
            if (start < 0) 
            {
                length = i;
                start = 0;
            }
            std::string block = hex.substr(start, length);
            
            uint32_t hblock = std::stoul(block, nullptr, 16);
            myNumbers.push_back(hblock);
        }
        remZero();
    }

    void printHex() const {
        std::cout << std::hex << std::uppercase << myNumbers.back();
        for (int i = myNumbers.size() - 2; i >= 0; --i) 
        {
            std::cout << std::setfill('0') << std::setw(8) << myNumbers[i];
        }
        std::cout << std::dec << std::endl;
    } 

    /// перевантаження від comparison
    bool operator==(const BigInt& other) const {
        return comparison(other) == 0; 
    }
    bool operator!=(const BigInt& other) const {
        return comparison(other) != 0; 
    }
    bool operator<(const BigInt& other) const  {
        return comparison(other) < 0; 
    }
    bool operator>(const BigInt& other) const  {
        return comparison(other) > 0; 
    }
    bool operator<=(const BigInt& other) const {
        return comparison(other) <= 0; 
    }
    bool operator>=(const BigInt& other) const {
        return comparison(other) >= 0;
    }
    //перевантаження  від ділення
    BigInt operator/(const BigInt& other) const {
        return divide(other).first;
    }
    BigInt operator%(const BigInt& other) const {
        return divide(other).second;
    }
    
    //перевантаження +
    BigInt operator+(const BigInt& other) const {
        BigInt res;
        res.myNumbers.clear();

        uint32_t carry = 0;
        size_t maxSize = std::max(myNumbers.size(), other.myNumbers.size());
        for (size_t i = 0; i < maxSize || carry > 0; ++i) 
        {
            uint64_t sum = carry;
            if (i < myNumbers.size()) 
            {
                sum += myNumbers[i];
            }
            if (i < other.myNumbers.size()) 
            {
                sum += other.myNumbers[i];
            }
            res.myNumbers.push_back(sum & 0xFFFFFFFF);
            carry = sum >> 32;
        }

        return res;
    }

    //перевантаження -
    BigInt operator-(const BigInt& other) const {
        if (*this < other) {
            throw std::invalid_argument("результат віднімання - від'ємне число!");
        }

        BigInt res;
        res.myNumbers.clear();

        uint32_t borrow = 0;
        for (size_t i = 0; i < myNumbers.size(); ++i) 
        {
            int64_t temp = static_cast<int64_t>(myNumbers[i]) - borrow;
            if (i < other.myNumbers.size()) 
            {
                temp -= other.myNumbers[i];
            }

            if (temp >= 0) 
            {
                res.myNumbers.push_back(static_cast<uint32_t>(temp));
                borrow = 0;
            } 
            else 
            {
                res.myNumbers.push_back(static_cast<uint32_t>(temp + (1ULL << 32)));
                borrow = 1;
            }
        }
        res.remZero();

        return res;
    }
    //перевантаження *
    BigInt operator*(const BigInt& other) const {
        BigInt res(0);
        for (size_t i = 0; i < other.myNumbers.size(); ++i) 
        {
            BigInt temp = this->simpleMultiply(other.myNumbers[i]);
            temp.leftshiftNumber(i);
            res = res + temp;
        }

        res.remZero();
        return res;
    }
    BigInt power(const BigInt& exponent) const {
        BigInt C(1);
        int m = exponent.bitLength();
        
        if (m == 0) return C;

        for (int i = m - 1; i >= 0; --i) 
        {
            if (exponent.getBit(i) == 1) {
                C = C * (*this);
            }
            if (i != 0) 
            {
                C = C * C;
            }
        }
        
        return C;
    }
};