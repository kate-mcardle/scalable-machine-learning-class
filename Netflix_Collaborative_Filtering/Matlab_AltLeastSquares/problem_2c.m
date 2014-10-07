% --------- Problem 2: Coordinate Descent -----------------------

% +++++++++ PART C) CODE +++++++++++++++++++++++++++++++++++++++++++++
% Using the optimal lambda from Problem 1, solve the Netflix challenge for
% each dataset, using cd_ridge for the ridge-regression sub-problem.
% Report the running time and test RMSE values in each case.
clear;
lambda = 1;
k = 10;

% SMALL:
small = load('small.mat');
R_small = small.R;
R_test_small = small.Rt;
disp('time for ALS using cd_ridge on small dataset:');
tic; [U_small, M_small] = alternating_least_squares_CD(k, lambda, R_small, 10); toc
% Elapsed time is 375.156130 seconds.
disp('time to get RMSE on small dataset:');
tic; rmse_small = get_rmse(R_test_small, U_small, M_small); toc
% Elapsed time is 0.056770 seconds.
rmse_small

% MEDIUM:
med = load('medium.mat');
R_med = med.R;
R_test_med = med.Rt;
disp('time for ALS using cd_ridge on medium dataset:');
tic; [U_med, M_med] = alternating_least_squares_CD(k, lambda, R_med, 10); toc
% Elapsed time is 599.813926 seconds.
disp('time to get RMSE on medium dataset:');
tic; rmse_med = get_rmse(R_test_med, U_med, M_med); toc
% Elapsed time is 0.179620 seconds.
rmse_med

% LARGE:
large = load('large.mat');
R_large = large.R;
R_test_large = large.Rt;
disp('time for ALS using cd_ridge on large dataset:');
tic; [U_large, M_large] = alternating_least_squares_CD(k, lambda, R_large, 10); toc
% Elapsed time is 4948.484378 seconds.
disp('time to get RMSE on large dataset:');
tic; rmse_large = get_rmse(R_test_large, U_large, M_large); toc
% Elapsed time is 1.208056 seconds.
rmse_large


% +++++++++ PART C) RESULTS ++++++++++++++++++++++++++++++++++++++++++ 
% total runtime for small: 375.16 seconds to train; 0.06 seconds for RMSE
% rmse_small = 1.1116

% total runtime for medium: 599.81 seconds to train; 0.18 seconds for RMSE
% rmse_medium = 0.8760

% total runtime for large: 4948.48 seconds to train; 1.21 seconds for RMSE
% rmse_large = 1.6096





