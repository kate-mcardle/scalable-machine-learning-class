% --------- Problem 2: Coordinate Descent ----------------------------

% +++++++++ PART A) CODE +++++++++++++++++++++++++++++++++++++++++++++
% Use Coordinate Descent to solve Ridge regression:
function w = cd_ridge(y, X, lambda)
    [~,k] = size(X);
    w = zeros(k,1);
    for iteration_cd = 1:20
        for j = 1:k
            % update w_j: find delta*, then w_j = w_j + delta*
            % (Equation for delta comes from class lecture slides)
            X_j = X(:,j);
            delta = -(X_j' * (X*w - y) + lambda*w(j))/(lambda + dot(X_j, X_j));
            w(j) = w(j) + delta;
        end
    end
end

