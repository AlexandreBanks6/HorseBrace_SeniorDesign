%{
Title: MATLABUI_MainScript.m
Date: March 14th, 2022
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
fig=uifigure('Position',[700,350,250,370]);
fig.Color='#ADD8E6';   %Royal blue background colour

%~~~~~~<Add 4 pushbuttons to the UI figure>~~~~~~~~~~~
% Button 1= Calibration ('C')
% Button 2=start recording ('I')
% Button 3=stop recording ('O')
% Button 4= See Live Data ('C')


%Calibration Button
b1=uibutton(fig,'push',...
    'Position',[75,250,100,50],...
    'ButtonPushedFcn',@(b1,event) Calibrate(b1,device));    %Calls the function to calibrate through the HC-05
b1.Text='Calibrate';
b1.FontName='Roboto Medium';
set(b1,'Backgroundcolor','#e18b6d');


%Start Recording
b2=uibutton(fig,'push',...
    'Position',[12.5,125,100,100],...
    'ButtonPushedFcn',@(b2,event) StartRecording(b2,device));    %Calls the function to start recording gait data
b2.Text='Start Recording';
b2.FontName='Roboto Medium';
set(b2,'Backgroundcolor','#e18b6d');


%Stop Recording
b3=uibutton(fig,'push',...
    'Position',[137.5,125,100,100],...
    'ButtonPushedFcn',@(b3,event) StopRecording(b3,device));    %Calls the function to stop recording gait data
b3.Text='Stop Recording';
b3.FontName='Roboto Medium';
set(b3,'Backgroundcolor','#e18b6d');

%See live data checkbox

b4=uibutton(fig,'state',...
    'Position',[75,50,100,50],...
    'ValueChangedFcn',@(b4,event) LiveData(b4,device));    %Calls the function to display live gait data
b4.Text='Live Data';
b4.FontName='Roboto Medium';
set(b4,'Backgroundcolor','#e18b6d');




%Adding Company Name
lblName=uilabel(fig);
lblName.Position=[60,320,200,50];
text=sprintf('%s\n%s','     TekBoot','Biomonitoring');
lblName.Text=text;
%lblName.WordWrap='on'; %Makes the text two lines
lblName.FontSize=21;
lblName.FontName='Roboto Light';

%Adding instructional Text for Calibration
lblIns=uilabel(fig);
lblIns.Position=[25,300,200,25]; %Sets the position

lblIns.Text='Ensure Horse is Standing Still at Calibration';
lblIns.HorizontalAlignment='center';
lblIns.FontSize=10;
lblIns.FontName='Roboto Light';

%Adding Year
lblYear=uilabel(fig);
lblYear.Position=[220,4,70,30];
text=sprintf('%s\n%s','V1.2','2022');
lblYear.Text=text;
%lblYear.WordWrap='on';
lblYear.FontSize=12;
lblYear.FontName='Roboto Light';






end


%% Bluetooth Interface Functions
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
t=datetime('now');val=datenum(t); %Gets the current date and time, and then converts it to a datenum value for serial transmission
SendStr=num2str(val)
%||||||||
%This can be extrated from the datenum value stored in excel using the
%following code, where "val" is the datenum value
%valnew=datetime(val,'ConvertFrom','datenum') when reading the data
%||||||||||

write(device,Command);  %Writes the calibrate command to the HC-05 module
write(device,SendStr,"string");
%{
write(device,val&&0xf); %writes the six bytes containing the date and time
write(device,bitsrl(val,4)&&0xf);
write(device,bitsrl(val,8)&&0xf);
write(device,bitsrl(val,12)&&0xf);
write(device,bitsrl(val,16)&&0xf);
write(device,bitsrl(val,20)&&0xf);
write(device,bitsrl(val,24)&&0xf);
write(device,bitsrl(val,28)&&0xf);
%}

end

%------------------------<Live Data Transmission>--------------------------
function LiveData(b4,device) %Function to control live data transmission
    %Set the brace to transmit live data
    if(b4.Value==1) %Checks that the checkbox was pressed
        Command='L'; %Sends the 'L' command to start transmitting live data from the device
        write(device,Command);  %Writes the stop recording command to the HC-05 module
        pause(0.001); %Pauses for 0.001 seconds
        flush(device); %Clears the device buffers
        
        %~~~~~~Initializing variables
        i=0; %Counter which is the number of live streamed samples
        count=0;
        maxcount=23; %The number of samples between plotting calls
        AXNum=18; %THe number of samples to the right/2 and left*2 of the current sample
        AXScale=maxcount*AXNum;
        
        fs=250; %Approx sampling frequency
        ScaleFacDown=(AXScale*2)/fs; %Scale factor which is how far to the left the data stream goes
        ScaleFacUp=(AXScale/2)/fs; %How far to the right the data stream goes
        
        xdat=zeros(1,AXNum*2); %Initializes the array to hold the x data
        ydat=zeros(1,AXNum*2); %Init array to hold the y-data
        
        %The following are used to convert the A/D flex sensor values to
        %angles
        
        BinMax=637;
        BinMin=245;
        
        
        
        
        %~~~~~~~~~~~Read Data
        data=read(device,4,"uint8");
        data=data(1)+data(2)*256+data(3)*(256^2)+data(4)*(256^3); %Converting the 4 bytes to an integer
        xdat(1)=i/fs;
        ydat(1)=180-((data-BinMin)/(BinMax-BinMin))*180;
        %Plotting Initial Data Point
        hPlot=plot(i/fs,data,'-');
        axis([i/fs-ScaleFacDown,i/fs+ScaleFacUp,-5,185]);
        title("Live Stream",'FontSize',20);
        ylabel('Degrees');
        xlabel('Time');       
        
    else %Stop the live stream
        Command='S'; %Sends the 'S' character to stop live stream
        write(device,Command);
        pause(0.001);
    end


    
    
    while(b4.Value==1) %loops while live stream data checkbox is pressed
        i=i+1; %Increment sample counter
        count=count+1; %Increment counter used to update plots
        %Reading data
        data=read(device,4,"uint8");
        if(length(data)~=4)
            continue;
        end
        data=data(1)+data(2)*256+data(3)*(256^2)+data(4)*(256^3); %Converting the 4 bytes to an integer
        
        
         
        
        
        
        
        if(count>maxcount) %Updates the plot every maxcount sampless
            
            %Updates the data vectors
            xdat(2:end)=xdat(1:end-1);
            xdat(1)=i/fs;
            ydat(2:end)=ydat(1:end-1);
            ydat(1)=180-((data-BinMin)/(BinMax-BinMin))*180;
            %Updates the plots
            set(hPlot,'XData',xdat,'YData',ydat);
            axis([i/fs-ScaleFacDown,i/fs+ScaleFacUp,-5,185]);
            hold on; 
            
           
            
            
            
            count=0;
        end
        
    
        pause(0.001);
       
        

    end
    hold off;

end
