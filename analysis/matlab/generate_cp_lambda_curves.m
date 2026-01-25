function generate_cp_lambda_curves(varargin)
% GENERATE_CP_LAMBDA_CURVES - Create Figure 2.1 for Chapter 02
%
% Syntax:
%   generate_cp_lambda_curves('OutputPath', path, 'DPI', 600)
%
% Inputs:
%   OutputPath - Directory to save figures (default: 'docs/figures/generated/ch02')
%   DPI - Resolution in dots per inch (default: 600)
%   Format - Cell array of formats {'png', 'svg'} (default: {'png'})
%   ColorScheme - 'clinical' or 'standard' (default: 'clinical')

%% Parse inputs
p = inputParser;
addParameter(p, 'OutputPath', 'docs/figures/generated/ch02', @ischar);
addParameter(p, 'DPI', 600, @isnumeric);
addParameter(p, 'Format', {'png'}, @iscell);
addParameter(p, 'ColorScheme', 'clinical', @ischar);
parse(p, varargin{:});

output_path = p.Results.OutputPath;
dpi = p.Results.DPI;
formats = p.Results.Format;
color_scheme = p.Results.ColorScheme;

% Create output directory
if ~exist(output_path, 'dir')
    mkdir(output_path);
end

%% Define clinical color palette
colors = struct( ...
    'blue', [30, 136, 229]/255, ...      % #1E88E5
    'green', [67, 160, 71]/255, ...      % #43A047
    'red', [229, 57, 53]/255, ...        % #E53935
    'orange', [251, 140, 0]/255, ...     % #FB8C00
    'purple', [123, 31, 162]/255, ...    % #7B1FA2
    'gray_light', [224, 224, 224]/255, ...  % #E0E0E0
    'gray_med', [117, 117, 117]/255, ... % #757575
    'text', [33, 33, 33]/255 ...         % #212121
);

%% Generate data
lambda = linspace(0, 10, 200);

% HAWT (modern utility-scale)
cp_hawt = zeros(size(lambda));
for i = 1:length(lambda)
    if lambda(i) >= 3 && lambda(i) <= 10
        % Gaussian-like peak at λ=7, Cp=0.48
        cp_hawt(i) = 0.48 * exp(-((lambda(i) - 7)^2) / (2 * 1.2^2));
    end
end

% Helical/Darrieus VAWT (case study turbine: λ_opt=2.0, Cp_max=0.35)
cp_vawt = zeros(size(lambda));
for i = 1:length(lambda)
    if lambda(i) >= 0.5 && lambda(i) <= 4.0
        % Broader plateau, peak at λ=2.0
        cp_vawt(i) = 0.35 * exp(-((lambda(i) - 2.0)^2) / (2 * 0.8^2));
    end
end

% Savonius VAWT (low efficiency)
cp_savonius = zeros(size(lambda));
for i = 1:length(lambda)
    if lambda(i) >= 0.3 && lambda(i) <= 2.5
        cp_savonius(i) = 0.20 * exp(-((lambda(i) - 1.0)^2) / (2 * 0.5^2));
    end
end

%% Create figure (A4 landscape at 600 DPI)
fig = figure('Units', 'inches', 'Position', [0, 0, 11.69, 8.27]); % A4 landscape
set(fig, 'Color', 'w', 'Renderer', 'painters');

hold on; grid on; box on;

% Plot curves
plot(lambda, cp_hawt, '-', 'Color', colors.blue, 'LineWidth', 3, 'DisplayName', 'HAWT (C_{p,max} ≈ 0.45–0.50)');
plot(lambda, cp_vawt, '-', 'Color', colors.green, 'LineWidth', 3, 'DisplayName', 'Helical VAWT (C_{p,max} ≈ 0.30–0.38)');
plot(lambda, cp_savonius, '-', 'Color', colors.orange, 'LineWidth', 3, 'DisplayName', 'Savonius VAWT (C_{p,max} ≈ 0.15–0.25)');

% Betz limit
yline(0.593, '--', 'Color', colors.gray_med, 'LineWidth', 2, 'Label', 'Betz Limit', 'LabelHorizontalAlignment', 'left');

% Operating regions (shaded vertical bands)
region_alpha = 0.15;
patch([0, 1.0, 1.0, 0], [0, 0, 0.6, 0.6], colors.red, 'FaceAlpha', region_alpha, 'EdgeColor', 'none', 'HandleVisibility', 'off');
patch([1.5, 2.5, 2.5, 1.5], [0, 0, 0.6, 0.6], colors.green, 'FaceAlpha', region_alpha, 'EdgeColor', 'none', 'HandleVisibility', 'off');
patch([2.5, 3.5, 3.5, 2.5], [0, 0, 0.6, 0.6], colors.blue, 'FaceAlpha', region_alpha, 'EdgeColor', 'none', 'HandleVisibility', 'off');
patch([3.5, 10, 10, 3.5], [0, 0, 0.6, 0.6], colors.orange, 'FaceAlpha', region_alpha, 'EdgeColor', 'none', 'HandleVisibility', 'off');

% Labels and formatting
xlabel('Tip Speed Ratio λ = ωR/v_w', 'FontSize', 12, 'FontWeight', 'bold', 'Color', colors.text);
ylabel('Power Coefficient C_p = P_{turbine} / (0.5ρAv³)', 'FontSize', 12, 'FontWeight', 'bold', 'Color', colors.text);
title('Power Coefficient vs. Tip Speed Ratio for Common Turbine Architectures', 'FontSize', 14, 'FontWeight', 'bold');

xlim([0, 10]);
ylim([0, 0.65]);
set(gca, 'FontSize', 11, 'GridColor', colors.gray_light, 'GridAlpha', 0.5);

legend('Location', 'northeast', 'FontSize', 10);

%% Add region annotations
text(0.5, 0.58, 'Region I: Start-Up', 'Rotation', 90, 'FontSize', 9, 'FontStyle', 'italic', 'Color', colors.gray_med, 'HorizontalAlignment', 'center');
text(2.0, 0.58, 'Region II: MPPT', 'Rotation', 90, 'FontSize', 9, 'FontStyle', 'italic', 'Color', colors.gray_med, 'HorizontalAlignment', 'center');
text(3.0, 0.58, 'Region III: Regulation', 'Rotation', 90, 'FontSize', 9, 'FontStyle', 'italic', 'Color', colors.gray_med, 'HorizontalAlignment', 'center');
text(5.0, 0.58, 'Region IV: Braking', 'Rotation', 90, 'FontSize', 9, 'FontStyle', 'italic', 'Color', colors.gray_med, 'HorizontalAlignment', 'center');

%% Save in requested formats
base_filename = 'ch02_fig01_cp_lambda_comparison';

for i = 1:length(formats)
    fmt = formats{i};
    filepath = fullfile(output_path, [base_filename, '.', fmt]);
    
    switch lower(fmt)
        case 'png'
            print(fig, filepath, '-dpng', sprintf('-r%d', dpi));
        case 'svg'
            print(fig, filepath, '-dsvg');
        case 'pdf'
            print(fig, filepath, '-dpdf');
        otherwise
            warning('Unsupported format: %s', fmt);
    end
    
    fprintf('✓ Saved: %s\n', filepath);
end

close(fig);

fprintf('Figure 2.1 generation complete!\n');

end
