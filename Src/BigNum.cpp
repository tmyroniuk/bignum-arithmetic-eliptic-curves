#include <BigNum.hpp>

#include <cassert>
#include <iterator>
#include <map>

namespace lab {

namespace {
/**
 * @brief Points to the max value BigNum array's cell can hold,
 *        same as basis in linear representation
 */
constexpr int NUM_BASE = 1000000000;

/**
 * @brief Points to number of digits in (NUM_BASE-1)
 */
constexpr char SECTION_DIGITS = 9;

} // <anonymous> namespace

/**
 * @brief Returns 10^power
 */
long long powTen(int power)
{
    int value = 1;
    for (int i = 1; i <= power; ++i) {
        value *= 10;
    }
    return value;
}
BigNum::BigNum(std::string_view num_str) {
    for (int i = num_str.size() - 1; i >= 0; i -= SECTION_DIGITS) {
        int pos = 0;
        long long value = 0;
        while ((pos != SECTION_DIGITS) && (i-pos >= 0)) {
            value = value + powTen(pos)*(num_str[i-pos] - '0');
            ++pos;
        }
        _digits.push_back(value);
    }
    while (!_digits.empty() && _digits.back() == 0) {
        _digits.pop_back();
    }
    if (_digits.empty()) {
        _digits.push_back(0);
    }
}

std::string to_string(const BigNum &num)
{
    bool is_empty = true;
    for (auto &pos : num._digits) {
        if (pos != 0) {
            is_empty = false;
        }
    }
    if (is_empty) {
        return "0";
    }
    std::string result;
    int leading_zeros = 0;

    for (int curr_pos = 0; curr_pos < num._digits.size(); ++curr_pos) {
        std::string temp = std::to_string(num._digits[curr_pos]);
        std::string offset;
        leading_zeros = 0;
        while (temp.size() + offset.size() != SECTION_DIGITS) {
            offset += '0';
            ++leading_zeros;
        }
        result = offset + temp + result;
    }

    result.erase(0, leading_zeros);
    return result;
}


const BigNum& BigNum::inf() {
    static const BigNum _inf = 99999999999999999999999999999999999999999999999999999999999999999999999999999999999999_bn;
    return _inf;
}

bool operator<(const BigNum& left, const BigNum& right) noexcept {
    if (left._digits.size() < right._digits.size()) {
        return true;
    }

    if (left._digits.size() > right._digits.size()) {
        return false;
    }

    for (int curr_pos = left._digits.size() - 1; curr_pos >= 0; --curr_pos) {
        if (left._digits[curr_pos] < right._digits[curr_pos]) {
            return true;
        }

        if (left._digits[curr_pos] > right._digits[curr_pos]) {
            return false;
        }
    }

    return false;
}

bool operator>(const BigNum& left, const BigNum& right)noexcept {
    return (right < left);
}

bool operator<=(const BigNum& left, const BigNum& right) noexcept {
    return !(right < left);
}

bool operator>=(const BigNum& left, const BigNum& right) noexcept {
    return !(left < right);
}

bool operator==(const BigNum& left, const BigNum& right) noexcept {
    return ((!(right < left)) && (!(left < right)));
}

bool operator!=(const BigNum& left, const BigNum& right) noexcept {
    return !(left == right);
}

BigNum operator+(const BigNum &left, const BigNum &right) {
    if (right._digits.size() > left._digits.size()) {
        auto result = right;
        int addition = 0;
        for (int curr_pos = 0; curr_pos < left._digits.size(); ++curr_pos) {
            result._digits[curr_pos] += left._digits[curr_pos] + addition;
            addition = result._digits[curr_pos] / NUM_BASE;
            result._digits[curr_pos] = result._digits[curr_pos] % NUM_BASE;
        }

        return result;
    } else {
        auto result = left;
        int addition = 0;

        for (int curr_pos = 0; curr_pos < right._digits.size(); ++curr_pos) {
            long long temp = static_cast<long long>(result._digits[curr_pos])
                    + static_cast<long long>(right._digits[curr_pos]) + addition;
            result._digits[curr_pos] = temp % NUM_BASE;
            addition = temp / NUM_BASE;
            result._digits[curr_pos] = result._digits[curr_pos] % NUM_BASE;
        }

        return result;
    }
}

BigNum operator-(const BigNum &left, const BigNum &right) {
    BigNum result = left;
    for (int curr_pos = 0; curr_pos < right._digits.size(); ++curr_pos) {
        if (result._digits[curr_pos] < right._digits[curr_pos]) {
            result._digits[curr_pos] = result._digits[curr_pos] - right._digits[curr_pos] + NUM_BASE;
            result._digits[curr_pos + 1] -= 1;
        } else {
            result._digits[curr_pos] = result._digits[curr_pos] - right._digits[curr_pos];
        }
    }
    if (left._digits.size() > right._digits.size()) {
        if (result._digits[right._digits.size()] < 0) {
            result._digits[right._digits.size()] += NUM_BASE;
            result._digits[right._digits.size() + 1] -= 1;
        }
    }
    while (!result._digits.empty() && (result._digits.back() == 0)) {
       result. _digits.pop_back();
    }
    if (result._digits.empty()) {
        result._digits.push_back(0);
    }
    return result;
}

std::vector<char> toOneDigit(const BigNum &num) {
    std::vector<char> fnum;
    for (int curr_pos = 0; curr_pos < num._digits.size(); ++curr_pos) {
        int counter = num._digits[curr_pos];
        for (int i = 0; i < SECTION_DIGITS; ++i) {
            fnum.push_back(counter % 10);
            counter = counter / 10;
        }
    }
    while (!fnum.empty() && fnum.back() == 0) {
        fnum.pop_back();
    }
    return fnum;
}

BigNum toBigNum(std::vector<char> &num_digits) {
    std::string str_num;
    for (int i = num_digits.size() - 1; i >= 0; --i) {
        str_num += ('0' + num_digits[i]);
    }
    BigNum result(str_num);
    return result;
}

BigNum operator*(const BigNum &left, int right) {
    BigNum result = left;
    unsigned long long addition = 0;
    for (int curr_pos = 0; curr_pos < result._digits.size(); ++curr_pos) {
        unsigned long long temp = static_cast<long long>(result._digits[curr_pos]) * right + addition;
        result._digits[curr_pos] = temp % NUM_BASE;
        addition = temp / NUM_BASE;
    }
    if (addition >= NUM_BASE) {
        result._digits.push_back(addition % NUM_BASE);
        result._digits.push_back(addition / NUM_BASE);
    } else {
        result._digits.push_back(addition);
    }
    while (!result._digits.empty() && result._digits.back() == 0)
        result._digits.pop_back();
    return result;
}

/**
 * @brief Compares vector of digits with operator <
 * @param step points to the number of digits from beginning of left var to compare
 */
bool isLowerDigits(const std::vector<char> &left, const std::vector<char> &right, int step) {
    if ((step > right.size()) && (step <= left.size())) return false;
    if ((step < right.size()) || (left.size() < right.size())) return true;
    if (step > left.size()) return true;
    for (int i = right.size() - 1; i >= 0; --i) {
        if (left[left.size() + i - step] < right[i]) return true;
        if (left[left.size() + i - step] > right[i]) return false;
    }
    return false;
}

bool isLowerDigits(const std::vector<char> &left, const std::vector<char> &right, int step);

auto addDigits(const std::vector <char> &lhs, const std::vector <char> &rhs) {

    constexpr auto BASE = 10;
    std::vector <char> result;


    if (isLowerDigits(lhs, rhs, 0)) {
        result = rhs;
        int addition{};
        for (std::size_t i = 0; i < rhs.size(); ++i) {
            result[i] += (i >= lhs.size()) ? addition : lhs[i] + addition;
            addition = result[i] / BASE;
            result[i] %= BASE;
        }
    } else {
        result = lhs;
        int addition{};
        for (std::size_t i = 0; i < rhs.size(); ++i) {
            result[i] += (i >= rhs.size()) ? addition : rhs[i] + addition;
            addition = result[i] / BASE;
            result[i] %= BASE;
        }
    }
    return result;
}

/**
 * @brief Multiplies vector of digits and int
 */
std::vector<char> multiplyDigits(std::vector<char> &num1, int num2) {
    std::vector<char> result = num1;
    int addition = 0;
    for (int curr_pos = 0; curr_pos < result.size(); ++curr_pos) {
        long long temp = result[curr_pos] * num2 + addition;
        result[curr_pos] = temp % 10;
        addition = temp / 10;
    }
    if (addition != 0) {
        result.push_back(addition);
    }
    return result;
}

/**
 * @brief Returns power of basis to make two numbers almost equal
 * @param fnum number to reach
 * @param snum number to multiply
 * @return Maximum power of basis to reach bottom border of fnum
 */
int getMaxPow(std::vector<char> &fnum, std::vector<char> &snum)
{
    if (snum.size() > fnum.size()) {
        return -1;
    }
    int current_pow = fnum.size() - snum.size();
    for (int i = snum.size() - 1; i >= 0; --i) {
        if (snum[i] > fnum[i + current_pow]) {
            return current_pow - 1;
        }
        if (snum[i] < fnum[i + current_pow]) {
            return current_pow;
        }
    }
    return current_pow;
}

/// TODO fix +=10 after changing basis
/**
 * @brief Subtracts two numbers, where second one is multiplied on basis with given power.
 *        Result stores in first parameter
 * @param curr_pow power of basis
 */
void subtractPow(std::vector<char> &fnum, std::vector<char> &snum, int curr_pow)
{
    for (int i = 0; i < snum.size(); ++i) {
        if (fnum[i + curr_pow] < snum[i]) {
            fnum[i + curr_pow] += 10;
            fnum[i + curr_pow + 1] -= 1;
        }
        fnum[i + curr_pow] -= snum[i];
    }
}

/**
 * @brief Compares with operator<= two numbers, where second one is multiplied on basis with given power
 * @param curr_pow power of basis
 */
bool lessNumPow(std::vector<char> &fnum, std::vector<char> &snum, int curr_pow)
{
    if (curr_pow + snum.size() < fnum.size()) {
        return true;
    }
    for (int i = snum.size() - 1; i >= 0; --i) {
        if (fnum[i + curr_pow] < snum[i]) {
            return false;
        }
        if (fnum[i + curr_pow] > snum[i]) {
            return true;
        }
    }
    return true;
}

std::pair<BigNum, BigNum> extract(const BigNum &left, const BigNum &right) {
    if (left < right) {
        return std::pair<BigNum, BigNum>(0_bn, left);
    }

    std::vector<char> fnum = toOneDigit(left);
    std::vector<char> snum = toOneDigit(right);
    std::vector<char> current = snum;
    std::vector<char> result;
    int curr_pow = getMaxPow(fnum, snum);
    int hight_rank = 1;
    while (true) {
        while (lessNumPow(fnum, current, curr_pow)) {
            ++hight_rank;
            current = multiplyDigits(snum, hight_rank);
        }
        current = multiplyDigits(snum, hight_rank - 1);
        subtractPow(fnum, current, curr_pow);
        result.push_back(hight_rank-1);
        int to_delete = current.size();
        while (!fnum.empty() && (fnum.back() == 0) && (to_delete > 0)) {
            fnum.pop_back();
            --to_delete;
        }
        while (!fnum.empty() && (fnum.back() == 0)) {
            fnum.pop_back();
            --curr_pow;
            result.push_back(0);
        }
        if ((fnum.size() <= curr_pow) && (curr_pow <= snum.size()) && !lessNumPow(fnum, snum, 0)) {
            for (int i = 0; i < fnum.size(); i++) {
                result.push_back(0);
            }
        }
        int new_pow = getMaxPow(fnum, snum);
        if (new_pow == -1) {
            break;
        }
        while (curr_pow != new_pow + 1) {
            --curr_pow;
            result.push_back(0);
        }
        --curr_pow;
        hight_rank = 1;
        current = snum;
    }

    std::reverse(result.begin(), result.end());
    return std::pair{toBigNum(result), toBigNum(fnum)};
}

void modify(BigNum& num, const BigNum& mod) {
    num = extract(num, mod).second;
}

BigNum add(const BigNum &left, const BigNum &right, const BigNum &mod) {
    auto t_num2 = right % mod;
    auto t_num1 = left % mod;
    BigNum result = t_num1 + t_num2;
    modify(result, mod);
    return result;
}

BigNum gcd(const BigNum& lhs, const BigNum& rhs) {
    if (lhs == BigNum("0")) {
        return rhs;
    }
    return gcd (rhs % lhs, lhs);
}

BigNum subtract(const BigNum &left, const BigNum &right, const BigNum &mod) {
    auto t_num2 = right % mod;
    auto t_num1 = left % mod;
    BigNum result;
    if (t_num1 >= t_num2) {
        result = t_num1 - t_num2;
    } else {
        result = mod - t_num2 + t_num1;
    }
    return result;
}

BigNum operator%(const BigNum& left, const BigNum& right) {
    return extract(left, right).second;
}

namespace {
    /*
    *  @brief Class for read-only contiguous range of elements.
    */
    template <typename T>
    class ArrayView
    {
    public:
        template <typename Iter,
                  typename = std::enable_if <std::is_same_v<typename std::iterator_traits<Iter>::iterator_category,
                                                            std::random_access_iterator_tag>
                                          || std::is_same_v <Iter, T*>>>
        explicit ArrayView(Iter begin, Iter end): 
            _begin(std::addressof(*begin)),
            _size(end - begin)
        { }

