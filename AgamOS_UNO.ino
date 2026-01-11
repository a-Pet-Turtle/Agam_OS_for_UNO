#include <LiquidCrystal.h>
#include <EEPROM.h>

/* Python utility to make custom charachters
import tkinter as tk

root = tk.Tk()
root.title("Character Builder for Arduino C++")

# 8 rows of 5 checkboxes
checkboxes = []
for row in range(8):
    row_checks = []
    frame = tk.Frame(root)
    frame.pack()
    for col in range(5):
        var = tk.IntVar()
        cb = tk.Checkbutton(frame, variable=var)
        cb.pack(side=tk.LEFT)
        row_checks.append(var)
    checkboxes.append(row_checks)

def build_char():
    lines = []
    for row in checkboxes:
        binary = ''.join(str(var.get()) for var in row)
        lines.append(f"  0b{binary}")
    
    output = "byte customChar[8] = {\n" + ",\n".join(lines) + "\n};"
    print(output)
    result_text.delete(1.0, tk.END)
    result_text.insert(1.0, output)

build_button = tk.Button(root, text="Build Character", command=build_char)
build_button.pack()

result_text = tk.Text(root, height=10, width=30, font=("Courier", 10))
result_text.pack()

root.mainloop()
*/

// Useful LCD commands:
// lcd.begin(16, 2);           // Initialize LCD size
// lcd.clear();                // Clear the display
// lcd.home();                 // Move cursor to position (0,0)
// lcd.setCursor(col, row);    // Move cursor to specific position
// lcd.print("text");          // Display text or numbers
// lcd.cursor();               // Show underline cursor
// lcd.noCursor();             // Hide cursor
// lcd.blink();                // Blinking block cursor
// lcd.noBlink();              // Stop blinking
// lcd.display();              // Turn display on
// lcd.noDisplay();            // Turn display off (keeps text in memory)
// lcd.scrollDisplayLeft();    // Scroll text left
// lcd.scrollDisplayRight();   // Scroll text right

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int key1 = 8; const int key2 = 9; const int key3 = 10;
const int speaker = 6;
String user = "";
int pass[3];

// Setup the custom charachters for the logo
byte logo1[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00001,
  0b00001,
  0b00011
};

byte logo2[8] = {
  0b00000,
  0b00000,
  0b00111,
  0b01000,
  0b10111,
  0b00000,
  0b00000,
  0b11100
};

byte logo3[8] = {
  0b00000,
  0b00000,
  0b11000,
  0b00100,
  0b11011,
  0b00001,
  0b00001,
  0b01111
};

byte logo4[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b11000,
  0b00100,
  0b00100,
  0b00100,
  0b00100
};

byte logo5[8] = {
  0b00101,
  0b01001,
  0b01000,
  0b00100,
  0b00011,
  0b00000,
  0b00000,
  0b00000
};

byte logo6[8] = {
  0b00011,
  0b00000,
  0b10000,
  0b01000,
  0b11111,
  0b00111,
  0b00000,
  0b00000
};

byte logo7[8] = {
  0b10000,
  0b00001,
  0b00011,
  0b00110,
  0b11100,
  0b11000,
  0b00000,
  0b00000
};

