double fuzzifier_();
int abss(int num1);
double minimum(double num1, double num2);
void Fuzz_Right(unsigned int Right_Sensor);
void Fuzz_Left(unsigned int Left_Sensor);
unsigned int left,right,middle;
double RGRAY, RBLACK, RWHITE, LWHITE, LBLACK, LGRAY;
double fwd, s_left, h_left, s_right, h_right, output;
//fwd - 0|| s_left - -50|| h_left - -100|| s_right - 50|| h_right - 100||
int motion;
unsigned int R_PWM=0, L_PWM=0;
void main() {
     TRISA=0x0F;
     TRISC=0;
     TRISB=0;
     PORTB=0;
     PWM1_Init(5000);
     PWM2_Init(5000);
     ADC_Init();
     while(1)
         {
            left = ADC_Read(0);  //LEFT
            right = ADC_Read(1);   //RIGHT
            middle = ADC_Read(2);   //MIDDLE
            Fuzz_Left(left);
            Fuzz_Right(right);
            //motion = (int)fuzzifier_();
            if(middle<=100)  //IF THE MIDDLE SENSOR IS IN BLACK
            {
               PWM1_Set_Duty(250);  //right wheel
               PWM2_Set_Duty(250);  //left wheel
               PWM1_Start();
               PWM2_Start();
            }
            else if(middle>100)
            {
              motion = (int)fuzzifier_();
              if(motion<10)
              {
                 R_PWM = 150 + abss(motion);
                 L_PWM = 150 - abss(motion);
                 PWM1_Set_Duty(R_PWM);  //right wheel
                 PWM2_Set_Duty(L_PWM);  //left wheel
                 PWM1_Start();
                 PWM2_Start();
                 //Delay_ms(20);
              }
              else if(motion>10)
              {
                 R_PWM = 150 - motion;
                 L_PWM = 150 + motion;
                 PWM1_Set_Duty(R_PWM);  //right wheel
                 PWM2_Set_Duty(L_PWM);  //left wheel
                 PWM1_Start();
                 PWM2_Start();
                 //Delay_ms(20);
              }
              else
              {
                PWM1_Set_Duty(200);  //right wheel
                PWM2_Set_Duty(200);  //left wheel
                PWM1_Start();
                PWM2_Start();
                //Delay_ms(20);
              }
            }
            Delay_ms(80);
         }
}
void Fuzz_Right(unsigned int Right_Sensor)
{
   if(Right_Sensor < 95)
    {
     RBLACK = 1;
     RWHITE = 0;
     RGRAY = 0;
    }
    else if(Right_Sensor <205)
    {
     RGRAY = (double)(Right_Sensor-95)/110;
     RBLACK = 1-RGRAY;
     RWHITE = 0;
    }
    else if(Right_Sensor < 315)
    {
     RWHITE = (double)(Right_Sensor-205)/110;
     RGRAY = 1-RWHITE;
     RBLACK = 0;
    }
    else
    {
     RWHITE = 1;
     RBLACK = 0;
     RGRAY = 0;
    }
}
void Fuzz_Left(unsigned int Left_Sensor){
    if(Left_Sensor < 56)
    {
     LBLACK = 1;
     LWHITE = 0;
     LGRAY = 0;
    }
    else if(Left_Sensor < 120)
    {
     LGRAY = (double)(Left_Sensor-56)/64;
     LBLACK = 1-LGRAY;
     LWHITE = 0;
    }
    else if(Left_Sensor < 184)
    {
     LWHITE = (double)(Left_Sensor-120)/64;
     LGRAY = 1-LWHITE;
     LBLACK = 0;
    }
    else
    {
     LWHITE = 1;
     LBLACK = 0;
     LGRAY = 0;
    }
}
double fuzzifier_(){
      fwd = minimum(RWHITE, LWHITE);
      s_left = minimum(LGRAY, RWHITE);
      h_left = minimum(RWHITE, LBLACK);
      s_right = minimum(RGRAY, LWHITE);
      h_right = minimum(RBLACK, LWHITE);
      output  = (fwd*0) + (s_left*(-25)) + (h_left*(-50)) + (s_right*25) + (h_right*50);
      output = output/(fwd + s_left + h_left + s_right + h_right);
      return output;
}

double minimum(double num1, double num2)
{
 return (num1 > num2) ? num2 : num1;
}
int abss(int num1)
{
 return (num1 < 0) ? (-num1) : num1;
}