        const T* begin() const noexcept {
            return _begin;
        }

        const T* end() const noexcept {
            return _begin + _size;
        }

        const auto size() const noexcept {
            return _size;
        }

        const auto& operator[](const std::size_t n) const noexcept {
            assert((n < _size) && "Out of view range.");
            return *(_begin + n);
        }

    private:
        const T* _begin;
        const std::size_t _size;
    };

    /**
     *  @brief Returns nearest number bigger than n that is degree of two
    */
    inline unsigned upperLog2(const unsigned n) {
        return std::pow(2, static_cast<int>(std::log2(n)) + 1);
    }

    std::vector<int64_t> naiveMultiplication(const ArrayView<int64_t>& lhs,
                                             const ArrayView<int64_t>& rhs) {
        std::vector<int64_t> result(lhs.size() + rhs.size());

        for (int i = 0; i < lhs.size(); ++i) {
            for (int j = 0; j < rhs.size(); ++j) {
                result[i + j] += lhs[i] * rhs[j];
            }
        }

        return result;
    }

    /**
     * @brief Minimum size of vector of digits to do
     *        fast multiplication instead of naive approach
     */
    constexpr inline int MIN_FOR_KARATSUBA = 32;

    /*
     * @brief Karatsuba's method implements fast multiplication of numbers [AB] and [CD] like
     *        like (A * 10 + B) * (C * 10 + D) = AC * 100 + BD + ((A + B) * (C + D) - AC - BD) * 10
     */

