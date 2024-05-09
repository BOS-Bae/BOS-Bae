#include <random>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;
using std::cout;
using std::uniform_int_distribution;
using std::uniform_real_distribution;
using std::random_device;
using std::mt19937;
using std::vector;

int cluster_diff(vector<vector<int>> Mat, int N){
	int	cluster_size = 0;
	for (int i=0; i<N; i++){
		if (Mat[0][i] == 1) cluster_size += 1;
	}
	int diff = N-2*cluster_size;

	return diff;
}

bool balance(vector<vector<int>> Mat, int N){
	int	check = 0;
	int bal = 0;

	bool condition_check = false;
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			for (int k=0; k<N; k++){
				check +=1;
				if (Mat[i][j]*Mat[i][k]*Mat[j][k] == 1) bal += 1;
			}
		}
	}
	if (bal == check) condition_check = true;

	return condition_check;
}

void print_mat(vector<vector<int>> Mat, int N){
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			cout << Mat[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

int L_t(double q, double rand_num){
	int maj = 6;
	int small = 4;
	int L_rule = 0;
	L_rule = (rand_num < q) ? small : maj;
	
	return L_rule;
}

int L6(vector<vector<int>> Mat, int o, int d, int r){
	int val = 0;
	val = Mat[o][r]*Mat[d][r];
	
	return val;
}

int L4(vector<vector<int>> Mat, int o, int d, int r){
	int val = 0;
	if (Mat[o][d] == 1 && Mat[d][r] == 1 && Mat[o][r] == -1) val = 1;
	else val = Mat[o][r]*Mat[d][r];
	
	return val;
}

double average_opinion(vector<vector<int>> Mat, int N){
	int sigma = 0;
	double aver_sigma = 0.0;
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++) sigma += Mat[i][j];
	}
	aver_sigma = (double)(sigma)/(double)(N*N);

	return aver_sigma;
}

int main(int argc, char *argv[]) {
	if (argc < 3){
		cout << "./small_frac_L4 N n_sample num_small \n";
		exit(1);
	}

	int N = atoi(argv[1]);
	int n_sample = atoi(argv[2]);
	int num_small = atoi(argv[3]);
	
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(0, N-1);
	uniform_real_distribution<> dist_f(0, 1);

	vector<double> total_val(n_sample, 0.0);
	double avg_val = 0;
	for (int s=0; s<n_sample; s++){
			vector<vector<int>> Mat = {};
			
			// initialization :
			for (int i=0; i<N; i++){
				vector<int>	vec = {};
				for (int j=0; j<N; j++){
					if (dist_f(gen) < 0.5) vec.push_back(1);
					else vec.push_back(-1);
				}
				Mat.push_back(vec);
			}

			int t = 0;
			while (true){
				t += 1;	
				int val_check = 0;
				int d = dist(gen);
				int r = dist(gen);
				vector<int> update_od = {};
				for (int o=0; o<N; o++) {
					if (o < num_small) update_od.push_back(L4(Mat, o, d, r));
					else update_od.push_back(L6(Mat, o, d, r));
				}
				for (int o=0; o<N; o++) Mat[o][d] = update_od[o];
					if (balance(Mat, N)) {
						total_val[s] = average_opinion(Mat,N);
						break;
					}
			}
	}
	for (int s = 0; s<n_sample; s++) avg_val += total_val[s];
	avg_val /= (double)n_sample;

	double val = 0.0;
	double std_err_val = 0.0;
	for (int s=0; s<n_sample; s++) val += pow((total_val[s] - avg_val),2);
	std_err_val = sqrt(val/(double)(n_sample*(n_sample-1)));
	
	cout << N << " " << num_small << " " << avg_val << " " << std_err_val << "\n";
	return 0;
}
