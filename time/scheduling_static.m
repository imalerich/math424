function scheduling_static
clear all;
close all;
hold on;

x = [1 2 4 8 16];

t0 = [ 5.993472 3.328531 2.053528 1.354964 1.217735 ];
plot(x, t0, 'LineWidth', 3, 'DisplayName', '(static, 1)');
t0 = [ 6.025800 3.281866 2.051981 1.359764 1.215193 ];
plot(x, t0, 'LineWidth', 3, 'DisplayName', '(static, 4)');
t0 = [ 6.019858 3.330946 2.069536 1.369063 1.266668 ];
plot(x, t0, 'LineWidth', 3, 'DisplayName', '(static, 8)');
t0 = [ 5.973761 3.290060 2.147081 1.576057 1.543994 ];
plot(x, t0, 'LineWidth', 3, 'DisplayName', '(static, 32)');
t0 = [ 5.957563 3.396595 2.342645 1.841425 1.891801 ];
plot(x, t0, 'LineWidth', 3, 'DisplayName', '(static, 64)');
t0 = [ 5.951189 3.550487 2.671375 2.297013 2.460522 ];
plot(x, t0, 'LineWidth', 3, 'DisplayName', '(static, 128)');
t0 = [ 5.962850 3.514727 2.288855 1.501498 1.304261 ];
plot(x, t0, 'LineWidth', 3, 'DisplayName', '(static)');

title('hpc-class');
xlabel('Number of Threads');
ylabel('Render Time');
legend('show', 'Location', 'NorthWest');

