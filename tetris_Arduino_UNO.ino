// DRONE PROTON TETRİS GAME CODES

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define WIDTH 64
#define HEIGHT 128

Adafruit_SSD1306 display(128, 64, &Wire, -1);

static const unsigned char PROGMEM logo [] = {
   0x00, 0x00, 0x18, 0x06, 0x01, 0xc0, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff  };
 

  const char pieces_S_l[2][2][4] = {{
                                      {0, 0, 1, 1}, {0, 1, 1, 2}
                                    },
                                    {
                                      {0, 1, 1, 2}, {1, 1, 0, 0}
                                    }};

 const char pieces_S_r[2][2][4]{{
                                    {1, 1, 0, 0}, {0, 1, 1, 2}
                                  },
                                  {
                                    {0, 1, 1, 2}, {0, 0, 1, 1}
                                  }};

 const char pieces_L_l[4][2][4] = {{
                                    {0, 0, 0, 1}, {0, 1, 2, 2}
                                  },
                                  {
                                    {0, 1, 2, 2}, {1, 1, 1, 0}
                                  },
                                  {
                                    {0, 1, 1, 1}, {0, 0, 1, 2}
                                  },
                                  {
                                    {0, 0, 1, 2}, {1, 0, 0, 0}
                                  }};

const char pieces_Sq[1][2][4] = {{
                                    {0, 1, 0, 1}, {0, 0, 1, 1}
                                  }};

 const char pieces_T[4][2][4] = {{
                                    {0, 0, 1, 0},{0, 1, 1, 2}
                                  },
                                  {
                                    {0, 1, 1, 2},{1, 0, 1, 1}
                                  },
                                  {
                                    {1, 0, 1, 1},{0, 1, 1, 2}
                                  },
                                  {
                                    {0, 1, 1, 2},{0, 0, 1, 0}
                                  }};

const char pieces_l[2][2][4] = {{
                                    {0, 1, 2, 3}, {0, 0, 0, 0}
                                  },
                                  {
                                    {0, 0, 0, 0}, {0, 1, 2, 3}
                                  }};

 const short MARGIN_TOP = 19; //- declares a constant variable MARGIN_TOP with a value of 19 of type short.
