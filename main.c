#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/** Constantes para as strings a serem lidas */
#define FIRST_SIZE_MY_HAND 7;
const int SIZE_CARD = 5;
#define MAX_LINE 100
#define MAX_ACTION 10
#define MAX_ID_SIZE 10

void debug(char *message){
  fprintf(stderr, "%s\n", message);
}

void debugMyHand(char **my_hand, int size) {
  for(int j=0; j<size; j++) {
    debug(my_hand[j]);
  }
}

void allocMyHand(char ***new_my_hand, int hand_size) {
  *new_my_hand = (char**) malloc(sizeof(char*)*hand_size);
  
  for(int j=0; j<hand_size; j++) {
    (*new_my_hand)[j] = (char*) malloc(sizeof(char)*SIZE_CARD);
  }
}

void reallocMyHand(char ***new_my_hand, int new_hand_size) {
  *new_my_hand = (char**) realloc(*new_my_hand, sizeof(char*)*(new_hand_size));
  for(int j=0; j<new_hand_size; j++) {
    (*new_my_hand)[j] = (char*) realloc((*new_my_hand)[j], sizeof(char)*SIZE_CARD);
  }
}

char** removeCard(char **matriz, int size_matriz, int indice){
  char **my_new_hand;
  
  my_new_hand = (char**) malloc(sizeof(char*)*(size_matriz-1));
  for(int j=0; j<size_matriz-1; j++) {
    my_new_hand[j] = (char*) malloc(sizeof(char)*SIZE_CARD);
  }
  
  for(int c = 0; c < indice; c++){
    strcpy(my_new_hand[c], matriz[c]);
  }
  for(int c = indice; c < size_matriz-1; c++){
    strcpy(my_new_hand[c], matriz[c+1]);
  }
  
  return my_new_hand;
}

void setMyHand(char temp[MAX_LINE], char **my_hand) {
  int cont = 0;
  sscanf(temp, "[ %[^]] ]", temp);

  char *card = strtok(temp, " ");

  strcpy(my_hand[0], card);

  while((card = strtok(NULL, " ")) != NULL) {
    cont++;
    strcpy(my_hand[cont], card);
  }
}

int main()
{
  char temp[MAX_LINE];     // string para leitura temporária de dados
  char my_id[MAX_ID_SIZE]; // identificador do seu bot

  /* Nossas variáveis */
  int hand_size = FIRST_SIZE_MY_HAND;
  char **my_hand;

  allocMyHand(&my_hand, hand_size);

  char card_on_the_table[SIZE_CARD];
  int discart = 0; //Para saber se discartou ou não para saber se irá comprar uma carta
  int get_four_or_two = 0;
   char naipe[100];

  setbuf(stdin, NULL);  
  setbuf(stdout, NULL); 
  setbuf(stderr, NULL);

  // === INÍCIO DA PARTIDA ===

  scanf("PLAYERS %[^\n]\n", temp);

  scanf("YOU %s\n", my_id);

  scanf("HAND %[^\n]\n", temp);

  sscanf(temp, "[ %[^]] ]", temp);

  setMyHand(temp, my_hand);

  //debugMyHand(my_hand, hand_size);

  scanf("TABLE %s\n", temp);
  
  strcpy(card_on_the_table, temp);
  
  //debug(card_on_the_table);

  // === PARTIDA ===

  char id[MAX_ID_SIZE];
  char action[MAX_ACTION];
  char complement[MAX_LINE];

  while (1){
    do{
      scanf("%s %s", action, complement);
       
      if(strcmp(action, "DISCARD") == 0){
        strcpy(card_on_the_table, complement);
      }
      
    }
    while (strcmp(action, "TURN") || strcmp(complement, my_id));

    /* Início das ações do bot main */
    
    discart = 0;

    strncpy(naipe, card_on_the_table+1, 3);
      for(int c = 0; c < hand_size; c++){
        if(my_hand[c][0] == card_on_the_table[0]){
          //caso pra letra igual
          printf("DISCARD %s\n", my_hand[c]);

          reallocMyHand(&my_hand, hand_size-1);
          
          my_hand = removeCard(my_hand, hand_size, c);
          hand_size --;
          //debugMyHand(my_hand, hand_size);
          discart = 1;
          //strcpy(card_on_the_table, my_hand[c]);
          //debug(card_on_the_table);
          break;
        }
        else if(strcmp(naipe, my_hand[c]+1) == 0){
          //caso pra naipe igual
          printf("DISCARD %s\n", my_hand[c]);
          
          reallocMyHand(&my_hand, hand_size-1);
          
          my_hand = removeCard(my_hand, hand_size, c);
          hand_size --;
          //debugMyHand(my_hand, hand_size);
          discart = 1;
          //strcpy(card_on_the_table, my_hand[c]);
          //debug(card_on_the_table);
          break;
        }
      }
      
    if(discart == 0){
      char card[SIZE_CARD];
      printf("BUY %d\n", 1);
      scanf("%s\n", card);
      hand_size ++;
      //teste
      my_hand = (char**) realloc(my_hand, sizeof(char*)*(hand_size));
          for(int j=0; j<hand_size; j++) {
            my_hand[j] = (char*) realloc(my_hand[j], sizeof(char)*SIZE_CARD);
      }
      strcpy(my_hand[hand_size-1], card);
      //debugMyHand(my_hand, hand_size);
    }
  }

  return 0;
}