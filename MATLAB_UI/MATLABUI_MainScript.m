%{
Title: MATLABUI_MainScript.m
Date: January 5ht, 2022
Description: Connects to the HC-05 bluetooth module. Writes command to
transfer data to computer, writes calibration command, writes record/stop
recording commands.

%}
clear
clc
close all

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
% list=bluetoothlist;
% %Connect to the HC-05 bluetooth module
device=bluetooth("TekBoot");  
ReHorseUI(device);

%% UI Function
function ReHorseUI(device)

%Creates a figure window
fig=uifigure('Position',[700,350,250,350]);
fig.Color='#ADD8E6';   %Royal blue background colour

%~~~~~~<Add 4 pushbuttons to the UI figure>~~~~~~~~~~~
% Button 1=read data file ('R')
% Button 2=start recording ('I')
% Button 3=stop recording ('O')
% Button 4=calibrate ('C')

b1=uibutton(fig,'push',...
    'Position',[25,200,100,100],...
    'ButtonPushedFcn',@(b1,event) ReadData(b1,device));    %Calls the function to read data through HC-05
b1.Text='Read Data File';
b1.FontName='Roboto Medium';
set(b1,'Backgroundcolor','#e18b6d');


b2=uibutton(fig,'push',...
    'Position',[125,200,100,100],...
    'ButtonPushedFcn',@(b2,event) StartRecording(b2,device));    %Calls the function to start recording gait data
b2.Text='Start Recording';
b2.FontName='Roboto Medium';
set(b2,'Backgroundcolor','#e18b6d');

b3=uibutton(fig,'push',...
    'Position',[25,100,100,100],...
    'ButtonPushedFcn',@(b3,event) StopRecording(b3,device));    %Calls the function to stop recording gait data
b3.Text='Stop Recording';
b3.FontName='Roboto Medium';
set(b3,'Backgroundcolor','#e18b6d');

b4=uibutton(fig,'push',...
    'Position',[125,100,100,100],...
    'ButtonPushedFcn',@(b4,event) Calibrate(b4,device));    %Calls the function to start recording gait data
b4.Text='Calibrate Device';
b4.FontName='Roboto Medium';
set(b4,'Backgroundcolor','#e18b6d');

% Adding Title Text to UI
lblTitle=uilabel(fig);
lblTitle.Position=[25,325,200,25];
lblTitle.Text='Make a Selection';
lblTitle.FontSize=16;
lblTitle.HorizontalAlignment='center';
lblTitle.FontName='Roboto Medium';


%Adding instructional Text for Calibration
lblIns=uilabel(fig);
lblIns.Position=[25,300,200,25]; %Sets the position
lblIns.Text='Ensure Horse is Standing Still at Calibration';
lblIns.HorizontalAlignment='center';
lblIns.FontSize=10;
lblIns.FontName='Roboto Light';

%Adding Company Name
lblName=uilabel(fig);
lblName.Position=[25,25,75,50];
lblName.Text='ReHorse Technologies';
lblName.WordWrap='on'; %Makes the text two lines
lblName.FontSize=12;
lblName.FontName='Playfair Display';

%Adding Year
lblYear=uilabel(fig);
lblYear.Position=[175,25,75,50];
lblYear.Text='V1.1,     Updated 2022';
lblYear.WordWrap='on';
lblYear.FontSize=12;
lblYear.FontName='Roboto Light';



end


%% Bluetooth Interface Functions
function ReadData(b1,device)
Command='G';  %Sends 'R' char to read the data file
write(device,Command);  %Writes the read command to the HC-05 module

end

function StartRecording(b2,device)
Command='I';  %Sends 'I' char to start recording data 
write(device,Command);  %Writes the start recording command to the HC-05 module

end

function StopRecording(b3,device)
Command='O';  %Sends 'O' char to start recording data 
write(device,Command);  %Writes the stop recording command to the HC-05 module

end

function Calibrate(b4,device)
Command='C';  %Sends 'C' char to calibrate the sensors 
write(device,Command);  %Writes the calibrate command to the HC-05 module

end



