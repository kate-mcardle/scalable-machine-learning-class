function rmse = get_rmse_reg(y, X, w)
    y_hat = X*w;
    rmse = sqrt(mean((y-y_hat).^2));
end

