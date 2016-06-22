//   light  monitor
#define PULSEPIN 8
#define LEDPIN 13
#define BUZZERPIN  6
unsigned long int duration;
unsigned long int start;

int innescato = 0;
void setup()
{
  Serial.begin(9600);
  pinMode(PULSEPIN, INPUT);
  pinMode(BUZZERPIN, OUTPUT);
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);
  innescato = 0;
  delay(1000);
  tone(BUZZERPIN, 500, 200); delay(500);

  //digitalWrite(BUZZERPIN, HIGH); delay(500);
  //digitalWrite(BUZZERPIN, LOW); delay(100);
}



void loop()

{
  delay(400);
  start = micros();
  duration = pulseIn(PULSEPIN, HIGH, 2000000L);
  start = micros() - start;
  if (start < 1300000 && !duration) {
    duration = 1;
    Serial.print ("CONTROL!");
  }
  Serial.print("duration "); Serial.println(duration);
  Serial.print("start "); Serial.println(start);
  Serial.print("innescato "); Serial.println(innescato);
  if (duration == 0) {
    innescato = 1;
    digitalWrite(LEDPIN, LOW);
  }
  if (innescato && duration) {

    tone(BUZZERPIN, 500, 600); delay(500);
    delay(500);

    // digitalWrite(BUZZERPIN, HIGH); delay(500);
    // digitalWrite(BUZZERPIN, LOW); delay(100);

  }

}
