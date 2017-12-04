function serial
clear all;
close all;
hold on;

% [war dragon blackdragon ironman]
x = [44115 61152 113955 596682];
% 1080p
tFHD = [1.441424 0.900596 1.456239 1.775110];
% 4K
tUHD = [5.382954 3.363639 5.476651 5.974851];

plot(x, tFHD, 'LineWidth', 3, 'DisplayName', '1920x1080');
plot(x, tUHD, 'LineWidth', 3, 'DisplayName', '3840x2160');
title('personal');
xlabel('Number of Vertices');
ylabel('Render Time');
legend('show', 'Location', 'SouthEast');

hold on;

% 1080p
tFHD = [];
% 4K
tUHD = [];

plot(x, tFHD, 'LineWidth', 3, 'DisplayName', '1920x1080');
plot(x, tUHD, 'LineWidth', 3, 'DisplayName', '3840x2160');
title('hpc-class');
xlabel('Number of Vertices');
ylabel('Render Time');
legend('show', 'Location', 'SouthEast');

end
