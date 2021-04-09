#include "LU_Master.h"


const int threadNumber = 256;

void LU_Master::LU(CSR A, std::vector<std::vector<double>>& L, std::vector<std::vector<double>>& U)
{
	int n = A.getSize();
	
	L.resize(n);
	U.resize(n);
	for (int i = 0; i < n; i++) {
		L[i].resize(n);
		U[i].resize(n);
	}

	for (int i = 0; i < n; i++) {   //ѕриравниваем U=A
		for (int j = 0; j < n; j++) {
			U[i][j] = A.get(i, j);
		}
	}

	for (int i = 0; i < n; i++)
		for (int j = i; j < n; j++)
			L[j][i] = U[j][i] / U[i][i];

	omp_set_num_threads(threadNumber);

	#pragma omp parallel for schedule(static)
	for (int k = 1; k < n; k++)
	{
		for (int i = k - 1; i < n; i++)
			for (int j = i; j < n; j++)
				L[j][i] = U[j][i] / U[i][i];

		for (int i = k; i < n; i++)
			for (int j = k - 1; j < n; j++)
				U[i][j] = U[i][j] - L[i][k - 1] * U[k - 1][j];
	}
	//что тут происходит € не знаю
}

void LU_Master::proisv(std::vector<std::vector <double>> A, std::vector<std::vector <double>> B,
	std::vector<std::vector<double>>& R, int n)
{
	omp_set_num_threads(threadNumber);

    #pragma omp parallel for schedule(static)

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			for (int k = 0; k < n; k++)
				R[i][j] += A[i][k] * B[k][j];
}

std::vector<double> LU_Master::solver_1(std::vector<std::vector<double>>& L, std::vector<std::vector<double>>& U, std::vector<double>& b)
{
	int n = L.size();
	std::vector<double> y(n); // делаем подстановку Ly = b и находим вектор y 
	std::vector<double> x(n); // находим вектор решений 
	double summ = 0.; 
	// ‘ормула следующа€ : y[i] = b[i] - ([сумма от j = 1 до i-1](L[i][j]*y[i]))   <- i-й элемент вектора y 
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < i; j++)
		{
			summ += L[i][j] * y[j]; 
		}
		y[i] = b[i] - summ; 
		summ = 0.; 
	}
	// после цикла есть вектор y и начинаетс€ обратна€ подстановка Ux = y; (т к U - верхнетреугольна€ то решение снизу вверх и формула
	// получаетс€ x[n-1] = 1/u[n-1][n-1] * (y[n-1] - [сумма от j = 0 до i-1](U[n-i][n-j]*x[n-j]))
	summ = 0.; 
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < i; j++)
		{
			summ += U[n - i - 1 ][n - j - 1] * x[n - j - 1]; 
		}
		x[n - i-1] = 1 / U[n - i-1][n - i-1] * (y[n - i-1] - summ); 
		summ = 0.; 
	}

	return x; 
}

CSR LU_Master::parse_matrix(std::string path)
{
	std::vector<double> aelem;


	std::vector<int> jptr;
	std::vector<int> iptr;
	std::vector<std::tuple<int, int, double>> JoeMama;
	


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
			std::regex num("[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)"); // [+\\-]{0,1}\\d+ 
			/* std::transform(std::sregex_token_iterator{ tmp.cbegin(), tmp.cend(), num },
				{},
				std::back_inserter(numbers),
				[](const auto& val)
				{
					return std::stod(val.str());
				}
			); */
			std::sregex_token_iterator coca_cola(tmp.cbegin(), tmp.cend(), num);
			int row = std::stoi(*coca_cola);
			coca_cola++;
			int col = std::stoi(*coca_cola);
			coca_cola++;
			double pepsi = std::stod(*coca_cola);
			JoeMama.push_back(std::make_tuple(row, col, pepsi));
		}
		std::sort(
			JoeMama.begin(),
			JoeMama.end(),
			[](const auto& a, const auto& b)
			{
				if (std::get<0>(a) == std::get<0>(b)) return (std::get<1>(a) < std::get<1>(b));
				return	(std::get<0>(a) < std::get<0>(b));
			}
		);

		int prev_row = -1; // дл€ сани

		for (int i = 0; i <  JoeMama.size(); i++)
		{
			aelem.push_back(std::get<2>(JoeMama[i]));
			jptr.push_back(std::get<1>(JoeMama[i]));
			if (std::get<0>(JoeMama[i]) != prev_row)
			{
				iptr.push_back(i+1);
				prev_row = std::get<0>(JoeMama[i]); 
			}

		}
		iptr.push_back(aelem.size()+1); 
	

		
		
	}
	
	CSR ret(aelem, jptr, iptr, iptr.size() - 1);

	return ret;
}
