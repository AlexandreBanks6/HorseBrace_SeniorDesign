%% Data_Analysis.m
% Julia Hill & Luc Bouvier
% 2022-03-08


% Mock Data
t = [0.1:0.1:3.2]';

theta1 = [2500;
2500;
2500;
2500;
2500;
2500;
2500;
2500;
2500;
2300;
1500;
1500;
1600;
1600;
2500;
2500;
2500];

theta2 = [2400;
2000;
1900;
2000;
1900;
1700;
1800;
2000;
2500;
2500;
2500;
2500;
2500;
2500;
2500;
2500;
2500];

theta1 = [theta1;theta1];
theta2 = [theta2;theta2];


accel = zeros(32, 1);
accel(2) = 1;
accel(15) = 1;
accel(31) = 1;

%% Analysis

% Convert angle measurements
theta_sum = zeros(size(theta1));
angle_conversion_factor = (pi/1400);

for i = 1:length(theta_sum)
    if(theta1(i) < 2500)
        theta_sum(i) = 2500 - theta1(i);
    else
        theta_sum(i) = theta2(i) - 2500;
    end
    
    theta_sum(i) = pi + angle_conversion_factor * theta_sum(i);
end


% Segmentation

peak_threshold = 0.5;
[a_values, a_index] = findpeaks(accel, 'MinPeakHeight', peak_threshold);

number_strides = length(a_index) - 1;

master_data = zeros(1, 2, number_strides); % time, angle, stride

for i = 1:number_strides
    
    min_index = a_index(i);
    max_index = a_index(i+1);
    
    theta_vector = theta_sum(min_index:max_index);
    time_vector = t(min_index:max_index);
    
    delta_t = max(time_vector) - min(time_vector);
    t_o = min(time_vector);
    
    for j = 1:(length(theta_vector))
        
        time_vector(j) = (time_vector(j) - t_o) / delta_t;
        
        master_data(j, 1, i) = time_vector(j);
        master_data(j, 2, i) = theta_vector(j);
    end
end


processed_time = [0:0.1:1]';

processed_data = zeros(length(processed_time), number_strides + 1);
processed_data(:,1) = processed_time;

for i = 2:number_strides+1
        
        x = master_data(:, 1, i-1);
        v_x = master_data(:, 2, i-1);
        x_q = processed_time;
        
        for j = 1:length(processed_time)
        
            processed_data(j,i) = interp1(x(v_x~=0), v_x(v_x~=0), x_q(j), 'pchip');
        end
        
end

 
    
    
    
    


