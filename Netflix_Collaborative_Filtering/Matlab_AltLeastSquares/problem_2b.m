% --------- Problem 2: Coordinate Descent ----------------------------

% +++++++++ PART B) CODE +++++++++++++++++++++++++++++++++++++++++++++
% Report the training and the test RMSE for lambda = {10, 1, 0.1} on
% bodyfat dataset, for lasso:
clear;
bodyfat = load('bodyfat.mat');
y = bodyfat.y;
X = bodyfat.X;
y_test = bodyfat.yt;
X_test = bodyfat.Xt;
lambdas = [10 1 0.1];
for lambda = lambdas
    lambda
    w = cd_lasso(y, X, lambda);
    rmse_train = get_rmse_reg(y,X,w)
    rmse_test = get_rmse_reg(y_test,X_test,w)
end

% +++++++++ PART B) RESULTS ++++++++++++++++++++++++++++++++++++++++++
% For lambda = 10:
% RMSE on training data = 0.1896
% RMSE on testing data = 0.1998

% For lambda = 1:
% RMSE on training data = 0.1309
% RMSE on testing data = 0.0701

% For lambda = 0.1:
% RMSE on training data = 0.1430
% RMSE on testing data = 0.0927