    std::vector<int64_t> karatsuba(const ArrayView<int64_t>& lhs, const ArrayView<int64_t>& rhs) {
        if (lhs.size() <= MIN_FOR_KARATSUBA) {
            return naiveMultiplication(lhs, rhs);
        }

        const auto length = lhs.size();
        std::vector<int64_t> result(length * 2);

        ArrayView<int64_t> lhsL(lhs.begin() + length / 2, lhs.end());
        ArrayView<int64_t> rhsL(rhs.begin() + length / 2, rhs.end());
        ArrayView<int64_t> lhsR(lhs.begin(), lhs.begin() + length / 2);
        ArrayView<int64_t> rhsR(rhs.begin(), rhs.begin() + length / 2);

        const auto c1 = karatsuba(lhsL, rhsL);
        const auto c2 = karatsuba(lhsR, rhsR);

        std::vector<int64_t> lhsLR(length / 2);
        std::vector<int64_t> rhsLR(length / 2);

        for (auto i = 0; i < length / 2; ++i) {
            lhsLR[i] = lhsL[i] + lhsR[i];
            rhsLR[i] = rhsL[i] + rhsR[i];
        }

        auto c3 = karatsuba(
            ArrayView<int64_t>{lhsLR.begin(), lhsLR.end()},
            ArrayView<int64_t>{rhsLR.begin(), rhsLR.end()}
        );

        for (auto i = 0; i < length; ++i){
            c3[i] -= c2[i] + c1[i];
        }

        for (auto i = 0; i < length; ++i) {
            result[i] = c2[i];
        }

        for (auto i = length; i < length * 2; ++i) {
            result[i] = c1[i - length];
        }

        for (auto i = length / 2; i < length + length / 2; ++i) {
            result[i] += c3[i - length / 2];
        }

        return result;
    }

