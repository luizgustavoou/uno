#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char number_card[3];
  char naipe_card[4];
} Card;

/** Constantes para as strings a serem lidas */
#define FIRST_SIZE_MY_HAND 7;
const int SIZE_CARD = 5;
const int SIZE_NAIPE = 4;
#define MAX_LINE 100
#define MAX_ACTION 10
#define MAX_ID_SIZE 10

void debugNum(int num) { fprintf(stderr, "%d\n", num); }

void debug(char *message) { fprintf(stderr, "%s\n", message); }

void debugMyHand(Card *my_hand, int size) {
  for (int j = 0; j < size; j++) {
    fprintf(stderr, "%s%s\n", my_hand[j].number_card, my_hand[j].naipe_card);
  }
}

void allocMyHand(Card **new_my_hand, int hand_size) {
  *new_my_hand = (Card *)malloc(sizeof(Card) * hand_size);
}

void reallocMyHand(Card **new_my_hand, int new_hand_size) {
  *new_my_hand = (Card *)realloc(*new_my_hand, sizeof(Card) * (new_hand_size));
}

int isTenCard(char card[SIZE_CARD]) {
  if((int) strlen(card) >= 5) return 1;

  return 0;
}

Card returnCard(char card[SIZE_CARD]) {
  Card c;
  if(isTenCard(card) == 1){
      c.number_card[0] = card[0];
      c.number_card[1] = card[1];
      c.number_card[2] = '\0';
      strcpy(c.naipe_card, card+2);

  }
  else{
    c.number_card[0] = card[0];
    c.number_card[1] = '\0';
    strcpy(c.naipe_card, card+1);

  }
  return c;
}

void setMyHand(char temp[MAX_LINE], Card *my_hand) {
  int cont = 0;
  sscanf(temp, "[ %[^]] ]", temp);

  char *card = strtok(temp, " ");
  
  my_hand[cont] = returnCard(card);
  
  while ((card = strtok(NULL, " ")) != NULL) {
    cont++;
    my_hand[cont] = returnCard(card);
  }
}

Card *removeCard(Card *my_hand, int size_hand, int indice) {
  Card *my_new_hand;

  my_new_hand = (Card *) malloc(sizeof(Card) * (size_hand));

  for (int c = 0; c < indice; c++) {
    my_new_hand[c] = my_hand[c];
  }
  for (int c = indice; c < size_hand; c++) {
    my_new_hand[c] = my_hand[c+1];
  }

  return my_new_hand;
}

Card count_naipe_hand(Card *my_hand, int hand_size, Card escolha){
  int num_copas = 0, num_espada = 0, num_paus = 0, num_ouro = 0;
  for(int c = 0; c < hand_size; c++){
    if(strcmp(my_hand[c].naipe_card, "♥") == 0){
      num_copas ++;
    }
    else if(strcmp(my_hand[c].naipe_card, "♠") == 0){
      num_espada ++;
    }
    else if(strcmp(my_hand[c].naipe_card, "♣") == 0){
      num_paus ++;
    }
    else if(strcmp(my_hand[c].naipe_card, "♦") == 0){
      num_ouro ++;
    }
  }
  if(num_copas >= num_espada && num_copas >= num_paus && num_copas >= num_ouro){
    strcpy(escolha.naipe_card, "♥");
  }
  else if(num_espada >= num_copas && num_espada >= num_paus && num_espada >= num_ouro){
    strcpy(escolha.naipe_card, "♠");
  }
  else if(num_paus >= num_copas && num_paus >= num_espada && num_paus >= num_ouro){
    strcpy(escolha.naipe_card, "♣");
  }
  else if(num_ouro >= num_copas && num_ouro >= num_espada && num_ouro >= num_paus){
   strcpy(escolha.naipe_card, "♦");
  }
  return escolha;
}

/* Funções de ações do bot */
void actionDiscard(Card **my_hand, int *hand_size, int index_card, Card *card_table, char last_action_buy[], char my_id[], int *change_naipe){

  Card naipe;
  
  if (strcmp((*my_hand)[index_card].number_card, "V") == 0 || strcmp((*my_hand)[index_card].number_card, "C") == 0) {
    strcpy(last_action_buy, my_id);
  }

  if(strcmp((*my_hand)[index_card].number_card, "A") == 0 || strcmp((*my_hand)[index_card].number_card, "C") == 0){
    naipe = count_naipe_hand(*my_hand, *hand_size, naipe);
    printf("DISCARD %s%s %s\n", (*my_hand)[index_card].number_card, (*my_hand)[index_card].naipe_card, naipe.naipe_card);
    *change_naipe = 1;
  }
  else{
     printf("DISCARD %s%s\n", (*my_hand)[index_card].number_card, (*my_hand)[index_card].naipe_card);
  }


  *card_table = (*my_hand)[index_card];


  *hand_size = *hand_size - 1;
  reallocMyHand(my_hand, (*hand_size));
  *my_hand = removeCard(*my_hand, *hand_size, index_card);

}

