#include "bates_mc_brute_force.h"
#include <math.h>
#include <stdio.h>
#include <time.h>
int main(){

	/*Option parameters*/
	double tt = 1;
	double H = 90.0;
	double K = 100.0;
	double r_premia = 10;
	double spot = 95.0;

	/*Heston model parameters*/
	double v0 = 0.1; /* initial volatility */
	double kappa = 2.0; /*heston parameter, mean reversion*/
	double theta = 0.2; /*heston parameter, long-run variance*/
	double sigma = 0.2; /*heston parameter, volatility of variance*/
	double omega = sigma; /*sigma is used everywhere, omega - in the variance tree*/
	double rho = 0.5; /*heston parameter, correlation*/

	/*Bates model parameters*/
	/*we assume Bates model to be Merton-like so we only pass jump parameters of compound Poisson process*/

	double mc_price = 0;
	int trajectories = 10000;

	mc_price = generate_bates_trajectory_return(tt, spot, H, K, r_premia, v0, kappa, theta, sigma, rho, trajectories);
	printf("avg_price %f", mc_price);

//	for (int trajectory = 0; trajectory < trajectories; trajectory++)
//	{
//		if(trajectory % 1000 == 0)
//			printf("trajectories left:  %d \n", trajectories - trajectory);
//		mc_price += generate_bates_trajectory_return(tt, spot, H, K, r_premia, v0, kappa, theta, sigma, rho, 10000);
//		printf("avg_price %f", mc_price);
//
//	}

//	mc_price = mc_price / double(trajectories);
//	printf("avg_price %f")//, mc_price);
//	getchar();
	getchar();
}
