% --------- Problem 2: Coordinate Descent ----------------------------

% +++++++++ PART A) CODE +++++++++++++++++++++++++++++++++++++++++++++
% Report the training and the test RMSE for lambda = {10, 1, 0.1} on
% bodyfat dataset, for ridge:
clear;
bodyfat = load('bodyfat.mat');
y = bodyfat.y;
X = bodyfat.X;
y_test = bodyfat.yt;
X_test = bodyfat.Xt;
lambdas = [10 1 0.1];
for lambda = lambdas
    lambda
    w = cd_ridge(y, X, lambda);
    rmse_train = get_rmse_reg(y,X,w)
    rmse_test = get_rmse_reg(y_test,X_test,w)
end

% +++++++++ PART A) RESULTS ++++++++++++++++++++++++++++++++++++++++++
% For lambda = 10:
% RMSE on training data = 0.1764
% RMSE on testing data = 0.1557

% For lambda = 1:
% RMSE on training data = 0.1324
% RMSE on testing data = 0.0775

% For lambda = 0.1:
% RMSE on training data = 0.1429
% RMSE on testing data = 0.0928
