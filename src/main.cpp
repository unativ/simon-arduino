#include "Arduino.h"
#include "pitches.h"
#include <elapsedMillis.h>

const int MAX_TURNS = 100;
const int SPEAKER_PIN = 12;

const String colors[4] = { "Green", "Red", "Yellow", "Blue" };
const int sounds[4] = { NOTE_E4, NOTE_A5, NOTE_CS5, NOTE_E5 };
const int button_pins[] = { 10, 11, 9, 8 };
const int led_pins[] = { 6, 7, 5, 4 };

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

void playSequence(int colors[], int players) {
    if (turn == MAX_TURNS) {
        Serial.println("Victory!!!!!!!!!");
        return;
    }

    int randNumber = random(0, players);
    int nextColor = colors[randNumber];
    sequence[turn] = nextColor;
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

int playerTurn(int colors [], int players) {
    elapsedMillis timeElapsed;
    int step = 0;
    while (step < turn) {
        boolean pressed = false;
        boolean timeout = false;
        int loosingButton = -1;
        int colorPressed = -1;

        while (!pressed && !timeout) {
            if (timeElapsed > 3000) {
                timeout = true;
                loosingButton = sequence[step];
                Serial.print("Time out for: ");
                Serial.println(loosingButton);
            }
            else {
                for (int button = 0; button < players; button++) {
                    int selectedColor = colors[button];
                    int switchState = digitalRead(button_pins[selectedColor]);
                    if ( switchState == HIGH) {
                        pressed = true;
                        colorPressed = colors[button];
                        break;
                    }
                }
            }
        }

        if (pressed) {
            Serial.print(colorPressed);
            Serial.println(" HIGH");

            tone(SPEAKER_PIN, sounds[colorPressed]);
            digitalWrite(led_pins[colorPressed], HIGH);
            delay(200);

            boolean buttonUp = false;
            while (!buttonUp) {
                int switchState = digitalRead(button_pins[colorPressed]);
                if (switchState == LOW) {
                    buttonUp = true;
                }
            }
            Serial.println("Button up.");

            noTone(SPEAKER_PIN);
            digitalWrite(led_pins[colorPressed], LOW);
            delay(200);

            if (colorPressed != sequence[step]) {
                loosingButton = colorPressed;
            }
        }

        if (loosingButton != -1) {
            Serial.println("YOU LOOSE :(");
            delay(300);

            tone(SPEAKER_PIN, NOTE_E1);
            delay(1500);
            noTone(SPEAKER_PIN);
            Serial.print("Loosing button");
            Serial.println(loosingButton);
            return loosingButton;
        }
        else {
            step++;
        }
    }
    return -1;
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

void showColors(int colors[], int playersLeft) {
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < playersLeft; i++) {
            int color = colors[i];
            digitalWrite(led_pins[color], HIGH);
        }
        delay(200);
        for (int i = 0; i < playersLeft; i++) {
            int color = colors[i];
            digitalWrite(led_pins[color], LOW);
        }
        delay(200);
    }
}

void fourPlayersGame() {
    Serial.println("fourPlayersGame");

    int colors[] = {0, 1, 2, 3};

    for (int playersLeft = 4; playersLeft > 1; playersLeft--) {
        Serial.println("Colors:");
        for (int i = 0; i < 4; i++) {
            Serial.print(colors[i]);
            Serial.print(", ");
        }
        Serial.println();

        showColors(colors, playersLeft);

        Serial.print("Players left: ");
        Serial.println(playersLeft);
        int wrongButton = -1;
        turn = 0;

        while (wrongButton == -1 && turn < MAX_TURNS) {
            Serial.print("turn:");
            Serial.println(turn);
            playSequence(colors, playersLeft);
            wrongButton = playerTurn(colors, playersLeft);
            delay(500);
        }

        int buttonIndex = 0;
        boolean found = false;
        while (!found) {
            if (colors[buttonIndex] == wrongButton) {
                found = true;
            }
            else {
                buttonIndex++;
            }
        }

        int tmp = colors[buttonIndex];
        colors[buttonIndex] = colors[playersLeft - 1];
        colors[playersLeft - 1] = tmp;
    }

    Serial.print("Player won: ");
    Serial.println(colors[0]);

    showColors(colors, 1);
}

void twoPlayersGame() {
    Serial.println("twoPlayersGame");
}

void animalsGame() {
    Serial.println("animnalsGame");
}

void defaultGame() {
     Serial.println("defaultGame");

    int winning = -1;
    turn = 0;
    int colors[] = {0, 1, 2, 3};

    while (winning == -1 && turn < MAX_TURNS) {
        Serial.print("turn:");
        Serial.println(turn);
        playSequence(colors, 4);
        winning = playerTurn(colors, 4);
        delay(500);
    }
}

void loop() {
    Serial.println("New game");

    int game = chooseGame();

    switch (game) {
    case 0:
        fourPlayersGame();
        break;
    case 1:
        twoPlayersGame();
        break;
    case 2:
        animalsGame();
        break;
    case 3:
        defaultGame();
        break;
    }
}