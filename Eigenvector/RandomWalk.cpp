#include <random>
#include <iostream>

double mu(int N, int j){
	int k = j + 1;
	return ((double)(N-k)/(double)N)*(1/(double)N);
}

double nu(int N, int j){
	int k = j;
	return ((1/(double)N)*((double)k/(double)N));
}

double S(int N, int j){
	int k = j;
	return ((double)k/(double)N)*((double)(N-k)/(double)N);
}

double P_plus(int N, int j){
	int k = j + 1;
	return ((double)(N-k)/(double)N)*(1/(double)N);
}

double P_minus(int N, int j){
	int k = j + 1;
	double val = j == 1 ? (((double)(N-1)/(double)N)*(1/(double)N)) : ((double)(k-1)/(double)N)*((double)(N-k)/(double)N);
	return val;
}

void RW(int N, int n_run, int t_check[], int n_check[]){
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dist(0.0, 1.0);

	int height = 2;
	int width = N;

	for (int n = 0; n < n_run; n++){
		//int ladder[height][width] = {0,};
		//ladder[0][width-2] = 1; // initial point is (N-1), in Fig. 5 (a).
		std::vector<int> ladder = {0, width-2}; // initial point is (N-1), in Fig. 5 (a).
		std::vector<int> ladder_f = {0, width-2};
		int t = 0;
		while (true) {
			ladder[0] = ladder_f[0];
			ladder[1] = ladder_f[1];
			t += 1;
			int i = ladder[0]; int j = ladder[1];
			if (i==0){
				if (dist(gen) < P_plus(N, j)) {ladder_f[0] = i;	ladder_f[1] = j+1;}
				else if (dist(gen) >= P_plus(N,j) && dist(gen) < (P_plus(N,j)+mu(N,j))) {
					if (j > 0) {ladder_f[0] = i+1; ladder_f[1] = j;}
					else if (j == 0) {ladder_f[0] = i; ladder_f[1] = j;}
				}
				else if (dist(gen) >= (P_plus(N,j)+mu(N,j)) && dist(gen) < (P_plus(N,j)+mu(N,j)+P_minus(N,j)) && i > 0){
					if (j > 0) {ladder_f[0] = i; ladder_f[1] = j-1;}
					else if (j == 0) {ladder_f[0] = i+1; ladder_f[1] = j;};
				}
				else {ladder_f[0] = i; ladder_f[1] = j;}
			}
			else if(i==1){
				if (dist(gen) < S(N,j)) {
					if (j > 0) {ladder_f[0] = i; ladder_f[1] = j-1;}
					else if (j == 0) {ladder_f[0] = i; ladder_f[1] = j;}
				}
				else if (dist(gen) >= S(N,j) && dist(gen) < (S(N,j) + nu(N,j))) {
					if (j > 0) {ladder_f[0] = i-1; ladder_f[1] = j;}
					else if (j == 0) {ladder_f[0] = i; ladder_f[1] = j;}
				}
				else {ladder_f[0] = i; ladder_f[1] = j;}
			}
			if (ladder_f[0] == 0 && ladder_f[1] == N-1) {
				n_check[n] = 2;
				t_check[n] = t;			
				break;
			}
			else if (ladder_f[0] == 1 && ladder_f[1] == 0) {
				n_check[n] = 1;
				t_check[n] = t;			
				break;
			}
			//std::cout << check << " " << check_t << "\n";
		}
	}
}

double avg_t(int n_run, int t_check[], int n_check[]){
	double avg = 0;
	double count = 0;
	for (int n = 0; n < n_run; n++){
		if (n_check[n] == 1){
			count += 1;
			avg += (double)t_check[n];
		}
	}
	avg /= count;

	return avg;
}

int main(int argc, char* argv[]) {
	if(argc<3){
   		printf("./RandomWalk N n_run \n");
   		exit(1);
	}
	int N = atoi(argv[1]);
	int n_run = atoi(argv[2]);
	int t_check[n_run] = {0,};
	int n_check[n_run] = {0,};
	RW(N, n_run, t_check, n_check);
	
	double val = avg_t(n_run, t_check, n_check);
	std::cout << N << " " << val << "\n";

	return 0;
}
