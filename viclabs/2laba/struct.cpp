#include "struct.hh"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <iomanip>

//алгоритмы из книги, ссылка была в фыайле hh 
void two_sum(double a, double b, double& s, double& e){
    s = a + b;
    double v = s - a;
    e = (a - (s - v)) + (b - v);
}

void two_prod(double a, double b, double& p, double& e){
    p = a * b;
    double ahi, alo, bhi, blo;
    split(a, ahi, alo);
    split(b, bhi, blo);
    e = ((ahi * bhi - p) + ahi * blo + alo * bhi) + alo * blo;
}

void split(double a, double& hi, double& lo){
    const double tmp = 134217729.0;
    double t = tmp * a;
    hi = t - (t - a);
    lo = a - hi;
}


//кароче при дальнейшем вызове + - * будет использоватьсчя МОЙ оператор 
dd_real operator+(const dd_real& a, const dd_real& b){
    double s1, e1;
    two_sum(a.hi, b.hi, s1, e1);
    
    double s2 = e1 + a.lo + b.lo;
    
    //нормализация
    double r1 = s1 + s2;
    double r2 = s2 - (r1 - s1);
    
    return dd_real(r1, r2);
}

dd_real operator-(const dd_real& a, const dd_real& b){
    return a + dd_real(-b.hi, -b.lo);
}

dd_real operator*(const dd_real& a, const dd_real& b){
    double p, e;
    two_prod(a.hi, b.hi, p, e);
    
    double t = e + a.hi * b.lo + a.lo * b.hi;
    
    // нормализация
    double r1 = p + t;
    double r2 = t - (r1 - p);
    
    return dd_real(r1, r2);
}

// ВСЕ ЧТО ПО ЗАДАНИЮ УЖЕ

// старшие 53 бита и оставшие младшие , для точнгости!!1
// пункт 5.1 все той же книги про эксопененту, приведение с помощью нат лога от 2
const double LN2_HI = 0.6931471805599453;
const double LN2_LO = 2.319046813846299e-17;

// ряд тейлора , ток для маленьких x по модулю , тож в книге
dd_real exp_small(const dd_real& x) {
    // exp(x) = сумма от n = 0 до беск (x**n)/n!
    dd_real result(1.0); // для результата 
    dd_real term(1.0); // для текущего члена ряда , пока это нулевой член, те 1
    
    for (int i = 1; i <= 20; i++) { //20 итераций дают норм точнсть , также 5.1 пункт 
        term = term * x * (1.0 / i);
        result = result + term;
        
        // выход раньше, если член становитсмя слишком млаеньким , тк оч маленький член не повлияет на результат 
        if (std::abs(term.hi) < 1e-30) break;
    }
    
    return result;
}

// тут сотвественно она и вычисляте экспоненту нуэной точности от x 
dd_real exp_dd(const dd_real& x) {
    // просто если степень нулевая 
    if (x.hi == 0.0 && x.lo == 0.0) {
        return dd_real(1.0);
    }
    
    // x = k * ln2 + r, где моджуль r  <= ln2 ( стр 386 учебнкиа из письма )
    // k = round(x / ln2) но будет с деление на 2 тк round 
    double k = std::round(x.hi / LN2_HI); // приближение к ln2
    
    // r = x - k * ln2
    dd_real k_ln2_hi = dd_real(k) * LN2_HI;
    dd_real k_ln2_lo = dd_real(k) * LN2_LO;
    dd_real r = x - (k_ln2_hi + k_ln2_lo); // остаток с высокой точностью 
    

    dd_real exp_r = exp_small(r);
    
    // exp(x) = exp(r) * 2^k ( та же 386 страница )
    if (k != 0) {
        exp_r = exp_r * std::ldexp(1.0, static_cast<int>(k));
    }
    
    return exp_r;
}
