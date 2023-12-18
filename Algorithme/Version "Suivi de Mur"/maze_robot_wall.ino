#define LIGHT_SENSOR_PIN A0

#define PROX_SENSOR_L_PIN A1
#define PROX_SENSOR_R_PIN 6
#define PROX_SENSOR_FL_PIN A3
#define PROX_SENSOR_FR_PIN A4
#define PROX_SENSOR_RL_PIN A5
#define PROX_SENSOR_RR_PIN 12
#define PROX_SENSOR_DL_PIN A2
#define PROX_SENSOR_DR_PIN 9

#define MOTOR_RF_PIN 2
#define MOTOR_RB_PIN 4
#define MOTOR_R_SPEED 3
#define MOTOR_LF_PIN 7
#define MOTOR_LB_PIN 8
#define MOTOR_L_SPEED 5

void hardware_setup() {
  new DCMotor_Hbridge(MOTOR_RF_PIN, MOTOR_RB_PIN, MOTOR_R_SPEED, "ePuck_rightJoint", 2.5, 3 * 3.14159, 1);  //RF = 1 = Turn forward motor R; RB = 0 = Turn backward motor R
  new DCMotor_Hbridge(MOTOR_LF_PIN, MOTOR_LB_PIN, MOTOR_L_SPEED, "ePuck_leftJoint", 2.5, 3 * 3.14159, 1);   //LF = 1 = Turn forward motor L; LB = 0 = Turn backward motor L

  new VisionSensor(LIGHT_SENSOR_PIN, "ePuck_lightSensor", 0.1);  //Black 0-0.1 and White 0.1-1; take a look at 0-1023 units of color before percentage

  new ProximitySensor(PROX_SENSOR_FL_PIN, "ePuck_proxSensor3", 0.1, 1);  //Front L
  new ProximitySensor(PROX_SENSOR_FR_PIN, "ePuck_proxSensor4", 0.1, 1);  //Front R
  new ProximitySensor(PROX_SENSOR_L_PIN, "ePuck_proxSensor1", 0.1, 1);   //L
  new ProximitySensor(PROX_SENSOR_R_PIN, "ePuck_proxSensor6", 0.1, 1);   //R
  new ProximitySensor(PROX_SENSOR_RL_PIN, "ePuck_proxSensor7", 0.1, 1);  //Rear L
  new ProximitySensor(PROX_SENSOR_RR_PIN, "ePuck_proxSensor8", 0.1, 1);  //Rear R
  new ProximitySensor(PROX_SENSOR_DL_PIN, "ePuck_proxSensor2", 0.1, 1);  //Diag 45° forward L
  new ProximitySensor(PROX_SENSOR_DR_PIN, "ePuck_proxSensor5", 0.1, 1);  //Diag 45° forward R
}


//                                    //                     //
//                                    // VARIABLE DEFINITION //
//                                    //                     //


//Variables for motors
int normal_speed = 100;
int high_speed = 255;
int speed = high_speed;

//Variables for sensors
//Contain the color between 0 and 1023
int color = 0;

//Contain the distance between 0 and 1023 except for the digital ones
int frontR_d = 0;
int frontL_d = 0;
int R_d = 0;  //digital sensor, distance is 0 or 1 if it detects something
int L_d = 0;
int rearR_d = 0;  //digital sensor, distance is 0 or 1 if it detects something
int rearL_d = 0;
int diagR_d = 0;  //digital sensor, distance is 0 or 1 if it detects something
int diagL_d = 0;

//Variables to optimize the program
//Each value lower than the current one will be considered as NO WALL and upper will be considered as WALL
int detection_frontR_d = 500;
int detection_frontL_d = 500;
int detection_R_d = 0;  //digital sensor, distance is 0 or 1 if it detects something
int detection_L_d = 650;
int detection_rearR_d = 0;  //digital sensor, distance is 0 or 1 if it detects something
int detection_rearL_d = 700;
int detection_diagR_d = 0;  //digital sensor, distance is 0 or 1 if it detects something
int detection_diagL_d = 725;

int time_BLACK_CELL = -(125000 / (high_speed * 0.85));  //Useful to avoid considering the black lines as the BLACK CELL

//Variables for millis()
bool FULL_STOP = false;          //true = end of the maze
bool COLOR_FOUND_SWITCH = true;  //true = looking for BLACK and false = looking for RED
bool read_currentTime = true;    //Useful to read only once the time when we're staying on the BLACK cell
float currentTime = 0;
float tpsHoraire = 0;      //Register the time between the RED cell and the BLACK cell
float tpsAntiHoraire = 0;  //Register the time between the BLACK cell and the RED cell

