#include "Arduino.h"
#include "pitches.h"

const int MAX_TURNS = 100;
const int SPEAKER_PIN = 12;

const String colors[4] = { "Green", "Red", "Yellow", "Blue" };
const int sounds[4] = { NOTE_E4, NOTE_A5, NOTE_CS5, NOTE_E5 };
const byte button_pins[] = { 10, 11, 9, 8 };
const byte led_pins[] = { 6, 7, 5, 4 };

const int button_state[] = { 0, 0, 0, 0 };

int sequence[MAX_TURNS]; 
int turn = 0;

void setup() {
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

void playSequence() {
    if (turn == MAX_TURNS) {
        Serial.println("Victory!!!!!!!!!");
        return;
    }

    int randNumber = random(0, 4);
    sequence[turn] = randNumber;
    turn++;

    int toneTime;
    if (turn <= 5) {
        toneTime = 420;
    }
    else if (turn <= 13) {
        toneTime = 320;
    }
    else {
        toneTime = 220;
    }

    Serial.println("Current sequence:");
    for (int i = 0; i < turn; i++) {
        const int led = sequence[i];
        
        Serial.println(colors[led]);
        tone(SPEAKER_PIN, sounds[led], toneTime);
        digitalWrite(led_pins[led], HIGH);
        delay(toneTime);
        digitalWrite(led_pins[led], LOW);
        delay(50);
    }
    Serial.println();
}

boolean playerTurn() {
    int step = 0;
    while (step < turn) {
        boolean pressed = false;
        int button = 0;
        while (!pressed) {
            for (button = 0; button < 4; button++) {
                int switchState = digitalRead(button_pins[button]);
                if (switchState == HIGH) {
                    pressed = true;
                    break;
                }
            }
        }
        Serial.print(colors[button]);
        Serial.println(" HIGH");

        tone(SPEAKER_PIN, sounds[button]);
        digitalWrite(led_pins[button], HIGH);
        delay(200);

        boolean buttonUp = false;
        while (!buttonUp) {
            int switchState = digitalRead(button_pins[button]);
            if (switchState == LOW) {
                buttonUp = true;
            }
        }
        Serial.println("Button up.");

        noTone(SPEAKER_PIN);
        digitalWrite(led_pins[button], LOW);
        delay(200);


        if (button != sequence[step]) {
            Serial.println("YOU LOOSE :(");
            delay(300);

            tone(SPEAKER_PIN, NOTE_E1);
            delay(1500);
            noTone(SPEAKER_PIN);
            return false;
        }
        else {
            step++;
        }
    }
    return true;
}

int chooseGame() {
    int game = -1;
    while (game == -1) {
        for (int button = 0; button < 4; button++){
            int switchState = digitalRead(button_pins[button]);
            if (switchState == HIGH) {
                game = button;
            }
        }
    }
    Serial.print("Game chosen: ");
    Serial.println(game);
    delay(1000);
    return game;
}

void loop() {

    Serial.println("New game");
    boolean winning = true;
    turn = 0;

    int game = chooseGame();


    while (winning && turn < MAX_TURNS) {
        Serial.print("turn:");
        Serial.println(turn);
        playSequence();
        winning = playerTurn();
        delay(500);
    }
}