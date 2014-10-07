% --------- Problem 2: Coordinate Descent ----------------------------

% +++++++++ PART B) CODE +++++++++++++++++++++++++++++++++++++++++++++
% Use Coordinate Descent to solve Lasso regression:
function w = cd_lasso(y, X, lambda)
    [~,k] = size(X);
    w = zeros(k,1);
    for iteration = 1:20
        for j = 1:k
            % update w_j: find delta*, then w_j = w_j + delta*
            % (See attached notes for derivation of delta*)
            delta = -(X(:,j)' * (X*w - y) + lambda)/(dot(X(:,j), X(:,j)));
            if delta > -1*w(j)
                w(j) = w(j) + delta;
            else
                delta = -(X(:,j)' * (X*w - y) - lambda)/(dot(X(:,j), X(:,j)));
                if delta < -1*w(j)
                    w(j) = w(j) + delta;
                % else delta = 0, so w(j) doesn't have to be updated
                end
            end
        end
    end
    
end

