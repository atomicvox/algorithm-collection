#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct rotor {
    char output[27];
    char notch;
    char turnover;
};

struct rotor createRotor (char output[27], char notch, char turnover){
    struct rotor rotor;
    strcpy{rotor.output, output};
    rotor.notch = notch;
    rotor.turnover = turnover;
}

struct machine{
    struct rotor ETW;
    struct rotor Reflector;
    struct rotor rotors[10];
    int rotorsCount;
};

struct machineSetting{
    int Ringstellung[10];//ring setting
    int rotorsCount;
}

static struct machine EnigmaI;

void creatEnigmaI(void){
    EnigmaI.rotors[0] = createRotor("EKMFLGDQVZNTOWYHXUSPAIBRCJ", "Y", "Q");
    EnigmaI.rotors[1] = createRotor("AJDKSIRUXBLHWTMCQGZNPYFVOE", "M", "W");
    EnigmaI.rotors[2] = createRotor("BDFHJLCPRTXVZNYEIWGAKMUSQO", "D", "V");
    EnigmaI.rotors[3] = createRotor("ESOVPZJAYQUIRHXLNFTGKDCMWB", "R", "J");
    EnigmaI.rotors[4] = createRotor("VZBRGITYUPSDNHLXAWMJQOFECK", "H", "Z");

    EnigmaI.rotorsCount = 5;
}

char *encryptMsg(char *msg, struct machine version){

}