byte logo8[8] = {
  0b01000,
  0b10000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte moleChar[8] = {
  0b01110,
  0b10001,
  0b11011,
  0b10001,
  0b11011,
  0b10101,
  0b10001,
  0b10001
};

byte moleHoleChar[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b01110,
  0b10001,
  0b10001,
  0b10001,
  0b01110
};

byte degreeSymbol[8] = {
  0b00000,
  0b01110,
  0b01010,
  0b01110,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

void setup() {
  pinMode(key1, INPUT);
  pinMode(key2, INPUT);
  pinMode(key3, INPUT);
  lcd.begin(16, 2);
  run();
}

void run() {
  lcd.createChar(0, logo1);
  lcd.createChar(1, logo2);
  lcd.createChar(2, logo3);
  lcd.createChar(3, logo4);
  lcd.createChar(4, logo5);
  lcd.createChar(5, logo6);
  lcd.createChar(6, logo7);
  lcd.createChar(7, logo8);
  logo();
  lcd.setCursor(2, 0);
  lcd.print("Ag@m OS ");
  lcd.write(byte(0));
  lcd.write(byte(1));
  lcd.write(byte(2));
  lcd.write(byte(3));
  lcd.setCursor(2, 1);
  lcd.print("For UNO ");
  lcd.write(byte(4));
  lcd.write(byte(5));
  lcd.write(byte(6));
  lcd.write(byte(7));
  tone(speaker, 220, 30);
  delay(50);
  tone(speaker, 230, 30);
  delay(50);
  tone(speaker, 350, 50);

  // Prompt for user
  delay(4000);
  lcd.clear();
  lcd.home();
  lcd.print("Who is using");
  lcd.setCursor(0, 1);
  lcd.print("Ag@m today?");
  delay(2000);
  lcd.home();
  lcd.clear();
  lcd.print(" Sai  Phin Guest");
  lcd.setCursor(0, 1);
  lcd.print(" [1]  [2]   [3]");
  waitKeypress();
  if (digitalRead(key1)==HIGH) {
    user = "Sai";
    pass[0] = 1;   // Set Sai's 3-digit password here
    pass[1] = 3;
    pass[2] = 2;
  } else if (digitalRead(key2)==HIGH) {
    user = "Phin";
    pass[0] = 3;   // Set Phin's 3-digit password here
    pass[1] = 3;
    pass[2] = 1;
  } else if (digitalRead(key3)==HIGH) {
    user = "Guest";
    // int pass[] = {1, 2, 3} // The guest has no password.
  }

  // Prompt for password
  bool accessDenied = true;
  if (user == "Guest") {
    accessDenied = false; // Set this to true to add a guest password, and set the password above.
  }
  waitNoKeypress();
  while (accessDenied) {
    lcd.clear();
    lcd.print("PASSWORD:");
    lcd.setCursor(0, 1);
    int entered[3];
    for (int c=0; c<3; c++) {
      waitKeypress();
      if (digitalRead(key1)==HIGH) {
        entered[c] = 1;
        waitNoKeypress();
      } else if (digitalRead(key2)==HIGH) {
        entered[c] = 2;
        waitNoKeypress();
      } else if (digitalRead(key3)==HIGH) {
        entered[c] = 3;
        waitNoKeypress();
      }
      lcd.print("*");
      delay(500);
    }
    bool check = true;
    for (int i=0; i<3; i++) {
      if (entered[i] != pass[i]) {
        check = false;
      }
    }
    if (check) {
      accessDenied = false;
    } else {
      lcd.clear();
      lcd.home();
      lcd.print("Access Denied");
      delay(1000);
    }
  }
  lcd.clear();
  lcd.home();
  lcd.print("Access Granted.");
  lcd.setCursor(0, 1);
  lcd.print("Welcome, ");
  lcd.print(user);
  delay(1000);

  // The fun part... HEHEHEHEE
  String currentApp = "Session Info";
  while (true) {    // User variables and passwords are declared in this scope, so we need to keep execution limited to run()
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("[prev] [] [next]");
    tone(speaker, 300, 20);
    if (currentApp == "Session Info") {
      waitNoKeypress();
      lcd.home();
      lcd.print("Session Info");
      waitKeypress();
      if (digitalRead(key1) == HIGH) {
        // goto prev app
      } else if (digitalRead(key2) == HIGH) {
        sessionInfo();
      } else if (digitalRead(key3) == HIGH) {
        currentApp = "Analog Reader";
      }
      delay(10);
    } else if (currentApp == "Analog Reader") {
      waitNoKeypress();
      lcd.home();
      lcd.print("Analog Reader");
      waitKeypress();
      if (digitalRead(key1) == HIGH) {
        currentApp = "Session Info";
      } else if (digitalRead(key2) == HIGH) {
        analogReader();
      } else if (digitalRead(key3) == HIGH) {
        currentApp = "Number Generator";
      }
      delay(10);
    } else if (currentApp == "Number Generator") {
      waitNoKeypress();
      lcd.home();
      lcd.print("Number Generator");
      waitKeypress();
      if (digitalRead(key1) == HIGH) {
        currentApp = "Analog Reader";
      } else if (digitalRead(key2) == HIGH) {
        rNum();
      } else if (digitalRead(key3) == HIGH) {
        currentApp = "Wak A Mole";
      }
      delay(10);
    } else if (currentApp == "Wak A Mole") {
      waitNoKeypress();
      lcd.home();
      lcd.print("Wak A Mole");
      waitKeypress();
      if (digitalRead(key1) == HIGH) {
        currentApp = "Number Generator";
      } else if (digitalRead(key2) == HIGH) {
        wakaMole();
      } else if (digitalRead(key3) == HIGH) {
        currentApp = "Piano";
      }
      delay(10);
    } else if (currentApp == "Piano") {
      waitNoKeypress();
      lcd.home();
      lcd.print("Piano");
      waitKeypress();
      if (digitalRead(key1) == HIGH) {
        currentApp = "Wak A Mole";
      } else if (digitalRead(key2) == HIGH) {
        piano();
      } else if (digitalRead(key3) == HIGH) {
        currentApp = "Calculator";
      }
      delay(10);
    } else if (currentApp == "Calculator") {
      waitNoKeypress();
      lcd.home();
      lcd.print("Calculator");
      waitKeypress();
      if (digitalRead(key1) == HIGH) {
        currentApp = "Piano";
      } else if (digitalRead(key2) == HIGH) {
        calculator();
      } else if (digitalRead(key3) == HIGH) {
        currentApp = "Tetris Theme";
      }
      delay(10);
    } else if (currentApp == "Tetris Theme") {
      waitNoKeypress();
      lcd.home();
      lcd.print("Tetris Theme");
      waitKeypress();
      if (digitalRead(key1) == HIGH) {
        currentApp = "Calculator";
      } else if (digitalRead(key2) == HIGH) {
        tetrisTheme();
      } else if (digitalRead(key3) == HIGH) {
        currentApp = "System";
      }
      delay(10);
    } else if (currentApp == "System") {
      waitNoKeypress();
      lcd.home();
      lcd.print("System");
      waitKeypress();
      if (digitalRead(key1) == HIGH) {
        currentApp = "Tetris Theme";
      } else if (digitalRead(key2) == HIGH) {
        system();
      } else if (digitalRead(key3) == HIGH) {
        //
      }
      delay(10);
    }
  }
}

void tetrisTheme() {
  waitNoKeypress();
  lcd.clear();
  lcd.home();
  lcd.print("- Tetris Theme -");
  lcd.setCursor(0, 1);
  lcd.print("  [any] = stop");
  
  // Note durations (in ms)
  int quarter = 400;
  int eighth = 200;
  int half = 800;
  
  // ===== Line 1 =====
  // E B C D C B A A C E D C B C D E C A A
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 659, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 494, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 523, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 587, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 523, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 494, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 440, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 440, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 523, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 659, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 587, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 523, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 494, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 523, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 587, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 659, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 523, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 440, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 440, quarter); delay(quarter + 50); noTone(speaker);
  
  // ===== Line 2 =====
  // D F A G F E C E D C B B C D E C A A
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 587, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 698, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 880, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 784, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 698, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 659, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 523, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 659, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 587, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 523, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 494, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 494, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 523, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 587, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 659, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 523, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 440, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 440, quarter); delay(quarter + 50); noTone(speaker);
  
  // ===== Line 3 (repeat of line 1) =====
  // E B C D C B A A C E D C B C D E C A A
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 659, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 494, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 523, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 587, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 523, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 494, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 440, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 440, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 523, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 659, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 587, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 523, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 494, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 523, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 587, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 659, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 523, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 440, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 440, quarter); delay(quarter + 50); noTone(speaker);
  
  // ===== Line 4 (repeat of line 2) =====
  // D F A G F E C E D C B B C D E C A A
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 587, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 698, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 880, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 784, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 698, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 659, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 523, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 659, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 587, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 523, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 494, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 494, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 523, eighth); delay(eighth + 25); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 587, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 659, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 523, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 440, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 440, quarter); delay(quarter + 50); noTone(speaker);
  
  // ===== Line 5 (Bridge) =====
  // E C D B C A Ab B E C D B C E A Ab
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 659, half); delay(half + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 523, half); delay(half + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 587, half); delay(half + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 494, half); delay(half + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 523, half); delay(half + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 440, half); delay(half + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 415, half); delay(half + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 494, half); delay(half + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 659, half); delay(half + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 523, half); delay(half + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 587, half); delay(half + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 494, half); delay(half + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 523, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 659, quarter); delay(quarter + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 880, half); delay(half + 50); noTone(speaker);
  
  if (keyPress()) { waitNoKeypress(); return; }
  tone(speaker, 830, half); delay(half + 50); noTone(speaker);
  
  waitNoKeypress();
}

