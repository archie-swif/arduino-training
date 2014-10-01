
// the setup function runs once when you press reset or power the board
void setup() {
 Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
 
 int ans = analogRead(A0);
 tone(9, ans, 100); 
 Serial.println(ans);
 delay(100);
}
