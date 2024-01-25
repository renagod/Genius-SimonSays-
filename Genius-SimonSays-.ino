/* Jogo Genius (Simon Says) Utilizando registradores  
 * do Arduíno Uno 
 *           
 *       
 */


int sequencia[20] = {}; //Adicione quantos níveis quiser (
int botoes[4] = { PINB0, PINB1, PINB2, PINB3 };
int leds[4] = { 2, 3, 4, 5 };
int tons[4] = { 262, 294, 330, 349 };

// variáveis usadas para determinar a dificuldade do jogo
int dificuldade1 = 1000;  // tempo de espera da rodada
int dificuldade2 = 300;   // velocidade de reprodução da sequência
int dificuldade3 = 200;   // velocidade de reprodução da sequência

// variáveis usadas no apoio a lógica do jogo
int rodada = 0;
int passo = 0;
int botaoPressionado = 0;
bool gameOver = false;
int vidas = 3;

void setup() {
  // Leds
  DDRD = DDRD | (1 << DD2);  //pinMode(2, OUTPUT);
  DDRD = DDRD | (1 << DD3);  //pinMode(3, OUTPUT);
  DDRD = DDRD | (1 << DD4);  //pinMode(4, OUTPUT);
  DDRD = DDRD | (1 << DD5);  //pinMode(5, OUTPUT);
  // Buzzer
  DDRD = DDRD | (1 << DD6);  //pinMode(6, OUTPUT);
  // Botões
  PORTB = PORTB | (1 << PB0);  //pinMode(8, INPUT_PULLUP);
  PORTB = PORTB | (1 << PB1);  //pinMode(9, INPUT_PULLUP);
  PORTB = PORTB | (1 << PB2);  //pinMode(10, INPUT_PULLUP);
  PORTB = PORTB | (1 << PB3);  //pinMode(11, INPUT_PULLUP);
  // referência a inicialização da função random()
  randomSeed(analogRead(A0)); // ou randomSeed(1234);
}

void loop() {
  // validação GAME OVER
  if (gameOver == true) {
    efeito1();
  } else {
    // lógica principal
   
    proximaRodada();
    reproduzirSequencia();
    aguardarJogador();
    delay(1000);
   

    // vencedor (jogador chegou até a última rodada)
    if (rodada == 5) {
      efeito3();
      gameOver = true;
    }
    if (vidas == 0) {
      perdeu();
      gameOver = true;
    }
  }
}

/* Função que sorteia a sequência da próxima rodada */
void proximaRodada() {
  
  sequencia[rodada] = random(4);
  rodada = rodada + 1;
}

/* Função que reproduz a sequência da rodada */
void reproduzirSequencia() {
  for (int i = 0; i < rodada; i++) {
    tone(6, tons[sequencia[i]]);
    PORTD = PORTD |(1 << leds[sequencia[i]]); //digitalWrite(leds[sequencia[i]], HIGH);
    delay(dificuldade2);
    noTone(6);
    PORTD = PORTD &~(1 << leds[sequencia[i]]); //digitalWrite(leds[sequencia[i]], LOW);
    delay(dificuldade3);
  }
}

/* Função que aguarda o jogador */
void aguardarJogador() {
  for (int i = 0; i < rodada; i++) {
    bool jogadaEfetuada = false;
    while (!jogadaEfetuada) {
      for (int i = 0; i <= 3; i++) {
        if ((PINB & (1 << botoes[i])) == LOW) { //digitalRead(botoes[i]) == 0) {
          botaoPressionado = i;
          tone(6, tons[i]);
          PORTD = PORTD |(1 << leds[i]);//digitalWrite(leds[i], HIGH);
          delay(300);
          PORTD = PORTD &~(1 << leds[i]);//digitalWrite(leds[i], LOW);
          noTone(6);
          jogadaEfetuada = true;
        }
      }
    }
    //verificar a jogada
    if (sequencia[passo] != botaoPressionado) {
      efeito2();
      vidas--;
      rodada--;
      passo --;
      
     
      break;
     
    }
    passo = passo + 1;
  }
  passo = 0;
}

/***** Efeitos luminosos / sonoros *****/

