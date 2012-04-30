
/////////////////////////////////////////////////////
//          David C. Dean - LU 2012.04.29          //
//                www.DavidCDean.com               //
/////////////////////////////////////////////////////
//     __ __             _             _  ___      //
//    |  \  \ ___  ___ _| |_ ___  _ _ | || . \     //
//    |     |<_/ |<_-<  | | / ._>| '_\| ||   /     //
//    |_|_|_|<_+_|/__/  |_| \___.|_|  |_||_\_\     //
//             MasterIRControl v02-alt             //
//                                                 //
/////////////////////////////////////////////////////

//  NOTE: I wanted the web interface to pass the IR
//  codes to the arduino, but sending the large
//  arrays proved... frustrating. Temp solution was
//  to put the codes in a switch under relayCmd().

// EXTERNAL LIBS
// CmdMessenger :: http://arduino.cc/playground/Code/CmdMessenger
// Arduino-Base64 :: https://github.com/adamvr/arduino-base64
// Streaming5 :: http://arduiniana.org/libraries/streaming/

#include <CmdMessenger.h>
#include <Base64.h>
#include <Streaming.h>


// CmdMessenger Setup
char field_separator = ',';
char command_separator = ';';
long timeoutInterval = 1000;
long previousMillis = 0;
int counter = 0;
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
  relayCmd,  // PC-to-Ard: 4,x;
  NULL
};


// Set our IR Emitter Pins
int IRPin1 = 13;
int IRPin2 = 11;
int IRPin3 = 9;
int IRPin4 = 7;


// Program Setup - IR Pins to OUT, Set up serial
void setup(){
  pinMode(IRPin1, OUTPUT);
  pinMode(IRPin2, OUTPUT);
  pinMode(IRPin3, OUTPUT);
  pinMode(IRPin4, OUTPUT);

  Serial.begin(9600);

  // Attach default and user callbacks
  cmdMessenger.attach(kARDUINO_READY, arduino_ready);
  cmdMessenger.attach(unknownCmd);
  attach_callbacks(messengerCallbacks);
}

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


// Listening for Server Serial Control Commands
void loop()
{
  // Handle Incoming Serial Commands
  cmdMessenger.feedinSerialData();

  // Codeset Test - Comcast RNG110
  // testRoutine();
  delay(500);
}


