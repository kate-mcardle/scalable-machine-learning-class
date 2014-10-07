function [best_lambda, best_k] = cross_validate(n_folds, R, lambdas, ks)
    % Divide R into 10 folds:
    [u, m] = size(R);
    R_folds = cell(1, n_folds);
    [rows, cols, ratings] = find(R);
    [n_ratings, ~] = size(ratings);
    r = mod(n_ratings, n_folds);
    n_ratings_per_fold = int64(floor(n_ratings/n_folds)) + 1;
    count = 1;
    random_indices = randperm(n_ratings);
    for i = 1:n_folds
        if i > r
           n_ratings_per_fold = int64(floor(n_ratings/n_folds)); 
        end
        indices = random_indices(count:count+n_ratings_per_fold-1);
        R_folds{1,i} = sparse(rows(indices), cols(indices), ratings(indices), u, m);
        count = count + n_ratings_per_fold;
    end
    
    % Iterate through combinations of lambda, k:
    best_lambda = lambdas(1);
    best_k = ks(1);
    best_rmse = realmax;
    for k = ks
        for lambda = lambdas
            rmse = 0;
            for i = 1:n_folds
                R_train = R - R_folds{1,i};
                [U, M] = alternating_least_squares(k, lambda, R_train, 10);
                rmse = rmse + get_rmse(R_folds{1,i}, U, M);
            end
            rmse = rmse / n_folds;
            if rmse < best_rmse
                best_rmse = rmse;
                best_lambda = lambda;
                best_k = k;
            end
        end
    end
                
end

