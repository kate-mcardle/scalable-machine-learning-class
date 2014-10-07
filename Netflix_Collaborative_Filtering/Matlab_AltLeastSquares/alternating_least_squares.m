% --------- Problem 1: Million Dollar Question -----------------------

% +++++++++ PART A) CODE +++++++++++++++++++++++++++++++++++++++++++++
% Implement the Alternating Least Squares Algorithm:
function [U,M] = alternating_least_squares(k, lambda, R, n_iterations)
    [u, m] = size(R);
    U = rand(k,u);
    M = rand(k,m);
    I = eye(k);
    
    % for movies with no ratings in R, initialize those columns in M to 0s,
    % otherwise store the index of the movie:
    movies_with_ratings = [];
    for i = 1:m
        if nnz(R(:,i)) == 0
            M(:,i) = zeros(k,1);
        else
            movies_with_ratings = cat(2, movies_with_ratings, i);
        end
    end
    % for each movie with ratings in R, store the corresponding indices of
    % users who have given that movie a rating and the ratings:
    [~, n_movies_with_ratings] = size(movies_with_ratings);
    movie_rows = cell(1, n_movies_with_ratings);
    movies_r_K = cell(1, n_movies_with_ratings);
    for j = movies_with_ratings
        [movie_rows{1,j}, ~, movies_r_K{1,j}] = find(R(:,j));
    end
    
    % for users with no ratings in R, initialize those columns in U to 0s,
    % otherwise store the index of the user:
    R_T = R';
    users_with_ratings = [];
    for i = 1:u
        if nnz(R_T(:,i)) == 0
            U(:,i) = zeros(k,1);
        else
            users_with_ratings = cat(2, users_with_ratings, i);
        end
    end
    % for each user with ratings in R, store the corresponding indices of
    % movies that user has rated and the ratings:
    [~, n_users_with_ratings] = size(users_with_ratings);
    user_rows = cell(1, n_users_with_ratings);
    users_r_T_K = cell(1, n_users_with_ratings);
    for i = users_with_ratings
        [user_rows{1,i}, ~, users_r_T_K{1,i}] = find(R_T(:,i));
    end
    
    % iterate through alternating least squares:
    for iteration = 1:n_iterations
        %iteration
        % Fix U constant and solve for M:
        for j = movies_with_ratings % only consider movies with ratings
            rows = movie_rows{1,j}; % rows in R with ratings for movie j
            r_j_K = movies_r_K{1,j};
            U_K_j = U(:,rows);
            M(:,j) = (U_K_j * U_K_j' + lambda * I)\(U_K_j * r_j_K);
        end
        % Fix M constant and solve for U:
        for j = users_with_ratings % only consider users with ratings
            rows = user_rows{1,j}; % rows in R_T with ratings for user j
            r_T_j_K = users_r_T_K{1,j};
            M_K_j = M(:,rows);
            U(:,j) = (M_K_j * M_K_j' + lambda * I)\(M_K_j * r_T_j_K);
        end
    end
end