% Tạo các tọa độ của đỉnh
x = [2, 1.5, (3.5 - (sqrt(2) / 4)), (3.5 + (sqrt(2) / 4)), 5.5, 5];
y = [2, 2.5, 5, 5, 2.5, 2];
z_bottom = [0, 0, 0, 0, 0, 0];  % Z tại mặt phẳng Z = 0
z_top = [1, 1.2, 2, 2, 1.2, 1];     % Z tại mặt phẳng Z = 1

% Vẽ hình đa giác dưới (tại z = 0)
fill3(x, y, z_bottom, 'w', 'EdgeColor', 'k', 'LineWidth', 2);
hold on;

% Vẽ hình đa giác trên (tại z = 1)
fill3(x, y, z_top, 'w', 'EdgeColor', 'k', 'LineWidth', 2);

% Vẽ các cạnh nối giữa các đỉnh của hai hình đa giác
for i = 1:length(x)
    plot3([x(i) x(i)], [y(i) y(i)], [z_bottom(i) z_top(i)], 'k', 'LineWidth', 2);
end

% Tùy chỉnh trục
axis equal;
grid on;
title('Hình đa giác 3D với hai mặt phẳng');
xlabel('Trục X');
ylabel('Trục Y');
zlabel('Trục Z');
view(3);  % Tạo góc nhìn 3D

hold off;