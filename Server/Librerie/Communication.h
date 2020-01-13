/*
  La libreria conterrà tutte le funzioni utili a gestire la comunicazione tra i Client e il Server.
*/

#include "../../codici_errori.h"
#include "../../messaggi.h"
#include "../../costanti.h"

int creaSocket();

int GameGridToText(GameGrid **p, char msg[], int giocatore);
