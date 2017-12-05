function parallel0
clear all;
close all;
hold on;

% [war dragon blackdragon ironman]
x = [44115 61152 113955 596682];
% 4K
tUHD1 = [5.488366 3.376499 5.508071 6.142129];
tUHD4 = [3.771093 1.889516 3.788177 2.748124];
tUHD8 = [4.907953 2.375056 4.200532 2.989861];
tUHD16 = [5.7382626 2.966125 5.029510 3.206479];

plot(x, tUHD1, 'LineWidth', 3, 'DisplayName', '1 thread');
plot(x, tUHD4, 'LineWidth', 3, 'DisplayName', '4 threads');
plot(x, tUHD8, 'LineWidth', 3, 'DisplayName', '8 threads');
plot(x, tUHD16, 'LineWidth', 3, 'DisplayName', '16 threads');
title('personal');
xlabel('Number of Vertices');
ylabel('Vertex Transformation Time');
legend('show', 'Location', 'NorthWest');

end
