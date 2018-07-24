#include "Arduino.h"
#include "pitches.h"

const String colors[4] = { "Green", "Red", "Yellow", "Blue",  };
const byte button_pins[] = { 10, 11, 9, 8 };
const byte led_pins[] = { 6, 7, 5, 4 };
const int button_state[] = { 0, 0, 0, 0 };

int sequence[4]; 
int turn = 0;

void setup() 
{
    Serial.begin(9600);
    Serial.println("Setup");

    randomSeed(analogRead(0));
    
    for (int i = 0; i < 4; i++)
    {
        pinMode(button_pins[i], INPUT);
    }
    for (int i = 0; i < 4; i++)
    {
        pinMode(led_pins[i], OUTPUT);
    }
}

// turn = 0 | sequence = []
// turn = 1 | sequence = [0]
// turn = 2 | sequence = [0, 3]
// turn = 3 | sequence = [0, 3, 2]
// turn = 4 | sequence = [0, 3, 2, 1]

void playSequence() {
    if (turn == 4) {
        Serial.println("Victory!!!!!!!!!");
        return;
    }

    int randNumber = random(0, 4);
    sequence[turn] = randNumber;
    turn++;

    Serial.println("Current sequence:");
    for (int i = 0; i < turn; i++) {
        const int led = sequence[i]; // 0
        
        Serial.println(colors[led]);
        digitalWrite(led_pins[led], HIGH);
        delay(500);
        digitalWrite(led_pins[led], LOW);
        delay(100);
    }
    Serial.println();
}

boolean playerTurn() {
    int step = 0;
    while (step < turn) {
        for (int button = 0; button < 4; button++) {
            int switchState = digitalRead(button_pins[button]);
            if (switchState == HIGH) {
                Serial.print(colors[button]);
                Serial.println(" HIGH");

                digitalWrite(led_pins[button], HIGH);
                delay(500);
                digitalWrite(led_pins[button], LOW);

                if (button != sequence[step]) {
                    Serial.println("YOU LOOSE :(");
                    return false;
                }
                else {
                    step++;
                }
            }
            else {
                switchState = LOW;
            }
        }
    }
    return true;
}

//boolean winning = true;

void loop()
{
    Serial.println("New game");
    boolean winning = true;
    turn = 0;

    while (winning && turn < 4) {
        Serial.print("turn:");
        Serial.println(turn);
        playSequence();
        winning = playerTurn();
        delay(500);
    }

    delay(1000);

}