    /*
     * @brief Normalizing number ceils to NUM_BASE
     */
    inline void finalize(std::vector<int64_t>& num) {
        for (auto i = 0; i < num.size(); ++i) {
            if (i != num.size() - 1) {
                num[i + 1] += num[i] / NUM_BASE;
            }
            num[i] %= NUM_BASE;
        }
    }

    /*
    *  @return Pair of x, y
    *          ax + by = gcd(a, b)
    */
    std::pair<BigNum, BigNum> extendedEuclid(const BigNum& a, const BigNum& b, const BigNum& mod) {

        if (b == BigNum("0")) {
            return std::pair(1_bn, 0_bn);
        }
        const auto& [int_part, remainder] = extract(a, b);
        const auto& [x, y] = extendedEuclid(b, remainder, mod);
        return std::pair(y, subtract(x, (int_part * y) % mod, mod));
    }

    bool isPrime(const BigNum& num) {
        if (num <= 1_bn) {
            return false;
        }
        if (num <= 3_bn) {
            return true;
        }

        if (num % 2_bn == 0_bn || num % 3_bn == 0_bn) {
            return false;
        }

        for (auto i = 5_bn; i * i <= num; i = i + 6_bn) {
            if (num % i == 0_bn || num % (i + 2_bn) == 0_bn) {
                return false;
            }
        }
        return true;
    }

