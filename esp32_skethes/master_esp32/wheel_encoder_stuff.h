#ifndef WHEEL_ENCODER_STUFF_H_
#define WHEEL_ENCODER_STUFF_H_


const byte MOTOR1 = 21;  // Motor 1 Interrupt Pin - INT 0
const byte MOTOR2 = 19;  // Motor 2 Interrupt Pin - INT 1
unsigned long rotary_time=1000;


volatile unsigned long counter1 = 0;
volatile unsigned long counter2 = 0;
 
// Float for number of slots in encoder disk
float diskslots = 20;  // Change to match value of encoder disk
 
// Interrupt Service Routines
 
// Motor 1 pulse count ISR
void IRAM_ATTR ISR_count1()  
{
  counter1++;  // increment Motor 1 counter value
} 
 
// Motor 2 pulse count ISR
void IRAM_ATTR ISR_count2()  
{
  counter2++;  // increment Motor 2 counter value
} 

void start_wheel_encoder(){
  attachInterrupt(digitalPinToInterrupt (MOTOR1), ISR_count1, RISING);  // Increase counter 1 when speed sensor pin goes High
  attachInterrupt(digitalPinToInterrupt (MOTOR2), ISR_count2, RISING);  // Increase counter 2 when speed sensor pin goes High
  
}

void reset_wheel_encoder_data(){

  Serial.println(counter1);
  Serial.println(counter2);
  counter1 = 0;
  counter2 = 0;
  
}

#endif