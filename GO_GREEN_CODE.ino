#define POWER_PIN  7
#define SIGNAL_PIN A5


int value = 0; // variable to store the sensor value

const int trigPin = 9;
const int echoPin = 10;
long duration;
int distanceCm, distanceInch;
int oldValue = 0 , newValue = 0;
const int AirValue = 620;   //you need to replace this value with Value_1
const int WaterValue = 310;  //you need to replace this value with Value_2
int soilMoistureValue = 0;
int soilmoisturepercent=0;


int flowPin = 2;    //This is the input pin on the Arduino
double flowRate;    //This is the value we intend to calculate.
volatile int count;


void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(POWER_PIN, OUTPUT);   // configure D7 pin as an OUTPUT
  digitalWrite(POWER_PIN, LOW);
  pinMode(flowPin, INPUT);           //Sets the pin as an input
  attachInterrupt(0, Flow, RISING); // turn the sensor OFF
  Serial.begin(9600);
  
}

void loop() {

  digitalWrite(POWER_PIN, HIGH);  // turn the sensor ON
  delay(10);                      // wait 10 milliseconds
  value = analogRead(SIGNAL_PIN); // read the analog value from sensor
  digitalWrite(POWER_PIN, LOW);   // turn the sensor OFF

  Serial.print("WATER RISE: ");
  Serial.println(value);

  delay(1000);
  if(value > 100){
    Serial.println("water rise");
    Serial.println("detected");
    
  }

    digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  
  distanceCm = duration * 0.0340 / 2;
  newValue = distanceCm;
  if(newValue != oldValue)
  {
    Serial.print("WATER LEVEL FROM THE OPENING OF THE TANK: ");
    Serial.println(distanceCm);
    //Serial.println(" cm");
    oldValue = newValue;
  }
  delay(2000);
  

  soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
  //Serial.println(soilMoistureValue);
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
  if(soilmoisturepercent >= 100)
  {
    Serial.println("100 %");
  }
  else if(soilmoisturepercent <=0)
  {
    Serial.println("moisture : 0 %");
  }
  else if(soilmoisturepercent >0 && soilmoisturepercent < 100)
  
  {
    Serial.print("moisture %: ");
    Serial.print(soilmoisturepercent);
    Serial.println("%");
    
  }
    delay(1000);


    count = 0;      // Reset the counter so we start counting from 0 again
  //interrupts();   //Enables interrupts on the Arduino
//  delay (1000);   //Wait 1 second
//  noInterrupts(); //Disable the interrupts on the Arduino

  //Start the math
  flowRate = (count * 2.25);        //Take counted pulses in the last second and multiply by 2.25mL
  flowRate = flowRate * 60;         //Convert seconds to minutes, giving you mL / Minute
  flowRate = flowRate / 1000;       //Convert mL to Liters, giving you Liters / Minute

  Serial.print("Flowrate: ");
  Serial.println(flowRate);    

  
}

void Flow()
{
   count++; //Every time this function is called, increment "count" by 1
}
