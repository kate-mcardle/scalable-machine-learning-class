function [ pi ] = incremental_k_means( X, tol, k )
% X: rows are word indices, cols are doc indices
% Note: X must be normalized such that norm of each col in X = 1
% (this is the transpose of input doc format!)
    [n_words, n_docs] = size(X);
    
    % Initialize:
    pi = randi([1 k],n_docs, 1);
    s = zeros(n_words, k);
    for i=1:n_docs
        s(:, pi(i)) = s(:, pi(i)) + X(:, i);
    end
    
    q = zeros(k, 1);
    Sim = zeros(n_docs, k);
    for z=1:k
        q(z) = norm(s(:, z));
        Sim(:, z) = s(:, z)'*X / q(z);
    end
    
    % Iterate:
    max_gain = tol + 5;
    n_iter = 1;
    while (max_gain > tol)
%         fprintf('iteration %i, max gain = %d\n', n_iter, max_gain);
        n_iter = n_iter + 1;
        best_doc = 0;
        best_cluster = 0;
        
        for i=1:n_docs
            c = pi(i);
            for j=1:k
                if j ~= c
                    gain = sqrt(q(j)*q(j) + 2*q(j)*Sim(i,j) + 1) - q(j) + sqrt(q(c)*q(c) - 2*q(c)*Sim(i,c) + 1) - q(c);
                    if (best_doc == 0) || (gain > max_gain)
                        max_gain = gain;
                        best_doc = i;
                        best_cluster = j;
                    end
                end
            end
        end
        
        old_c = pi(best_doc);
        
        % Update s(:, best_cluster) and s(:, old_c):
        s(:, best_cluster) = s(:, best_cluster) + X(:, best_doc);
        s(:, old_c) = s(:, old_c) - X(:, best_doc);
        
        % Update q(best_cluster) and q(old_c):
        q(best_cluster) = norm(s(:, best_cluster));
        q(old_c) = norm(s(:, old_c));
        
        % Update Sim(:, best_cluster) and Sim(:, old_c):
        Sim(:, best_cluster) = s(:, best_cluster)'*X / q(best_cluster);
        Sim(:, old_c) = s(:, old_c)'*X / q(old_c);
        
        % Update cluster assignment:
        pi(best_doc) = best_cluster;
    end
    fprintf('final number of iterations: %i; final gain: %d\n', n_iter, max_gain);
    
end

