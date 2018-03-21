#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

//funkcja odpowiedzialna za utworzenie tablicy argv[0] -> program; argv[1..n] -> argumenty wywołania programu
char * przypiszArgument(char * iM) {
    char * argumentZwracany = malloc(strlen(iM));
    strcpy(argumentZwracany, iM);
    return argumentZwracany;
}

int main() {
    
    printf("Wpisz \'close\' w celu zamknięcia programu ...\n");

    pid_t p;
    int e;
    char input[256];
    
    //dopóki użytkownik nie wpisze "close"
    while (strcmp(input, "close")) {
        
        printf("> ");

        if (fgets(input, 256, stdin)==NULL) {
            perror("Błąd odczytu linii!");
        }
        
        input[strlen(input) - 1] = 0;
        
        //nie uruchamiaj tej części kodu, jeśli użytkownik wpisał "close"
        if (strcmp(input, "close")) {
           
            //utwórz wskaźnik na pierwszy element tablicy argumentów; podziel input użytkownika; utwórz proces potomny, a w nim uruchom program
            char * argumenty[100];
            int argument = 0;
            char * inputZmodyfikowany = strtok(input, " ");
            while (inputZmodyfikowany!=NULL) {
                argumenty[argument] = przypiszArgument(inputZmodyfikowany);
                argument++;
                inputZmodyfikowany = strtok(NULL, " ");
            }
            argumenty[argument] = NULL;
            
            if ((p = fork()) == 0) {
                if ((e=execvp(argumenty[0], argumenty))==-1) {
                    perror("Błąd uruchomienia programu!");
                    exit(1);
                }
            } else if (p == -1){
                perror("Błąd utworzenia procesu!");
            } else {
                //todo zwalnianie pamięci
                if (wait(NULL)==-1) {
                    perror("Błąd oczekiwania na potomka!");
                }
            }
            
        }
        
    }
    
    
    return 0;
}
