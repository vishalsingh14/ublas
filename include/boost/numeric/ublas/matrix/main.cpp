
#include "matrix.hpp"

int main()
{
    comptest::matrix<double> A {{12, 5, 45},
                                {32, 43, 55},
                                {23, 53, 66}};

    std::cout << "A\n" << A << std::endl;

    std::tuple<comptest::matrix<double>, comptest::matrix<double>> qr = comptest::qr(A);

    comptest::matrix<double> q = std::get<0>(qr);
    comptest::matrix<double> r = std::get<1>(qr);

    std::cout << "q\n" << q << std::endl;
    std::cout << "r\n" << r << std::endl;
    std::cout << "q*r\n" << q*r << std::endl;

    if(A == q*r) {
        std::cout << "qr decomposition works correctly \n";
    }
    else {
        std::cout << "qr decomposition does not works correctly\n";
    }

}