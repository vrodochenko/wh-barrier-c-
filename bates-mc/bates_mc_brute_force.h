#pragma once
#include <cmath>
#include <time.h>
#include <pnl/pnl_random.h>
using namespace std;

double payoff(double S, double K)
{
	if(K-S>0){
		return K-S;
	}
	else{
		return 0;
	}

}

double generate_bates_trajectory_return(double tt, double spot, double H, double K, double r_premia,
		double v0, double kappa, double theta, double sigma, double rho, int N){

	PnlRng* mersenne_generator = pnl_rng_create(PNL_RNG_MERSENNE); // declaring a generator for 1-st process
	clock_t mersenne_generator_seed = time(NULL); // using current time in seconds from EPOCH as a seed
	pnl_rng_sseed(mersenne_generator, mersenne_generator_seed); // initialization with a seed

	double random_variable_price = 0;
	double random_variable_variance = 0;

	//	/*simulates Heston monte-carlo for Down-and-out put directly through equations*/

	double r = log(r_premia / 100.0 + 1.0);
	double dt = double(tt) / double(N);
	double sqrt_dt = sqrt(dt);
	// trajectory started
	// initials
	double S_t = spot;
	double V_t = v0;
	double t = 0.0;
	double T = tt;

	double dZ_V, dZ_S, dV_t, dS_t;

	double lambda = 0.1;
//	for(int i=0; i<10; i++)
//	{
//		random_variable_price = pnl_rng_normal(mersenne_generator);
//		random_variable_variance = pnl_rng_normal(mersenne_generator);
//
//		printf("rand_num  is:%f", random_variable_price);
//		printf("rand_num2  is:%f", random_variable_variance);
//	}

	while(t <= T){

		random_variable_variance = pnl_rng_normal(mersenne_generator);
		dZ_V = random_variable_price * sqrt_dt;

		random_variable_price = pnl_rng_poisson(lambda, mersenne_generator);
		random_variable_price = rho * random_variable_variance + sqrt(1 - pow(rho, 2.0)) * random_variable_price;

		dZ_S = random_variable_price * sqrt_dt + random_variable_price;


		// equation for V
		dV_t = kappa * (theta - V_t) * dt + sigma * sqrt(V_t) * sqrt_dt * dZ_V;
		V_t += dV_t;
		// equation for S
		dS_t = S_t * r * dt + S_t * sqrt(V_t) * dZ_S;
		S_t += dS_t;
		// trajectory ended
		t += dt;
		// check barrier crossing on each step
		if (S_t <= H)
		{
			return 0.0;
		}
	}
	return payoff(K, S_t);
}
