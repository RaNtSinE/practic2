#include <iterator>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <locale.h>
#include <math.h>

using namespace std;

void MedianLinks()
{
	string str;
	ifstream In("practic.txt");
	vector<string> v;
	while (!In.eof())
	{
		getline(In, str);
		v.push_back(str);
	}

	vector<string> region;
	vector<double> factor_x;
	vector<double> res_y;

	string delimiter = "\t";

	for (int i = 0; i < v.size() - 2; i++)
	{
		string token = v[i].substr(0, v[i].find(delimiter));
		string numbers = v[i].substr(v[i].find(delimiter) + 1, v[i].length() - 1);
		string number_1 = numbers.substr(0, numbers.find(delimiter));
		string number_2 = numbers.substr(numbers.find(delimiter) + 1, numbers.length() - 1);
		region.push_back(token);
		factor_x.push_back(::atof(number_1.c_str()));
		res_y.push_back(::atof(number_2.c_str()));
	}

	double mid_x, mid_y, sq_x, sq_y;

	for (int i = 2; i > 0; i--)
	{
		string buffer = v[v.size() - i].substr(1, v[v.size() - i].length() - 1);
		string number_1 = buffer.substr(0, buffer.find(delimiter));
		string number_2 = buffer.substr(buffer.find(delimiter) + 1, buffer.length() - 1);

		if (i == 2)
		{
			mid_x = ::atof(number_1.c_str());
			mid_y = ::atof(number_2.c_str());
		}
		else
		{
			sq_x = ::atof(number_1.c_str());
			sq_y = ::atof(number_2.c_str());
		}
	}

	for (int i = 0; i < region.size(); i++)
	{
		factor_x[i] = (factor_x[i] - mid_x) / sq_x;
		res_y[i] = (res_y[i] - mid_y) / sq_y;
	}

	vector<vector<double>> rasst;

	for (int i = 0; i < region.size(); i++)
	{
		vector<double> temp;
		for (int j = 0; j < region.size(); j++)
		{
			temp.push_back(0);
		}
		rasst.push_back(temp);
	}

	int limit = 1;
	float ran = 1;

	for (int i = 0; i < region.size(); i++)
	{
		for (int j = limit; j < region.size(); j++)
		{
			double buf = sqrt((factor_x[j] - factor_x[i]) * (factor_x[j] - factor_x[i]) + (res_y[j] - res_y[i]) * (res_y[j] - res_y[i]));
			rasst[i][j] = buf;
			rasst[j][i] = buf;
		}
		limit++;
	}

	ofstream out;
	out.open("rasst.xls");
	if (out.is_open())
	{
		out << "\t";
		for (int i = 0; i < region.size(); i++)
		{
			out << region[i] << "\t";
		}
		out << endl;
		for (int i = 0; i < region.size(); i++)
		{
			out << region[i] << "\t";
			for (int j = 0; j < region.size(); j++)
			{
				out << rasst[i][j] << '\t';
			}
			out << endl;
		}
	}
	out.close();

	vector<vector<int>> clusters;
	for (int i = 0; i < region.size(); i++)
	{
		vector<int> temp;
		temp.push_back(i);
		clusters.push_back(temp);
	}
	vector<double> center_cluster_x;
	vector<double> center_cluster_y;
	for (int i = 0; i < region.size(); i++)
	{
		center_cluster_x.push_back(factor_x[i]);
		center_cluster_y.push_back(res_y[i]);
	}

	int clust_id_1 = 0;
	int clust_id_2 = 0;

	ofstream out_2;
	out_2.open("last_data.xls");

	for (int i = 0; i < region.size() - 4; i++)
	{
		int clust_id_1 = 0;
		int clust_id_2 = 0;
		double min = 1000;
		limit = 1;
		for (int j = 0; j < clusters.size(); j++)
		{
			for (int l = limit; l < clusters.size(); l++)
			{
				if (rasst[j][l] < min)
				{
					min = rasst[j][l];
					clust_id_1 = l;
					clust_id_2 = j;
				}
			}
			limit++;
		}

		if (out_2.is_open())
		{
			for (int j = 0; j < clusters[clust_id_1].size(); j++)
			{
				out_2 << region[clusters[clust_id_1][j]] << ',';
			}
			out_2 << '\t';
			for (int j = 0; j < clusters[clust_id_2].size(); j++)
			{
				out_2 << region[clusters[clust_id_2][j]] << ',';
			}
			out_2 << '\t';
			out_2 << min << '\t';
			out_2 << endl;
		}

		double middle_x = (center_cluster_x[clust_id_1] + center_cluster_x[clust_id_2]) / 2;
		double middle_y = (center_cluster_y[clust_id_1] + center_cluster_y[clust_id_2]) / 2;



		if (clust_id_1 < clust_id_2)
		{
			center_cluster_x.erase(center_cluster_x.begin() + clust_id_2);
			center_cluster_y.erase(center_cluster_y.begin() + clust_id_2);

			for (int j = 0; j < clusters[clust_id_2].size(); j++)
			{
				clusters[clust_id_1].push_back(clusters[clust_id_2][j]);
			}
			clusters.erase(clusters.begin() + clust_id_2);
			rasst.erase(rasst.begin() + clust_id_2);
			for (int j = 0; j < rasst.size(); j++)
			{
				rasst[j].erase(rasst[j].begin() + clust_id_2);
			}

			for (int j = 0; j < rasst.size(); j++)
			{
				if (clust_id_1 != j)
				{
					double buf = sqrt((middle_x - center_cluster_x[j]) * (middle_x - center_cluster_x[j]) + (middle_y - center_cluster_y[j]) * (middle_y - center_cluster_y[j]));
					rasst[j][clust_id_1] = buf;
					rasst[clust_id_1][j] = buf;
				}
			}
		}
		else
		{
			center_cluster_x.erase(center_cluster_x.begin() + clust_id_1);
			center_cluster_y.erase(center_cluster_y.begin() + clust_id_1);

			for (int j = 0; j < clusters[clust_id_1].size(); j++)
			{
				clusters[clust_id_2].push_back(clusters[clust_id_1][j]);
			}
			clusters.erase(clusters.begin() + clust_id_1);
			rasst.erase(rasst.begin() + clust_id_1);
			for (int j = 0; j < rasst.size(); j++)
			{
				rasst[j].erase(rasst[j].begin() + clust_id_1);
			}
			for (int j = 0; j < rasst.size(); j++)
			{
				if (clust_id_2 != j)
				{
					double buf = sqrt((middle_x - center_cluster_x[j]) * (middle_x - center_cluster_x[j]) + (middle_y - center_cluster_y[j]) * (middle_y - center_cluster_y[j]));
					rasst[j][clust_id_2] = buf;
					rasst[clust_id_2][j] = buf;
				}
			}
		}

		for (int j = 0; j < clusters.size(); j++)
		{
			int count = 0;
			double sum_x = 0;
			double sum_y = 0;
			for (int l = 0; l < clusters[j].size(); l++)
			{
				sum_x += factor_x[clusters[j][l]];
				sum_y += res_y[clusters[j][l]];
				count++;
			}
			center_cluster_x[j] = sum_x / count;
			center_cluster_y[j] = sum_y / count;
		}
	}

	if (out_2.is_open())
	{
		for (int i = 0; i < clusters.size(); i++)
		{
			for (int j = 0; j < clusters[i].size(); j++)
			{
				out_2 << region[clusters[i][j]] << '\t';
			}
			out_2 << endl;
		}
	}
	out_2.close();
}

