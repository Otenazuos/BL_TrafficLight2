#include "mbed.h"

InterruptIn button(p5); //Interrupção para o push button
DigitalOut RedLed(p6); // Led Vermelho
DigitalOut YellowLed(p7); // Led Amarelo
DigitalOut GreenLed(p8); // Led verde (Azul no simulador)

Timeout timeout; // Objeto TimeOut
Ticker tick; // Objeto Ticker

enum States{OFF, ALERT, RED, YELLOW , GREEN}; // Definição dos possíveis estados

States EstadoAtual = RED; // Estado inicial do sistema
States EstadoAnterior = RED;

float TimeButton = 0;
bool BotaoPress = false;

// Protótipos das Funções:
void ButtonPressing();
void ButtonRelease();
void Desligado();
void stdAlert();
void stdRed();
void stdYellow();
void YellowAlert();
void stdGreen();
void stdAnterior();
void Reset();

int main(){
    stdRed(); // Estado inicial Led vermelhor
    button.rise(&ButtonPressing);
    button.fall(&ButtonRelease);
    while(1){
        if (BotaoPress == true){
            TimeButton += 0.2;
            wait_ms(200);
        }
    }
}

void ButtonPressing(){ // Botão pressionado
    TimeButton = 0; // Contador tempo do botão
    BotaoPress = true; // Flag para botão pressionado
}

void ButtonRelease(){
    printf("Botão pressionado %.2fs \n",TimeButton);
    if (TimeButton < 3.0 && EstadoAtual == GREEN){
        stdYellow();
    } 
    else if (TimeButton >= 3 && TimeButton <= 10){
        if (EstadoAtual != ALERT){
        stdAlert();
        }
        if (EstadoAtual == ALERT){
            stdAnterior();
        }
    }
    else if (TimeButton > 10) {
        if (EstadoAtual == OFF){
            stdRed();
        }else{
        Desligado();
        }
    }
    BotaoPress = false;
}

void Desligado(){
    Reset();
    printf("Sistema desligado \n");
    EstadoAtual = OFF;
}
void stdRed(){
    Reset();
    printf("Sinal Vermelho \n");
    RedLed = 1;
    EstadoAtual = RED;
    timeout.attach(&stdGreen, 10);
}
void stdGreen(){
    Reset();
    printf("Sinal verde \n");
    GreenLed = 1;
    EstadoAtual = GREEN;
    timeout.attach(&stdYellow, 20);
}
void stdYellow(){
    Reset();
    printf("Sinal Amarelo \n");
    YellowLed = 1;
    EstadoAtual = YELLOW;
    timeout.attach(&stdRed, 4);
}
void YellowAlert(){
    YellowLed = !YellowLed;
}
void stdAlert(){
    Reset();
    printf("Modo de alerta \n");
    EstadoAnterior = EstadoAtual;
    EstadoAtual = ALERT;
    tick.attach(&YellowAlert, 0.5);

}
void stdAnterior(){
    switch(EstadoAnterior){
        case OFF:
        Desligado();
        break;
        
        case GREEN:
        stdGreen();
        break;

        case YELLOW:
        stdYellow();
        break;

        case RED:
        stdRed();
        break;

        case ALERT:
        stdAlert();
        break;
    }
}
void Reset(){
    GreenLed = 0;
    YellowLed = 0;
    RedLed = 0;
    timeout.detach();
    tick.detach();
}