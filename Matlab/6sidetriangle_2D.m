% Tạo các tọa độ của đỉnh
x = [2, 1.5, (3.5 - (sqrt(2) / 4)), (3.5 + (sqrt(2) / 4)), 5.5, 5];
y = [2 2.5 5 5 2.5 2];

% Vẽ hình đa giác
fill(x, y, 'w', 'EdgeColor', 'k', 'LineWidth', 2);
axis equal;

% Tùy chỉnh thêm
grid on;
title('Hình ngũ giác tùy chỉnh');
xlabel('Trục X');
ylabel('Trục Y');