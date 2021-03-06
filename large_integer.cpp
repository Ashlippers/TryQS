//#include <iostream>
#include <cstring>
#include <cstdlib>
#include "large_integer.h"
//#include <cstdio>
//using namespace std;


BigInt::BigInt()
{
    valid = true;
    negative = false;
    for (int i = 0; i < DIGIT_MAX; ++i)
        num[i] = 0;
}
BigInt::BigInt(const char*s)
{
    valid = true;
    int i = 0, l = strlen(s);
    memset(num, 0, sizeof(num));
    if (s[0] == '-')
    {
        negative = true;
        ++i;
    }
    else
        negative = false;
    for (; i < l; ++i)
        num[(l - i - 1) / 8] = num[(l - i - 1) / 8] * 10 + s[i] - '0';
}
BigInt::BigInt(int i)
{
    valid = true;
    if (i < 0)
    {
        negative = true;
        i = -i;
    }
    else
        negative = false;
    for (int j = 0; j < 3; ++j)
    {
        num[j] = (i % 100000000);
        i /= 100000000;
    }
    for (int j = 3; j < DIGIT_MAX; ++j)
        num[j] = 0;
}
BigInt::BigInt(long i)
{
    valid = true;
    if (i < 0)
    {
        negative = true;
        i = -i;
    }
    else
        negative = false;
    for (int j = 0; j < 3; ++j)
    {
        num[j] = (i % 100000000);
        i /= 100000000;
    }
    for (int j = 3; j < DIGIT_MAX; ++j)
        num[j] = 0;
}

int BigInt::compare(const BigInt &b) const //>:1 ; =:0 ; <:-1
{
    if (!negative&&b.negative)
        return 1;
    if (negative&&!b.negative)
        return -1;
    if (negative&&b.negative)
    {
        for (int i = DIGIT_MAX - 1; i >= 0; --i)
        {
            if (num[i] < b.num[i])
                return 1;
            if (num[i] > b.num[i])
                return -1;
        }
        return 0;
    }
    if (!negative&&!b.negative)
    {
        for (int i = DIGIT_MAX - 1; i >= 0; --i)
        {
            if (num[i] > b.num[i])
                return 1;
            if (num[i] < b.num[i])
                return -1;
        }
        return 0;
    }
}
int BigInt::kcompare(const BigInt &b, int k) const
{
    for (int i = DIGIT_MAX - 1; i >= k; --i)
    {
        if (num[i] < b.num[i - k])
            return -1;
        if (num[i] > b.num[i - k])
            return 1;
    }
    return 0;
}

bool BigInt::operator == (const BigInt &b) const
{
    if (negative^b.negative)
        return false;
    for (int i = 0; i < DIGIT_MAX; ++i)
        if (num[i] != b.num[i])
            return false;
    return true;
}
bool BigInt::operator != (const BigInt &b) const
{
    if (negative^b.negative)
        return true;
    for (int i = 0; i < DIGIT_MAX; ++i)
        if (num[i] != b.num[i])
            return true;
    return false;
}
bool BigInt::operator < (const BigInt &b) const
{
    if (compare(b) < 0)
        return true;
    return false;
}
bool BigInt::operator >= (const BigInt &b) const
{
    if (compare(b) >= 0)
        return true;
    return false;
}

