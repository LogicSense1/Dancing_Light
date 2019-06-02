import controlP5.*; //import ControlP5 library
import processing.serial.*;

Serial port;

ControlP5 cp5; //create ControlP5 object
PFont font1;
PFont font2;

int mycolor=255;

void setup(){ //same as arduino program

  size(400, 400);    //window size, (width, height)
  
  //printArray(Serial.list());   
  
  port = new Serial(this,"/dev/cu.HHHH-DevB",9600);  
  
  //lets add buton to empty window
  
  cp5 = new ControlP5(this);
  
  font1 = createFont("Futura-Bold", 15);    // custom fonts for buttons and title
  font2 = createFont("Futura-Bold", 25);
 
  cp5.addButton("Heart_Beat")     //the name of button
    .setPosition(100, 100)  //x and y coordinates of upper left corner of button
    .setSize(200, 80)      //(width, height)
    .setFont(font1)
  ;
  
  cp5.addButton("Music_Dancing")     //the name of button
    .setPosition(100, 220)  //x and y coordinates of upper left corner of button
    .setSize(200,80)      //(width, height)
    .setFont(font1)
  ;
  
}

void draw(){  //same as loop in arduino
  
  background(mycolor); // background color of window (r, g, b) or (0 to 255)
  
  //lets give title to our window
  fill(62,176,247);               //text color (r, g, b)
  textFont(font2);
  text("MODE CONTROL", 90, 60);  // ("text", x coordinate, y coordinat)
}

//lets add some functions to our buttons
//so whe you press any button, it sends perticular char over serial port


void Heart_Beat(){
  port.write('1');
  if (mycolor<10) {mycolor=255;}
  else{mycolor=mycolor-50;}
}

void Music_Dancing(){
  port.write('0');
  if (mycolor<10) {mycolor=255;}
  else{mycolor=mycolor-50;}
}
