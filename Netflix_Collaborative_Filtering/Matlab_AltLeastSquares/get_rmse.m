function rmse = get_rmse(R_test, U, M)
% This function only calculates the RMSE for nonzero entries in R_test
    [rows_with_ratings, cols_with_ratings, actual_ratings] = find(R_test);
    [n_ratings, ~] = size(actual_ratings);
    pred_ratings = zeros(n_ratings, 1);
    for i = 1:n_ratings
        pred_ratings(i,1) = U(:, rows_with_ratings(i))' * M(:, cols_with_ratings(i));
    end
    rmse = sqrt(mean((actual_ratings - pred_ratings).^2));
end