BigInt BigInt::operator - () const
{
    BigInt tmp(*this);
    if (tmp != 0)
        tmp.negative = !negative;
    return tmp;
}
BigInt BigInt::operator +(const BigInt &b)
{
    BigInt tmp(*this);
    if (compare(0)*b.compare(0) < 0)
        return tmp - (-b);
    for (int i = 0; i < DIGIT_MAX; ++i)
    {
        tmp.num[i] += b.num[i];
        if (tmp.num[i]>99999999)
        {
            tmp.num[i] -= 100000000;
            tmp.num[i + 1]++;
        }
    }
    if (compare(0) == 0)
        tmp.negative = b.negative;
    return tmp;
}
BigInt BigInt::operator -(const BigInt &b)
{
    BigInt tmp(*this);
    if ((negative&&!b.negative) || (!negative&&b.negative))
        return tmp + (-b);
    if (compare(b) == 0)
        return 0;
    else if (compare(b) < 0)
        return -(-(-b) - tmp);
    else if (negative&&b.negative)
        return (-b) - (-tmp);
    bool tag = false;
    for (int i = 0; i < DIGIT_MAX; ++i)
    {
        if (tmp.num[i] < b.num[i] + tag)
        {
            tmp.num[i] += 100000000;
            tmp.num[i] -= b.num[i] + tag;
            tag = true;
        }
        else
        {
            tmp.num[i] -= b.num[i] + tag;
            tag = false;
        }
    }
    tmp.negative = false;
    return tmp;
}
BigInt BigInt::operator *(const BigInt &b)
{
    int da = DIGIT_MAX - 1, db = DIGIT_MAX - 1;
    while (num[da] == 0 && da > 0)
        --da;
    while (b.num[db] == 0 && db > 0)
        --db;
    
    BigInt tmp(0);
    for (int i = 0; i <= da; ++i)
        for (int j = 0; j <= db; ++j)
            if (num[i] > 0 && b.num[j] > 0)
            {
                if (i + j >= DIGIT_MAX)
                {
                    tmp = 0;
                    tmp.valid = false;
                    cerr << "multiplication error: too many digits!" <<i<<' '<<j<< endl;
                    return tmp;
                }
                tmp.num[i + j] += num[i] * b.num[j];
            }
    for (int i = 0; i < DIGIT_MAX - 1 && i <= da + db; ++i)
    {
        tmp.num[i + 1] += (tmp.num[i] / 100000000);
        tmp.num[i] %= 100000000;
    }
    if (tmp.num[DIGIT_MAX - 1] > 99999999)
    {
        tmp = 0;
        tmp.valid = false;
        cerr << "multiplication error: too many digits!" << endl;
        return tmp;
    }
    if (compare(0)*b.compare(0) >= 0)
        tmp.negative = false;
    else
        tmp.negative = true;
    return tmp;
}
BigInt BigInt::operator /(const BigInt &b) //a,b are positive
{
    if (b == 0)
    {
        cerr << "cannot divided by 0!" << endl;
        return 0;
    }
    if (compare(0)<0 || b<0)
    {
        cerr << "cannot solve this division..." << endl;
        return 0;
    }
    if (compare(b) < 0)
        return 0;
    int da = DIGIT_MAX - 1, db = DIGIT_MAX - 1;
    while (num[da] == 0 && da > 0)
        --da;
    while (b.num[db] == 0 && db > 0)
        --db;
    
    BigInt divisor(b), quotient(0), remainder(*this), tmp(0);
    for (int i = da - db; i >= 0; --i)
    {
        int l = 0, r = 99999999;
        while (l < r - 1)
        {
            int mid = (l + r) / 2;
            BigInt dtmp = divisor*mid;
            int res = remainder.kcompare(dtmp, i);
            if (res < 0)
                r = mid;
            else if (res > 0)
                l = mid;
            else
            {
                l = mid;
                break;
            }
        }
        quotient.num[i] = l;
        tmp = divisor*l;
        for (int k = DIGIT_MAX - 1; k >= i; --k)
            tmp.num[k] = tmp.num[k - i];
        for (int k = i - 1; k >= 0; --k)
            tmp.num[k] = 0;
        remainder = remainder - tmp;
    }
    return quotient;
}
BigInt BigInt::operator %(const BigInt &b) //a,b are positive
{
    if (b == 0)
    {
        cerr << "cannot divided by 0!" << endl;
        return 0;
    }
    if (compare(0)<0 || b<0)
    {
        cerr << "cannot solve this complementation..." << endl;
        return 0;
    }
    if (compare(b) < 0)
        return *this;
    int da = DIGIT_MAX - 1, db = DIGIT_MAX - 1;
    while (num[da] == 0 && da > 0)
        --da;
    while (b.num[db] == 0 && db > 0)
        --db;
    
    BigInt divisor(b), quotient(0), remainder(*this), tmp(0);
    for (int i = da - db; i >= 0; --i)
    {
        int l = 0, r = 99999999;
        while (l < r - 1)
        {
            int mid = (l + r) / 2;
            BigInt dtmp = divisor*mid;
            int res = remainder.kcompare(dtmp, i);
            if (res < 0)
                r = mid;
            else if (res > 0)
                l = mid;
            else
            {
                l = mid;
                break;
            }
        }
        quotient.num[i] = l;
        tmp = divisor*l;
        for (int k = DIGIT_MAX - 1; k >= i; --k)
            tmp.num[k] = tmp.num[k - i];
        for (int k = i - 1; k >= 0; --k)
            tmp.num[k] = 0;
        remainder = remainder - tmp;
    }
    return remainder;
}
BigInt BigInt::bigsqrt()
{
    if (compare(0) < 0)
    {
        cerr << "cannot solve this sqaure root: n < 0" << endl;
        return 0;
    }
    int digit = DIGIT_MAX - 1;
    while (num[digit] == 0 && digit > 0)
        --digit;
    
    BigInt ans(0), remainder(*this);
    for (int i = digit / 2; i >= 0; --i)
    {
        BigInt partial_ans(0), partial_remainder(0);
        for (int j = 0; j < digit / 2 - i; ++j)
            partial_ans.num[j] = ans.num[i + j + 1];
        for (int j = 0; j <= digit - 2 * i; ++j)
            partial_remainder.num[j] = remainder.num[2*i + j];
        
        int l = 0, r = 99999999;
        while (l < r - 1)
        {
            int mid = (l + r) / 2;
            int res = partial_remainder.compare((partial_ans * 200000000 + mid)*mid);
            if (res < 0)
                r = mid;
            else if (res>0)
                l = mid;
            else
            {
                l = mid;
                break;
            }
        }
        ans.num[i] = l;
        partial_remainder = partial_remainder - (partial_ans * 200000000 + l)*l;
        for (int j = 0; j <= digit - 2 * i; ++j)
            remainder.num[2 * i + j] = partial_remainder.num[j];
    }
    return ans;
}
bool BigInt::isprime()
{
	bool binary[3500] = {};
	BigInt tmp(*this);
	tmp = tmp - 1;
	int i;
	for (i = 0; i < 3500; ++i)
	{
		if (tmp.num[0] % 2 == 0)
			binary[i] = 0;
		else
			binary[i] = 1;
		tmp = tmp / 2;
		if (tmp == 0)
			break;
	}
	for (i = 3499; i >= 0; --i)
	if (binary[i])
		break;
	
	BigInt c2(1);
	for (i = 3499; i >= 0; --i)
	{
		c2 = (c2*c2) % (*this);
		if (binary[i])
			c2 = (c2*2) % (*this);
	}
	BigInt c3(1);
	for (i = 3499; i >= 0; --i)
	{
		c3 = (c3*c3) % (*this);
		if (binary[i])
			c3 = (c3*3) % (*this);
	}
	if (c2 == 1 && c3 == 1)
		return true;
	return false;
}

