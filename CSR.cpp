#include "CSR.h"
#include <algorithm>

CSR::CSR(std::vector<double>& tmp_aelem_, std::vector<int>& tmp_jptr_, std::vector<int>& tmp_iptr_, int size_)
{
	aelem = tmp_aelem_; 
	jptr = tmp_jptr_; 
	iptr = tmp_iptr_; 
	size = size_; 

}

CSR::CSR():size(0){}

double CSR::get(int i, int j)
{
	for (int k = iptr[i] - 1; k < iptr[i + 1] - 1; k++) {
		if (jptr[k] == j + 1) {
			return aelem[k];
		}
	}
	return 0;
}

void CSR::make_null()
{
	aelem.clear(); 
	jptr.clear(); 
	iptr.clear(); 
	size = 0; 
}

void CSR::loadCSR(std::string& path)
{
	std::ifstream in_stream;
	in_stream.open(path);
	std::string tmp;
	int num;
	double num1;
	if (!in_stream.is_open())
	{
		std::cout << "ERROR" << std::endl;
	}
	else
	{
		getline(in_stream, tmp);
		std::stringstream iss(tmp);
		while (iss >> num1)
		{
			this->aelem.push_back(num1);
		}

		getline(in_stream, tmp);
		std::stringstream iss1(tmp);
		while (iss1 >> num)
		{
			this->jptr.push_back(num);
		}

		getline(in_stream, tmp);
		std::stringstream iss2(tmp);
		while (iss2>> num)
		{
			this->iptr.push_back(num);
		}

		this->size = *(std::max_element(this->jptr.begin(), this->jptr.end()));
	}
}

void CSR::printCSR()
{
	int amount_of_numbers_in_cur_row = 0;
	int now_reading = 0;
	double num_to_put = 0;

	for (int i = 0; i < this->size; i++)
	{
		amount_of_numbers_in_cur_row = this->iptr[i + 1] - this->iptr[i];
		for (int k = 0; k < this->size; k++)
		{
			num_to_put = this->aelem[now_reading];
			if ((k == this->jptr[now_reading]) && (amount_of_numbers_in_cur_row != 0))
			{
				now_reading++;
				std::cout << "   " << num_to_put << "";
				amount_of_numbers_in_cur_row--;
			}
			else
			{
				std::cout << "   0";
			}
		}
		std::cout << std::endl << std::endl;
	}
}

int CSR::getSize()
{
	return size;
}

std::vector<double> CSR::operator*(const std::vector<double>& p_vec)
{
	std::vector<double> res;
	res.reserve(this->size);
	res.resize(this->size);

	if (size != p_vec.size())
		throw(std::exception("yikies!"));

	#pragma omp parallel for schedule(static)
	for (int row = 0; row < size; row++)
	{
		double elem = 0;
		for (int col = iptr[row] - 1; col < iptr[row + 1] - 1; col++)
		{
			elem += p_vec[jptr[col] - 1] * aelem[col];
		}
		res[row] = elem;
	}
	return res;
}


std::list<CSR> load(std::string& path)
{
	std::list<CSR> matrix_return; 
	std::ifstream in_stream; 
	in_stream.open(path); 
	std::string tmp;
	int num;
	double num1; 
	std::vector<double> tmp_aelem;
	std::vector<int> tmp_jptr;
	std::vector<int> tmp_iptr;
	int tmp_size;
	if (!in_stream.is_open())
	{
		std::cout << "ERROR" << std::endl; 
	}
	else
	{
		while (in_stream)
		{
			getline(in_stream, tmp); 
			if (tmp == "")
			{
				continue; 
			} 
			tmp_aelem.clear(); 
			tmp_jptr.clear(); 
			tmp_iptr.clear(); 
			tmp_size = 0; 
			for (int i = 0; i < 4; ++i)
			{
				
				if (i == 0)
				{
					getline(in_stream, tmp); 
					std::stringstream iss(tmp);
					while (iss >> num1)
					{
						tmp_aelem.push_back(num1); 
					}

				}

				else if (i == 1)
				{
					getline(in_stream, tmp);
					std::stringstream iss(tmp);
					while (iss >> num)
					{
						tmp_jptr.push_back(num);
					}
				}
				else if (i == 2)
				{
					getline(in_stream, tmp);
					std::stringstream iss(tmp);
					while (iss >> num)
					{
						tmp_iptr.push_back(num);
					}
				}
				else if (i == 3)
				{
					getline(in_stream, tmp);
					std::stringstream iss(tmp);
					while (iss >> num)
					{
						tmp_size = num; 
					}
				}
				
				
			}
			CSR tmp_matrix(tmp_aelem, tmp_jptr, tmp_iptr, tmp_size);
			matrix_return.push_back(tmp_matrix);
		}
	}
	return matrix_return; 
}
