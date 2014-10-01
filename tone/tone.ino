static int freq_b = 110;
int max_freq = 220;
int iter =0;
double step;
int freq;

void setup()  { 
  pinMode(9, OUTPUT);    
  Serial.begin(9600);
} 
 
void loop()  { 
  tone(8, 400,100);  
  delay(100);
}
 

