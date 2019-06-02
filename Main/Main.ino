#include "arduinoFFT.h"
#define SAMPLES 256            
#define SAMPLING_FREQUENCY 40000
#define blueToothSerial Serial2

// R is the Rows of LED Matrix
// C is the Columns of LED Matrix
int R[] = {52, 50, 48, 46, 44, 42, 40, 38};
int C[] = {45, 43, 41, 39, 37, 35, 33, 31};
unsigned int sampling_period_us;
unsigned long microseconds;
int output[8] = {0, 0, 0, 0, 0, 0, 0, 0};
double vReal[SAMPLES];
double vImag[SAMPLES];
int char_recived = 0;
arduinoFFT FFT = arduinoFFT();

// The image of a big heart in pixel
unsigned char biglove[8][8] =
{
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 0, 0, 1, 1, 0,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  0, 1, 1, 1, 1, 1, 1, 0,
  0, 0, 1, 1, 1, 1, 0, 0,
  0, 0, 0, 1, 1, 0, 0, 0,
};

// The image of a small heart in pixel
unsigned char smalllove[8][8] =
{
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 1, 0, 0, 1, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 0,
  0, 1, 1, 1, 1, 1, 1, 0,
  0, 0, 1, 1, 1, 1, 0, 0,
  0, 0, 0, 1, 1, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
};


void setupBlueToothConnection()
{
  blueToothSerial.begin(38400);                           // Set BluetoothBee BaudRate to default baud rate 38400
  blueToothSerial.print("\r\n+STWMOD=0\r\n");             // set the bluetooth work in slave mode
  blueToothSerial.print("\r\n+STNA=HHHH\r\n");            // set the bluetooth name as "SeeedBTSlave"
  blueToothSerial.print("\r\n+STOAUT=1\r\n");             // Permit Paired device to connect me
  blueToothSerial.print("\r\n+STPIN =0000\r\n");          // Set the connection pin as '0000'
  blueToothSerial.print("\r\n+STAUTO=0\r\n");             // Auto-connection should be forbidden here
  delay(2000);                                            // This delay is required.
  blueToothSerial.print("\r\n+INQ=1\r\n");                // make the slave bluetooth inquirable
  Serial.println("The slave bluetooth is inquirable!");   
  delay(2000);                                            // This delay is required.
  blueToothSerial.flush();
}


void LED_Matrix(double peak, int vReal[]) {
  int i = 0;
  int j = 0;
  int led_display[8];
  
  // Fix the value of sampled frequency in the range of 1 to 8
  for (i = 0; i <= 7; i++) {
    led_display[i] = int(double(vReal[i]) / 150 * 8);
    if (led_display[i] > 8) {
      led_display[i] = 8;
    }
  }

  // Display the spectrum column by column
  for (i = 0; i <= 7; i++) {
    // Select the ith column and write LOW
    digitalWrite(i * 2 + 31, LOW);
    // Light up the selected LEDs on this column
    for (j = 0; j <= led_display[i]; j++) {
      digitalWrite(j, HIGH);
    }
    for (j = 38; j <= led_display[i] * 2 + 38; j = j + 2) {
      digitalWrite(j, HIGH);
    }
    delayMicroseconds(500);
    // Turn off the selected LEDs on this column
    for (j = 38; j <= led_display[i] * 2 + 38; j = j + 2) {
      digitalWrite(j, LOW);
    }
    for (j = 6; j <= led_display[i]; j++) {
      digitalWrite(j, LOW);
    }
    // Unselected the ith column and write HIGH
    digitalWrite(i * 2 + 31, HIGH);
  }
}

// Display the given image
void Display(unsigned char dat[8][8])
{
  for (int c = 0; c < 8; c++)
  {
    digitalWrite(C[c], LOW);
    for (int r = 0; r < 8; r++)
    {
      digitalWrite(R[r], dat[r][c]);
    }
    delay(1);
    Clear();
  }
}

// Clear the LEDs
void Clear()
{
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(R[i], LOW);
    digitalWrite(C[i], HIGH);
  }
}


void setup() {
  Serial.begin(115200);
  setupBlueToothConnection();
  // Calculate the sampling_period in us
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
  pinMode(A0, INPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  // Set Columns as output
  pinMode(31, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(37, OUTPUT);
  pinMode(39, OUTPUT);
  pinMode(41, OUTPUT);
  pinMode(43, OUTPUT);
  pinMode(45, OUTPUT);
  // Set Rows as output
  pinMode(38, OUTPUT);
  pinMode(40, OUTPUT);
  pinMode(42, OUTPUT);
  pinMode(44, OUTPUT);
  pinMode(46, OUTPUT);
  pinMode(48, OUTPUT);
  pinMode(50, OUTPUT);
  pinMode(52, OUTPUT);

}

void loop() {
  /*SAMPLING*/
  for (int i = 0; i < SAMPLES; i++)
  {
    microseconds = micros();    //Overflows after around 70 minutes!
    vReal[i] = analogRead(0);
    vImag[i] = 0;
    //LED_Matrix(0, output);
    while (micros() < (microseconds + sampling_period_us)) {
    }
  }
  /*FFT*/
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  int i, j;
  // Calculate the sums of 8 equal intervals and write those values into output[8]
  for (i = 0; i < 8; i++) {
    output[i] = 0;
    for (j = 4 * i + 2; j < 4 * (i + 1) + 2; j++) {
      output[i] = output[i] + vReal[j];
    }
  }
  
  // Calculate the sum of all values in output[]
  int sum = 0;
  for (i = 0; i < 8; i++) {
    sum = sum + output[i];
  }
  int max_value = 0;
  int min_value = output[0];
  // Calculte the max value and min value in output[]
  for ( i = 0; i < 8; i++)
  {
    if (max_value < output[i])
    {
      max_value = output[i];
    }
  }
  for ( i = 0; i < 8; i++)
  {
    if (min_value > output[i])
    {
      min_value = output[i];
    }
  }

  // Default Mode: Display spectrum
  if (char_recived == 0) {
    LED_Matrix(0, output);
  }

  // Heart Mode: Display heart
  else {
    // The duration of a single heart image is based on volumn
    // The maximum duration is 20
    int duration;
    duration = 2000 / sum;
    if (duration > 20) {
      duration = 20;
    }

    // Find the Rhythm of music
    // Display big heart if it is the drumbeat point
    if (max_value / min_value > 5) {
      for (int i = 0 ; i < duration; i++)
      {
        Display(biglove);
      }
    }

    // Display smalllove in default. The duration is based on volumn
    for (int i = 0 ; i < duration ; i++)
    {
      Display(smalllove);
    }
  }

  // If transfer a particular value through Bluetooth, switch mode between spectrum and heart
  if (blueToothSerial.available()){ 
    char_recived = blueToothSerial.parseInt();
  }
}
