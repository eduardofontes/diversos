int latchPin = 8;
int dataPin = 11;
int clockPin = 12;

#define ROW_ALL_ON 255
#define ROW_ALL_OFF 0
#define COL_ALL_ON 0
#define COL_ALL_OFF 255

byte D[8][8] = { \
  {1,1,1,1,1,0,0,0}, \
  {1,1,1,1,1,1,0,0}, \
  {1,1,0,0,1,1,0,0}, \
  {1,1,0,0,1,1,0,0}, \
  {1,1,0,0,1,1,0,0}, \
  {1,1,0,0,1,1,0,0}, \
  {1,1,1,1,1,1,0,0}, \
  {1,1,1,1,1,0,0,0}  \
};
int linha[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };
int coluna[8] = { 254, 253, 251, 247, 239, 223, 191, 127 };

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  reset();
  // Debug
//  Serial.begin(9600);
}

void reset() {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, ROW_ALL_OFF); // linhas
  shiftOut(dataPin, clockPin, MSBFIRST, COL_ALL_OFF); // colunas
  digitalWrite(latchPin, HIGH);
}

void show() {
  for (int l = 0; l < 8; l++) {
    int b = 255;
    for (int c = 0; c < 8; c++) {
      if (D[l][c]) {
        b = b & coluna[c];
      }
    }
    // Debug
//    Serial.println(linha[l]);
//    Serial.println(b);
    // Fim debug
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, linha[l]); // linhas
    shiftOut(dataPin, clockPin, MSBFIRST, b); // colunas
    digitalWrite(latchPin, HIGH);  
  }
}

void loop() {
  show();
}
