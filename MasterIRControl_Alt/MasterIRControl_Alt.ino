
/////////////////////////////////////////////////////
//          David C. Dean - LU 2012.05.22          //
//                www.DavidCDean.com               //
/////////////////////////////////////////////////////
//     __ __             _             _  ___      //
//    |  \  \ ___  ___ _| |_ ___  _ _ | || . \     //
//    |     |<_/ |<_-<  | | / ._>| '_\| ||   /     //
//    |_|_|_|<_+_|/__/  |_| \___.|_|  |_||_\_\     //
//               MasterIRControl v05               //
//                                                 //
/////////////////////////////////////////////////////


//  REFERENCE
//    
//    selectReceiver()
//    startCmd()
//    getCmd()
//    ProcessCode()
//    PulsePin(int ControlPin, long MicroSeconds)
//
//    EEPROM_Store(int loc, int value)
//    EEPROM_Fetch(int loc)
//    
//    setup()
//    loop()
//    attach_callbacks(messengerCallbackFunction* callbacks)
//    arduino_ready()
//    unknownCmd()


// EXTERNAL LIBS
// CmdMessenger :: http://arduino.cc/playground/Code/CmdMessenger
// Arduino-Base64 :: https://github.com/adamvr/arduino-base64
// Streaming5 :: http://arduiniana.org/libraries/streaming/

#include <CmdMessenger.h>
#include <Base64.h>
#include <Streaming.h>
#include <EEPROM.h>


// CmdMessenger Setup
char field_separator = '|';
char command_separator = ';';
CmdMessenger cmdMessenger = CmdMessenger(Serial, field_separator, command_separator);


// Standard CmdMessenger Responses
enum
{
  kCOMM_ERROR    = 000, // Report Comm Error to  PC
  kACK           = 001, // Acknowledges Cmd Receive
  kARDUINO_READY = 002, // Com Open, 02 from PC to Arduino
  kERR           = 003, // Report Bad Cmd (Format/Unknown)
  kSEND_CMDS_END, // Term - Mandatory
};


// Our CmdMessenger Functions Map
messengerCallbackFunction messengerCallbacks[] = 
{
  selectReceiver, // 4, Switch Receiver Control
  startCmd,       // 5, Wipe Code and Start
  getCmd,         // 6, Store New Cmd
  ProcessCode,    // 7, Process Code
  NULL
};


// Set our IR Emitter Pins
int IRPin1 = 13;
int IRPin2 = 11;
int IRPin3 = 9;
int IRPin4 = 7;

// Set active Receiver
int RelayCode[72];
int ActiveReceiver;
int CodeCount;
boolean StartCode = true;


// Program Setup - IR Pins to OUT, Set up serial
void setup(){
  pinMode(IRPin1, OUTPUT);
  pinMode(IRPin2, OUTPUT);
  pinMode(IRPin3, OUTPUT);
  pinMode(IRPin4, OUTPUT);
  ActiveReceiver = IRPin1;
  Serial.begin(115200);
  cmdMessenger.print_LF_CR();

  // Attach default and user callbacks
  cmdMessenger.attach(kARDUINO_READY, arduino_ready);
  cmdMessenger.attach(unknownCmd); 
  attach_callbacks(messengerCallbacks);
  arduino_ready();
}


// Listening for Server Serial Control Commands
void loop()
{
  // Handle Incoming Serial Commands
  cmdMessenger.feedinSerialData();

  // Codeset Test - Comcast RNG110
  //testRoutine();
  delay(500);
}


// Switch Working Receiver
void selectReceiver()
{
  while(cmdMessenger.available()){
    char buf[350] = {'\0'};
    cmdMessenger.copyString(buf,350);
    if (buf[0]){
      int SwitchInt = atoi(buf);
      switch(SwitchInt){
      case 1: 
        { 
          ActiveReceiver = IRPin1; 
          cmdMessenger.sendCmd(kACK,"Switched 1");
          break;
        }
      case 2: 
        { 
          ActiveReceiver = IRPin2; 
          cmdMessenger.sendCmd(kACK,"Switched 2");
          break;
        }
      case 3: 
        { 
          ActiveReceiver = IRPin3; 
          cmdMessenger.sendCmd(kACK,"Switched 3");
          break;
        }
      case 4: 
        { 
          ActiveReceiver = IRPin4; 
          cmdMessenger.sendCmd(kACK,"Switched 4");
          break;
        }
      } // END SWITCH
    }// END IF
  }// END AVAILABLE
}// END selectReceiver


