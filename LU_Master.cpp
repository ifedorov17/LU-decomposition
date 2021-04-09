#include "LU_Master.h"

void LU_Master::LU(CSR A, std::vector<std::vector<double>>& L, std::vector<std::vector<double>>& U)
{
	unsigned size = A.getSize();

	L.resize(size);
	U.resize(size);
	for (int i = 0; i < size; i++) {
		L[i].resize(size);
		U[i].resize(size);
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			U[i][j] = A.get(i, j);
			L[j][i] = U[j][i] / U[i][i];
		}
	}
	
	for (int k = 1; k < size; k++)
	{
#pragma omp parallel for schedule(static)
		for (int i = k - 1; i < size; i++) {
			for (int j = i; j < size; j++) {
				L[j][i] = U[j][i] / U[i][i];
			}
		}

#pragma omp parallel for schedule(static)
		for (int i = k; i < size; i++) {
			for (int j = k - 1; j < size; j++) {
				U[i][j] = U[i][j] - L[i][k - 1] * U[k - 1][j];
			}
		}
	}
}

void LU_Master::proisv(std::vector<std::vector <double>> A, std::vector<std::vector <double>> B,
	std::vector<std::vector<double>>& R, unsigned size)
{
    #pragma omp parallel for schedule(static)
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < size; k++) {
				R[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

CSR LU_Master::parse_matrix(std::string path)
{
	std::vector<double> aelem;
	std::vector<int> jptr;
	std::vector<int> iptr;
	std::vector<std::tuple<int, int, double>> tmpMatrix;

	std::string tmp;
	double num;

	std::ifstream file_stream(path);
	if (!file_stream.is_open())
	{
		std::cout << "ERROR" << std::endl;
		throw(std::exception()); 
	}
	else
	{
		while (std::getline(file_stream, tmp))
		{
			std::regex num("[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)");
			std::sregex_token_iterator tokenIt(tmp.cbegin(), tmp.cend(), num);
			int row = std::stoi(*tokenIt);
			tokenIt++;
			int col = std::stoi(*tokenIt);
			tokenIt++;
			double value = std::stod(*tokenIt);
			tmpMatrix.push_back(std::make_tuple(row, col, value));
		}
		std::sort(
			tmpMatrix.begin(),
			tmpMatrix.end(),
			[](const auto& a, const auto& b)
			{
				if (std::get<0>(a) == std::get<0>(b)) return (std::get<1>(a) < std::get<1>(b));
				return	(std::get<0>(a) < std::get<0>(b));
			}
		);

		int prev_row = -1;

		for (int i = 0; i <  tmpMatrix.size(); i++)
		{
			aelem.push_back(std::get<2>(tmpMatrix[i]));
			jptr.push_back(std::get<1>(tmpMatrix[i]));
			if (std::get<0>(tmpMatrix[i]) != prev_row)
			{
				iptr.push_back(i+1);
				prev_row = std::get<0>(tmpMatrix[i]); 
			}

		}
		iptr.push_back(aelem.size()+1); 
	}
	
	CSR result(aelem, jptr, iptr, iptr.size() - 1);

	return result;
}

double LU_Master::sumY(unsigned size, std::vector<std::vector<double>> L, std::vector<double> b, std::vector<double> y, int i)
{
	double result = 0.;
	for (int k = 0; k < i; k++)
	{
		result += L[i][k] * y[k];
	}
	return result;
}

double LU_Master::sumX(unsigned size, std::vector<std::vector<double>> U, std::vector<double> x, int i)
{
	double result = 0.;
	for (int k = size - 1; k > i; k--)
	{
		result += U[i][k] * x[k];
	}
	return result;
}

void LU_Master::solver(unsigned size, std::vector<std::vector<double>> L, std::vector<std::vector<double>> U, std::vector<double> b, std::vector<double>& y, std::vector<double>& x)
{
	for (int i = 0; i < size; i++)
	{
		y[i] = (b[i] - LU_Master::sumY(size, L, b, y, i)) / L[i][i];
	}
	for (int i = size - 1; i >= 0; i--)
	{
		x[i] = (y[i] - LU_Master::sumX(size, U, x, i)) / U[i][i];
	}
}
