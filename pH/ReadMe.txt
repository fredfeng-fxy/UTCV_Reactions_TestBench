This folder contains the code for the pH Sensor.
The code contains the following functions:
<Comment here to add new functions>

The sensor type is Gravity Analog pH Meter Kit, which can be bought with this link: https://www.banggood.com/PH-Value-Data-Detection-and-Acquisition-Sensor-Module-Acidity-and-Alkalinity-Sensor-Monitoring-and-Control-p-1547652.html?utm_source=googleshopping&utm_medium=cpc_organic&gmcCountry=AE&utm_content=minha&utm_campaign=minha-ae-en-pc&currency=AED&cur_warehouse=CN&createTmp=1&ID=532942&utm_source=googleshopping&utm_medium=cpc_bgs&utm_content=frank&utm_campaign=frank-ssc-ae-en-all-0812&ad_id=455952503503&gclid=CjwKCAiAr6-ABhAfEiwADO4sfdRB-7-5cyHS6Yo7xxJLqlAj49UZvCw1wz7lyKh142ReM9GHI_iGCBoCYdgQAvD_BwE 

Tutorial for using this sensor can be found with this link: https://www.robotshop.com/ca/en/gravity-analog-ph-meter-kit.html

Sensor Specifications
  Module power: 5.00V
  Measuring temperature: 0-60 ℃
  Measuring pH: 0-14
  Response time: ≤ 2 min
  pH sensor with BNC connector
  pH2.0 interface ( 3 foot patch )
  Gain adjustment Potentiometer
  Power indicator LED
  
  
Code:
   
float calibration_value = 21.34 - 0.7;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;
 
float ph_act;

 
void setup() 
{
 Serial.begin(9600);
 pinMode(A0,INPUT);
}



void loop() {
 for(int i=0;i<10;i++) 
 { 
 buffer_arr[i]=analogRead(A0);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buffer_arr[i]>buffer_arr[j])
 {
 temp=buffer_arr[i];
 buffer_arr[i]=buffer_arr[j];
 buffer_arr[j]=temp;
 }
 }
 }
 avgval=0;
 for(int i=2;i<8;i++)
 avgval+=buffer_arr[i];
 float volt=(float)avgval*5.0/1024/6; 
  ph_act = -5.70 * volt + calibration_value;
 
 Serial.println(",");
 Serial.print(ph_act);
 delay(1000);
}
 
