/*
B0 = 2;   I have tried to make this lab relevant to some of the things you will encounter on exams.
B1 = 3;   here we are going to give the system an 8-bit signal we will call A. we are going to use  
B2 = 4;   this signal to manipulate an "output signal" that will write to an output variable B, which 
B3 = 5;   has 8 bits. The numbers on the right side of the = sign are the pins that connect to outputs 
B4 = 6;   B0-B7. You will notice that this section is commented out. This is because there is a better 
B5 = 7;   and more efficient way to connect these pins to this variable that we have created.
B6 = 8;
B7 = 9;
*/

/* 
  You can ignore these two global variables; you do not need to manipulate them 
*/
char myData[20];
byte myHex[2];

/* 
  This is the most efficient way to associate the pins on the Arduino to the bits on 
  the int array B. This implementation is not required, but we consider it to be clean 
  and efficient.
*/
int B[8] = {2,3,4,5,6,7,8,9};

/*
  The setup() function runs once when the program begins
*/
void setup() {

  /*
    This for-loop sets each of the pins associated with array B to an output. 
    Your code will access these pins directly on the board by using B[*]. 
  */
  for(int i = 0; i < 8 ; i++){
    pinMode(B[i],OUTPUT);
  }

  /* 
    This line of code is required to initialize the serial monitor. It is required for
    every lab assignment. Without it, your program is unlikely to work correctly.
  */
  Serial.begin(9600);
}

/*
  getHex() is an example of a helper function, which is called from loop() below. 
  This function reads and writes to global arrays myData and myHex, which were
  declared above. This function involves the manipulation of hexadecimal variables,
  bitwise operations (|), and shifting (<<), which are covered in Chapter 2 of the
  zyBook. 
*/
int getHex()
{
  for (int i = 0; i < 2; i++)
  {
    byte y = myData[i];
    if (y <= 0x39)
    {
      myHex[i] = y - 0x30;
    }
    else
    {
      myHex[i] = y - 0x37;
    }
  }
  return ((unsigned int)(myHex[0] << 4) | (unsigned int)(myHex[1]));
}

void writeB (unsigned char x) {
  unsigned char mask = 1 << 7;
  int i = 7;
  while (mask != 0) {
  	digitalWrite(B[i--], x & mask? HIGH: LOW);
    mask = mask >> 1;
  }
}

/*
  The loop() function iterates infinitely. Your program will not "end,"
  which is quite different from other programs you have written in your
  academic career thus far.
*/
void loop() {

  /*
    Serial.print() should be your go-to tool for debugging purposes. 
    We strongly recommend that you use it extensively throughout the quarter. 
    Serial.print() writes data directly to the serial monitor.
  */
  Serial.print("What is the input A?\n");

  /*
    Serial.available() returns the amount of data that has been supplied to the
    serial monitor. If there is no data, then Serial.available() will return a 
    value of 0. This line pauses your loop until some data is provided to
    the serial monitor.
  */
  while(Serial.available() == 0){}
  
  /*
    These lines of code preserve the input transmitted to the serial monitor
    as 8-bit hexadecimal values in the range 0x00 to 0xFF for you to use 
    during the rest of this laboratory exercise. For this exercise, you
    will manipulate the data read from input A to obtain information that
    you will write to output B. You are not required or expected to understand
    precisely what this code is doing.
  */
  byte m = Serial.readBytesUntil('\n', myData, 20);
  myData[m] = '\0';
  unsigned int reading = getHex();
  Serial.println(reading,HEX);
  Serial.println(reading,BIN);
  Serial.println(reading);

  /* Uncomment the following statement for the laboratory exercise */
  Serial.print("Which bit of A would you like to access?\n(enter a value 0-7)\n");
  
  /* 
     Here, you will need to write some of your own code that uses a function called
     Serial.parseInt() to assign a value written by the user to the serial monitor
     to a variable in the program. 

     For example: int x = Serial.parseInt(); 

     Here, if the user inputs 75 to the serial monitor, then calling Serial.parseInt()
     will assign value 75 to variable x. 

     While this may seem trivial, what is actually happening is that the input provided
     to the serial monitor is a string (i.e., a sequence of characters), which must be
     converted to a numerical value. This is a service provided by the Arduino API for
     you to use. 
  */

  unsigned char x = Serial.parseInt() % 7;
  
  /* Uncomment the following statement for the laboratory exercise */
  Serial.print("masking...\n");

  writeB((unsigned char) reading);
  x = ((unsigned char) reading) & (1 << x);

  while (x > 0) {
    delay(1000);
    writeB(x);
    x=x>>1; 
  }

  /*  
    The bottom of loop() is where the fun begins. You have now provided the system with 
    an input (a signal) and you have specified a bit that you would like view, so that 
    you can determine whether its value is 0 or 1. This is similar to how RIMS input A 
    and output B work on in-class exercises and exams.

    For this assignment, you will need to use masking and bit manipulation (see zyBook 
    Sections 2.4-2.7) to identify and extract the desired bit and to transform
    it into a usable value.

    We provide the following suggestions to help you with this exercise:
    - Create one or more helper functions to keep your code clean and modular
    - Use the Arduino delay() function to mimic the demo video
      NOTE: Using the delay() function in later labs will create significant problems.     
            Please do not call this function unless directed to do so. 
      WARNING: Starting with Laboratory Assignment #2, we will impose significant grading 
               penalties if you write arbitrary code that calls delay()
      WARNING: Quizzes and exams are based on RIMS, not the Arduino API. 
  */
  
}