//Variables for calculing the race
float INIT_TIME = 0;
float FINAL_TIME = 0;
bool read_INIT_TIME = true;   //Useful to read only once the time when we're beginning on the RED cell
bool read_FINAL_TIME = true;  //Useful to read only once the time when we're ending on the RED cell


//                                    //                //
//                                    // SETUP FUNCTION //
//                                    //                //


void setup() {
  Serial.begin(4800);

  //Setup motor
  pinMode(MOTOR_RF_PIN, OUTPUT);
  pinMode(MOTOR_RB_PIN, OUTPUT);
  pinMode(MOTOR_R_SPEED, OUTPUT);
  pinMode(MOTOR_LF_PIN, OUTPUT);
  pinMode(MOTOR_LB_PIN, OUTPUT);
  pinMode(MOTOR_L_SPEED, OUTPUT);

  //Setup sensors
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  pinMode(PROX_SENSOR_FL_PIN, INPUT);
  pinMode(PROX_SENSOR_FR_PIN, INPUT);
  pinMode(PROX_SENSOR_L_PIN, INPUT);
  pinMode(PROX_SENSOR_R_PIN, INPUT);
  pinMode(PROX_SENSOR_RL_PIN, INPUT);
  pinMode(PROX_SENSOR_RR_PIN, INPUT);
  pinMode(PROX_SENSOR_DL_PIN, INPUT);
  pinMode(PROX_SENSOR_DR_PIN, INPUT);
}


//                                    //               //
//                                    // LOOP FUNCTION //
//                                    //               //


void loop() {
  global_detection_distance();  //Using all sensors to determine all the distances

  if ((vision_color() == 2) && (read_INIT_TIME == true)) {  //Register start time of the race
    INIT_TIME = millis();
    Serial.print("Temps initiale : ");
    Serial.print(INIT_TIME / 1000);
    Serial.println(" s");
    read_INIT_TIME = false;
  }

  if (COLOR_FOUND_SWITCH == true) {  //true = we're are looking for the BLACK cell
    black_found();                   //If we're one the BLACK cell, turn false COLOR_FOUND_SWITCH
  }

  if (COLOR_FOUND_SWITCH == false) {  //false = we're are looking for the RED cell
    red_found();                      //If we're one the RED cell, turn true FULL_STOP
  }

  if (FULL_STOP == true) {  //We're on the RED cell, so we register the end time of the race and calculate the race time
    stop();
    if (read_FINAL_TIME == true) {
      FINAL_TIME = millis();
      Serial.print("Temps entre ROUGE et NOIRE : ");
      Serial.print((tpsHoraire - INIT_TIME) / 1000);
      Serial.println(" s");
      Serial.print("Temps entre NOIRE et ROUGE : ");
      Serial.print((tpsAntiHoraire - tpsHoraire) / 1000);
      Serial.println(" s");
      Serial.print("Temps final : ");
      Serial.print((FINAL_TIME - INIT_TIME) / 1000);
      Serial.println(" s");
      if (((tpsAntiHoraire - tpsHoraire) / 1000) > ((tpsHoraire - INIT_TIME) / 1000)){
        Serial.println("Le chemin ROUGE vers NOIRE est plus court, il faut donc parcourir le labyrinthe dans le sens Horaire");
      }
      else{
        Serial.println("Le chemin NOIRE vers ROUGE est plus court, il faut donc parcourir le labyrinthe dans le sens Antihoraire");
      }
      read_FINAL_TIME = false;
    }
  }


  //                                    //                     //
  //                                    // MAIN MOVEMENT LOGIC //
  //                                    //                     //


  if (FULL_STOP == false) {                                                    //We're not on the black cell, so the robot is allowed to move
    if ((frontR_d > detection_frontR_d) && (frontL_d > detection_frontL_d)) {  //Move forward until we reach a wall in front on us
      speed = high_speed;
      forward();
    }
    if ((frontR_d < detection_frontR_d) || (frontL_d < detection_frontL_d)) {  //Turn right to keep a safe distance between the robot and the left wall
      speed = normal_speed;
      turn_right();
    }
    if ((diagL_d > detection_diagL_d) && (L_d > detection_L_d) && (rearL_d > detection_rearL_d)) {  //Turn left if all left sensors don't detect a wall
      speed = normal_speed;
      turn_left();
    }
  }
}


//                                    //                  //
//                                    // VISION FUNCTIONS //
//                                    //                  //


void vision_wall() {  //Print distance of front left and front right sensor
  Serial.print("Left = ");
  Serial.print(L_d);
  Serial.print(" ; Right = ");
  Serial.println(R_d);
}

