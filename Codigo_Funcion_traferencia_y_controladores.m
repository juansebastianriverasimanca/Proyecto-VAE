clear 
clc
Kp1 = 0.026807;
Ti1 = 127.5;
Td1 = 18.87;
Kp = 343.6367;
Ti = 72.5;
Td = 18.125;
T = 1/4000;
A= xlsread('datos_proyeto','Hoja1');
A=A';
ultA= length(A);
%plot (A (1, :), A (2, :),'bo-');
%grid on;
%Primer Metodo
L=0;
A_est=143.2;
% calculo la constante de tiempo
A_tao =(A_est) *0.632;
for i = 1:ultA-1
    if A_tao>A (2, i) && A_tao<A (2, i+1)
        tao = A (1, i);
    end
end
% Calculo la funciónde transferencia   
Ret=pade(L,0);
num=A_est*Ret;
den= [tao 1];
gp = tf(num,den);
% Genera gráfica de la respuesta de la planta 
%A partir de la función de transferencia obtenida
%bode (num, den);
%grid on;
%pause;
step (gp,'go-');
hold on;
xlabel ('Tiempo (segundos)');
ylabel('Amplitud');
title ('Identificaciónclásicade sistemas dinámicos');
grid on;



[A, B, C, D] = tf2ss(num, den);

% Mostrar matrices de variables de estado
disp('Matriz A:');
disp(A);
disp('Matriz B:');
disp(B);
disp('Matriz C:');
disp(C);
disp('Matriz D:');
disp(D);

Q = diag([1]);
R = 1;
N = zeros(size(B, 2), size(C, 1)); % Matriz de tamaño de B x C, todo en cero por defecto
[K, S, P] = lqr(A, B, Q, R, N);


p = [-2999.43];
Kr = place(A,B,p);

