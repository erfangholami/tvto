int sharp[8];
int leiser = 0;
int leiser1 = 0;
char input[25000] = { 0 };
char inputt[25000] = { 0 };
int i = 0 ;
int j = 0;
byte sendd[12] = { 0 };
bool leiserr;
bool leiserr1;
byte detect[4] = { 0 , 0 , 62 , 0};
byte firstSharp, secSharp;
bool first = false;
bool seconde = false;
bool ir[2];
void setup() 
{
  pinMode(43 , OUTPUT);
  pinMode(45 , OUTPUT);
  pinMode(47 , OUTPUT);
  pinMode(49 , OUTPUT);
  pinMode(51 , OUTPUT);
  pinMode(53 , OUTPUT);
  
  Serial.begin(9600);
  Serial1.begin(19200);
  Serial2.begin(19200);
  Serial3.begin(38400);   

  sendd[0] = (byte)0xff; 
  sendd[1] = (byte)0xff;
  for (int k = 2 ; k < 12 ; k++)
    sendd[k] = 0;
}
void check() {
  byte ans = 0;
  for (int z = 2 ; z < 11; z++)
    ans += sendd[z];
  sendd[11] = (ans & 255);
}
void loop() 
{
  firstSharp = (detect[2] & 7);
  secSharp = detect[2] >> 3;
  if((detect[1] & 0x01) == 1)
  {
     if(!first)
     {
      Serial1.write("D");
      Serial1.write("D");
      Serial1.write("D");
      first = true;
     }
     leiserr = true;
  }
  else
  {
     first = false;
     leiserr = false;
     sendd[6] = 0x00;
     sendd[7] = 0x00;
     for(int l=0; l<20;l++)
     {
      input[l]=0;
     }
     leiser=0;  
  }
  if((detect[1] & 0x02) == 2)
  {
     if(!seconde)
     {
      Serial2.write("D");
      Serial2.write("D");
      Serial2.write("D");
      seconde = true;
     }
     leiserr1 = true;
  }
  else
  {
     seconde = false;
     leiserr1 = false;
     sendd[8] = 0;
     sendd[9] = 0;
     for(int k=0; k < 20;k++)
     {
      inputt[k] = 0;
     }
     leiser1 = 0;  
  }
  if((detect[1] & 0x04) == 4)
    digitalWrite(43 , HIGH);
  else
    digitalWrite(43 , LOW);
  if((detect[1] & 0x08) == 8)
    digitalWrite(45 , HIGH);
  else
    digitalWrite(45 , LOW);
  if((detect[1] & 0x10) == 16)
    digitalWrite(47 , HIGH);
  else
    digitalWrite(47 , LOW);
  if((detect[1] & 0x20) == 32)
    digitalWrite(49 , HIGH);
  else
    digitalWrite(49, LOW);
  if((detect[1] & 0x40) == 64)
    digitalWrite(51 , HIGH);
  else
    digitalWrite(51 , LOW);
  if((detect[1] & 0x80) == 128)
    digitalWrite(53 , HIGH);
  else
    digitalWrite(53 , LOW);
     


  

//  Serial.print(detect & 0x01);
//  Serial.print("     ");
//  Serial.print(detect & 0x02);
//  Serial.print("     ");
//  Serial.print(detect & 0x04);
//  Serial.print("     ");
//  Serial.println(detect & 0x08);
//  if (detect<2){
//    leiserr = false;
//    sendd[2] = 0;
//    sendd[3] = 0;
//  }
  
  sharp[0] = analogRead(A0);
  sharp[1] = analogRead(A1);
  sharp[2] = analogRead(A2);
  sharp[3] = analogRead(A3);
  sharp[4] = analogRead(A4);
  sharp[5] = analogRead(A5);
  sharp[6] = analogRead(A6);
  sharp[7] = analogRead(A7);



  
  if(analogRead(A8) > 400)
    ir[0] = true;
  else
    ir[0] = false;
  if(analogRead(A9) > 400)
    ir[1] = true;
  else
    ir[1] = false;
   

  sendd[0] = 255;
  sendd[1] = 255;
  sendd[10] = 0;
  for(int k = 0; k < 8; k++)
  {
    if(firstSharp == k)
    {
      sendd[2] = sharp[k] & 255;
      sendd[3] = sharp[k] >> 8;
      sendd[10] = sendd[10] | (k << 2);
    }
    if(secSharp == k)
    {
      sendd[4] = sharp[k] & 255;
      sendd[5] = sharp[k] >> 8;
      sendd[10] = sendd[10] | (k << 5);
    }
  }
  
  if(ir[0])
    sendd[10] = sendd[10] | 1;
  else
    sendd[10] = sendd[10] & 254;
  if(ir[1])
    sendd[10] = sendd[10] | 2;
  else
    sendd[10] = sendd[10] & 253;

  //leiserr = true; 
  //Serial1.write("D");
  //Serial.println(sharp[0]);
  //Serial.println(sharp[1]);
  //Serial.println();
  check();
   for (int z = 0 ; z < 12 ; z++){
     Serial3.write(sendd[z]);
   }
  sendd[10] = 0;
  //delay(10);
}
void serialEvent1()
{
   while (Serial1.available() > 0)
   {
    char x = Serial1.read();
    input[i] = x;
    if (x == 68) 
    {
     i = 0;
    }
    
    i++;
   }

  if(input[2] != 'E' && input[10]!=0 && input[11]!=0 && input[12]!=0)
  {
    leiser =  (int)(((input[3] - '0') * 1000) + ((input[5]- '0') * 100) + ((input[6]- '0') * 10) + (input[7]- '0'));
   // Serial.print("         ");
    sendd[6] = leiser & 255;
    sendd[7] = leiser >> 8;
    //check();
  } 
  else if(input[2] == 'E')  {
    sendd[6] = (byte)0xfe;
    sendd[7] = (byte)0xfe;
    //check();
  }
  if (leiserr)
  {
     Serial1.write("D");
     Serial1.write("D");
     Serial1.write("D");
  }
   
}
void serialEvent2()
{
   while (Serial2.available() > 0)
   {
    char x = Serial2.read();
    inputt[j] = x;
    if (x == 68) 
    {
     j = 0;
    }
    
    j++;
   }

  if(inputt[2] != 'E' && inputt[10]!=0 && inputt[11]!=0 && inputt[12]!=0)
  {
    leiser1 =  (int)(((inputt[3] - '0') * 1000) + ((inputt[5]- '0') * 100) + ((inputt[6]- '0') * 10) + (inputt[7]- '0'));
   // Serial.print("         ");
    sendd[8] = leiser1 & 255;
    sendd[9] = leiser1 >> 8;
    //check();
  } 
  else if(inputt[2] == 'E')  {
    sendd[8] = (byte)0xfe;
    sendd[9] = (byte)0xfe;
    //check();
  }
  if (leiserr1)
  {
    Serial2.write("D");
    Serial2.write("D");
    Serial2.write("D");
  }
    
}
void serialEvent3() 
{
  byte readd[4] = { 0 };
  while (Serial3.available() > 0)
  {
    readd[0] = Serial3.read();
    if(readd[0] == 255)
    {
      readd[1] = Serial3.read();
      readd[2] = Serial3.read();
      readd[3] = Serial3.read();
      if(((readd[1] + readd[2]) & 255 ) == readd[3])
      {
        for(int k = 0; k < 4; k++)
        {
          detect[k] = readd[k];
        }
        readd[0] = 0;
      }
    }
    
  }
}

