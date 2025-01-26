const int adc = 0;
const int pwm_pin =8;
const int start_pin =9;
const int stop_pin =10;
#define pin_adc_job_time A0
#define pin_adc_run_time A1
#define pin_adc_rest_time A2
#define pin_adc_freq A3
#define pin_adc_duty A4


#define freq_max  90
#define freq_min  10
#define run_max   2000
#define run_min   400
#define rest_max  2200
#define rest_min  500
#define duty_max  60
#define duty_min  0
#define pulse_width  100
#define time_max  45*60
#define time_min  1*60

long run_time=800;
long rest_time=1000;
uint8_t duty=50;
uint16_t  freq=20;
uint32_t job_time=50000;
uint8_t time_minute=0;
uint16_t time_second=0;
long mils;
long fris=0;
#define idle_mode  0
#define run_mode  1
#define rest_mode  2
uint8_t mode=idle_mode;
uint8_t pinval=0;
uint16_t tonrunfreq=500/10;
uint16_t toffrunfreq;
uint16_t trun=10000;
uint16_t trest =1000;
long micfreqton=0;
long micfreqtoff=0;
long micfreqpulselen=0;
long micfreqtimepulsechange;
long micfreqtimepulseon;
long micfreqtimepulseoff;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(start_pin,INPUT_PULLUP);
pinMode(stop_pin,INPUT_PULLUP);
pinMode(pwm_pin,OUTPUT);
digitalWrite(pwm_pin,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
  if(mode==idle_mode){
    readadcvolumdata();
    if(digitalRead(start_pin)==LOW){
      mode=run_mode;
      job_time=millis()/1000+time_second;
    }
  }else{
    if(digitalRead(stop_pin)==LOW){
      mode=idle_mode;
      digitalWrite(pwm_pin,LOW);
    }
    if(millis()/1000>job_time){
      mode = idle_mode;
      digitalWrite(pwm_pin,LOW);
    }
    //
    //
    if(mode==run_mode){
    
      if(millis()>run_time){
         rest_time=millis()+trest;
        digitalWrite(pwm_pin,LOW);
        mode=rest_mode;
      }else{
         if(micros()>micfreqtimepulsechange){
           digitalWrite(pwm_pin,1);
          micfreqtimepulseoff= micros()+micfreqton;
          micfreqtimepulsechange=micros()+micfreqpulselen;
        }else{
          if(micros()>micfreqtimepulseoff){
            digitalWrite(pwm_pin,LOW);
          }
        }
      }
    }else 
    {
      digitalWrite(pwm_pin,LOW);
      readadcvolumdata();
      
      
      if(millis()>rest_time){
        micfreqpulselen = 500000/freq;
        micfreqton = micfreqpulselen/pulse_width;
        micfreqton*=duty;
        micfreqtimepulsechange= micros()+micfreqpulselen;
        micfreqtimepulseoff= micros()+micfreqton;
        run_time=millis()+trun;
        
        mode=run_mode;
        printdatareadedtoserial();
      }
    }
    
  }

  
}


void readadcvolumdata(){
      trun = map(analogRead(pin_adc_run_time), 0, 1023, run_min, run_max);
      trest = map(analogRead(pin_adc_rest_time), 0, 1023, rest_min, rest_max);
      time_second = map(analogRead(pin_adc_job_time), 0, 1023, time_min, time_max);
      duty = map(analogRead(pin_adc_duty), 0, 1023, duty_min, duty_max);
      freq = map(analogRead(pin_adc_freq), 0, 1023, freq_min, freq_max);
      printdatareadedtoserial();
}

void printdatareadedtoserial(){
  
  Serial.print("\t _mode = ");
  Serial.print(mode);
  Serial.print("\t ");
  Serial.print(digitalRead(start_pin));
  Serial.print(" , ");
  Serial.print(digitalRead(stop_pin));
  Serial.print("\t_timer = ");
  Serial.print(time_second);
  Serial.print("\t run = ");
  Serial.print(trun);
  Serial.print("\t rest = ");
  Serial.print(trest);
  Serial.print("\t _freq = ");
  Serial.print(freq);
  Serial.print("\t duty = ");
  Serial.print(duty);
  Serial.println("");
  
}
