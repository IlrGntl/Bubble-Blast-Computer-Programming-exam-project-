#include <stdio.h>
#include <stdlib.h> /* Questa libreria contiene le seguenti funzioni usate:
                    malloc(), rand(), srand() */

#include <time.h> /* Usata per la generazione di un numero casuale, per la
                    quale abbiamo usato la funzione time() */

#include <conio.h> /* conio.h per Windows, ncurses.h per Unix. 
                    Permette di raccogliere l'input dall'utente */ 

#include <string.h> // Aggiunta per usare memcpy()

#include "Bubbleblast.h" // Raggruppa funzioni con scope globale

// Funzione dichiarata nel file header:

void initializeBoard(enum CellState (*board)[COLS]) 
{
    /*creazione di una tabella con distribuzione casuale e proporzioni 
    controllate di: bolle EMPTY (primo stadio), BUBBLE_SMALL (leggermente 
    gonfie), BUBBLE_LARGE (pre-esplosione) e NO_BUBBLE (spazio vuoto).
    srand() cerca un numero casuale a partire da un seed che specifico essere
    il valore ridato da time(NULL). time(NULL) restituisce il numero di secondi
    passati dall'epoch. Questo assicura che il risultato sia sempre diverso*/

    srand(time(NULL));
    for (int i = 0; i < ROWS; i++) //il numero di righe e colonne è nell'header
    {
        for (int j = 0; j < COLS; j++) //per ogni indice di riga aggiungo indice di colonna
        {
            /*definizione dei valori di probabilità di uscita di spazi vuoti e bolle;
            fatto per limitare partite risolvibili con una mossa o con troppe mosse.
            I valori sono diversi per fare in modo che abbia senso il metodo
            scelto per l'assegnazione delle bolle in base al numero casuale generato */
            
            float empty_prob = 0.24;
            float small_bubble_prob = 0.1;
            float large_bubble_prob = 0.26;

            /*la funzione seguente genera un numero intero casuale, trasformato in 
            decimale, e lo salva nella variabile random.
            La divisione lo converte in un numero compreso tra 0 e 1.
            RAND_MAX è il valore massimo che può raggiungere la funzione rand(),
            nella libreria standard, e dovrebbe essere 32767. Poteva essere usato
            qualsiasi altro numero */

            float random = (float)rand() / (float)RAND_MAX;

            /*Se  il numero trovato ricade nell'intervallo 0-0.1, viene messa
            una bolla di secondo livello nella posizione indicata dagli indici
            di quella reiterazione del ciclo */

            if (random < small_bubble_prob) 
            {
                board[i][j] = BUBBLE_SMALL;
            } 

            /* Se invece ricade tra 0 e la somma di probabilità che esca la bolla di secondo
            livello e quella di primo, viene posizionata una bolla di primo livello */
            else if (random < small_bubble_prob + empty_prob) 
            {
                board[i][j] = EMPTY;
            } 
            /* Se ricade tra 0 e la somma di primo, secondo e terzo livello, viene posizionata
            la volla di terzo livello, quella prossima all'esplosione */
            else if (random < empty_prob + small_bubble_prob + large_bubble_prob) 
            {
                board[i][j] = BUBBLE_LARGE;
            } 
            // In tutti gli altri casi, lo spazio rimane vuoto
            else 
            {
                board[i][j] = NO_BUBBLE;
            }
        }
    }
}

/* Funzione per trovare le mosse vincenti fino a una certa profondità
nell'albero delle possibilità. La funzione è dichiarata nel file header. 
Gli input sono: il numero di mosse eseguibili, che chiamo depth perché a ogni
mossa si sblocca un nuovo scenario che aggiunge un livello di profondità all'analisi
della partita; lo stato in cui si trova la tabella (celle con o senza bolle)
indicato con puntatore; un record che raccolga le mosse fatte, anche loro 
referenziate con puntatore; infine il numero di mosse, ossia l'output di questa 
funzione. Dichiarazioni nel file di header */

