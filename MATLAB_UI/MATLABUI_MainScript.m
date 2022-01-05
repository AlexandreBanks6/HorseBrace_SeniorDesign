%{
Title: MATLABUI_MainScript.m
Date: January 5ht, 2022
Description: Connects to the HC-05 bluetooth module. Writes command to
transfer data to computer, writes calibration command, writes record/stop
recording commands.

%}

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

%% UI Functions
function ReHorseUI
fig=uifigure('Position',[700,350,200,200]);

%Create a pushbutton
b1=uibutton(fig,'push',...
    'Position',[50,50,100,100],...
    'ButtonPushedFcn',@(b1,event) readDataButtonPushed(b1));    %Calls the function to read data from HC-05
b1.Text='Read Data';
end

function readDataButtonPushed(b1)
ReadDataCommand=['ReadData',0];  %Reads data command is sent with nul termination
device=bluetooth("HC-05"); 
write(device,ReadDataCommand);  %Writes the read data command to the HC-05 module

end



