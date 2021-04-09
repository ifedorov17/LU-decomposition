#pragma once
#include "CSR.h"
#include <regex>
#include <omp.h>
#include <tuple>
#include <exception>
class LU_Master
{
public:
	static void LU(CSR A, std::vector<std::vector<double>>& L, std::vector<std::vector<double>>& U);
	static void proisv(std::vector<std::vector <double>> A, std::vector<std::vector <double>> B,std::vector<std::vector <double>>& R, unsigned size);
	static CSR parse_matrix(std::string path); 
	static void solver(unsigned size, std::vector<std::vector<double>> L, std::vector<std::vector<double>> U, std::vector<double> b, std::vector<double>& y, std::vector<double>& x);

private:
	static double sumY(unsigned size, std::vector<std::vector<double>> L, std::vector<double> b, std::vector<double> y, int i);
	static double sumX(unsigned size, std::vector<std::vector<double>> U, std::vector<double> x, int i);
};