// Sostituisci la riga 88 nel .c e la riga 36 nel .h con questa:
int find_winning_move_at_depth(int depth, enum CellState board[ROWS][COLS], struct Move *moves, int *num_moves)
{
    if (depth == 0) 
    {
        if (is_game_over(board)) /*is_game_over è lo scenario di partita vinta, ed
                                   è dichiarato nel file header. Prende in input 
                                   lo stato corrente della matrice, e l'if controlla 
                                   se sia vuota. Viene fatto a numero terminato di 
                                   mosse consentite; è qui sopra altrimenti il gioco
                                   risulta perso quando il giocatore completa la 
                                   partita una volta arrivato a depth = 0, che invece 
                                   va ancora bene */
        {
            return 1; //cioè True
        }
        return 0; /* Per uscire. L'if ha controllato se la tabella, a mosse terminate,
                    fosse vuota, ma non lo era */
    }
    /* Se depth != 0 il gioco può continuare, quindi va visto lo stato della
    tabella. */

    for (int i = 0; i < ROWS; i++) 
    {
        for (int j = 0; j < COLS; j++) 
        {
            if (board[i][j] == NO_BUBBLE) 
            {
                continue; 
            }

            /* Se incontro anche solo una bolla (partita non terminata),
            dichiaro una nuova matrice con le stesse caratteristiche */

            enum CellState board_copy[ROWS][COLS];

            /* memcpy si trova nel file header string.h.
            Il primo valore in input è la posizione in cui salvo la nuova tabella,
            quella copiata; board è la tabella da copiare; segue la dimensione
            occupata dalla nuova tabella, che è quella di un elemento in CellState 
            moltiplicato per le righe e per le colonne */

            memcpy(board_copy, board, sizeof(enum CellState) * ROWS * COLS);

            /* change_state è una funzione che troviamo in Bubbleblast.h.
            Le viene dato come terzo input la tabella copiata e in essa viene 
            cambiato lo stato della cella agli indici forniti */

            change_state(i, j, board_copy);

            /* Se dopo il cambio di stato la partita viene vinta, eseguire il
            contenuto di qusto blocco if: salvare gli indici della mossa vincente
            nel vettore moves e aumentare num_moves di 1, che tiene il conto
            delle mosse necessarie a vincere */

            if (find_winning_move_at_depth(depth - 1, board_copy, moves, num_moves) == 1) 
            {
                moves[*num_moves].row = i;
                moves[*num_moves].col = j;
                (*num_moves)++;
                return 1; // Si torna a leggere dal blocco più esterno in poi
            }
        }
    }

    return 0;
}

int is_game_over(enum CellState board[ROWS][COLS]) 
{
    int count = 0; // Inizializzo un counter
    for (int i = 0; i < ROWS; i++) 
    {
        for (int j = 0; j < COLS; j++) 
        {
            if (board[i][j] == NO_BUBBLE) 
            {
                count++; // Lo incremento di uno ogni volta che la cella è vuota
            }
        }
    }
    if (count == ROWS * COLS) // Gioco vinto se tutte le celle sono vuote
    {
        return 1;
    }

    return 0;
}

/* Rappresentazione della tabella col cursore che si sposta tra le celle.
Necessario per mostrare il cursore dentro l'eventuale bolla, altrimenti l'avrebbe
rimpiazzata passandoci sopra e il giocatore non avrebbe potuto vedere il valore della
cella su sui si trovava se non spostandosi */

void displayBoard(int cursorRow, int cursorCol, enum CellState (*board)[COLS]) 
{
    for (int i = 0; i < ROWS; i++) 
    {
        for (int j = 0; j < COLS; j++) 
        {   
            if (i == cursorRow && j == cursorCol) 
            {
                switch (board[i][j]) 
                { 
                    /* Se al passare del cursore c'è questo tipo di bolla,
                    rappresenta invece la bolla in questo modo - segue rappresentaz.
                    della bolla con la x all'interno */

                    case EMPTY:  
                        printf("   (x)   ");
                        break;
                    case BUBBLE_SMALL:
                        printf("  ( x )  ");
                        break;
                    case BUBBLE_LARGE:
                        printf(" (  x  ) ");
                        break;
                    case NO_BUBBLE:
                        printf("    x    ");
                        break;
                }
            } 
            else 
            {
                /* Rappresentazione delle bolle e dello spazio vuoto senza il 
                cursore all'interno */

                switch (board[i][j]) 
                {
                    case EMPTY:
                        printf("   ( )   ");
                        break;
                    case BUBBLE_SMALL:
                        printf("  (   )  ");
                        break;
                    case BUBBLE_LARGE:
                        printf(" (     ) ");
                        break;
                    case NO_BUBBLE:
                        printf("         ");
                        break;
                }
            }
        }
        printf("\n\n");
    }
}

/* Funzione definita nel file di header, prende la posizione iniziale del cursore e 
le direzioni comandate dall'input dell'utente */

void moveCursor(int *cursorRow, int *cursorCol, enum Direction direction) 
{
    // Move the cursor
    switch (direction) {
        case UP:
            if (*cursorRow > 0) (*cursorRow)--;
            break;
        case DOWN:
            if (*cursorRow < ROWS - 1) (*cursorRow)++;
            break;
        case LEFT:
            if (*cursorCol > 0) (*cursorCol)--;
            break;
        case RIGHT:
            if (*cursorCol < COLS - 1) (*cursorCol)++;
            break;
    }
}

