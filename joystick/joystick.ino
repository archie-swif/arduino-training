
// the setup function runs once when you press reset or power the board
void setup() {
 Serial.begin(9600);
 pinMode(7, INPUT);
}

// the loop function runs over and over again forever
void loop() {
 
 int button = digitalRead(7);
 int x = analogRead(A0);
 int y = analogRead(A1);
 Serial.print(x);
 Serial.print(":");
 Serial.print(y);  
 Serial.print(" but:");  
 Serial.println(button);   
 
 
 delay(100);
}
