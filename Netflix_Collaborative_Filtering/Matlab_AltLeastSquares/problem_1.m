% --------- Problem 1: Million Dollar Question -----------------------

% +++++++++ PART B) CODE +++++++++++++++++++++++++++++++++++++++++++++
% Select lambda = 0 and compute U and M on the small dataset. 
% What problems do you face?
clear;
small = load('small.mat');
R_small = small.R;
lambda = 0;
%[U, M] = alternating_least_squares(10, lambda, R_small, 10);
% +++++++++ PART B) RESULTS ++++++++++++++++++++++++++++++++++++++++++
% The matrices (U_K_j * U_K_j' + lambda * I) and
% (M_K_j * M_K_j' + lambda * I) are singular; they must be invertible
% for the exact closed-form solution to work.

% +++++++++ PART C) CODE +++++++++++++++++++++++++++++++++++++++++++++
% Using 10-fold cross validation on the small training data, find the
% optimal lambda. For the optimal lambda, find out the RMSE on the test
% data.

n_folds = 10;
lambdas = [10^-2 10^-1 1];
ks = [10 20 50];
[lambda_opt_small, k_opt_small] = cross_validate(n_folds, R_small, lambdas, ks)
disp('time for ALS on small dataset:');
tic; [U_small, M_small] = alternating_least_squares(k_opt_small, lambda_opt_small, R_small, 10); toc
% Elapsed time is 1.852298 seconds.
R_test_small = small.Rt;
disp('time to get RMSE on small dataset:');
tic; rmse_small = get_rmse(R_test_small, U_small, M_small); toc
% Elapsed time is 0.067812 seconds.
rmse_small
% +++++++++ PART C) RESULTS ++++++++++++++++++++++++++++++++++++++++++ 
% lambda_opt_small = 1, k_opt_small = 10
% rmse_small = 1.0990

% +++++++++ PART D) CODE +++++++++++++++++++++++++++++++++++++++++++++
% Repeat part (c) with medium and large datasets:
% MEDIUM:
med = load('medium.mat');
R_med = med.R;
R_test_med = med.Rt;
[lambda_opt_med, k_opt_med] = cross_validate(n_folds, R_med, lambdas, ks)
disp('time for ALS on medium dataset:');
tic; [U_med, M_med] = alternating_least_squares(k_opt_med, lambda_opt_med, R_med, 10); toc
% Elapsed time is 3.463154 seconds.
disp('time to get RMSE on medium dataset:');
tic; rmse_med = get_rmse(R_test_med, U_med, M_med); toc
% Elapsed time is 0.167996 seconds.
rmse_med
% LARGE:
large = load('large.mat');
R_large = large.R;
R_test_large = large.Rt;
[lambda_opt_large, k_opt_large] = cross_validate(n_folds, R_large, lambdas, ks)
disp('time for ALS on large dataset:');
tic; [U_large, M_large] = alternating_least_squares(k_opt_large, lambda_opt_large, R_large, 10); toc
% Elapsed time is 31.698557 seconds.
disp('time to get RMSE on large dataset:');
tic; rmse_large = get_rmse(R_test_large, U_large, M_large); toc
% Elapsed time is 1.145394 seconds.
rmse_large

% +++++++++ PART D) RESULTS+++++++++++++++++++++++++++++++++++++++++++
% lambda_opt_med = 1, k_opt_med = 10
% rmse_med = 0.8701
% lambda_opt_large = 1, k_opt_large = 10
% rmse_large = 1.5969




