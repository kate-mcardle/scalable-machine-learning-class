% Incremental k-means:
% X = spconvert(load('Prob1_TFIDF/output_to_submit/classic3893_output'));
[Y, X] = svmlread('classic3893_output_with_labels');

X = X';
[n_words, n_docs] = size(X);
% normalize X so that each col's norm = 1
for i=1:n_docs
    X(:,i) = X(:,i)/norm(X(:,i));
end

fprintf('Incremental:\n');
tic;
pi_incremental = incremental_k_means(X, .01, 3);
toc

fprintf('Batch:\n');
tic;
pi_batch = batch_k_means(X, .01, 3);
toc

k = 3;
% Compute the confusion matrices:
conf_incremental = zeros(k, k);
for i=1:n_docs
    actual = Y(i)+1;
    pred = pi_incremental(i);
    conf_incremental(actual, pred) = conf_incremental(actual, pred) + 1;
end
conf_incremental

conf_batch = zeros(k, k);
for i=1:n_docs
    actual = Y(i)+1;
    pred = pi_batch(i);
    conf_batch(actual, pred) = conf_batch(actual, pred) + 1;
end
conf_batch

