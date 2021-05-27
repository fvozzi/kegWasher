#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); 



const int startButtonPin = 2;
const int optionButtonPin = 3;
const int pumpPin = 13;
const int waterFlowPin = 12;
const int alkalineFlowPin = 11;
const int peraceticFlowPin = 10;
const int alkalineReturnPin = 9;
const int peraceticReturnPin = 8;
const int airFlowPin = 7;
const int co2FlowPin = 6;
const int kegOutputPin = 5;
const int drainOutputPin = 4;
const int waterRinseDuration = 3000;
const int airFlowDuration = 3000;
const int drainDuration = 3000;
const int alkalineFlowDuration = 3000;
const int alkalineReturnDuration = 3000;
const int peraceticFlowDuration = 3000;
const int peraceticReturnDuration = 3000;
const int co2FlowDuration = 3000;
const int co2SweepDuration = 3000;

int startButtonState = 0;
int optionButtonState = 0;
String programs[15];
int selectedProgram = 0;
int indexOfPrograms = 14;
int pins[15];


void setup() {
  // put your setup code here, to run once:
  pinMode(pumpPin, OUTPUT);
  pinMode(waterFlowPin, OUTPUT);
  pinMode(alkalineFlowPin, OUTPUT);
  pinMode(peraceticFlowPin, OUTPUT);
  pinMode(alkalineReturnPin, OUTPUT);
  pinMode(peraceticReturnPin, OUTPUT);
  pinMode(airFlowPin, OUTPUT);
  pinMode(co2FlowPin, OUTPUT);
  pinMode(kegOutputPin, OUTPUT);
  pinMode(drainOutputPin, OUTPUT);
  pinMode(startButtonPin, INPUT);
  pinMode(optionButtonPin, INPUT);
  optionButtonState = digitalRead(optionButtonPin);
  startButtonState = digitalRead(startButtonPin);
  Serial.begin(9600);
  Serial.print(optionButtonState);
  Serial.print(startButtonState);
  // Initiate the LCD:
  lcd.init();
  lcd.backlight();
  initializePrograms ();
  initializePins();
  lcd.setCursor(14, 0); // Set the cursor on the third column and first row.
  lcd.print("Lagoon");
  lcd.setCursor(0, 1);
  lcd.print("P");
  lcd.print(selectedProgram);
  lcd.print(":");
  lcd.print(programs[selectedProgram]);

}

void loop() {
  // put your main code here, to run repeatedly:
  startButtonState = digitalRead(startButtonPin);
  optionButtonState = digitalRead(optionButtonPin);

  if (startButtonState == HIGH)  {
      waterRinseProgram (selectedProgram);
      alkalineWashProgram (selectedProgram);
      alkalineWashProgram (selectedProgram);
      peraceticWashProgram (selectedProgram);
      co2SweepProgram (selectedProgram);
      startOrShutdownActions (selectedProgram);
      }

  if (optionButtonState == LOW)  {
      Serial.print(programs[selectedProgram]);
      Serial.print(selectedProgram);
      selectedProgram = selectedProgram + 1;
      Serial.print(selectedProgram);
      delay(1000);
      if (selectedProgram > indexOfPrograms) {
        selectedProgram = 0;
        }
      lcd.setCursor(0, 1); // Set the cursor on the third column and first row.
      lcd.print("                    ");
      lcd.setCursor(0, 1);
      lcd.print("P");
      lcd.print(selectedProgram);
      lcd.print(":");
      lcd.print(programs[selectedProgram]);
      //Si es una acción, actualiza el estado de la salida
      if (selectedProgram >= 5) {
        printStateOf(pins[selectedProgram]);  
        }
       else {
        lcd.setCursor(1, 3);
        lcd.print("   ");
       }
      }
  }






void waterRinseProgram (int program) {
  if ((program==0) || (program==1)) {
    if (program==0) {
      lcd.setCursor(1, 2); 
      lcd.print(programs[1]);}
    lcd.setCursor(1, 3);
    lcd.print("Llenando barril    ");
     /*Abre el agua y prende la bomba*/ 
    digitalWrite(waterFlowPin, HIGH);
    digitalWrite(pumpPin, HIGH);
    /*Espera un tiempo predetermidado*/
    delay (waterRinseDuration);
    /*Cierra el agua y apaga la bomba*/
    digitalWrite(pumpPin, LOW);
    digitalWrite(waterFlowPin, LOW);
    lcd.setCursor(1, 3); 
    lcd.print("Presurizando (air)");
    /*Prende la inyección de aire desde el compresor*/
    digitalWrite(airFlowPin, HIGH);
    /*Espera el tiempo de inyección de airea*/
    delay (airFlowDuration);
    /*Apaga la inyección de airea*/
    digitalWrite(airFlowPin, LOW);
    lcd.setCursor(1, 3);
    lcd.print("Vaciando barril     ");
    /*Abre la salida del barril y el desagüe*/
    digitalWrite(kegOutputPin, HIGH);
    digitalWrite(drainOutputPin, HIGH);
    /*Espera un tiempo a que se vacie*/
    delay (drainDuration);
    /*Cierra la salida del barrril y el drenaje*/
    digitalWrite(kegOutputPin, LOW);
    digitalWrite(drainOutputPin, LOW);
    lcd.setCursor(0, 2);
    lcd.print("                    ");
    lcd.setCursor(0, 3);
    lcd.print("                    ");
  }
}
    
