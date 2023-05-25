#define buttonpin 2
int LED1 = 5;
int LED2 = 6;

void swap(int* a, int* b); // function prototype

void setup() {
  
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(buttonpin,INPUT_PULLUP);
  
  Serial.begin(9600);
  

}

void loop() {

  if (!digitalRead(buttonpin) == HIGH) {// if button closed detected *negates high voltage from PULLUP register 
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    
    swap(&LED1,&LED2);// call swap function
    Serial.println("Button pressed!");
    
  }
}

void swap(int* a, int* b)// function declaration 
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
    
}