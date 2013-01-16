
/////////////////////////////////////////////////////
//            David C. Dean - 2013.01.15           //
//                www.DavidCDean.com               //
/////////////////////////////////////////////////////
//     __ __             _             _  ___      //
//    |  \  \ ___  ___ _| |_ ___  _ _ | || . \     //
//    |     |<_/ |<_-<  | | / ._>| '_\| ||   /     //
//    |_|_|_|<_+_|/__/  |_| \___.|_|  |_||_\_\     //
//            MasterIRControl Prototype            //
//                                                 //
/////////////////////////////////////////////////////


// Preamble Sequence - Verbatim
int PreambleSeq[] = {320,760,280,1620,280,660,260,2720,260,1240,240,1260,200,1140,220,2640,200,12680,220,860,220};

// Individual Key Codes - Partially Stripped - Obviously this can be condensed a LOT further and then EEPROM stored.
int CodeSeq[12][25] = {
  {720,740,740,740,2780,720,740,14464,860,1680,720,2780,1260,1280,1140,2620,12680,860,1820,1820,720,720,2780,720,720},  // Power
  {1820,720,720,1000,1420,740,740,14484,860,1680,720,2780,1280,1280,1140,2640,12700,880,720,1820,720,980,1400,720,720}, // Enter
  {2760,720,720,720,720,720,720,14484,840,1680,720,2780,1280,1260,1120,2640,12700,860,1680,1820,740,740,740,740,740},   // Zero
  {2620,720,720,720,860,720,720,14484,880,1680,720,2760,1260,1280,1120,2640,12680,860,1540,1820,720,720,860,720,720},   // One
  {2500,740,740,720,1000,720,720,14464,860,1680,720,2760,1280,1260,1120,2640,12680,860,1420,1820,740,720,1000,740,720}, // Two
  {2360,740,740,740,1140,720,720,14464,860,1680,720,2780,1280,1280,1120,2640,12700,880,1280,1820,720,720,1140,720,720}, // Three
  {2220,740,740,720,1280,720,740,14484,860,1680,720,2760,1260,1280,1120,2640,12680,860,1120,1820,720,720,1260,720,720}, // Four
  {2080,720,720,720,1400,720,720,14464,880,1680,720,2760,1280,1280,1120,2640,12680,860,1000,1800,720,720,1420,740,720}, // Five
  {1960,740,740,740,1540,720,720,14464,860,1680,740,2780,1280,1260,1140,2640,12680,860,880,1820,720,720,1540,720,720},  // Six
  {1820,720,720,720,1680,740,720,14484,860,1680,720,2780,1260,1280,1140,2620,12680,860,720,1820,740,740,1680,720,720},  // Seven
  {1680,720,720,720,1820,740,740,14484,860,1680,720,2760,1260,1280,1140,2640,12680,860,2760,1820,720,720,1820,720,720}, // Eight
  {1540,720,720,720,1940,720,720,14464,860,1660,720,2780,1280,1260,1140,2640,12680,860,2640,1820,720,720,1960,740,740}  // Nine
};

// Set our IR Emitter Pins
int IRPin1 = 13;
int IRPin2 = 11;
int IRPin3 = 9;
int IRPin4 = 7;

// Global Variables
int ActiveReceiver;

// Program Setup - IR Pins to OUT, Default Receiver, Set up serial
void setup(){
  pinMode(IRPin1, OUTPUT);
  pinMode(IRPin2, OUTPUT);
  pinMode(IRPin3, OUTPUT);
  pinMode(IRPin4, OUTPUT);
  ActiveReceiver = IRPin1;
  Serial.begin(115200);
}

// Listening for Server Serial Control Commands
void loop()
{
  // Testing - Sends Pwr On/Off every 5s
  //delay(5000);
  //ProcessCode(0);

}

// Pulse IR Emitter On 'ControlPin' for 'MicroSeconds'
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

void ProcessCode(int KeyIndex){

  int PreambleSize = (sizeof(PreambleSeq)/sizeof(int));
  int CodeSize = (sizeof(CodeSeq[KeyIndex])/sizeof(int));
  
  int i;
  
  boolean isOn = true;
  digitalWrite(IRPin1, HIGH);
  
  for(i=0;i < PreambleSize;i++) {
    if (isOn == true) {
      PulsePin(IRPin1, PreambleSeq[i]);
      isOn = false; 
    }
    else
    {
      delayMicroseconds(PreambleSeq[i]);
      isOn = true;
    }
  }
  
   for(i=0;i < CodeSize;i++) {
      delayMicroseconds(CodeSeq[KeyIndex][i]);
      PulsePin(IRPin1, 225);
   }
}