/* Game over */
void efeito1() {
  //PORTx = PORTx | (1 << Pxn)
  PORTD = PORTD | (1 << PD2);  //digitalWrite(2, HIGH)
  PORTD = PORTD | (1 << PD3);  // digitalWrite(3, HIGH);
  PORTD = PORTD | (1 << PD4);  //digitalWrite(4, HIGH);
  PORTD = PORTD | (1 << PD5);  //digitalWrite(5, HIGH);
  delay(1000);
  //PORTx = PORTx &~(1 << Pxn)
  PORTD = PORTD & ~(1 << PD2);  //digitalWrite(2, LOW);
  PORTD = PORTD & ~(1 << PD3);  //digitalWrite(3, LOW);
  PORTD = PORTD & ~(1 << PD4);  //digitalWrite(4, LOW);
  PORTD = PORTD & ~(1 << PD5);  //digitalWrite(5, LOW);
  delay(1000);
}

/* Errou a sequência */
void efeito2() {
  for (int i = 0; i <= 3; i++) {
    tone(6, 70);
    PORTD = PORTD | (1 << leds[i]);//digitalWrite(leds[i], HIGH);
    delay(100);
    PORTD = PORTD & ~(1 << leds[i]);//digitalWrite(leds[i], LOW);
    noTone(6);
  }
}

/* Campeão !!! */
void efeito3() {
  // melodia (notas musicais)
  int nota[] = { 660, 660, 660, 510, 660, 770, 380, 510, 380, 320, 440, 480, 450, 430, 380, 660, 760, 860, 700, 760, 660, 520, 580, 480, 510, 380, 320, 440, 480, 450, 430, 380, 660, 760, 860, 700, 760, 660, 520, 580, 480, 500, 760, 720, 680, 620, 650, 380, 430, 500, 430, 500, 570, 500, 760, 720, 680, 620, 650, 1020, 1020, 1020, 380, 500, 760, 720, 680, 620, 650, 380, 430, 500, 430, 500, 570, 585, 550, 500, 380, 500, 500, 500, 500, 760, 720, 680, 620, 650, 380, 430, 500, 430, 500, 570, 500, 760, 720, 680, 620, 650, 1020, 1020, 1020, 380, 500, 760, 720, 680, 620, 650, 380, 430, 500, 430, 500, 570, 585, 550, 500, 380, 500, 500, 500, 500, 500, 500, 500, 580, 660, 500, 430, 380, 500, 500, 500, 500, 580, 660, 870, 760, 500, 500, 500, 500, 580, 660, 500, 430, 380, 660, 660, 660, 510, 660, 770, 380 };
  // duraçao de cada nota da melodia
  int duracaoNota[] = { 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 80, 100, 100, 100, 80, 50, 100, 80, 50, 80, 80, 80, 80, 100, 100, 100, 100, 80, 100, 100, 100, 80, 50, 100, 80, 50, 80, 80, 80, 80, 100, 100, 100, 100, 150, 150, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 150, 200, 80, 80, 80, 100, 100, 100, 100, 100, 150, 150, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 150, 150, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 150, 200, 80, 80, 80, 100, 100, 100, 100, 100, 150, 150, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 60, 80, 60, 80, 80, 80, 80, 80, 80, 60, 80, 60, 80, 80, 80, 80, 80, 60, 80, 60, 80, 80, 80, 80, 80, 80, 100, 100, 100, 100, 100, 100, 100 };
  // pausa depois que cada nota da melodia é reproduzida
  int pausaNota[] = { 150, 300, 300, 100, 300, 550, 575, 450, 400, 500, 300, 330, 150, 300, 200, 200, 150, 300, 150, 350, 300, 150, 150, 500, 450, 400, 500, 300, 330, 150, 300, 200, 200, 150, 300, 150, 350, 300, 150, 150, 500, 300, 100, 150, 150, 300, 300, 150, 150, 300, 150, 100, 220, 300, 100, 150, 150, 300, 300, 300, 150, 300, 300, 300, 100, 150, 150, 300, 300, 150, 150, 300, 150, 100, 420, 450, 420, 360, 300, 300, 150, 300, 300, 100, 150, 150, 300, 300, 150, 150, 300, 150, 100, 220, 300, 100, 150, 150, 300, 300, 300, 150, 300, 300, 300, 100, 150, 150, 300, 300, 150, 150, 300, 150, 100, 420, 450, 420, 360, 300, 300, 150, 300, 150, 300, 350, 150, 350, 150, 300, 150, 600, 150, 300, 350, 150, 150, 550, 325, 600, 150, 300, 350, 150, 350, 150, 300, 150, 600, 150, 300, 300, 100, 300, 550, 575 };
  for (int i = 0; i < 156; i++) {
    tone(6, nota[i], duracaoNota[i]);
    PORTD = PORTD | (1 << PD2);  //digitalWrite(2, HIGH)
    PORTD = PORTD | (1 << PD3);  // digitalWrite(3, HIGH);
    PORTD = PORTD | (1 << PD4);  //digitalWrite(4, HIGH);
    PORTD = PORTD | (1 << PD5);  //digitalWrite(5, HIGH);
    delay(15);
    PORTD = PORTD & ~(1 << PD2);  //digitalWrite(2, LOW);
    PORTD = PORTD & ~(1 << PD3);  //digitalWrite(3, LOW);
    PORTD = PORTD & ~(1 << PD4);  //digitalWrite(4, LOW);
    PORTD = PORTD & ~(1 << PD5);  //digitalWrite(5, LOW);
    delay(pausaNota[i]);
    noTone(6);
  }
}