void calculator() {
  waitNoKeypress();
  lcd.clear();
  lcd.home();
  String operators[4] = {"+", "-", "*", "/"};
  int num1 = 1;
  String sign = "+"; // +
  int num2 = 1;
  int ans = 0;
  while (true) {
    lcd.clear();
    lcd.home();
    lcd.print(num1);
    lcd.print(sign);
    lcd.print(num2);
    lcd.print("=");
    if (sign=="+") {
      ans = num1 + num2;
    } else if (sign=="-") {
      ans = num1 - num2;
    } else if (sign=="*") {
      ans = num1 * num2;
    } else if (sign=="/") {
      ans = num1 / num2;
    }
    lcd.print(ans);
    lcd.setCursor(0, 1);
    lcd.print("[+-1] [-op+] [+-2]");
    waitKeypress();
    delay(20);
    if (digitalRead(key1)==HIGH && digitalRead(key3)==HIGH) {
      waitNoKeypress();
      return;
    } else if (digitalRead(key1)==HIGH && digitalRead(key2)==HIGH) {
      num1 --;
      waitNoKeypress();
    } else if (digitalRead(key2)==HIGH && digitalRead(key3)==HIGH) {
      num2 --;
      waitNoKeypress();
    } else if (digitalRead(key1)==HIGH) {
      num1 ++;
      waitNoKeypress();
    } else if (digitalRead(key2)==HIGH) {
      tone(speaker, 293, 50);
      if (sign=="+") {
      sign = "-";
    } else if (sign=="-") {
      sign = "*";
    } else if (sign=="*") {
      sign = "/";
    } else if (sign=="/") {
      sign = "+";
    }
    waitNoKeypress();
    } else if (digitalRead(key3)==HIGH) {
      num2 ++;
      waitNoKeypress();
    }
  }
}

