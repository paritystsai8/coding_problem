/*

09/15/2014

You have two arrays of integers, V={V1,V2,…,VN} and P={P1,P2,…,PN}, where both have N number of elements. Consider the following function:

score = 0

int Go(step, energy) {
if (step == N) {
score += V[step];
return (score);
}
else {
int way = random(1, 2);
if (way == 1) {
score += V[step];
}
else {
energy = P[step];
}
if (energy > 0) {
Go(step + 1, energy - 1);
}
else {
KillTheWorld();
}
}
}
What is the maximum possible value of score that we can get in the end, if we call Go(1,0)?. 
Note that the function should never invoke KillTheWorld function. And random(1,2) generates a random integer from set [1, 2]. It is guaranteed there will be a solution that wont kill the world.

Input format

The first line contains an integer N. Each of the following N lines contains a pair of integers. The i-th line contains a pair of numbers, Vi, Pi, separated by space.

Output Format 
Derive the maximum score given by return (score);.

Constraints 
1≤N≤5×105 
0≤Vi≤109 
0≤Pi≤105

Sample input

4
4 2
0 2
4 0
3 4

Sample output

7

*/

/*
 *
 * Solution algorithm
 *
 * Use DP on max_possible_value = Table(step, energy). Given a partivular step, the table is a monotonic non-decreasing function of energy with O(N - step) steps. Start from step = N. 
 *
 * Moreover use two cooridinate transforms: 
 * 
 * true_value = sum(accumulated V_i) - table_value
 * true_energy = N - 1 - step + energy
 *
 * In this way update per step takes O(log(N))
 * 
 *
 */

#include<sys/types.h>
#include<dirent.h>

#include<algorithm>
#include<iostream>
#include<cstring>
#include<string>
#include<vector>
#include<cstdio>
#include<cstdlib>
#include<cassert>
#include<sstream>
#include<cmath>
#include<fstream>
#include<map>
#include<set>
#include<queue>
#include<tr1/unordered_map>

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define ABS(x) ((x) > 0 ? (x) : -(x))
#define lt(x, y)	((x) >= 0 && ((x) < (y) || (y) < 0))

#define SWAP(x, y) {(x) += (y); (y) = (x) - (y); (x) -= (y);}

#define EPS 1e-6
#define PI 3.14159265358979323846

using namespace std;

long long inf = 1000000000000000;

//sum-v is the true value
long long v[600000];

//N-1-x+end_pos is the true position
long long end_pos[600000];

long long nstack;

long long N;

long long V[600000];
long long P[600000];

long long to_true_value(long long sum, long long v){
	return v == inf ? -1 : sum - v;
}
long long to_value(long long sum, long long true_value){
	return true_value < 0 ? inf : sum - true_value;
}

long long to_pos(long long x, long long true_pos){
	return true_pos - (N - 1 - x);
}

void update_stack(long long value, long long pos){

	if(nstack && v[nstack - 1] - value)
		v[nstack++] = value;

	end_pos[nstack - 1] = pos;
}

//find the smallest idx s.t. end_pos[idx] <= x
long long find_idx(long long x){

	long long R = nstack - 1;
	long long L = 0;

	while(R - L){

		long long m = (R + L) >> 1;

		if(end_pos[m] <= x)
			R = m;
		else
			L = m + 1;
	}

	return L;
}

int main()
{
	scanf("%lld", &N);

	for(long long n = 0; n < N; n++)
		scanf("%lld %lld", &V[n], &P[n]);


	/*
	 * initialize x = N - 1
	 */

	v[0] = 0;
	end_pos[0] = 0;
	nstack = 1;
	long long sum = V[N - 1];

	for(long long x = N - 2; x >= 0; x--){

		long long pre_sum = sum;
		sum += V[x];

		//find the smallest idx s.t. end_pos[idx] <= to_pos(x + 1, P[x] - 1)
	
		long long idx = find_idx(to_pos(x + 1, P[x] - 1));

		assert(idx < nstack && idx >= 0);

		// if pick P[x]
		long long ref = P[x] ? to_true_value(pre_sum, v[idx]) : -1;

		while(nstack && to_true_value(sum, v[nstack - 1]) < ref){
			nstack--;
		}

		update_stack(to_value(sum, ref), to_pos(x, 0));

		assert(nstack > 0);
		assert(end_pos[nstack - 1] == -(N - 1 - x));
	}
	
	//D[0][0]
	long long ans = to_true_value(sum, v[nstack - 1]);

	assert(ans >= 0);
	assert(N == 1 ? ans == sum : ans <= sum - V[0]);

	printf("%lld\n", ans);

	return 0;
}

// vi: ts=2 sw=2