void relayCmd()
{
// Switch on Desired Key Param
  while ( cmdMessenger.available() )
  {
    //cmdMessenger.sendCmd(kACK,"Test Received.");
    char buf[350] = { 
      '\0'     };
    cmdMessenger.copyString(buf, 350);
    if(buf[0]) {
      //cmdMessenger.sendCmd(kACK, buf);
      int SwitchInt = atoi(&buf[0]);
      switch(SwitchInt){
        case 0: {
          // NUM 0
          int RelayCode[] = {
          320,760,280,1620,260,680,260,2720,260,1240,240,1240,220,1140,200,2640,220,12680,220,860,220,2760,220,720,220,720,220,720,220,720,220,720,220,720,200,14484,220,840,220,1680,220,720,220,2780,200,1280,220,1260,220,1120,220,2640,200,12700,200,860,220,1680,220,1820,200,740,200,740,200,740,200,740,200,740,200  };
          ProcessCode(RelayCode, (sizeof(RelayCode)/sizeof(int)));
          break;
        }
        case 1:{
          // NUM 1
          int RelayCode[] = {
          320,760,280,1620,280,660,280,2700,260,1240,240,1240,240,1120,220,2620,220,12680,220,860,220,2620,220,720,220,720,220,720,220,860,220,720,220,720,200,14484,200,880,200,1680,220,720,220,2760,220,1260,220,1280,220,1120,220,2640,200,12680,220,860,220,1540,220,1820,200,720,220,720,220,860,220,720,220,720,220  };
          ProcessCode(RelayCode, (sizeof(RelayCode)/sizeof(int)));
          break;
        }
        case 2:{
          // NUM 2
          int RelayCode[] = {
          320,760,280,1620,280,660,260,2720,260,1240,240,1260,200,1140,220,2640,200,12680,220,860,220,2500,200,740,200,740,200,720,220,1000,220,720,220,720,220,14464,220,860,200,1680,220,720,220,2760,220,1280,220,1260,220,1120,220,2640,200,12680,220,860,220,1420,200,1820,200,740,220,720,220,1000,200,740,200,720,220  };
          ProcessCode(RelayCode, (sizeof(RelayCode)/sizeof(int)));
          break;
        }
        case 3:{
          // NUM 3
          int RelayCode[] = {
          320,760,300,1600,280,660,260,2740,240,1260,220,1260,220,1120,220,2620,220,12680,220,860,220,2360,200,740,200,740,200,740,200,1140,220,720,220,720,220,14464,200,860,220,1680,220,720,220,2780,200,1280,200,1280,220,1120,220,2640,200,12700,200,880,200,1280,200,1820,220,720,220,720,220,1140,200,720,220,720,220  };
          ProcessCode(RelayCode, (sizeof(RelayCode)/sizeof(int)));
          break;
        }
        case 4:{
          // NUM 4
          int RelayCode[] = {
          300,780,280,1600,300,640,280,2720,260,1240,240,1240,220,1140,220,2620,220,12680,220,860,220,2220,200,740,200,740,220,720,200,1280,220,720,200,740,200,14484,200,860,220,1680,220,720,220,2760,220,1260,220,1280,220,1120,220,2640,200,12680,220,860,220,1120,220,1820,220,720,220,720,220,1260,220,720,220,720,220  };
          ProcessCode(RelayCode, (sizeof(RelayCode)/sizeof(int)));
          break;
        }
        case 5:{
          // NUM 5
          int RelayCode[] = {
          280,800,280,1620,280,660,260,2720,260,1240,240,1240,220,1140,220,2620,220,12680,220,860,220,2080,220,720,220,720,220,720,220,1400,220,720,220,720,220,14464,200,880,200,1680,220,720,220,2760,220,1280,200,1280,220,1120,220,2640,200,12680,220,860,220,1000,220,1800,220,720,220,720,220,1420,200,740,200,720,220  };
          ProcessCode(RelayCode, (sizeof(RelayCode)/sizeof(int)));
          break;
        }
        case 6:{
          // NUM 6
          int RelayCode[] = {
          300,780,300,1600,280,660,260,2720,260,1240,240,1260,220,1120,220,2640,200,12700,200,860,220,1960,200,740,200,740,200,740,200,1540,220,720,220,720,220,14464,220,860,220,1680,200,740,200,2780,200,1280,220,1260,220,1140,200,2640,220,12680,220,860,200,880,200,1820,220,720,220,720,220,1540,220,720,220,720,200  };
          ProcessCode(RelayCode, (sizeof(RelayCode)/sizeof(int)));
          break;
        }
        case 7:{
          // NUM 7
          int RelayCode[] = {
          300,780,280,1600,300,640,300,2680,280,1240,240,1240,220,1140,200,2640,220,12680,220,860,200,1820,220,720,220,720,220,720,220,1680,200,740,200,720,220,14484,200,860,220,1680,220,720,200,2780,220,1260,220,1280,200,1140,220,2620,220,12680,220,860,220,720,220,1820,200,740,200,740,200,1680,220,720,220,720,220  };
          ProcessCode(RelayCode, (sizeof(RelayCode)/sizeof(int)));
          break;
        }
        case 8:{
          // NUM 8
          int RelayCode[] = {
          280,780,300,1600,300,640,260,2720,280,1220,260,1240,220,1140,200,2640,220,12680,200,860,220,1680,220,720,220,720,220,720,220,1820,200,740,200,740,200,14484,200,860,220,1680,220,720,220,2760,220,1260,220,1280,200,1140,220,2640,200,12680,220,860,220,2760,220,1820,220,720,220,720,200,1820,220,720,220,720,220  };
          ProcessCode(RelayCode, (sizeof(RelayCode)/sizeof(int)));
          break;
        }
        case 9:{
          // NUM 9
          int RelayCode[] = {
          320,760,280,1620,280,660,280,2720,240,1240,240,1260,200,1140,220,2620,220,12700,200,860,220,1540,220,720,220,720,220,720,220,1940,220,720,220,720,220,14464,220,860,220,1660,220,720,220,2780,200,1280,220,1260,220,1140,200,2640,220,12680,220,860,200,2640,220,1820,200,720,220,720,220,1960,200,740,200,740,200  };
          ProcessCode(RelayCode, (sizeof(RelayCode)/sizeof(int)));
          break;
        }
        case 10:{
          // POWER
          int RelayCode[] = {
          300,780,280,1600,300,640,280,2720,260,1240,240,1240,220,1140,200,2640,220,12680,220,860,220,720,200,740,200,740,200,740,200,2780,220,720,200,740,200,14464,220,860,220,1680,220,720,200,2780,220,1260,220,1280,200,1140,220,2620,220,12680,220,860,220,1820,200,1820,220,720,220,720,200,2780,220,720,220,720,220  };
          ProcessCode(RelayCode, (sizeof(RelayCode)/sizeof(int)));
          break;
        }
        case 11:{
          // ENTER
          int RelayCode[] = {
          320,760,280,1600,280,660,280,2740,240,1240,240,1240,220,1140,220,2620,220,12680,220,860,200,1820,220,720,220,720,220,1000,200,1420,200,740,200,740,200,14484,220,860,200,1680,220,720,220,2780,200,1280,200,1280,220,1140,200,2640,200,12700,200,880,200,720,220,1820,220,720,220,980,220,1400,220,720,220,720,220  };
          ProcessCode(RelayCode, (sizeof(RelayCode)/sizeof(int)));
          break;
        }
        case 12:{
          // EXIT
          int RelayCode[] = {
            };
          ProcessCode(RelayCode, (sizeof(RelayCode)/sizeof(int)));
          break;
        }
        case 13:{
          // LAST
          int RelayCode[] = {
            };
          ProcessCode(RelayCode, (sizeof(RelayCode)/sizeof(int)));
          break;
        }
        case 14:{
          // GUIDE
          int RelayCode[] = {
            };
          ProcessCode(RelayCode, (sizeof(RelayCode)/sizeof(int)));
          break;
        }
      }
  }
}
}


