function method1
clear all;
close all;
hold on;

x = [1 2 4 8 16];

t3 = [ 6.188218 3.666473 3.143014 3.566216 4.543530 ];
plot(x, t3, 'LineWidth', 3, 'DisplayName', 'Method 1');
t3 = [ 6.164862 3.697491 3.107172 3.507663 4.476351 ];
plot(x, t3, 'LineWidth', 3, 'DisplayName', 'Method 2');
t3 = [ 5.931814 3.498120 2.295993 1.504625 1.302732 ];
plot(x, t3, 'LineWidth', 3, 'DisplayName', 'Method 3');

title('hpc-class');
xlabel('Number of Threads');
ylabel('Render Time');
legend('show', 'Location', 'NorthWest');

