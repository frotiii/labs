#include "struct.hh"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <boost/multiprecision/cpp_bin_float.hpp>

using namespace boost::multiprecision;

cpp_bin_float_100 dd_to_mp100(const dd_real& x) {
    return cpp_bin_float_100(x.hi) + cpp_bin_float_100(x.lo);
}

void test_extreme_cases() {
    std::cout << "ТЕСТ 1" << std::endl;
    
    double test_values[] = {1.0, -1.0, 2.0, -2.0, 4e-300};
    
    std::cout << std::scientific << std::setprecision(30);
    
    for (int i = 0; i < 5; i++) {
        double val = test_values[i];
        dd_real x(val);
        dd_real res = exp_dd(x);
        
        // 100 бит
        cpp_bin_float_100 exact = exp(dd_to_mp100(x));

        std::cout << "\n x = " << val << std::endl;
        std::cout << "   double-double = " << res.hi << "+" <<  res.lo << std::endl;
        std::cout << "    с библиотеки 100 бит = " << exact << std::endl;
        std::cout << "      разница моего и библиотечного ( но в 100битной арифметике , те с приведением к тому типу ) " << (exact - dd_to_mp100(res)) << std::endl;
        
    }
}


void test_deg() {
    std::cout << "ТЕСТ 2: ПРОВЕРКА СВОЙСТВА exp(a+b) = exp(a)*exp(b)" << std::endl;

    std::pair<double, double> pairs[] = {
        {0.1, 0.2},
        {1e-8, 2e-9},
        {5.32434542111, 0.334333321111111},
        {0.83748423920048582234356, 1.934842921392982} ,
        {-700.132412432423111132, 400.493292919911132234},
        {-445.93843434222222342, -8.3242424224}
    };
    
    std::cout << std::scientific << std::setprecision(30);
    
    for (const auto& p : pairs) {
        double a = p.first;
        double b = p.second;
        
        dd_real x(a);
        dd_real y(b);
        
        dd_real exp_ab_dd = exp_dd(x + y);
        dd_real exp_prod_dd = exp_dd(x) * exp_dd(y);
        

        cpp_bin_float_100 aa = a;
        cpp_bin_float_100 bb = b;
        cpp_bin_float_100 exp_sum_mp = exp(aa + bb);
        cpp_bin_float_100 exp_prod_mp = exp(aa) * exp(bb);
        cpp_bin_float_100 diff_mp = exp_sum_mp - exp_prod_mp;
        
        std::cout << "\n a = " << a << ", b = " << b << std::endl;
        
        std::cout << "\nМОЯ АРИФМЕТИКА" << std::endl;
        std::cout << "     exp(a+b)      = " << exp_ab_dd.hi << " + " << exp_ab_dd.lo << std::endl;
        std::cout << "     exp(a)*exp(b) = " << exp_prod_dd.hi << " + " << exp_prod_dd.lo << std::endl;

        dd_real diff_dd = exp_ab_dd - exp_prod_dd;
        std::cout << "     РАЗНИЦА       = " << diff_dd.hi << " + " << diff_dd.lo << std::endl;
        
        std::cout << "\nЭТАЛОН" << std::endl;
        std::cout << "     exp(a+b)      = " << exp_sum_mp << std::endl;
        std::cout << "     exp(a)*exp(b) = " << exp_prod_mp << std::endl;
        std::cout << "     РАЗНИЦА       = " << diff_mp << std::endl;
    }

}
int main() {
    std::cout << "ТЕСТИРОВАНИЕ" << std::endl;
    std::cout << "Эталон 100 бит" << std::endl;
    
    test_extreme_cases();
    test_deg();
    
    std::cout << "ВСЕ ТЕСТЫ ЗАВЕРШЕНЫ" << std::endl;
    
    return 0;
}