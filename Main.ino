/*
 * Bluetooth car Arduino side program.
 * 
 * Tasks:
 *  accept serial Bluetooth stream - done
 *  build h brige function - done
 *  build steering fnction - done 
 *  remap x/y to input to h bridge
 *  Bluetooth error handeling and input filtering - done 
 */

int mottor_a1 = 5;// mottor a contol pins - right back
int mottor_a2 = 4;
int enable_a = 0; //enable for mottor a

int mottor_b1 = 9;//mottor b control pins - left front
int mottor_b2 = 10;
int enable_b = 0; //enable for mottor b

int mottor_c1 = 5;//mottor c control pins - left back
int mottor_c2 = 6;
int enable_c = 0; //enable for mottor c

int mottor_d1 = 11;//mottor d control pins - right front
int mottor_d2 = 12;
int enable_d = 0; //enable for mottor d


char inSerial[10];

void setup() {
  Serial.begin(9600);

  pinMode(mottor_a1, OUTPUT);
  pinMode(mottor_a2, OUTPUT);
  pinMode(enable_a, OUTPUT);
  
  pinMode(mottor_b1, OUTPUT);
  pinMode(mottor_b2, OUTPUT);
  pinMode(enable_b, OUTPUT);
  
  pinMode(mottor_c1, OUTPUT);
  pinMode(mottor_c2, OUTPUT);
  pinMode(enable_c, OUTPUT);

  pinMode(mottor_d1, OUTPUT);
  pinMode(mottor_d2, OUTPUT);
  pinMode(enable_d, OUTPUT);
  
  
}

void motor(int pin1, int pin2, int speed_pin, int m_speed, int dir){
  if (dir == 0){
    analogWrite(pin1, m_speed);
    digitalWrite(pin2, LOW);
    //analogWrite(speed_pin, m_speed);
  }
  if (dir == 1){
    digitalWrite(pin1, LOW);
    analogWrite(pin2, m_speed);
    //analogWrite(speed_pin, m_speed);
  }
  if (dir == 2){
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);  
  }
}

String stering(int x){ //function needs to return two ints (0-100 - a percentage of real speed) for dir control
  if(x == 0){
    return "100:100";//return 100% speed for each mottor
  }
  if(x < 0){//if user is turning left
    String a = String(map(x, -10, 0, 25, 100)); 
    return "0" + a + ":100";
    }
  if(x > 0){//if user is turning left
    String a = String(map(x, 0, 10, 100, 25)); 
    return "100:0" + a;
    }
}

void loop() {
  int i = 0;
  if (Serial.available() > 2){ //check is any data is avaliable to read             
     while (Serial.available() > 0){
       inSerial[i]=Serial.read(); 
       i++;}
     i = 0;
 
     }
  //int x = inSerial[0].toInt();//splits incoming serial stream in to x var
  //int y = inSerial[2];//splits incoming serial stream in to y var

  String in_str(inSerial);//converts input char to sting
  int x = in_str.substring(0,1).toInt();//converts input string to int
  int y = in_str.substring(2).toInt();//converts input string to int
    
  if (x > 6){x = 6;}//basic inout filtering for x
  if (x < 0){x = 0;}
  if (y > 6){y = 6;}//basic inout filtering for y
  if (y < 0){y = 0;}
  

  int x_remaped = map(x, 0, 6, -10, 10);//remaps inout data to a more usable format 
  int y_remaped = map(y, 0, 6, -10, 10);
  
  if(y > 0){
    motor(mottor_a1, mottor_a2, enable_a, map(x, 0, 10, 0, 200) * stering(x_remaped).substring(0,2).toInt(), 1); //left hand mottor 1
    motor(mottor_b1, mottor_b2, enable_b, map(x, 0, 10, 0, 200) * stering(x_remaped).substring(0,2).toInt(), 1); //left hand mottor 2
    motor(mottor_c1, mottor_c2, enable_c, map(x, 0, 10, 0, 200) * stering(x_remaped).substring(3,6).toInt(), 1); //right hand mottor 1
    motor(mottor_d1, mottor_d2, enable_d, map(x, 0, 10, 0, 200) * stering(x_remaped).substring(3,6).toInt(), 1); //right hand mottor 2
  }
  
  if(y < 0){
    motor(mottor_a1, mottor_a2, enable_a, map(x, 0, 10, 0, 200) * stering(x_remaped).substring(3,6).toInt(), 0); //left hand mottor 1
    motor(mottor_b1, mottor_b2, enable_b, map(x, 0, 10, 0, 200) * stering(x_remaped).substring(3,6).toInt(), 0); //left hand mottor 2
    motor(mottor_c1, mottor_c2, enable_c, map(x, 0, 10, 0, 200) * stering(x_remaped).substring(0,2).toInt(), 0); //right hand mottor 1
    motor(mottor_d1, mottor_d2, enable_d, map(x, 0, 10, 0, 200) * stering(x_remaped).substring(0,2).toInt(), 0); //right hand mottor 2
  }
  }