//perdeu playboy
void perdeu() {
  int melody[] = {
    392, 392, 392, 311, 466, 392, 311,
    466, 392, 622, 587, 554, 523, 466, 493,
    392, 784, 740, 622, 587, 554, 523, 466,
    392, 784, 740, 622, 587, 554, 523, 466,
    392, 392, 392, 311, 466, 392, 311,
    466, 392, 622, 587, 554, 523, 466, 493,
    392, 784, 740, 622, 587, 554, 523, 466,
    392, 784, 740, 622, 587, 554, 523, 466
  };

  // Definição das durações das notas
  int noteDurations[] = {
    // Durações das notas (em milissegundos)
    500, 500, 500, 350, 150, 500, 350,
    150, 1000, 500, 350, 150, 500, 350, 150,
    1000, 500, 350, 150, 500, 350, 150, 1000,
    500, 350, 150, 500, 350, 150, 1000, 500,
    500, 500, 500, 350, 150, 500, 350,
    150, 1000, 500, 350, 150, 500, 350, 150,
    1000, 500, 350, 150, 500, 350, 150, 1000,
    500, 350, 150, 500, 350, 150, 1000
  };
  int pausas[] = {
    500, 500, 500, 350, 150, 500, 350,
    150, 1000, 500, 350, 150, 500, 350, 150,
    1000, 500, 350, 150, 500, 350, 150, 1000,
    500, 350, 150, 500, 350, 150, 1000, 500,
    500, 500, 500, 350, 150, 500, 350,
    150, 1000, 500, 350, 150, 500, 350, 150,
    1000, 500, 350, 150, 500, 350, 150, 1000,
    500, 350, 150, 500, 350, 150, 1000
  };
  for (int i = 0; i < 156; i++) {
    tone(6, melody[i], noteDurations[i]);
    PORTD = PORTD | (1 << PD2);  //digitalWrite(2, HIGH)
    PORTD = PORTD | (1 << PD3);  // digitalWrite(3, HIGH);
    PORTD = PORTD | (1 << PD4);  //digitalWrite(4, HIGH);
    PORTD = PORTD | (1 << PD5);  //digitalWrite(5, HIGH);
    delay(50);
    PORTD = PORTD & ~(1 << PD2);  //digitalWrite(2, LOW);
    PORTD = PORTD & ~(1 << PD3);  //digitalWrite(3, LOW);
    PORTD = PORTD & ~(1 << PD4);  //digitalWrite(4, LOW);
    PORTD = PORTD & ~(1 << PD5);  //digitalWrite(5, LOW);
    delay(pausas[i]);
    noTone(6);
  }
}