ostream &operator << (ostream &o, const BigInt &a)
{
    if (a.negative)
        o << '-';
    int i = DIGIT_MAX - 1;
    for (; i >= 0; --i)
        if (a.num[i])
            break;
    if (i < 0)
    {
        o << 0;
        return o;
    }
    o << a.num[i];
    --i;
    for (; i >= 0; --i)
    {
        if (a.num[i] < 10000000)
            o << 0;
        if (a.num[i] < 1000000)
            o << 0;
        if (a.num[i] < 100000)
            o << 0;
        if (a.num[i] < 10000)
            o << 0;
        if (a.num[i] < 1000)
            o << 0;
        if (a.num[i] < 100)
            o << 0;
        if (a.num[i] < 10)
            o << 0;
        o << a.num[i];
    }
    return o;
}

BigInt biggcd(BigInt a,BigInt b)
{
    if (a == 0)
        return b;
    if (b == 0)
        return a;
    if (a < b)
        return biggcd(b%a, a);
    return biggcd(a%b, b);
}
/*
 int main()
 {
	char s1[5001], s2[5001];
	
	while (cin >> s1 >> s2) {
 BigInt a(s1);
 BigInt b(s2);
 cout << a + b << endl;
 cout << a - b << endl;
 cout << b - a << endl;
 cout << a*b << endl;
 cout << a/b << endl;
 cout << a%b << endl;
 cout << a.bigsqrt() << ' ' << b.bigsqrt() << endl;
	}
 }*/
