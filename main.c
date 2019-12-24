#include <stdio.h>
#include <stdlib.h>
#include<windows.h>

// MULYI LINE EDITOR

#define ENTER 13
#define ESC 27
#define EXTENDED -32
#define UP_ARROW 72
#define DOWN_ARROW 80
#define RIGHT_ARROW 77
#define LEFT_ARROW 75
#define DELETE 83
#define BACKSPACE 8
#define INSERT 82


COORD coord={0,0};
void gotoxy(int x,int y)
{
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

void printString(char *[], int *[], int);

void main()
{
    int numberOfLines;
    char userInput;
    int index;
    int xLocation = 0;
    int yLocation = 0;
    int isInsert = 0;

    printf("*********************************************************************\n");
    printf("*pressing enter with data in the last row will not insert a new line*\n");
    printf("*********************************************************************\n\n");
    printf("Please enter the number of lines: ");
    scanf("%d", &numberOfLines);
    system("cls");

    char * prows[numberOfLines];
    int sizes[numberOfLines];

    for(int i = 0; i < numberOfLines; i++){
        prows[i] = (char *) malloc(100 * sizeof(char));
        if(!prows[i]){
        printf("out of memory!");
        exit(1);
    }
        sizes[i] = 0;
    }

    do{
        _flushall();
        gotoxy(xLocation, yLocation);
        userInput = getch();

        switch(userInput){
        case EXTENDED:
            userInput = getch();
            switch(userInput){

            case UP_ARROW:
                if (!(yLocation == 0)){
                    if (sizes[yLocation - 1] < xLocation){
                        xLocation = sizes[yLocation - 1];
                    }
                    yLocation--;
                }
                gotoxy(xLocation, yLocation);
                break;

            case DOWN_ARROW:
                if (yLocation < numberOfLines - 1){
                    if (sizes[yLocation + 1] < xLocation){
                        xLocation = sizes[yLocation + 1];
                    }
                    yLocation++;
                }
                gotoxy(xLocation, yLocation);
                break;

            case LEFT_ARROW:
                if (!(xLocation == 0)){
                    xLocation--;
                }
                gotoxy(xLocation, yLocation);
                break;

            case RIGHT_ARROW:
                if (xLocation != sizes[yLocation]){
                    xLocation++;
                }
                gotoxy(xLocation, yLocation);
                break;

            case INSERT:
                if (sizes[yLocation] < 100){
                    if (isInsert){
                        isInsert = 0;
                    } else {
                        isInsert = 1;
                    }
                }
                break;

            case DELETE:
                if (xLocation != sizes[yLocation] && yLocation <= numberOfLines - 1){
                    index = xLocation;
                    for (int i = 0; i < sizes[yLocation] - xLocation; i++){
                        prows[yLocation][index] = prows[yLocation][index + 1];
                        index++;
                    }
                    sizes[yLocation]--;

                } else if (yLocation <= numberOfLines - 1){
                    index = sizes[yLocation];
                    sizes[yLocation] += sizes[yLocation + 1];
                    for (int i = 0; i < sizes[yLocation + 1]; i++){
                        prows[yLocation][index] = prows[yLocation + 1][i];
                        index++;
                    }
                    index = yLocation + 1;
                    for (int i = 0; i < numberOfLines - yLocation - 2; i++){
                        sizes[index] = sizes[index + 1];
                        for (int j = 0; j < sizes[index + 1]; j++){
                            prows[index][j] = prows[index + 1][j];
                        }
                        index++;
                    }
                    sizes[numberOfLines - 1] = 0;
                }
                printString(prows,sizes,numberOfLines);
                break;

            default:
                break;
            }
            break;

        case BACKSPACE:
            if (xLocation != 0 && sizes[yLocation] != 0){
                if (xLocation == sizes[yLocation]){
                    sizes[yLocation]--;
                    xLocation--;
                } else {
                    sizes[yLocation]--;
                    index = --xLocation;
                    for(int i = 0; i < sizes[yLocation] - xLocation; i++){
                        prows[yLocation][index] = prows[yLocation][index + 1];
                        index++;
                    }
                }
                printString(prows,sizes,numberOfLines);
            } else if (!(xLocation != 0) && yLocation != 0){
                index = xLocation = sizes[yLocation - 1];
                sizes[yLocation - 1] += sizes[yLocation];
                for (int i = 0; i <= sizes[yLocation]; i++){
                    prows[yLocation - 1][index] = prows[yLocation][i];
                    index++;
                }
                index = yLocation;
                for (int i = 0; i < numberOfLines - yLocation - 1; i++){
                    sizes[index] = sizes[index + 1];
                    for (int j = 0; j < sizes[index + 1]; j++){
                        prows[index][j] = prows[index + 1][j];
                    }
                    index++;
                }
                sizes[numberOfLines - 1] = 0;
                yLocation--;
            }
            printString(prows,sizes,numberOfLines);
            gotoxy(xLocation, yLocation);
            break;

        case ENTER:
            if (sizes[numberOfLines - 1] == 0){
                if (xLocation == sizes[yLocation]){
                    index = numberOfLines - 1;
                    for (int i = 0; i < numberOfLines - yLocation - 2; i++){
                        sizes[index] = sizes[index - 1];
                        for (int j = 0; j < sizes[index - 1]; j++){
                            prows[index][j] = prows[index - 1][j];
                        }
                        index--;
                    }
                    sizes[yLocation + 1] = 0;
                    xLocation = 0;
                    yLocation++;
                    printString(prows,sizes,numberOfLines);
                    gotoxy(xLocation, yLocation);
                } else {
                    index = numberOfLines - 1;
                    for (int i = 0; i < numberOfLines - yLocation - 2; i++){
                        sizes[index] = sizes[index - 1];
                        for (int j = 0; j < sizes[index - 1]; j++){
                            prows[index][j] = prows[index - 1][j];
                        }
                        index--;
                    }
                    sizes[yLocation + 1] = sizes[yLocation] - xLocation;
                    int rest = xLocation;
                    int counter = 0;
                    for (int i = 0; i < sizes[yLocation] - xLocation; i++){
                        prows[index][i] = prows[yLocation][rest];
                        counter++;
                        rest++;
                    }
                    sizes[yLocation] -= counter;
                    xLocation = 0;
                    yLocation++;
                    printString(prows,sizes,numberOfLines);
                    gotoxy(xLocation, yLocation);
                }
            }
            break;

        default:
            if (isInsert && xLocation != sizes[yLocation] && sizes[yLocation] != 0){
                index = ++sizes[yLocation];
                for(int i = 0; i < sizes[yLocation] - xLocation; i++){
                    prows[yLocation][index] = prows[yLocation][index - 1];
                    index--;
                }
                prows[yLocation][index] = userInput;
                printString(prows,sizes,numberOfLines);
                xLocation++;
            }else{
                prows[yLocation][xLocation] = userInput;
                if (xLocation == sizes[yLocation]){
                    sizes[yLocation]++;
                }
                printString(prows,sizes,numberOfLines);
                xLocation++;
            }
            break;
        }


    }while (userInput != ESC);

    for(int i = 0; i < numberOfLines; i++)
        free(prows[i]);
}


void printString(char * pcurrent[], int * sizes[], int numberOfLines){
    system("cls");
    for (int i = 0; i < numberOfLines; i++){
        for (int j = 0; j < sizes[i]; j++){
            printf("%c", * (pcurrent[i]+j) );
        }
        printf("\n");
    }
}




