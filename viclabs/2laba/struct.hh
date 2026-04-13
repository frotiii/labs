struct dd_real {
    double hi; // старшие 53 бита точности числа
    double lo; // младшие 53 бита 
    // так и получается 106 бит почти 4ая точнгость 

    dd_real(double h = 0.0): hi(h), lo(0.0) {}
    dd_real(double h, double l): hi(h), lo(l) {}
};
// ДЛЯ ОТЧЕТА: тут опора также на ту же книгу, что и ниже, 
dd_real operator+(const dd_real& a, const dd_real& b);
dd_real operator-(const dd_real& a, const dd_real& b);
dd_real operator*(const dd_real& a, const dd_real& b);

// ДЛЯ ОТЧЕТА: тут ссылка на алгоритмы 4, 5 и 6 из Library for Double-Double and Quad-Double Arithmetic∗
void two_sum(double a, double b, double& s, double& e);
void two_prod(double a, double b, double& p, double& e);
void split(double a, double& hi, double& lo);

dd_real exp_dd(const dd_real& x);
