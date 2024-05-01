typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

class fixed {
public:

    fixed(int value, int decimal) : val(value), dec(decimal) {};
    fixed(float value) {
        val = (int)value;
        dec = (int)((value - val) * 1000);
    }

    fixed operator+(fixed f) {
        fixed result(0, 0);
        uint32_t sum = ((val + f.val) << 16) + ((dec + f.dec) << 16);
        result.val = sum >> 16;
        result.dec = sum & 0xFFFF;
        return result;
    }

    fixed operator-(fixed f) {
        fixed result(0, 0);
        uint32_t diff = ((val - f.val) << 16) + ((dec - f.dec) << 16);
        result.val = diff >> 16;
        result.dec = diff & 0xFFFF;
        return result;
    }

    fixed operator*(fixed f) {
        fixed result(0, 0);
        uint32_t product = ((val * f.val) << 16) + ((dec * f.dec) << 16);
        result.val = product >> 16;
        result.dec = product & 0xFFFF;
        return result;
    }

    fixed operator/(fixed f) {
        fixed result(0, 0);
        uint32_t quotient = ((val / f.val) << 16) + ((dec / f.dec) << 16);
        result.val = quotient >> 16;
        result.dec = quotient & 0xFFFF;
        return result;
    }

    fixed operator%(fixed f) {
        fixed result(0, 0);
        uint32_t remainder = ((val % f.val) << 16) + ((dec % f.dec) << 16);
        result.val = remainder >> 16;
        result.dec = remainder & 0xFFFF;
        return result;
    }

    fixed operator+=(fixed f) {
        *this = *this + f;
        return *this;
    }

    fixed operator-=(fixed f) {
        *this = *this - f;
        return *this;
    }

    fixed operator*=(fixed f) {
        *this = *this * f;
        return *this;
    }

    fixed operator/=(fixed f) {
        *this = *this / f;
        return *this;
    }

    fixed operator%=(fixed f) {
        *this = *this % f;
        return *this;
    }

    bool operator==(fixed f) {
        return val == f.val && dec == f.dec;
    }

    bool operator!=(fixed f) {
        return val != f.val || dec != f.dec;
    }

    bool operator>(fixed f) {
        return val > f.val || (val == f.val && dec > f.dec);
    }

    bool operator<(fixed f) {
        return val < f.val || (val == f.val && dec < f.dec);
    }

    bool operator>=(fixed f) {
        return val > f.val || (val == f.val && dec >= f.dec);
    }

    bool operator<=(fixed f) {
        return val < f.val || (val == f.val && dec <= f.dec);
    }

    uint16_t val;
    uint16_t dec;
};