void piano() {
  waitNoKeypress();
  lcd.clear();
  lcd.home();
  lcd.print("     Piano      ");
  lcd.setCursor(0, 1);
  lcd.print("  [1] [2] [3]   ");
  while (true) {
    waitKeypress();
    delay(20);
    if (digitalRead(key1)==HIGH && digitalRead(key3)==HIGH) {
      waitNoKeypress();
      return;
    } else if (digitalRead(key1)==HIGH) {
      tone(speaker, 261, 50);
    } else if (digitalRead(key2)==HIGH) {
      tone(speaker, 293, 50);
    } else if (digitalRead(key3)==HIGH) {
      tone(speaker, 329, 50);
    }
  }
}

void system() {
  waitNoKeypress();
  lcd.clear();
  lcd.home();
  lcd.print("[1] Clear EEPROM");
  lcd.setCursor(0, 1);
  lcd.print("[2] Sign Out");
  waitKeypress();
  if (digitalRead(key1)==HIGH) {
    lcd.clear();
    lcd.home();
    tone(speaker, 200, 20);
    lcd.print("Are you sure?");
    lcd.setCursor(0, 1);
    lcd.print("[1] Yes [2-3] No");
    waitKeypress();
    if (digitalRead(key1)==HIGH) {
      for (int i = 0 ; i < EEPROM.length() ; i++) {
        EEPROM.write(i, 0);
      }
    } else {
        return;
    }
  } else if (digitalRead(key2)==HIGH) {
    run();
  } else if (digitalRead(key3)==HIGH) {
    return;
  }
}

