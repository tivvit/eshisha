
#include <Arduino.h>
#include <PID_v1.h>
#include <math.h>

int Thermistor1Pin = 0;
int PotentiometerPin = 1;
int Thermistor2Pin = 2;
int MosfetPin = 9;

double Kp = 22, Ki = 0.5, Kd = 150;
double Setpoint, Input, Output, T1, T2;
int T1_raw, T2_raw;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

const int TABLE[] = {-200,-199,-198,-197,-196,-196,-195,-194,-193,-192,-191,-191,-190,-189,-188,-187,-186,-185,-184,-184,-183,-182,-181,-180,-179,-178,-177,-176,-176,-175,-174,-173,-172,-171,-170,-169,-168,-167,-166,-165,-164,-163,-162,-161,-160,-159, -159, -158, -157, -156, -155, -154, -153, -152, -151, -150, -149, -148, -147, -146, -145, -144, -143, -143, -142, -141, -140, -139, -138, -137, -135, -134, -133, -132, -131, -130, -129, -128, -127, -126, -125, -124, -123, -122, -121, -120, -119, -118, -117, -116, -115, -114, -113, -111, -110, -109, -108, -107, -106, -105, -104, -103, -102, -101, -99, -98, -97, -96, -95, -94, -93, -92, -90, -89, -88, -87, -86, -85, -84, -82, -81, -80, -79, -78, -77, -75, -74, -73, -72, -71, -69, -68, -67, -66, -65, -63, -62, -61, -60, -58, -57, -56, -55, -54, -52, -51, -50, -49, -47, -46, -45, -43, -42, -41, -40, -38, -37, -36, -34, -33, -32, -31, -29, -28, -27, -25, -24, -23, -21, -20, -19, -17, -16, -15, -13, -12, -10, -9, -8, -6, -5, -4, -2, -1, 0, 1, 2, 4, 5, 7, 8, 10, 11, 12, 14, 15, 17, 18, 20, 21, 23, 24, 26, 27, 29, 30, 32, 33, 35, 36, 38, 39, 41, 42, 44, 45, 47, 48, 50, 51, 53, 55, 56, 58, 59, 61, 62, 64, 66, 67, 69, 70, 72, 74, 75, 77, 79, 80, 82, 84, 85, 87, 89, 90, 92, 94, 95, 97, 99, 100, 102, 104, 106, 107, 109, 111, 113, 114, 116, 118, 120, 122, 123, 125, 127, 129, 131, 132, 134, 136, 138, 140, 142, 143, 145, 147, 149, 151, 153, 155, 157, 159, 160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 180, 182, 184, 186, 188, 190, 192, 194, 196, 198, 200, 202, 204, 206, 209, 211, 213, 215, 217, 219, 221, 223, 226, 228, 230, 232, 234, 236, 239, 241, 243, 245, 248, 250, 252, 254, 257, 259, 261, 263, 266, 268, 270, 273, 275, 277, 280, 282, 285, 287, 289, 292, 294, 297, 299, 301, 304, 306, 309, 311, 314, 316, 319, 321, 324, 326, 329, 332, 334, 337, 339, 342, 345, 347, 350, 353, 355, 358, 361, 363, 366, 369, 371, 374, 377, 380, 383, 385, 388, 391, 394, 397, 399, 402, 405, 408, 411, 414, 417, 420, 423, 426, 429, 432, 435, 438, 441, 444, 447, 450, 453, 456, 459, 462, 466, 469, 472, 475, 478, 482, 485, 488, 491, 495, 498, 501, 505, 508, 511, 515, 518, 522, 525, 528, 532, 535, 539, 542, 546, 550, 553, 557, 560, 564, 568, 571, 575, 579, 582, 586, 590, 594, 597, 601, 605, 609, 613, 617, 621, 625, 629, 632, 636, 640, 645, 649, 653, 657, 661, 665, 669, 673, 678, 682, 686, 691, 695, 699, 704, 708, 712, 717, 721, 726, 730, 735, 739, 744, 749, 753, 758, 763, 767, 772, 777, 782, 787, 792, 796, 801, 806, 811, 816, 821, 827, 832, 837, 842, 847, 850};

float getThermistor1Temperature() {
  T1_raw = analogRead(Thermistor1Pin);
  return TABLE[T1_raw - 54];
}

float getThermistor2Temperature(int pin) {
  T2_raw = analogRead(pin);
  float SERIESRESISTOR = 1000.0;
  float BCOEFFICIENT = 3950.0;
  float THERMISTORNOMINAL = 100000.0;
  float TEMPERATURENOMINAL = 25.0;

  float R = 1023.0 / T2_raw - 1;
  R = SERIESRESISTOR / R;
 
  float steinhart;
  steinhart = R / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15; 
  
  return steinhart;
}

float getPotTemperature() {
  return 500.0 / 1024 * analogRead(PotentiometerPin);
//    return (analogRead(PotentiometerPin) / 100) * 50;
}

void setup() {
  Serial.begin(9600);
  pinMode(MosfetPin, OUTPUT);

  Input = getThermistor1Temperature();
  Setpoint = getPotTemperature();
  
  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(200);
}

void print() {
  Serial.print("{");
  Serial.print("\"input\": "); 
  Serial.print(Input);
  Serial.print(", \"t1\": "); 
  Serial.print(T1);
  Serial.print(", \"t1_raw\": "); 
  Serial.print(T1_raw);
  Serial.print(", \"t2\": "); 
  Serial.print(T2);
  Serial.print(", \"t2_raw\": "); 
  Serial.print(T2_raw);
  Serial.print(", \"pot1\": "); 
  Serial.print(Setpoint);
  Serial.print(", \"output\": "); 
  Serial.print(Output);
  Serial.print(", \"kp\": "); 
  Serial.print(Kp);
  Serial.print(", \"ki\": "); 
  Serial.print(Ki);
  Serial.print(", \"kd\": "); 
  Serial.print(Kd);
  Serial.println("}");
}

void loop() {
  T1 = getThermistor2Temperature(Thermistor1Pin);  
  T2 = getThermistor2Temperature(Thermistor2Pin);
  Setpoint = getPotTemperature();
  Input = T2;

  myPID.Compute();

  if (Setpoint >= 10.0)
    analogWrite(MosfetPin, Output);
  else {
    analogWrite(MosfetPin, 0.0);
  }    

  if (Serial.available() > 0) {
    Serial.readBytes((byte*) &Kp, 4);
    Serial.readBytes((byte*) &Ki, 4);
    Serial.readBytes((byte*) &Kd, 4);
    myPID.SetTunings(Kp, Ki, Kd);
  }

  print();
  
  delay(200);
} 