    BigNum pow(const BigNum& num, const BigNum& degree, const BigNum& mod) {
        if (degree == 0_bn) {
            return 1_bn;
        }

        auto result = pow(num, extract(degree, 2_bn).first, mod) % mod;
        result = (result * result) % mod;
        return degree % 2_bn == 0_bn ? result : (result * num) % mod;
    }
}

BigNum operator*(const BigNum& lhs, const BigNum& rhs) {
    auto lhsTemp = lhs._digits;
    auto rhsTemp = rhs._digits;
    const int maxSize = std::max(lhsTemp.size(), rhsTemp.size());

    lhsTemp.resize(upperLog2(maxSize));
    rhsTemp.resize(upperLog2(maxSize));

    auto nums = karatsuba(
        ArrayView<int64_t>{lhsTemp.begin(), lhsTemp.end()},
        ArrayView<int64_t>{rhsTemp.begin(), rhsTemp.end()}
    );

    finalize(nums);

    while (!nums.empty() && nums.back() == 0) {
        nums.pop_back();
    }

    BigNum result;
    result._digits.assign(nums.begin(), nums.end());
    return result;
}

BigNum multiply(const BigNum& lhs, const BigNum& rhs, const BigNum& mod) {
        return (lhs % mod * rhs % mod) % mod;
}

BigNum inverted(const BigNum& num, 
                const BigNum& mod,
                const BigNum::InversionPolicy policy = BigNum::InversionPolicy::Euclid) {
    if (policy == BigNum::InversionPolicy::Euclid) {
        if (gcd(num, mod) != 1_bn) {
            throw std::invalid_argument("Nums must be coprime.");
        }

        auto inverted = extendedEuclid(num, mod, mod).first;
        while (!inverted._digits.empty() && inverted._digits.back() == 0) {
            inverted._digits.pop_back();
        }

        return inverted;
    } else {
#ifdef ENABLE_IS_PRIME_CHECK
        if (!isPrime(mod)) {
            throw std::invalid_argument("Mod must be prime.");
        }
#endif
        if (gcd(num, mod) != 1_bn) {
            throw std::invalid_argument("Nums must be coprime.");
        }

        return pow (num, mod - 2_bn, mod);
    }
}

BigNum operator/(const BigNum& left, const BigNum& right) {
    return extract(left, right).first;
}

std::optional<std::pair<BigNum, BigNum>> sqrt(const BigNum& n, const BigNum& p)
{
    // NOTE: Names of variables are taken directly from Wikipedia for better understanding

    /// If it doesn't satisfy Fermat's little theorem than we can't find result
    if (pow(n, (p - 1_bn) / 2_bn, p) != 1_bn) {
        return {};
    }

    /// Attempt to find trivial solution
    const auto& [q, s] = [&] {
        auto q = p - 1_bn;
        auto s = 0_bn;
        while (q % 2_bn == 0_bn) {
            q = q / 2_bn;
            s = s + 1_bn;
        }

        return std::pair{q, s};
    }();

    /// If p = 3 (mod 4) than solutions are trivial
    if (s == 1_bn) {
        const auto x = pow(n, (p + 1_bn) / 4_bn, p);
        return std::pair{x, p - x};
    }

    /// Select a quadric non-residue (mod p)
    const auto z = [&] {
        for (auto i = 1_bn; i < p; i = i + 1_bn) {
            if (pow(i, (p - 1_bn) / 2_bn, p) != 1_bn) {
                return i;
            }
        }

        return 0_bn;
    }();

    auto c = pow(z, q, p);
    auto r = pow(n, (q + 1_bn) / 2_bn, p);
    auto t = pow(n, q, p);
    auto m = s;

    while (t != 1_bn) {
        const auto& [i, x] = [&] {
            auto i = 1_bn;
            auto x = t * t % p;
            while (x != 1_bn) {
                x = x * x % p;
                i = i + 1_bn;
            }

            return std::pair(i, x);
        }();
        
        const auto b = pow(c, pow(2_bn, (m - i - 1_bn), p), p);

        r = (r * b) % p;
        c = (b * b) % p;
        t = (t * c) % p;
        m = i;
    }
    
    return std::pair{r, p - r};
}

int countDigit(long long n) {
    int count = 0;
    while (n != 0) {
        n = n / 10;
        ++count;
    }
    return count;
}

int length(const BigNum& num) {
    int num_size = (num._digits.size() - 1) * SECTION_DIGITS;
    num_size += countDigit(num._digits[num._digits.size() - 1]);
    return num_size;
}

BigNum calculateMontgomeryCoefficient(const BigNum& mod) {
    if (mod == 5_bn) {
        return 100_bn;
    }
    std::string res(length(mod) + 1, '0');
    res[0] = '1';
    return BigNum(res);
}

BigNum convertToMontgomeryForm(const BigNum& num, const BigNum& mod, const BigNum& montgomery_coefficient) {
    return  multiply(num, montgomery_coefficient, mod);
}

/**
 * @brief Multiply BigNums in Montogomery form in the range [0, mod)
 * @param coefficient = (montgomery_coefficient(montgomery_coefficient^(−1) % mod)−1) / mod .
 *        it is always the same, so pass it not to calculate it on each call of pow
 */
BigNum multiplyMontgomery(const BigNum& left, const BigNum& right, const BigNum& mod, const BigNum& montgomery_coefficient, const BigNum& coefficient) {
    if(left >= mod || right >= mod) {
        throw std::invalid_argument("Left and right in multiplyMontgomery must be < mod");
    }
    BigNum product = left * right;
    BigNum temp = multiply(product, coefficient, montgomery_coefficient);
    temp = product + temp * mod;
    temp = extract(temp, montgomery_coefficient).first;
    if(temp < mod) {
        return temp;
    } else {
        return temp - mod;
    }
}

BigNum powMontgomery(const BigNum& base, BigNum degree, const BigNum& mod) {
    BigNum montgomery_coefficient = calculateMontgomeryCoefficient(mod);
    BigNum mc_inverted = inverted(montgomery_coefficient, mod, BigNum::InversionPolicy::Euclid);
    BigNum coefficient = extract(montgomery_coefficient * mc_inverted - 1_bn, mod).first;
    std::pair<BigNum, BigNum> extraction;
    BigNum base_mf = convertToMontgomeryForm(base, mod, montgomery_coefficient);
    BigNum result = convertToMontgomeryForm(1_bn,mod,montgomery_coefficient);
    while(degree > 0_bn) {
        extraction = extract(degree, 2_bn);
        if(extraction.second == 1_bn) {
            result = multiplyMontgomery(result, base_mf, mod, montgomery_coefficient, coefficient);
        }
        degree = extraction.first;
        base_mf = multiplyMontgomery(base_mf, base_mf, mod, montgomery_coefficient, coefficient);
    }
    result = multiply(result, mc_inverted, mod);
    return result;
}

BigNum sqrt(const BigNum& num) {
    if (num == 1_bn) {
        return 1_bn;
    }

    BigNum res = num / 2_bn;
    BigNum left = 0_bn, right = num;

    while(true) {
        BigNum sqr = res * res;
        BigNum res_plus = res + 1_bn;
        BigNum res_minus = res - 1_bn;

        if (sqr == num) {
            return res;
        }

        if (sqr < num) {
            if ((res_plus) * (res_plus) > num) {
                return res;
            }

            left = res;
            try {
                res = (right + left) * 2 / 4_bn;
            } catch(std::exception& e) {
                std::cout << left << std::endl << right << std::endl << std::endl;
            }

        } else {
            if ((res_minus) * (res_minus) < num) {
                return res_minus;
            }
            right = res;
            try {
                res = (right + left) * 2 / 4_bn;
            } catch(std::exception& e) {
                std::cout << left << std::endl << right << std::endl << std::endl;
            }
        }
    }
}

BigNum logStep(const BigNum& num, const BigNum& base, const BigNum& mod) {
    if (num == 1_bn) {
        return 0_bn;
    }
    BigNum sqrt_mod = sqrt(mod);
    if (sqrt_mod * sqrt_mod != mod) {
        sqrt_mod = sqrt_mod + 1_bn;
    }

    std::map<BigNum, BigNum> base_powers;
    for (BigNum i = 0_bn; i < sqrt_mod; i = i + 1_bn) {
        base_powers[powMontgomery(base, i, mod)] = i;
    }

    //calculating the base in mod power to reduce the overall log calculating time
    BigNum base_in_power = powMontgomery(inverted(base, mod, BigNum::InversionPolicy::Fermat), sqrt_mod, mod);

    BigNum curr_base = base_in_power;
    BigNum index = 1_bn;

    while (true) {
        BigNum key = multiply(num, curr_base, mod);
        if (base_powers.count(key) == 1) {
            return (multiply(index, sqrt_mod, mod) + base_powers[key]) % mod;
        }

        curr_base = multiply(curr_base, base_in_power, mod);
        index = index + 1_bn;
    }

}


BigNum Pollard_Num(const BigNum& num){
    BigNum res;
    BigNum a = 2_bn;
    BigNum b = 2_bn;
    BigNum d;
    for (int i = 0; i >= 0; i++){
        a = (a * a + 1_bn) % num;
        b = (b * b + 1_bn) % num;
        b = (b * b + 1_bn) % num;
        if (a > b){
            d = gcd(a - b, num);
        }
        else{
            d = gcd(b - a, num);
        }
        if (d > 1_bn && d < num){
            return d;
        }
        else if (d == num) {
            return num;
        }
    }
    return num;
}
    std::vector<BigNum> Pollard(const BigNum& num){
        if (num == 1_bn) return {};
        BigNum res = Pollard_Num(num);
        std::vector<BigNum> result(Pollard(num/res));
        result.push_back(res);
        return result;
    }

