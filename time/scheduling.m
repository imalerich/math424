function static1
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

t0 = [ 7.176597 4.249530 3.100026 2.932752 2.823958 ];
plot(x, t0, 'LineWidth', 3, 'DisplayName', '(dynamic, 1)');
t0 = [ 6.283623 3.586096 2.277317 1.646120 1.478188 ];
plot(x, t0, 'LineWidth', 3, 'DisplayName', '(dynamic, 4)');
t0 = [ 6.147951 3.464589 2.200541 1.576933 1.395637 ];
plot(x, t0, 'LineWidth', 3, 'DisplayName', '(dynamic, 8)');

t0 = [ 5.937296 3.396720 2.190465 1.868807 2.267183 ];
plot(x, t0, 'LineWidth', 3, 'DisplayName', '(guided, 1)');
t0 = [ 5.933095 3.405738 2.162900 1.678488 1.761131 ];
plot(x, t0, 'LineWidth', 3, 'DisplayName', '(guided, 4)');
t0 = [ 5.933393 3.392965 2.162468 1.621884 1.578440 ];
plot(x, t0, 'LineWidth', 3, 'DisplayName', '(guided, 8)');

title('hpc-class');
xlabel('Number of Threads');
ylabel('Render Time');
legend('show', 'Location', 'NorthWest');

