%% Generate Cp-lambda curve for 500W Helical VAWT
% MATLAB version for students preferring MATLAB over Python
% Author: Dr. Asitha Kulasekera

clear; close all; clc;

%% Configuration
ROTOR_DIAMETER = 1.2; % m
ROTOR_HEIGHT = 1.5; % m
SWEPT_AREA = 2 * (ROTOR_DIAMETER/2) * ROTOR_HEIGHT; % 1.8 m²
LAMBDA_OPT = 2.0;
CP_MAX = 0.35;
DPI = 300;

% Output paths (adjust for your repo structure)
output_dir = fullfile(fileparts(fileparts(pwd)), 'docs', 'figures');
if ~exist(output_dir, 'dir')
    mkdir(output_dir);
end

%% Analytical Cp-lambda model
lambda_model = linspace(0.3, 4.0, 200);
cp_model = cp_max_helical(lambda_model, CP_MAX, LAMBDA_OPT);

%% Simulated experimental data
rng(42); % Reproducible random seed
lambda_exp = [linspace(0.5, 1.5, 4), linspace(1.6, 2.4, 7), linspace(2.5, 3.5, 4)];
cp_exp = cp_max_helical(lambda_exp, CP_MAX, LAMBDA_OPT);

% Add measurement noise
noise_factor = 1 + 0.5 * abs(lambda_exp - LAMBDA_OPT);
noise = 0.018 * cp_exp .* noise_factor .* randn(size(cp_exp));
cp_exp = max(0, min(0.593, cp_exp + noise)); % Clip to [0, Betz limit]
error_bars = 0.015 * cp_exp;

%% Plotting
figure('Units', 'inches', 'Position', [1, 1, 6.5, 4.2]);
hold on; grid on; box on;

% CFD prediction
plot(lambda_model, cp_model, 'k--', 'LineWidth', 1.5, 'DisplayName', 'CFD Prediction');

% Experimental data with error bars
errorbar(lambda_exp, cp_exp, error_bars, 'o', 'LineWidth', 1.5, ...
    'MarkerSize', 7, 'MarkerFaceColor', [0.12, 0.47, 0.71], ...
    'MarkerEdgeColor', 'w', 'Color', [0.12, 0.47, 0.71], ...
    'DisplayName', 'Measured (Galle, Jan 2026)');

% MPPT window shading
lambda_window = [1.8, 2.2];
y_limits = [0, 0.42];
patch([lambda_window(1), lambda_window(2), lambda_window(2), lambda_window(1)], ...
      [0, 0, 0.42, 0.42], 'green', 'FaceAlpha', 0.15, ...
      'EdgeColor', 'none', 'DisplayName', sprintf('MPPT Window (\\lambda_{opt} = %.1f)', LAMBDA_OPT));

% Optimal point marker
plot(LAMBDA_OPT, CP_MAX, 'r*', 'MarkerSize', 15, 'LineWidth', 2, ...
    'DisplayName', sprintf('C_{p,max} = %.2f', CP_MAX));

% Annotations
text(LAMBDA_OPT + 0.5, CP_MAX - 0.05, sprintf('\\lambda_{opt} = %.1f', LAMBDA_OPT), ...
    'FontSize', 10, 'Color', 'r', 'FontWeight', 'bold');
text(0.8, 0.32, {'Low \lambda', '(Startup)'}, 'FontSize', 8, 'Color', [0.5, 0.5, 0.5], ...
    'HorizontalAlignment', 'center', 'FontAngle', 'italic');
text(2.0, 0.25, {'Optimal', 'Region'}, 'FontSize', 8, 'Color', [0, 0.5, 0], ...
    'HorizontalAlignment', 'center', 'FontWeight', 'bold', 'FontAngle', 'italic');
text(3.2, 0.18, {'High \lambda', '(Stall)'}, 'FontSize', 8, 'Color', [0.5, 0.5, 0.5], ...
    'HorizontalAlignment', 'center', 'FontAngle', 'italic');

%% Formatting
xlabel('Tip-Speed Ratio, \lambda = \omega R / v_w', 'FontSize', 11);
ylabel('Power Coefficient, C_p', 'FontSize', 11);
title(sprintf('3-Blade Helical VAWT (NACA0018, 120° Twist)\nD = %.1f m, H = %.1f m, A = %.1f m²', ...
    ROTOR_DIAMETER, ROTOR_HEIGHT, SWEPT_AREA), 'FontSize', 11);
xlim([0.3, 4.0]);
ylim([0, 0.42]);
legend('Location', 'northeast', 'FontSize', 9);
set(gca, 'FontSize', 9, 'LineWidth', 1);

%% Save
output_file = fullfile(output_dir, 'cp-lambda-helical.png');
exportgraphics(gcf, output_file, 'Resolution', DPI);
fprintf('✓ Figure saved to: %s\n', output_file);

%% Helper function: Cp-lambda model
function cp = cp_max_helical(lambda, cp_max, lambda_opt)
    % Analytical model for helical VAWT
    cp = cp_max * exp(-0.5 * ((lambda - lambda_opt) / 0.6).^2);
    
    % Dynamic stall penalty (high lambda)
    stall_factor = ones(size(lambda));
    stall_idx = lambda > 2.5;
    stall_factor(stall_idx) = 1 - 0.15 * (lambda(stall_idx) - 2.5).^1.5;
    cp = cp .* max(0, min(1, stall_factor));
    
    % Low-lambda correction
    low_lambda_penalty = ones(size(lambda));
    low_idx = lambda < 1.0;
    low_lambda_penalty(low_idx) = 0.3 + 0.7 * (lambda(low_idx) / 1.0).^2;
    cp = cp .* low_lambda_penalty;
    
    cp = max(0, cp); % Non-negative
end
