%{
Title: MATLABUI_MainScript.m
Date: January 5ht, 2022
Description: Connects to the HC-05 bluetooth module. Writes command to
transfer data to computer, writes calibration command, writes record/stop
recording commands.

%}
clear
clc
%% Defining the app
% fig=uifigure;
% fig.Name="ReHORSE Tech";
% 
% %Set the APP layout
% gl=uigridlayout(fig,[2,2]); %2x2 grid
% 
% %Square app for now
% gl.RowHeight={30,30};
% gl.ColumnWidth={30,30};
% 
% %Creating button
% b1=uibutton(gl);    %Creates the button to read data
% 
% %Position components on the grid
% b1.Layout.Row=1;    %read data button in top row
% b1.Position=[10,20,10,5];

%% Connecting to the bluetooth module
%show the bluetooth list (for debugging only)
list=bluetoothlist;
%Connect to the HC-05 bluetooth module
device=bluetooth("HC-05");  
ReHorseUI(device);

%% UI Functions
function ReHorseUI(device)
fig=uifigure('Position',[700,350,200,200]);

%Create a pushbutton
b1=uibutton(fig,'push',...
    'Position',[50,50,50,100],...
    'ButtonPushedFcn',@(b1,event) WriteGreen(b1,device));    %Calls the function to turn on the green LED through HC-05
b1.Text='Green';
b2=uibutton(fig,'push',...
    'Position',[100,50,50,100],...
    'ButtonPushedFcn',@(b2,event) WriteRed(b2,device));    %Calls the function to turn on the green LED through HC-05
b2.Text='Red';

end

function WriteGreen(b1,device)
ReadDataCommand='G';  %Sends 'G' char to turn on the green LED
%device=bluetooth("HC-05"); 
write(device,ReadDataCommand);  %Writes the green command to the HC-05 module

end

function WriteRed(b2,device)
ReadDataCommand='R';  %Sends 'R' char to turn on the red LED
%device=bluetooth("HC-05"); 
write(device,ReadDataCommand);  %Writes the red command to the HC-05 module

end