void Kmeans()
{
	string str;
	ifstream In("practic.txt");
	vector<string> v;
	while (!In.eof())
	{
		getline(In, str);
		v.push_back(str);
	}

	vector<string> region;
	vector<double> factor_x;
	vector<double> res_y;

	string delimiter = "\t";

	for (int i = 0; i < v.size() - 2; i++)
	{
		string token = v[i].substr(0, v[i].find(delimiter));
		string numbers = v[i].substr(v[i].find(delimiter) + 1, v[i].length() - 1);
		string number_1 = numbers.substr(0, numbers.find(delimiter));
		string number_2 = numbers.substr(numbers.find(delimiter) + 1, numbers.length() - 1);
		region.push_back(token);
		factor_x.push_back(::atof(number_1.c_str()));
		res_y.push_back(::atof(number_2.c_str()));
	}

	double mid_x, mid_y, sq_x, sq_y;

	for (int i = 2; i > 0; i--)
	{
		string buffer = v[v.size() - i].substr(1, v[v.size() - i].length() - 1);
		string number_1 = buffer.substr(0, buffer.find(delimiter));
		string number_2 = buffer.substr(buffer.find(delimiter) + 1, buffer.length() - 1);

		if (i == 2)
		{
			mid_x = ::atof(number_1.c_str());
			mid_y = ::atof(number_2.c_str());
		}
		else
		{
			sq_x = ::atof(number_1.c_str());
			sq_y = ::atof(number_2.c_str());
		}
	}


	for (int i = 0; i < region.size(); i++)
	{
		factor_x[i] = (factor_x[i] - mid_x) / sq_x;
		res_y[i] = (res_y[i] - mid_y) / sq_y;
	}

	int K = 4;

	vector<double> Kx;
	vector<double> Ky;
	double ymin = 1000;
	double ymax = -1000;


	for (int i = 0; i < region.size(); i++)
	{
		if (res_y[i] < ymin)
		{
			ymin = res_y[i];
		}
		if (res_y[i] > ymax)
		{
			ymax = res_y[i];
		}
	}

	for (int i = 0; i < K; i++)
	{
		int randNumber = rand() % region.size();
		Kx.push_back(factor_x[randNumber]);
		Ky.push_back(res_y[randNumber]);

		if (i > 0)
		{
			while (Kx[i] == Kx[i - 1] && Ky[i] == Ky[i - 1])
			{
				int randNumber = rand() % region.size();
				Kx.push_back(factor_x[randNumber]);
				Ky.push_back(res_y[randNumber]);
			}
		}
	}

	bool exit = false;

	vector<vector<int>> clusters;

	while (!exit)
	{
		clusters.clear();
		for (int i = 0; i < K; i++)
		{
			vector<int> temp;
			temp.push_back(-1);
			clusters.push_back(temp);
		}

		vector<double> Kxold;
		vector<double> Kyold;
		for (int i = 0; i < K; i++)
		{
			Kxold.push_back(Kx[i]);
			Kyold.push_back(Ky[i]);
		}
		for (int i = 0; i < region.size(); i++)
		{
			int cluster = 1;
			double way = 1000;
			for (int j = 0; j < K; j++)
			{
				double buf = sqrt((factor_x[i] - Kx[j]) * (factor_x[i] - Kx[j]) + (res_y[i] - Ky[j]) * (res_y[i] - Ky[j]));
				if (buf < way)
				{
					way = buf;
					cluster = j;
				}
			}
			if (clusters[cluster][0] == -1)
			{
				clusters[cluster].clear();
			}
			clusters[cluster].push_back(i);
		}

		for (int i = 0; i < K; i++)
		{
			int count = 0;
			double sum_x = 0;
			double sum_y = 0;
			for (int j = 0; j < clusters[i].size(); j++)
			{
				if (clusters[i][0] != -1)
				{
					sum_x += factor_x[clusters[i][j]];
					sum_y += res_y[clusters[i][j]];
					count++;
				}
			}
			if (sum_x != 0 && sum_y != 0 && count != 0)
			{
				Kx[i] = sum_x / count;
				Ky[i] = sum_y / count;
			}
		}

		int exitCount = 0;

		for (int i = 0; i < K; i++)
		{
			if (Kxold[i] == Kx[i] && Kyold[i] == Ky[i])
			{
				exitCount++;
			}
		}
		if (exitCount == K)
		{
			exit = true;
		}
	}

	ofstream out_2;
	out_2.open("last_data_kmeans.xls");
	if (out_2.is_open())
	{
		for (int i = 0; i < clusters.size(); i++)
		{
			for (int j = 0; j < clusters[i].size(); j++)
			{
				out_2 << region[clusters[i][j]] << '\t';
			}
			out_2 << endl;
		}
	}
	out_2.close();

}

int main()
{
	setlocale(LC_ALL, "Russian");
	//system("chcp 1251");

	MedianLinks();
	Kmeans();

	return 0;
}
