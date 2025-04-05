% Tạo các tọa độ của đỉnh
x = [2, 1.5, (3.5 - (sqrt(2) / 4)), (3.5 + (sqrt(2) / 4)), 5.5, 5];
y = [2, 2.5, 5, 5, 2.5, 2];
z = [0, 0, 0, 0, 0, 0];  % Z cố định tại mặt phẳng Z = 0

% Vẽ hình đa giác 3D
fill3(x, y, z, 'w', 'EdgeColor', 'k', 'LineWidth', 2);
axis equal;

% Tùy chỉnh thêm
grid on;
title('Hình đa giác 3D');
xlabel('Trục X');
ylabel('Trục Y');
zlabel('Trục Z');
view(3);  % Tạo góc nhìn 3D
