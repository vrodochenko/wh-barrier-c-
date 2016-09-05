#pragma once
#define uint unsigned int
#include <cmath>
#include "pnl/pnl_random.h"
#include <time.h>

double payoff(double S, double K)
{
	return MAX(0, K - S);
}
double generate_heston_trajectory_return(double T, double S0, double H, double K, double r_premia,
	double V0, double kappa, double theta, double sigma, double rho, uint N)
{
	/*simulates Heston monte-carlo for Down-and-out put directly through equations*/
	/* N means iterations */
	double r = log(r_premia / 100.0 + 1.0);
	double dt = float(T) / float(N);
	double sqrt_dt = sqrt(dt);
	// trajectory started
	// initials
	double S_t = S0;
	double V_t = V0;
	double t = 0.0;
	double random_value_for_V;
	double random_value_for_S;
	double dZ_V, dZ_S, dV_t, dS_t;
	PnlRng *randvar1 = pnl_rng_create(PNL_RNG_MERSENNE);
	PnlRng *randvar2 = pnl_rng_create(PNL_RNG_MERSENNE);

	while(t <= T)
	{
		time_t rawtime;
		// random walk for V
		pnl_rng_sseed(randvar1, time(&rawtime));
		random_value_for_V = pnl_rng_normal(randvar1);
		dZ_V = random_value_for_V * sqrt_dt;
		// random walk for S + correlation
		time_t now;
		pnl_rng_sseed(randvar2, time(&now));
		random_value_for_S = pnl_rng_normal(randvar2);
		random_value_for_S = rho * random_value_for_V + sqrt(1 - pow(rho, 2.0)) * random_value_for_S;
		dZ_S = random_value_for_S * sqrt_dt;

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
	pnl_rng_free(&randvar1);
	pnl_rng_free(&randvar2);
	return payoff(S_t, K);
}
void testrnd()
{
	int i, M;
	PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
	PnlVect *v = pnl_vect_new();
	M = 10000;

	/* rng must be initialized. When sseed=0, a default
	value depending on the generator is used */
	pnl_rng_sseed(rng, 0);

	for (i = 0; i<M; i++)
	{
		/* Simulates a normal random vector in R^{10} */
		pnl_vect_rng_normal(v, 10, rng);
		/* Do something with v */
	}

	pnl_vect_free(&v);
	pnl_rng_free(&rng); /* Frees the generator */
	exit(0);
}