int vision_color() {  //Return a number linked to the color under the robot
  //990 < WHITE < 1030     WHITE is linked to 0
  //0   < BLACK < 40       BLACK is linked to 1
  //270 < RED   < 310       RED  is linked to 2
  color = analogRead(LIGHT_SENSOR_PIN);
  if ((color > 270) && (color < 310)) {
    //Serial.println("Color = RED");
    return 2;
  }
  if ((color > 990) && (color < 1030)) {
    //Serial.println("Color = WHITE");
    return 0;
  }
  if ((color > 0) && (color < 40)) {
    //Serial.println("Color = BLACK");
    return 1;
  }
}

void global_detection_distance() {  //Activation of all sensors at once to read all the distances
  frontR_d = analogRead(PROX_SENSOR_FR_PIN);
  frontL_d = analogRead(PROX_SENSOR_FL_PIN);
  R_d = digitalRead(PROX_SENSOR_R_PIN);
  L_d = analogRead(PROX_SENSOR_L_PIN);
  rearR_d = digitalRead(PROX_SENSOR_RR_PIN);
  rearL_d = analogRead(PROX_SENSOR_RL_PIN);
  diagR_d = digitalRead(PROX_SENSOR_DR_PIN);
  diagL_d = analogRead(PROX_SENSOR_DL_PIN);
}


//                                    //                       //
//                                    // COLOR_FOUND FUNCTIONS //
//                                    //                       //


void black_found() {          //Turn false COLOR_FOUND_SWITCH if we're on the BLACK cell, to now look for the RED cell
  if (vision_color() == 0) {  //If we see WHITE
    read_currentTime = true;  //Allow to register the time when we're going to see BLACK
  }
  if (vision_color() == 1) {         //If we see BLACK
    if (read_currentTime == true) {  //Register once the time see begin to see BLACK
      read_currentTime = false;
      currentTime = millis();
    }
    if (currentTime - millis() < time_BLACK_CELL) {  //If we see BLACK for a certain period of time // (currentTime - millis() < -(125000 / (speed / 5)))
      Serial.println("Case noire trouvée !");
      tpsHoraire = millis();
      COLOR_FOUND_SWITCH = false;  //Now we're looking for the RED cell
    }
  }
}

void red_found() {                                       //Turn true FULL_STOP if we're on the RED cell, end of the maze
  if ((vision_color() == 0) || (vision_color() == 1)) {  //If we see WHITE or BLACK
    read_currentTime = true;                             //Allow to register the time when we're going to see RED
  }
  if (vision_color() == 2) {         //If we see RED
    if (read_currentTime == true) {  //Register once the time see begin to see RED
      read_currentTime = false;
      currentTime = millis();
    }
    if (currentTime - millis() < time_BLACK_CELL) {  //If we see RED for a certain period of time // (currentTime - millis() < -(125000 / (speed / 5)))
      tpsAntiHoraire = millis();
      FULL_STOP = true;  //End of the maze
    }
  }
}


//                                    //                    //
//                                    // MOVEMENT FUNCTIONS //
//                                    //                    //


void stop() {  //Turn off the engines
  analogWrite(MOTOR_R_SPEED, 0);
  analogWrite(MOTOR_L_SPEED, 0);
}

void forward() {  //Turn on the engines forward
  digitalWrite(MOTOR_RF_PIN, HIGH);
  digitalWrite(MOTOR_RB_PIN, LOW);
  digitalWrite(MOTOR_LF_PIN, HIGH);
  digitalWrite(MOTOR_LB_PIN, LOW);

  analogWrite(MOTOR_R_SPEED, speed);
  analogWrite(MOTOR_L_SPEED, speed);
}

void backward() {  //Turn on the engines backward
  digitalWrite(MOTOR_RF_PIN, LOW);
  digitalWrite(MOTOR_RB_PIN, HIGH);
  digitalWrite(MOTOR_LF_PIN, LOW);
  digitalWrite(MOTOR_LB_PIN, HIGH);

  analogWrite(MOTOR_R_SPEED, speed);
  analogWrite(MOTOR_L_SPEED, speed);
}

void turn_right() {  //Turn on the engines to turn right
  digitalWrite(MOTOR_RF_PIN, LOW);
  digitalWrite(MOTOR_RB_PIN, HIGH);
  digitalWrite(MOTOR_LF_PIN, HIGH);
  digitalWrite(MOTOR_LB_PIN, LOW);

  analogWrite(MOTOR_R_SPEED, speed);
  analogWrite(MOTOR_L_SPEED, speed);
}

void turn_left() {  //Turn on the engines to turn left
  digitalWrite(MOTOR_RF_PIN, HIGH);
  digitalWrite(MOTOR_RB_PIN, LOW);
  digitalWrite(MOTOR_LF_PIN, LOW);
  digitalWrite(MOTOR_LB_PIN, HIGH);

  analogWrite(MOTOR_R_SPEED, speed);
  analogWrite(MOTOR_L_SPEED, speed);
}