    std::vector<BigNum> Naive(const BigNum& num){
        BigNum N = num;
        std::vector<BigNum> result;
        BigNum a = 2_bn;
        while (N != 1_bn){
            if (N % a != 0_bn)
                a = a + 1_bn;
            else{
                result.push_back(a);
                N = N / a;
            }
        }
        return result;
    }


    std::vector<std::pair<BigNum, BigNum>> factorization(BigNum n) {
        std::vector<std::pair<BigNum, BigNum>> result;
        for (BigNum i = 2_bn; i * i <= n; i = i + 1_bn) {
            BigNum k = 0_bn;
            while (n % i == 0_bn) {
                k = k + 1_bn;
                n = n / i;
            }
            if (k != 0_bn) result.emplace_back(i, k);

        }
        if (n != 1_bn)
            result.emplace_back(n, 1_bn);
        return result;
    }


BigNum totientEulerFunc(BigNum mod) {
    BigNum result = mod;
    for(auto i = 2_bn; i * i <= mod; i = i + 1_bn) {
        if(mod % i == 0_bn) {
            while(mod % i == 0_bn) mod = mod / i;
            result = result - (result / i);
        }
    }
    if(mod > 1_bn) result = result - (result / mod);
    return result;
}

BigNum elementOrder(const BigNum &num, const BigNum &mod) {
    if(gcd(num, mod) != 1_bn){
        throw std::invalid_argument("Not an element of the group. Nums must be coprime");
    }
    /// Group order.
    BigNum result = totientEulerFunc(mod);
    /// Prime factorization of group order.
    auto pf = factorization(result);
    BigNum temp;

    for(const auto& i : pf) {
        result = result / pow(i.first, i.second, mod);
        temp = pow(num, result, mod);
        while(temp != 1_bn) {
            temp = pow(temp, i.first, mod);
            result = result * i.first;
        }
    }
    return result;
}

} // namespace lab