void wakaMole() {
  int hsadr = 0;
  int highScore = 0;
  if (user=="Sai") {
    hsadr = 0;
  } else if (user=="Phin") {
    hsadr = 1;
  }
  if (user != "Guest") {
    EEPROM.get(hsadr, highScore);
  }
  bool spot1 = false;
  bool spot2 = false;
  bool spot3 = false;
  int score = 0;
  lcd.createChar(0, moleChar);
  lcd.createChar(1, moleHoleChar);
  waitNoKeypress();
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Wak ");
  tone(speaker, 300, 20);
  delay(500);
  lcd.print("A ");
  tone(speaker, 300, 20);
  delay(500);
  lcd.print("Mole");
  tone(speaker, 300, 20);
  delay(100);
  lcd.setCursor(0, 1);
  for (int p = 0; p < 16; p++) {
    lcd.print("|");
    tone(speaker, ((p/2)*10)+400, 20);
    delay(50);
  }
  for (int p = 15; p > -1; p--) {
    lcd.setCursor(p, 1);
    lcd.print(" ");
    tone(speaker, ((p/2)*10)+400, 20);
    delay(50);
  }
  lcd.setCursor(0, 1);
  for (int c = 0; c < 3; c++) {
    lcd.setCursor(0, 1);
    for (int p = 0; p < 16; p++) {
      lcd.write(byte(0));
    }
    tone(speaker, 500, 50);
    delay(200);
    lcd.setCursor(0, 1);
    lcd.print("                ");
    tone(speaker, 400, 50);
    delay(200);
  }
  lcd.setCursor(6, 1);
  lcd.print("[2]");
  while (digitalRead(key2)==LOW) {}
  waitNoKeypress();
  tone(speaker, 400, 80);
  tone(speaker, 500, 50);
  lcd.clear();
  while (true) {
    lcd.setCursor(3, 0);
    lcd.print("             ");
    lcd.setCursor(3, 0);
    lcd.print(score);
    lcd.setCursor(9, 0);
    lcd.print("/");
    lcd.print(highScore);
    if (score > highScore) {
      highScore = score;
    }
    if (keyPress()) {
      delay(10);
    }
    if (digitalRead(key1)==HIGH && digitalRead(key3)==HIGH) {
      waitNoKeypress();
      if (user != "Guest") {
        EEPROM.put(hsadr, highScore);
      }
      return;
    } else if (digitalRead(key1)==HIGH && spot1) {
      score = score + 10;
      spot1 = false;
      tone(speaker, 400, 20);
      waitNoKeypress();
    } else if (digitalRead(key2)==HIGH && spot2) {
      score = score + 10;
      spot2 = false;
      tone(speaker, 400, 20);
      waitNoKeypress();
    } else if (digitalRead(key3)==HIGH && spot3) {
      score = score + 10;
      spot3 = false;
      tone(speaker, 400, 20);
      waitNoKeypress();
    } else if (digitalRead(key1)==HIGH && !spot1) {
      score = score - 10;
      spot1 = false;
      tone(speaker, 200, 20);
      waitNoKeypress();
    } else if (digitalRead(key2)==HIGH && !spot2) {
      score = score - 10;
      spot2 = false;
      tone(speaker, 200, 20);
      waitNoKeypress();
    } else if (digitalRead(key3)==HIGH && !spot3) {
      score = score - 10;
      spot3 = false;
      tone(speaker, 200, 20);
      waitNoKeypress();
    }
    if (random(1, 30)==10) {
      spot1 = true;
      tone(speaker, 200, 30);
    }
    if (random(1, 30)==10) {
      spot2 = true;
      tone(speaker, 200, 30);
    }
    if (random(1, 30)==10) {
      spot3 = true;
      tone(speaker, 200, 30);
    }
    if (spot1) {
      lcd.setCursor(3, 1);
      lcd.write(byte(0));
    } else {
      lcd.setCursor(3, 1);
      lcd.write(byte(1));
    }
    if (spot2) {
      lcd.setCursor(7, 1);
      lcd.write(byte(0));
    } else {
      lcd.setCursor(7, 1);
      lcd.write(byte(1));
    }
    if (spot3) {
      lcd.setCursor(12, 1);
      lcd.write(byte(0));
    } else {
      lcd.setCursor(12, 1);
      lcd.write(byte(1));
    }
    delay(50);
  }
}