void explode(int i, int j, enum CellState (*board)[COLS]) {
    board[i][j] = NO_BUBBLE;

    /* Cambia lo stato della prima cella incontrata che non sia uno spazio vuoto,
    andando verso l'alto, secondo le regole di change_state */

    for(int i_1 = i; i_1 >= 0; i_1--) {
        if(board[i_1][j] != NO_BUBBLE) {
            change_state(i_1, j, board);
            break;
        }
    }

    //change the state of the first cell on the lower direction that is not "NO_BUBBLE" to "EXPLODED"
    for(int i_1 = i; i_1 < ROWS; i_1++) {
        if(board[i_1][j] != NO_BUBBLE) {
            change_state(i_1, j, board);
            break;
        }
    }

    //change the state of the first cell on the left direction that is not "NO_BUBBLE" to "EXPLODED"
    for(int j_1 = j; j_1 >= 0; j_1--) {
        if(board[i][j_1] != NO_BUBBLE) {
            change_state(i, j_1, board);
            break;
        }
    }

    //change the state of the first cell on the right direction that is not "NO_BUBBLE" to "EXPLODED"
    for(int j_1 = j; j_1 < COLS; j_1++) {
        if(board[i][j_1] != NO_BUBBLE) {
            change_state(i, j_1, board);
            break;
        }
    }
}

void change_state(int i, int j, enum CellState (*board)[COLS]) {
    if (board[i][j] == BUBBLE_LARGE) {
        explode(i, j, board);
    } else {
        switch (board[i][j]) {
            case EMPTY:
                board[i][j] = BUBBLE_SMALL;
                break;
            case BUBBLE_SMALL:
                board[i][j] = BUBBLE_LARGE;
                break;
        }
    }
}

int main() {

    enum CellState board[ROWS][COLS];
    initializeBoard(board); /* Qui board è il puntatore. Se non passiamo il puntatore
                            ma direttamente i valori della matrice, viene modificata
                            una copia della matrice e, salvo ritorno della matrice alla
                            fine col return, verrebbe persa uscendo */ 
    
    /* Dichiaro le variabili: numero minimo delle mosse risolutive e una variabile 
    che può essere 0 o 1 tipo boolean per dichiarare trovate o meno le soluzioni */

    int num_moves;
    int sol_found = 0;

    /* Controllo della validità del gioco generato: se è risolvibile entro le
    8 mosse, come da costante dichiarata in Bubleblast.h (MAX_DEPTH), sol_found sarà
    1, altrimenti sarà 0 */

    while(sol_found == 0)
    {
        struct Move moves[MAX_DEPTH]; // Move è la selezione della bolla premendo invio
        num_moves = 0;

        for (int depth = 1; depth <= MAX_DEPTH; depth++) 
        {
            if (find_winning_move_at_depth(depth, board, moves, &num_moves) == 1) 
            {
                sol_found = 1;
                break;
            }
        }

        if(sol_found == 0)
            initializeBoard(board);
    }

    int max_number_player_moves = num_moves + 2;
    int cursorRow = 0;
    int cursorCol = 0;
    int game_started = 0;
    int welcome_screen = 1;

    while(welcome_screen == 1)
    {
        system("cls"); // Clear the screen for Windows
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("~~~~~~~~Welcome to Bubbleblast!~~~~~~~~\n");
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
        printf("Rules...\n");
        printf("Commands are:\n");
        printf("Use keys 'w', 'a', 's', 'd' to move the cursor.\n");
        printf("ENTER changes the bubble state.\n\n");
        printf("Press ENTER to start the game...\n");

        int key = _getch();
        switch (key) 
        {
            case 13:
                game_started = 1;
                welcome_screen = 0;
                break;
        }                

    }

    while(game_started == 1) 
    {
        system("cls"); // Clear the screen for Windows
        displayBoard(cursorRow, cursorCol, board);

        //print all the winning moves in a txt file TODO
        /*
        for (int i = 0; i < num_moves; i++) 
        {
            printf("Mossa %d: (%d, %d)\n", i + 1, moves[i].row, moves[i].col);
        }
        */
        //display the number of moves and the winning move
        printf("Minimum number of moves required to win: %d\n", num_moves);
        printf("Moves remaining: %d\n", max_number_player_moves);

        if(is_game_over(board) == 1)
        {
            printf("\n\n*******You won :D******\n\n");
            return 0;
        }
        else if(max_number_player_moves == 0)
        {
            printf("\n\n******You lost :(******\n\n");
            return 0;            
        }

        // Get user input for arrow keys
        int key = _getch();
        switch (key) 
        {
            case 'w':
                moveCursor(&cursorRow, &cursorCol, UP);
                break;
            case 's':
                moveCursor(&cursorRow, &cursorCol, DOWN);
                break;
            case 'a':
                moveCursor(&cursorRow, &cursorCol, LEFT);
                break;
            case 'd':
                moveCursor(&cursorRow, &cursorCol, RIGHT);
                break;
            case 13: // Enter key
            {
                max_number_player_moves--;
                change_state(cursorRow, cursorCol, board);
                break;
            }
            case 27:  // Escape key
                return 0;  // Exit the game
            
        }

    }




    return 0;
}