// Set RelayCode Start Bit
void startCmd()
{
  StartCode = true;
}


void getCmd()
{
  // Switch on Desired Key Param
  while ( cmdMessenger.available() )
  {
    char buf[350] = { 
      '\0'         };
    cmdMessenger.copyString(buf, 350);
    if(buf[0]) {
      int i = 0;
      if (StartCode == true) {
        i = 0;
      } else {
        i = CodeCount;
      }
      char *str;
      char *p = buf;
      while ((str = strtok_r(p, ",", &p)) != NULL)
        {
          RelayCode[i] = atoi(str);
          Serial.print(str); Serial.print(",");
          i++;
        }
      cmdMessenger.sendCmd(kACK,"CSOK");
    }// END IF(BUF)
  }// END WHILE AVAILABLE
}// END getCmd


void ProcessCode() //int SendCode[], int SCSize
{
  cmdMessenger.sendCmd(kACK,"Processing.");
//  Serial.println(SCSize);
//  Serial.println(SendCode[0]);
//  
  digitalWrite(ActiveReceiver, HIGH);
  boolean isOn = true;
  int i;

  for(i=0;i < 73 - 1;i++) {
    if (isOn == true) {  
      PulsePin(ActiveReceiver, RelayCode[i]);
      isOn = false; 
    }
    else
    {
      delayMicroseconds(RelayCode[i]);
      isOn = true;
    }
  }
}


void PulsePin(int ControlPin, long MicroSeconds){
  // Thanks Lady Ada. You're the best.
  cli();  // Stop Interrupts
  while (MicroSeconds > 0) {
    digitalWrite(ControlPin, HIGH); // ~ 3 microseconds
    delayMicroseconds(10);
    digitalWrite(ControlPin, LOW); // ~ 3 microseconds
    delayMicroseconds(10);
    MicroSeconds -= 26;  // Decrement Pulse Time Accordingly  (26us ~= 38 kHz)
  }
  sei();  // Restore Interrupts
}



void EEPROM_Store(int loc, int value) {
  int a = value/256;
  int b = value % 256;
  EEPROM.write(loc,a);
  EEPROM.write(loc+1,b);
}

int EEPROM_Fetch(int loc) {
  int a=EEPROM.read(loc);
  int b=EEPROM.read(loc+1);
  return (a*256+b);
}

// CmdMessenger Default Callbacks
void attach_callbacks(messengerCallbackFunction* callbacks)
{
  int i = 0;
  int offset = kSEND_CMDS_END;
  while(callbacks[i])
  {
    cmdMessenger.attach(offset+i, callbacks[i]);
    i++;
  }
}
// Cont.
void arduino_ready()
{
  cmdMessenger.sendCmd(kACK,"Arduino ready");
}
// Cont.
void unknownCmd()
{
  cmdMessenger.sendCmd(kERR,"Unknown command");
}
// End Defaults


//// Comcast RNG110 Test Routine - Power On/Off
//void testRoutine(){
//  Serial.print("Starting IR Burst...");
//  int PowerCode[] = {
//    300,780,280,1600,300,640,280,2720,260,1240,240,1240,220,1140,200,2640,220,12680,220,860,220,720,200,740,200,740,200,740,200,2780,220,720,200,740,200,14464,220,860,220,1680,220,720,200,2780,220,1260,220,1280,200,1140,220,2620,220,12680,220,860,220,1820,200,1820,220,720,220,720,200,2780,220,720,220,720,220      };
//  digitalWrite(IRPin1, HIGH);
//  boolean isOn = true;
//  int i;
//  int ItemCount = (sizeof(PowerCode)/sizeof(int)); 
//
//  //Serial.println("Run Loop...");
//
//  for(i=0;i < ItemCount - 1;i++) {
//    if (isOn == true) {
//      PulsePin(IRPin1, PowerCode[i]);
//      isOn = false; 
//      //Serial.print("On: ");
//      //Serial.println(PowerCode[i]);
//    }
//    else
//    {
//      delayMicroseconds(PowerCode[i]);
//      isOn = true;
//      //Serial.print("Off: ");
//      //Serial.println(PowerCode[i]);
//    }
//  }
//  Serial.println("Done.");
//}