void sessionInfo() {
  waitNoKeypress();
  lcd.clear();
  lcd.home();
  lcd.print("User: ");
  lcd.print(user);
  while (true) {
    lcd.setCursor(0, 1);
    lcd.print("T+s boot : ");
    lcd.print(round(millis()/1000));
    if (keyPress()) {
      waitNoKeypress();
      return;
    }
  }
}

void rNum() {
  waitNoKeypress();
  lcd.clear();
  lcd.home();
  lcd.print("Number Generator");
  lcd.setCursor(6, 1);
  lcd.print("[2]");
  int max = 101;
  while (true) {
    waitKeypress();
    if (digitalRead(key2)) {
      lcd.setCursor(4, 1);
      lcd.print("   /");
      lcd.print(max-1);         // Print and subtract 1 so that the user never sees that extra 1.
      lcd.setCursor(4, 1);
      lcd.print(random(max));
      //waitNoKeypress();
    } else if (digitalRead(key3)) {
      waitNoKeypress();
      return;
    } else if (digitalRead(key1)) {
      if (max == 101) {   // You need to add the extra 1 so the maximum number can pick itself!
        max = 1001;
      } else if (max == 1001) {
        max = 11;
      } else if (max == 11) {
          max = 51;
      } else if (max == 51) {
          max = 101;
        }
      lcd.setCursor(8, 1);
      lcd.print("    ");
      lcd.setCursor(8, 1);
      lcd.print(max-1);
    } 
    waitNoKeypress();
  }
}

void analogReader() {
  // Temp sensor conv: (((((sensorVal/1024.0)*5.0)-0.5)*100)*1.8)+32
  waitNoKeypress();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.createChar(0, degreeSymbol);
  lcd.write(byte(0));
  lcd.print("F:");
  lcd.setCursor(0, 1);
  lcd.print("Light:");
  while (true) {
    lcd.setCursor(4, 0);
    lcd.print("         ");
    lcd.setCursor(4, 0);
    lcd.print((((((analogRead(A0)/1024.0)*5.0)-0.5)*100)*1.8)+32);
    lcd.setCursor(7, 1);
    lcd.print("         ");
    lcd.setCursor(7, 1);
    lcd.print(analogRead(A1));
    if (keyPress()) {
      waitNoKeypress();
      return;
    }
    delay(10);
  }
}

void logo() {
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.write(byte(0));
  lcd.write(byte(1));
  lcd.write(byte(2));
  lcd.write(byte(3));
  lcd.setCursor(2, 1);
  lcd.setCursor(6, 1);
  lcd.write(byte(4));
  lcd.write(byte(5));
  lcd.write(byte(6));
  lcd.write(byte(7));
  delay(1000);
  for (int p = 7; p < 11; p++) {
    lcd.clear();
    lcd.setCursor(p, 0);
    lcd.write(byte(0));
    lcd.write(byte(1));
    lcd.write(byte(2));
    lcd.write(byte(3));
    lcd.setCursor(2, 1);
    lcd.setCursor(p, 1);
    lcd.write(byte(4));
    lcd.write(byte(5));
    lcd.write(byte(6));
    lcd.write(byte(7));
    delay(300);
  }
}

bool keyPress() {
  return (!(digitalRead(key1)==LOW && digitalRead(key2)==LOW && digitalRead(key3)==LOW));
}
void waitKeypress() {
  /* Waits for the user to press any key */
  while (digitalRead(key1)==LOW && digitalRead(key2)==LOW && digitalRead(key3)==LOW) {}
}
void waitNoKeypress() {
  /* Waits for the user to press any key */
  while (digitalRead(key1)==HIGH || digitalRead(key2)==HIGH || digitalRead(key3)==HIGH) {}
}