const short MARGIN_LEFT = 3;// - declares a constant variable MARGIN_LEFT with a value of 3 of type short.
const short SIZE = 5;// - declares a constant variable SIZE with a value of 5 of type short.
const short TYPES = 6; //- declares a constant variable TYPES with a value of 6 of type short.
#define SPEAKER_PIN 3 //- creates a macro with the name SPEAKER_PIN and a value of 3. This allows the code to refer to SPEAKER_PIN throughout the rest of the code and substitute it with the value 3.
const int MELODY_LENGTH = 10; //- declares a constant variable MELODY_LENGTH with a value of 10 of type int.
const int MELODY_NOTES[MELODY_LENGTH] = {262, 294, 330, 262}; //- declares an array MELODY_NOTES of size MELODY_LENGTH (which is 10), and initializes it with four integer values.
const int MELODY_DURATIONS[MELODY_LENGTH] = {500, 500, 500, 500}; //- declares an array MELODY_DURATIONS of size MELODY_LENGTH (which is 10), and initializes it with four integer values.
int click[] = { 1047 }; //- declares an array click of size 1, and initializes it with one integer value.
int click_duration[] = { 100 }; //- declares an array click_duration of size 1, and initializes it with one integer value.
int erase[] = { 2093 }; //- declares an array erase of size 1, and initializes it with one integer value.
int erase_duration[] = { 100 }; //- declares an array erase_duration of size 1, and initializes it with one integer value.
word currentType, nextType, rotation; //- declares three variables of type word, named currentType, nextType, and rotation.
short pieceX, pieceY; //- declares two variables of type short, named pieceX and pieceY.
short piece[2][4]; //- declares a two-dimensional array piece of size 2x4 with elements of type short.
int interval = 20, score; //- declares two variables, interval and score, of type int. interval is initialized with a value of 20.
long timer, delayer; //- declares two variables, timer and delayer, of type long.
boolean grid[10][18]; //- declares a two-dimensional array grid of size 10x18 with elements of type boolean.
boolean b1, b2, b3; //- declares three variables of type boolean, named b1, b2, and b3.
  int left=11;
  int right=9;
  int change=12;
  int speed=10;

  void checkLines(){
    boolean full;
    for(short y = 17; y >= 0; y--){
      full = true;
      for(short x = 0; x < 10; x++){
        full = full && grid[x][y];
      }
      if(full){
        breakLine(y);
        y++;
      }
    }
  }
  void breakLine(short line){
      tone(SPEAKER_PIN, erase[0], 1000 / erase_duration[0]); 
      delay(100);
      noTone(SPEAKER_PIN);
    for(short y = line; y >= 0; y--){
      for(short x = 0; x < 10; x++){
        grid[x][y] = grid[x][y-1];
      }
    }
    for(short x = 0; x < 10; x++){
      grid[x][0] = 0;
    }
    display.invertDisplay(true);
    delay(50);
    display.invertDisplay(false);
    score += 10;
  }

  void refresh(){
      display.clearDisplay();
      drawLayout();
      drawGrid();
      drawPiece(currentType, 0, pieceX, pieceY);
      display.display();
  }
  void drawGrid(){
    for(short x = 0; x < 10; x++)
      for(short y = 0; y < 18; y++)
        if(grid[x][y])
          display.fillRect(MARGIN_LEFT + (SIZE + 1)*x, MARGIN_TOP + (SIZE + 1)*y, SIZE, SIZE, WHITE);
  }
  boolean nextHorizontalCollision(short piece[2][4], int amount){
    for(short i = 0; i < 4; i++){
      short newX = pieceX + piece[0][i] + amount;
      if(newX > 9 || newX < 0 || grid[newX][pieceY + piece[1][i]])
        return true;
    }
    return false;
  }
  boolean nextCollision(){
    for(short i = 0; i < 4; i++){
      short y = pieceY + piece[1][i] + 1;
      short x = pieceX + piece[0][i];
      if(y > 17 || grid[x][y])
        return true;
    }
    return false;
  }

  void generate(){
    currentType = nextType;
    nextType = random(TYPES);
    if(currentType != 5)
      pieceX = random(9);
    else
      pieceX = random(7);
    pieceY = 0;
    rotation = 0;
    copyPiece(piece, currentType, rotation);
  }
  void drawPiece(short type, short rotation, short x, short y){
    for(short i = 0; i < 4; i++)
      display.fillRect(MARGIN_LEFT + (SIZE + 1)*(x + piece[0][i]), MARGIN_TOP + (SIZE + 1)*(y + piece[1][i]), SIZE, SIZE, WHITE);
  }
  void drawNextPiece(){
    short nPiece[2][4];
    copyPiece(nPiece, nextType, 0);
    for(short i = 0; i < 4; i++)
      display.fillRect(50 + 3*nPiece[0][i], 4 + 3*nPiece[1][i], 2, 2, WHITE);
  }

   void copyPiece(short piece[2][4], short type, short rotation){
    switch(type){
    case 0: //L_l
      for(short i = 0; i < 4; i++){
        piece[0][i] = pieces_L_l[rotation][0][i];
        piece[1][i] = pieces_L_l[rotation][1][i];
      }
      break;
    case 1: //S_l
      for(short i = 0; i < 4; i++){
        piece[0][i] = pieces_S_l[rotation][0][i];
        piece[1][i] = pieces_S_l[rotation][1][i];
      }
      break;
    case 2: //S_r
      for(short i = 0; i < 4; i++){
        piece[0][i] = pieces_S_r[rotation][0][i];
        piece[1][i] = pieces_S_r[rotation][1][i];
      }
      break;
    case 3: //Sq
      for(short i = 0; i < 4; i++){
        piece[0][i] = pieces_Sq[0][0][i];
        piece[1][i] = pieces_Sq[0][1][i];
      }
      break;
      case 4: //T
      for(short i = 0; i < 4; i++){
        piece[0][i] = pieces_T[rotation][0][i];
        piece[1][i] = pieces_T[rotation][1][i];
      }
      break;
      case 5: //l
      for(short i = 0; i < 4; i++){
        piece[0][i] = pieces_l[rotation][0][i];
        piece[1][i] = pieces_l[rotation][1][i];
      }
      break;
    }
  }
  short getMaxRotation(short type){
    if(type == 1 || type == 2 || type == 5)
      return 2;
    else if(type == 0 || type == 4)
      return 4;
    else if(type == 3)
      return 1;
    else
      return 0;
  }
  boolean canRotate(short rotation){
    short piece[2][4];
    copyPiece(piece, currentType, rotation);
    return !nextHorizontalCollision(piece, 0);
  }


   void drawLayout(){
    display.drawLine(0, 15, WIDTH, 15, WHITE);
    display.drawRect(0, 0, WIDTH, HEIGHT, WHITE);
    drawNextPiece();
    char text[6];
    itoa(score, text, 10);
    drawText(text, getNumberLength(score), 7, 4);
  }
  short getNumberLength(int n){
    short counter = 1;
    while(n >= 10){
      n /= 10;
      counter++;
    }
    return counter;
  }
  void drawText(char text[], short length, int x, int y){
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(WHITE); // Draw white text
    display.setCursor(x, y);     // Start at top-left corner
    display.cp437(true);         // Use full 256 char 'Code Page 437' font
    for(short i = 0; i < length; i++)
      display.write(text[i]);
  }

