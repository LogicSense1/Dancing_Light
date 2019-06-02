# DANCING LIGHTS MATRIX

this repo use the library of arduino FFT https://github.com/kosme/arduinoFFT

In this project, we use Arduino DUE to supply power and control a LED Matrix, which is composed of 8*8
LEDs connected together. After receiving external inputs of specific patterns from the smartphone by using BT or
from PC through Bluetooth input controlled by GUI, the LED Matrix can display two different patterns with given
signals by turning the LED lights on or off in the specific position continuously. In spectrum displaying mode, the
Matrix can analyse the frequency spectrum of music and display the spectrum with LED lights. The processed
frequency information will then be utilized to analyse the rhythm of music in order to display in the heart-beat
mode.

After collecting the raw audio analog data from the analog input pin which connected to the microphone, the
analog data would be coded in digital data format with the help of the build-in ADC inside the SMA3X. This digital
data represents the time domain variety of the sound signal. By using the ArduinoFFT library, we were able to create
an FFT object in the IDE, which will initialize the basic operation of the Fast Fourier Transform. The FFT operation
will take the array of time domain data as input and calculate the frequency components of the original signal and
saving both real and image component of the frequency into 2 new arrays. After, we use the function of arduinoFFT
called FFT.ComplexToMagnitude(vReal, vImag, SAMPLES), which will take both arrays and transform the
complex number into magnitude components of the signal frequency spectrum. The set of real number is easy for
further calculation and display. After some test, we find that most songs tested shared a similar frequency range from
300Hz to 6000Hz. This range of frequency will be divided into 8 groups for display, and the height of each column is
decided by the sum of each value of magnitude in the same group. By setting a fix value of threshold of turn on all the
lights in one column, the exact height of each column can be calculated by,

Meanwhile, we found that we can take the advantage of the frequency domain array to achieve more functions
besides spectrum display. By observing how the frequency spectrum changes when there is a strong beat happening
is the music, we found that there will be a significant high magnitude value compared to other frequency components.
So, in the rhythm analysis mode, we used two patterns which were big heart to represent the strong beat and small
heart to represent calm music. By determining the ratio between the highest magnitude and lowest magnitude, a beat
could be found if the ratio was higher than a certain threshold value. The beat detector implemented in the program
would trigger the big heart to indicate the rhythm of the music.

We implemented wireless communication between Arduino and GUI on PC by the Seeed Bluetooth Shield. The
Tx and Rx pins in the Bluetooth Module were connected to the Serial 2 Tx and Rx pins in Arduino, which allows
Arduino to receive and send messages with wireless communication. Then, we connected the Bluetooth Module with
PC and turn the Bluetooth port into a Serial port, named ¡°/dev/cu.HHHH-DevB¡±, by a serial port terminal application
on PC.
We designed a GUI by Processing and as shown in the following code, Processing allows GUI to transform
messages via the serial port.

