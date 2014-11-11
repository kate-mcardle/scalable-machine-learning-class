function [ pi ] = batch_k_means( X, tol, k )
% X: rows are word indices, cols are doc indices
% Note: X must be normalized such that norm of each col in X = 1
% (this is the transpose of input doc format!)
    % Initialize:
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
    Q = sum(q);
    
    % Iterate:
    delta = tol + 5;
    n_iter = 1;
    while (delta > tol)
        fprintf('iteration %i, delta = %d\n', n_iter, delta);
        n_iter = n_iter + 1;
        new_s = s;
%         closest_centroids = max(Sim, [], 2);
        for i=1:n_docs
            pi(i) = find(ismember(Sim(i,:), max(Sim(i,:))));
%             if new_cluster ~= pi(i)
% %                 new_s(:,pi(i)) = new_s(:, pi(i)) - X(:, i);
% %                 new_s(:, new_cluster) = new_s(:, new_cluster) + X(:, i);
%                 pi(i) = new_cluster;
%             end            
        end
        % recalculate s:
        s = zeros(n_words, k);
        for i=1:n_docs
            s(:, pi(i)) = s(:, pi(i)) + X(:, i);
        end
%         s = new_s;
        for z=1:k
            q(z) = norm(s(:, z));
            Sim(:, z) = s(:, z)'*X / q(z);
        end
        new_Q = sum(q);
        delta = new_Q - Q;
        Q = new_Q;
    end
    fprintf('final number of iterations: %i; final delta: %d\n', n_iter, delta);
end