void actionBuy(Card **my_hand, int quant, int *hand_size) {
  char card[SIZE_CARD];
  printf("BUY %d\n", quant);
  for (int c = 0; c < quant; c++) {
    scanf("%s\n", card);
    *hand_size = *hand_size + 1;
    reallocMyHand(my_hand, (*hand_size));

    (*my_hand)[*hand_size - 1] = returnCard(card);

  }
}
/* FIM Funções de ações do bot  */

/* Função para verificar a carta*/
int checkCard(Card **my_hand, int *hand_size, Card *card_table, char naipe[]) {
  
  if (strcmp(card_table->number_card, "V") == 0) {
    actionBuy(my_hand, 2, hand_size);
    return 0;
  } else if (strcmp(card_table->number_card, "C") == 0) {
    actionBuy(my_hand, 4, hand_size);
    strcpy(card_table->number_card, "C");
    strcpy(card_table->naipe_card, naipe);
    return 0;

  } else if (strcmp(card_table->number_card, "A") == 0) {
    strcpy(card_table->number_card, "A");
    strcpy(card_table->naipe_card, naipe);
    return 1;
  }
}
/* FIM Funções para verificar a carta */
int main() {
  char temp[MAX_LINE];     // string para leitura temporária de dados
  char my_id[MAX_ID_SIZE]; // identificador do seu bot

  /* Nossas variáveis */
  int hand_size = FIRST_SIZE_MY_HAND;
  Card *my_hand;
  char last_action_buy[MAX_ID_SIZE];
  int can_buy = 0;

  allocMyHand(&my_hand, hand_size);

  Card card_on_the_table;
  int discard = 0; // Para saber se discardou ou não para saber se irá comprar uma carta

  char naipe_table[SIZE_NAIPE];

  setbuf(stdin, NULL);
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);

  // === INÍCIO DA PARTIDA ===

  scanf("PLAYERS %[^\n]\n", temp);

  scanf("YOU %s\n", my_id);

  scanf("HAND %[^\n]\n", temp);

  sscanf(temp, "[ %[^]] ]", temp);

  setMyHand(temp, my_hand);

  scanf("TABLE %s\n", temp);

  card_on_the_table = returnCard(temp);

  // === PARTIDA ===

  char id[MAX_ID_SIZE];
  char action[MAX_ACTION];
  char complement[MAX_LINE];
  char complement2[MAX_LINE];
  char naipe_select[SIZE_NAIPE];
  int casas, cont_card_table = 0;
  int vef_change_naipe = 0;

  //Card test;

  while (1) {
    do {
      scanf("%s %s", action, complement);
      if (strcmp(action, "DISCARD") == 0) {
        vef_change_naipe = 0;
        if (complement[0] == 'A' || complement[0] == 'C') {
          scanf(" %s", complement2);
        }

        if (complement[0] == 'V' || complement[0] == 'C')
          strcpy(last_action_buy, "");
          can_buy = 1;

        
        card_on_the_table = returnCard(complement);
 

      } else if (strcmp(action, "TURN") == 0) {}

    } while (strcmp(action, "TURN") || strcmp(complement, my_id));
    
    //test = count_naipe_hand(my_hand, hand_size, test);
    //debug(test.naipe_card);

    /* Início das ações do bot main */

    discard = 0;
    /* int continue_program = 1;

    if (can_buy == 1 && strcmp(last_action_buy, my_id) != 0 && (strcmp(card_on_the_table.number_card, "A") == 0 || strcmp(card_on_the_table.number_card, "V") == 0 || strcmp(card_on_the_table.number_card, "C") == 0)) {
      continue_program = checkCard(&my_hand, &hand_size, &card_on_the_table, complement2);
    }

    if(continue_program == 0) return 0; */

    
    
    if (can_buy == 1 && strcmp(last_action_buy, my_id) != 0 && (strcmp(card_on_the_table.number_card, "V") == 0 || strcmp(card_on_the_table.number_card, "C") == 0)) {
      checkCard(&my_hand, &hand_size, &card_on_the_table, complement2);
      can_buy = 0;
      
    } else {
      if (strcmp(card_on_the_table.number_card, "A") == 0 && cont_card_table != 0 &&  vef_change_naipe == 0) {
        checkCard(&my_hand, &hand_size, &card_on_the_table, complement2);
      }

      for (int c = 0; c < hand_size; c++) {
        if (strcmp(my_hand[c].number_card, card_on_the_table.number_card) == 0 || strcmp(card_on_the_table.naipe_card, my_hand[c].naipe_card) == 0) {
          // Verifica se tem letra/numero ou naipes correspondente
          actionDiscard(&my_hand, &hand_size, c, &card_on_the_table, last_action_buy, my_id, &vef_change_naipe);
          discard = 1;
          break;
        }
      }

      if (discard == 0) actionBuy(&my_hand, 1, &hand_size);
    }
    cont_card_table ++;
  }

  return 0;
}