void alkalineWashProgram (int program) {
  if ((program==0) || (program==2)) {
    if (program==0) {
      lcd.setCursor(1, 2); 
      lcd.print(programs[2]);}
    lcd.setCursor(1, 3);
    lcd.print("Llenando barril    ");
    /*Abre la salida de alacalino y prende la bomba*/
    digitalWrite(alkalineFlowPin, HIGH);
    digitalWrite(pumpPin, HIGH);
    /*Espera un tiempo predetermidado*/
    delay (alkalineFlowDuration);
    /*Cierra el alcalino y apaga la bomba*/
    digitalWrite(pumpPin, LOW);
    digitalWrite(alkalineFlowPin, LOW);
    lcd.setCursor(1, 3); 
    lcd.print("Presurizando (air) ");
    /*Prende la inyección de aire desde el compresor*/
    digitalWrite(airFlowPin, HIGH);
    /*Espera el tiempo de inyección de airea*/
    delay (airFlowDuration);
    /*Apaga la inyección de airea*/
    digitalWrite(airFlowPin, LOW);
    /*Abre la salida del barril y el retorno de alcalino*/
    digitalWrite(kegOutputPin, HIGH);
    digitalWrite(alkalineReturnPin, HIGH);
    lcd.setCursor(1, 3);
    lcd.print("Vaciando barril     ");
    /*Espera un tiempo a que se vacie*/
    delay (alkalineReturnDuration);
    /*Cierra la salida del barrril y el retorno de alcalino*/
    digitalWrite(kegOutputPin, LOW);
    digitalWrite(alkalineReturnPin, LOW);
    lcd.setCursor(0, 2);
    lcd.print("                    ");
    lcd.setCursor(0, 3);
    lcd.print("                    ");
  }
}

void peraceticWashProgram (int program) {
  if ((program==0) || (program==3)) {
    if (program==0) {
      lcd.setCursor(1, 2); 
      lcd.print(programs[3]);}
    lcd.setCursor(1, 3);
    lcd.print("Llenando barril");
    /*Abre la salida de peracetico y prende la bomba*/
    digitalWrite(peraceticFlowPin, HIGH);
    digitalWrite(pumpPin, HIGH);
    /*Espera un tiempo predetermidado*/
    delay (peraceticFlowDuration);
    /*Cierra el peracetico y apaga la bomba*/
    digitalWrite(pumpPin, LOW);
    digitalWrite(peraceticFlowPin, LOW);
    /*Prende la inyección de CO2 desde el compresor*/
    digitalWrite(co2FlowPin, HIGH);
    lcd.setCursor(1, 3); 
    lcd.print("Presurizando (co2)");
    /*Espera el tiempo de inyección de CO2*/
    delay (co2FlowDuration);
    /*Apaga la inyección de CO2*/
    digitalWrite(co2FlowPin, LOW);
    /*Abre la salida del barril y el retorno de alca*/
    digitalWrite(kegOutputPin, HIGH);
    digitalWrite(peraceticReturnPin, HIGH);
    lcd.setCursor(1, 3);
    lcd.print("Vaciando barril     ");
    /*Espera un tiempo a que se vacie*/
    delay (peraceticReturnDuration);
    /*Cierra la salida del barrril y el retorno de alcalino*/
    digitalWrite(kegOutputPin, LOW);
    digitalWrite(peraceticReturnPin, LOW);
    lcd.setCursor(0, 2);
    lcd.print("                    ");
    lcd.setCursor(0, 3);
    lcd.print("                    ");
  }
}
void co2SweepProgram (int program) {
  if ((program==0) || (program==4)) {
    if (program==0) {
      lcd.setCursor(1, 2); 
      lcd.print(programs[4]);}
    /*Prende la inyección de CO2*/
    digitalWrite(co2FlowPin, HIGH);
    /*Espera el tiempo de inyección de CO2 para barrido*/
    delay (co2SweepDuration);
    /*Apaga la inyección de CO2*/
    digitalWrite(co2FlowPin, LOW);  
    lcd.setCursor(0, 2);
    lcd.print("                    ");
    lcd.setCursor(0, 3);
    lcd.print("                    ");
  }
}


void startOrShutdownActions (int program) {
  Serial.println(program);
  if (program>=5) {
      if (digitalRead(pins[program]) == LOW) {
        digitalWrite(pins[program], HIGH);
        delay(1000);
      }
      else {
        digitalWrite(pins[program], LOW);  
        delay(1000);
      }
      printStateOf(pins[program]);
   }
  
}

void printStateOf (int pin) {
   if (digitalRead(pin) == HIGH) {
        lcd.setCursor(1, 3);
        lcd.print("ON ");
        }
   else {
        lcd.setCursor(1, 3);
        lcd.print("OFF");
        }    
   }

void initializePrograms() {
  programs[0] = "Lavado";
  programs[1] = "Enguaje Agua";
  programs[2] = "Alcalino";
  programs[3] = "Sanitizado";
  programs[4] = "Barrido CO2";
  programs[5] = "Bomba";
  programs[6] = "Electro Agua";
  programs[7] = "Electro Alcalino";
  programs[8] = "Electro Peracet";
  programs[9] = "Electro R. Alc.";
  programs[10] = "Electro R. Per.";
  programs[11] = "Electro Aire";
  programs[12] = "Electro CO2";
  programs[13] = "Electro R.Barril";
  programs[14] = "Electro Desague";
  }

void initializePins() {
  //Mapeo de Pins y programas
  pins[5] = pumpPin;
  pins[6] = waterFlowPin;
  pins[7] = alkalineFlowPin;
  pins[8] = peraceticFlowPin;
  pins[9] = alkalineReturnPin;
  pins[10] = peraceticReturnPin;
  pins[11] = airFlowPin;
  pins[12] = co2FlowPin;
  pins[13] = kegOutputPin;
  pins[14] = drainOutputPin;  
}