void ProcessCode(int SendCode[], int SCSize)
{
  digitalWrite(IRPin1, HIGH);
  boolean isOn = true;
  int i;

  for(i=0;i < SCSize - 1;i++) {
    if (isOn == true) {  
      PulsePin(IRPin1, SendCode[i]);
      isOn = false; 
    }
    else
    {
      delayMicroseconds(SendCode[i]);
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


// CmdMessenger Default Callbacks
void arduino_ready()
{
  cmdMessenger.sendCmd(kACK,"Arduino ready");
}
void unknownCmd()
{
  cmdMessenger.sendCmd(kERR,"Unknown command");
}
// End Defaults


//// Comcast RNG110 Test Routine - Power On/Off
//void testRoutine(){
//  Serial.println("Starting IR Burst...");
//  int PowerCode[] = {
//    300,780,280,1600,300,640,280,2720,260,1240,240,1240,220,1140,200,2640,220,12680,220,860,220,720,200,740,200,740,200,740,200,2780,220,720,200,740,200,14464,220,860,220,1680,220,720,200,2780,220,1260,220,1280,200,1140,220,2620,220,12680,220,860,220,1820,200,1820,220,720,220,720,200,2780,220,720,220,720,220    };
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
//  Serial.println("Starting IR Burst...");
//}