void setup() {
    pinMode(left, INPUT_PULLUP);
    pinMode(right, INPUT_PULLUP);
    pinMode(change, INPUT_PULLUP);
    pinMode(speed, INPUT_PULLUP);
    pinMode(SPEAKER_PIN, OUTPUT);
    Serial.begin(9600);
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
    }
    display.setRotation(1);
    display.clearDisplay();
    display.drawBitmap(3, 23, logo, 64, 82,  WHITE);
    display.display();
    delay(2000);
    display.clearDisplay();
    drawLayout();
    display.display();
    randomSeed(analogRead(0));
    nextType = random(TYPES);
    generate();
    timer = millis();
  }


   void loop() {
   if(millis() - timer > interval){
      checkLines();
      refresh();
      if(nextCollision()){
        for(short i = 0; i < 4; i++)
          grid[pieceX + piece[0][i]][pieceY + piece[1][i]] = 1;
        generate();
      }else
        pieceY++;
      timer = millis();
    }
    if(!digitalRead(left)){
      tone(SPEAKER_PIN, click[0], 1000 / click_duration[0]);
      delay(100);
      noTone(SPEAKER_PIN);
      if(b1){
        if(!nextHorizontalCollision(piece, -1)){
          pieceX--;
          refresh();
        }
        b1 = false;
      }
    }else{
      b1 = true;
    }
    if(!digitalRead(right)){
      tone(SPEAKER_PIN, click[0], 1000 / click_duration[0]);
      delay(100);
      noTone(SPEAKER_PIN);
      if(b2){
        if(!nextHorizontalCollision(piece, 1)){
          pieceX++;
          refresh();
        }
        b2 = false;
      }
    }else{
      b2 = true;
    }
    if(!digitalRead(speed)){
      interval = 20;
    } else{
      interval = 400;
      }
    if(!digitalRead(change)){
      tone(SPEAKER_PIN, click[0], 1000 / click_duration[0]);
      delay(100);
      noTone(SPEAKER_PIN);
      if(b3){
        if(rotation == getMaxRotation(currentType) - 1 && canRotate(0)){
          rotation = 0;
        }else if(canRotate(rotation + 1)){
          rotation++;
        }  
        copyPiece(piece, currentType, rotation);
        refresh();
        b3 = false;
        delayer = millis();
      }
    }else if(millis() - delayer > 50){
      b3 = true;
    }
  }