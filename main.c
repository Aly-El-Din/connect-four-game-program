#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include <malloc.h>
#include <windows.h>
#include <conio.h>
int score_row_A = 0, score_row_B = 0, score_col_A = 0, score_col_B = 0, diag_score_A = 0, diag_score_B = 0, rev_diag_score_A = 0, rev_diag_score_B = 0;
int res, k = 1, pop_flag, start;
int choose_mode;
int temp1, temp2;
char** board;
char lastMove;
time_t t;
int startAgain = 1;
typedef struct
{
    char name[10000];
    int moves, score, rank, symbol;
}player;
typedef struct
{
    char name[10000];
    int score;
} information;
player plA, plB;
information top_players[10000];
///creating a stack with its functions
///stack which involves columns indexes in which undo has been done
///which will help us undo and redo 2 stacks the first stores all moves played
///and the second will store the undid moves so we can redo
int MAXSIZE;
int* stack2;
int top2 = 0;
int isempty2() {
    if (top2 == 0)
        return 1;
    else
        return 0;
}
int isfull2() {
    if (top2 == MAXSIZE)
        return 1;
    else
        return 0;
}
int peek2() {
    return stack2[top2];
}
int pop2() {
    int data2;
    if (!isempty2()) {
        data2 = stack2[top2];
        top2 = top2 - 1;
        return data2;
    }
    else {
        printf("Could not retrieve data, Stack is empty.\n");
    }
}
void push2(int data2) {
    if (!isfull2()) {
        top2 = top2 + 1;
        stack2[top2] = data2;
    }
    else {
        printf("Could not insert data, Stack is full.\n");
    }
}
///stack which involves each done move in the game so that we can undo in any time
int* stack1;
int top1 = 0;
int isempty1() {
    if (top1 == 0)
        return 1;
    else
        return 0;
}
int isfull1() {
    if (top1 == MAXSIZE)
        return 1;
    else
        return 0;
}
int peek1() {
    return stack1[top1];
}
int pop1() {
    int data1;
    if (!isempty1()) {
        data1 = stack1[top1];
        top1 = top1 - 1;
        return data1;
    }
    else {
        printf("Could not retrieve data, Stack is empty.\n");
    }
}
void push1(int data1) {
///colour functions
    if (!isfull1()) {
        top1 = top1 + 1;
        stack1[top1] = data1;
    }
    else {
        printf("Could not insert data, Stack is full.\n");
    }
}
void red()
{
    HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console_color, 4);
}
void lightBlue(){
    HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console_color, 9);
}
void cyan(){
    HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console_color, 3);
}
void green()
{
    HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console_color, 2);
}
void blue()
{
    HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console_color, 1);
}
void yellow()
{
    HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console_color, 6);
}
void reset()
{
    HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console_color, 15);
}
///function to start new game
void startNewGame() {
///function to end the game and save the name and score of the winner player
    int row;
    while (!isempty1()) {
        row = availableRow(stack1[top1], 1);
        board[row][stack1[top1]] = ' ';
        pop1();
    }
    plA.score=0;
    plB.score=0;
    plA.moves=0;
    plB.moves=0;
    if(choose_mode==2){
        k=1;
    }
    main();
}
void endOfGame(int score1, int score2)
{
///function converts the string name of the winner player to lowercase characters
        information player;
        if (score1 > score2)
        {
            lightBlue();
            printf("\nPlease enter your name:\n");
            reset();
            scanf(" %s",player.name);
            reset();
            convertToLower(player.name);
            player.score = score1;
        }
        else if(score2>score1){
            lightBlue();
            printf("\nPlease enter your name:\n");
            reset();
            scanf(" %s",player.name);
            reset();
            convertToLower(player.name);
            player.score = score2;
        }
        lightBlue();
        printf("saving your name to the top high scores...");
        reset();
        sleep(3);
        FILE* file;
        int number_of_saves = 0, identically = 0;
        if ((file = fopen("Topscores.binary", "rb")) == NULL)
        {
            red();
            printf("File doesn");
            sleep(3);
            reset();
            mainMenu();
        }
        while (fread(&top_players[number_of_saves], sizeof(information), 1, file))
        {
            if (!(strcmp(player.name, top_players[number_of_saves].name)))
            {
                if (player.score >= top_players[number_of_saves].score)
                    top_players[number_of_saves].score = player.score;
                identically = 1;
            }
            number_of_saves++;

        }
        if (identically == 0)
        {
            strcpy(top_players[number_of_saves].name, player.name);
            top_players[number_of_saves].score = player.score;
            number_of_saves++;
        }
        fclose(file);
        mergeSort(top_players, 0, number_of_saves - 1);
        FILE* file2;
        file2 = fopen("Topscores.binary", "wb");
        for (int i = 0; i < number_of_saves; ++i)
            fwrite(&top_players[i], sizeof(information), 1, file2);
        fclose(file2);
        startAgain=1;
        startNewGame();
}
void convertToLower(char name[])
{
///merge sorting algorithm
    int i = 0;
    while (name[i] != '\0')
    {
        name[i] = tolower(name[i]);
        ++i;
    }
}
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    /* create temporary arrays */
    int L[n1], R[n2];

    /* Copy data to temporary arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i] >= R[j]) {
            arr[k] = L[i++];
        }
        else {
            arr[k] = R[j++];
        }
        k++;
    }
    /* Copy the remaining elements of L[], if there
    are any */
    while (i < n1) {
        arr[k++] = L[i++];
    }
    /* Copy the remaining elements of R[], if there
    are any */
    while (j < n2) {
        arr[k++] = R[j++];
    }
}
void mergeSort(int arr[], int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}
int Index = 0;
///function represents the top 10 players
void Top_players(int number_of_top)
{

    FILE* file3;
    file3 = fopen("Topscores.binary", "r");
    while (fread(&top_players[Index], sizeof(information), 1, file3))
        Index++;
    blue();
    printf(" %-5s %-30s\n", "Score", "Name");
    reset();
    if (Index - 1 > number_of_top)
    {
        for (int i = Index - 1; i > Index - 1 - number_of_top; --i)
            printf(" %-5d %-30s\n", top_players[i].score, top_players[i].name);
    }
    else {
        for (int i = Index - 1; i >= 0; --i)
            printf(" %-5d %-30s\n", top_players[i].score, top_players[i].name);
    }
    fclose(file3);
}
bool FlagIn = false;
int ErrorFlag = 0;
int Width, Height, Highscore;
///xml file implementation
void xml()
{
///function searches for a string
    static int PathError = 0; char FilePath[200];
    if (!PathError)
        strcpy(FilePath, "mytesterfile.xml");
    else
    {
        if (PathError < 4)
        {
            printf("Error in configuration file , Enter the path to try again :");
            gets(FilePath);
        }
        else
        {
            Width = 6;
            Height = 7;
            Highscore = 10;
            return;
        }
    }
    char ch, MyFile[500], conf_1[] = "<Configurations>", width_1[] = "<Width>", height_1[] = "<Height>", highscores_1[] = "<Highscores>";
    char conf_2[] = "</Configurations>", width_2[] = "</Width>", height_2[] = "</Height>", highscores_2[] = "</Highscores>";
    int i = 0, ConfigStart, ConfigEnd, WidthStart, WidthEnd, HeightStart, HeightEnd, HighScoreStart, HighScoreEnd;
    FILE* file;
    if ((file = fopen(FilePath, "r")) == NULL) {
        PathError++;
        xml();
        return;
    }
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '<') {
            FlagIn = true;
        }
        else if (ch == '>') {
            FlagIn = true;
        }
        if (ch != ' ' && ch != '\n' && ch != '\t') {
            FlagIn = false;
            MyFile[i] = ch;
            i++;
        }
        else if (!FlagIn)
            MyFile[i++] = ch;
    }

    MyFile[i] = '\0';
    fclose(file);
    ConfigStart = SearchForString(conf_1, MyFile);
    ConfigEnd = SearchForString(conf_2, MyFile);
    int Range = ConfigEnd - ConfigStart + 1;

    if (ConfigStart == -1 || ConfigEnd == -1 || ConfigEnd < ConfigStart) {
        PathError++;
        return xml();
    }

    WidthStart = SearchForString(width_1, MyFile);
    WidthEnd = SearchForString(width_2, MyFile);
    int RangeA = WidthEnd - WidthStart + 1;

    if (WidthStart == -1 || WidthEnd == -1 || WidthEnd < WidthStart) {
        PathError++;
        return xml();
    }
    char width[RangeA];
    for (int p = 0, j = WidthStart + 7; j < WidthEnd; j++, p++)
    {
        ErrorFlag = 0;
        if (isdigit(MyFile[j]))
        {
            width[p] = MyFile[j];
        }
        else
        {
            ErrorFlag = 0;
            PathError++;
            break;
        }
    }
    if (ErrorFlag)
    {
        return xml();
    }
    width[WidthEnd] = '\0';
    HeightStart = SearchForString(height_1, MyFile);
    HeightEnd = SearchForString(height_2, MyFile);
    int RangeB = HeightEnd - HeightStart + 1;

    if (HeightStart == -1 || HeightEnd == -1 || HeightEnd < HeightStart) {
        PathError++;
        return xml();
    }
    char height[RangeB];
    for (int h = 0, v = HeightStart + 8; v < HeightEnd; v++, h++)
    {
        ErrorFlag = 0;
        if (isdigit(MyFile[v]))
        {
            height[h] = MyFile[v];
        }
        else
        {
            ErrorFlag = 0;
            PathError++;
            break;
        }
    }
    if (ErrorFlag)
    {
        return xml();
    }
    height[HeightEnd] = '\0';
    HighScoreStart = SearchForString(highscores_1, MyFile);
    HighScoreEnd = SearchForString(highscores_2, MyFile);
    int RangeC = HighScoreEnd - HighScoreStart + 1;

    if (HighScoreStart == -1 || HighScoreEnd == -1 || HighScoreEnd < HighScoreEnd) {
        PathError++;
        return xml();
    }
    char highscores[RangeC];
    for (int p = 0, j = HighScoreStart + 12; j < HighScoreEnd; j++, p++)
    {
        ErrorFlag = 0;
        if (isdigit(MyFile[j]))
        {
            highscores[p] = MyFile[j];
        }
        else
        {
            ErrorFlag = 0;
            PathError++;
            break;
        }
    }
    if (ErrorFlag)
    {
        return xml();
    }
    highscores[HighScoreEnd] = '\0';

    if (atoi(height) < 4 || atoi(width) < 4 || atoi(highscores) < 4)
    {
        PathError++;
        return xml();
    }
    Width = atoi(width);
    Height = atoi(height);
    Highscore = atoi(highscores);
}
int SearchForString(char s1[], char s2[]) {
    int counter = -1, i = 0;
    while (s2[i] != '\0') {
        int j = 0;
        if (s2[i] == s1[j]) {
            int k = i;
            while (s2[k] == s1[j] && s1[j] != '\0') {
                k++;
                j++;
            }
            if (s1[j] == '\0') {
                counter = i;
            }
        }
        i++;
    }
    return counter;
}
///the main menu
void instructions(){
    system("cls");
    char startChoice;
    FILE    *textfile;
    char    *text;
    long    numbytes;

    textfile = fopen("readme.txt", "r");
    if(textfile == NULL)
        return 1;
    fseek(textfile, 0L, SEEK_END);
    numbytes = ftell(textfile);
    fseek(textfile, 0L, SEEK_SET);
    text = (char*)calloc(numbytes, sizeof(char));
    if(text == NULL)
        return 1;
    fread(text, sizeof(char), numbytes, textfile);
    fclose(textfile);
    printf(text);
    do{
        scanf(" %c",&startChoice);
    }while(startChoice!='1');
    mainMenu();
}
void mainMenu()
{
    system("cls");
    char v, choose;
    cyan();
    printf("\t\t\t\t\twelcome to connect four\n\n\n\nChoose:\nA.Start Game\nB.Load a game\nC.Top scores\nD.quit\nE.Instructions\n");
    printf("What's your choice?\n");
    reset();
    do {
        scanf(" %c", &choose);
    } while (choose != 'A' &&choose != 'a' &&choose != 'b' && choose != 'c' &&choose != 'd' &&choose!='e' && choose != 'B' && choose != 'C' && choose != 'D' && choose!='E');
    choose=toupper(choose);
    switch (choose) {
    case 'A':
        if (startAgain == 1) {
            cyan();
            printf("Choose your mode\nA.Play with player\nB.play with computer\nor return to main menu by entering 'y'\n");
        }
        else{
            cyan();
            printf("Do you want a new game(empty grid)??\n choose 'y' for yes,'n' for no");
            char newAns;
            do{
                scanf(" %c",&newAns);
            }while(newAns!='y' && newAns!='n');
            if(newAns=='y'){
                printf("Choose your mode\nA.Play with player\nB.play with computer\nor return to main menu by entering 'y'\n");
            }
            else{
                loadToReturn();
            }
        }
        break;
    case 'B':
        reset();
        load();
        break;
    case 'C':Top_players(10);
        break;
    case 'D':quit();
        break;
    case 'E':instructions();
    default:
        printf("Enter a valid input");
    }
}
int main(void)
{
///functions calculate score
    static int startProg=1;
    if(startProg==1){
        xml();
        startProg++;
    }
    t = time(NULL);
    MAXSIZE = Height * Width;
    stack1 = (int*)malloc(Height * Width * sizeof(int));
    stack2 = (int*)malloc(Height * Width * sizeof(int));
    board = (char**)malloc(Height * sizeof(char*));
    for (int i = 0; i < Height; i++) {
        board[i] = (char*)malloc(Width * sizeof(char));
    }
    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Width; j++) {
            board[i][j] = ' ';
        }
    }
    char choice;
    mainMenu();
    do{
        scanf(" %c", &choice);
    }while(choice!='a' && choice!='A' && choice !='B' && choice!='b' && choice!='Y' && choice!='y');
    if (choice == 'A' || choice=='a') {
        printBoard();
        printf("\n\n\n");
        choose_mode = 1;
        updateBoard(t);
        end();
    }
    else if (choice == 'B' || choice=='b') {
        printBoard();
        printf("\n\n\n");
        choose_mode = 2;
        vsComputer(t);
        end();
    }
    else if(choice=='Y' || choice =='y'){
        mainMenu();
    }
    return 0;
}
void rev_diag_score(int DRC_A, int DRC_B, int Height, int Width, char** Board) {
    static total_score_A = 0, total_score_B = 0;
    int i, k;
    for (i = Height - 4; i >= 0; i--) {
        for (k = 0; k < Width - 4; k++) {
            if (Board[i][k] == 'x') {
                if (Board[i][k] == Board[i + 1][k + 1] && Board[i + 1][k + 1] == Board[i + 2][k + 2] && Board[i + 2][k + 2] == Board[i + 3][k + 3]) {
                    rev_diag_score_A++;
                }
            }
            else if (Board[i][k] == 'o') {
                if (Board[i][k] == Board[i + 1][k + 1] && Board[i + 1][k + 1] == Board[i + 2][k + 2] && Board[i + 2][k + 2] == Board[i + 3][k + 3]) {
                    rev_diag_score_B++;
                }

            }
            else {
                continue;
            }
        }
    }
    total_score_A = rev_diag_score_A + DRC_A;
    total_score_B = rev_diag_score_B + DRC_B;
    red();
    printf("Player A Score : %d\n", total_score_A);
    yellow();
    printf("Player B Score : %d\n", total_score_B);
    reset();
    score_row_A = 0;
    score_row_B = 0;
    score_col_A = 0;
    score_col_B = 0;
    diag_score_A = 0;
    diag_score_B = 0;
    rev_diag_score_A = 0;
    rev_diag_score_B = 0;
    plA.score = total_score_A;
    plB.score = total_score_B;
}
void diag_score(int sum_RC_A, int sum_RC_B, int Height, int Width, char** Board) {
    static sum_DRC_A = 0, sum_DRC_B = 0;
    int i, k;
    for (i = Height - 1; i >= 3; i--) {
        for (k = 0; k < Width - 3; k++) {
            if (Board[i][k] == 'x') {
                if (Board[i][k] == Board[i - 1][k + 1] && Board[i - 1][k + 1] == Board[i - 2][k + 2] && Board[i - 2][k + 2] == Board[i - 3][k + 3]) {
                    diag_score_A++;
                }
            }
            else if (Board[i][k] == 'o') {
                if (Board[i][k] == Board[i - 1][k + 1] && Board[i - 1][k + 1] == Board[i - 2][k + 2] && Board[i - 2][k + 2] == Board[i - 3][k + 3]) {
                    diag_score_B++;
                }
            }
            else {
                continue;
            }
        }
    }
    sum_DRC_A = sum_RC_A + diag_score_A;
    sum_DRC_B = sum_RC_B + diag_score_B;
    rev_diag_score(sum_DRC_A, sum_DRC_B, Height, Width, Board);
}
void col_score(int row_score_A, int row_score_B, int Height, int Width, char** Board) {
    int i, j, k;
    static int sum_A = 0, sum_B = 0;
    for (i = Height - 4; i >= 0; i--) {
        for (k = 0; k < Width; k++) {
            if (Board[i][k] == 'x') {
                if (Board[i][k] == Board[i + 1][k] && Board[i + 1][k] == Board[i + 2][k] && Board[i + 2][k] == Board[i + 3][k]) {
                    score_col_A++;
                }
            }
            else if (Board[i][k] == 'o') {
                if (Board[i][k] == Board[i + 1][k] && Board[i + 1][k] == Board[i + 2][k] && Board[i + 2][k] == Board[i + 3][k]) {
                    score_col_B++;
                }
            }
            else {
                continue;
            }
        }
    }
    sum_A = row_score_A + score_col_A;
    sum_B = row_score_B + score_col_B;
    diag_score(sum_A, sum_B, Height, Width, Board);
}
void row_score(int Height, int Width, char** Board) {
    int i, j, k;
    for (i = Height - 1; i >= 0; i--) {
        for (k = 0; k < Width - 3; k++) {
            if (Board[i][k] == 'x') {
                if (Board[i][k] == Board[i][k + 1] && Board[i][k + 1] == Board[i][k + 2] && Board[i][k + 2] == Board[i][k + 3]) {
                    score_row_A++;
                }
            }
            else if (Board[i][k] == 'o') {
                if (Board[i][k] == Board[i][k + 1] && Board[i][k + 1] == Board[i][k + 2] && Board[i][k + 2] == Board[i][k + 3]) {
                    score_row_B++;
                }
            }
            else {
                continue;
            }
        }
    }
    col_score(score_row_A, score_row_B, Height, Width, Board);
}
///function ends the game
void end() {
    char answer[5];
    int ans;
    char winnername[30];
    static int undo_count = 1;
        if (undo_count % 2 == 0 && Height!=Width) {
            ans = 2;
        }
        else {
            lightBlue();
            printf("Warning! the grid is full\nDo you want to\nend:press 1\nundo:press 2\n");
            reset();
            do{
                scanf(" %s",&answer);
                ans=atoi(answer);
            }while(ans!=1 && ans!=2);
        }
        if (choose_mode == 1 && ans == 1) {
            if (plA.score > plB.score) {
                printf("Winner is player 1\n");
                endOfGame(plA.score, plB.score);
            }
            else if (plA.score < plB.score) {
                printf("Winner is player 2\n");
                endOfGame(plA.score, plB.score);
            }
            else if (plA.score == plB.score) {
                printf("\n\nno winner\n");
                sleep(5);
                endOfGame(plA.score, plB.score);
            }
        }
        else if (choose_mode == 2 && ans == 1) {
            if (plA.score > plB.score) {
                printf("\nWinner is player 1\n");
                endOfGame(plA.score, plB.score);
            }
            else if (plA.score < plB.score) {
                printf("\nWinner is the computer!!\n");
                endOfGame(plA.score, plB.score);
            }
            else if (plA.score == plB.score) {
                printf("\n\n no winner\n");
                endOfGame(plA.score, plB.score);
            }
        }
        else if (ans == 2 && choose_mode == 1) {
            res = availableRow(stack1[top1], 1);
            push2(stack1[top1]);
            k++;
            undo(res, stack1[top1], k, 0);
            pop_flag = 1;
            updateBoard(t);
        }
        else if (ans == 2 && choose_mode == 2) {
            res = availableRow(stack1[top1], 1);
            push2(stack1[top1]);
            undo_count++;
            if(Width!=Height && lastMove!='x'){
                undo(res, stack1[top1], k, 1);
            }
            else{
                k--;
                undo(res, stack1[top1], k, 0);
            }
            pop_flag = 1;
            k = 1;
            vsComputer(t);
        }
}
void quit()
{
///function which prints the board
    printf("Good bye\n");
    exit(0);
}
void printBoard() {
///redo and undo algorithms using stack
    system("cls");
    printf("\n\n\n");
    if (!start) {
        for (int i = Height - 1; i >= 0; i--)
        {
            printf("\t\t\t\t");
            for (int j = Width; j >= 0; j--)
            {
                green();
                printf(" | ");
            }
            printf(" | \n");
        }
    }
    else {
            printf("\n\n\n");
        for (int i = Height - 1; i >= 0; i--)
        {
            printf("\t\t\t\t");
            for (int j = 0; j < Width; j++)
            {
                green();
                printf("|");
                reset();
                if (board[i][j] == 'x') {
                    red();
                }
                else if (board[i][j] == 'o')
                {
                    yellow();
                }
                printf(" %c ", board[i][j]);
                reset();
            }
            green();
            printf("|\n");
            reset();
        }
    }
    start++;
}
void redo(int row, int col, int turn, int comp_redo) {
    pop2();
    if (turn % 2 == 1) {
        board[row][col] = 'x';
        printBoard();
        row_score(Height, Width, board);
        if (comp_redo == 1) {
            plA.moves++;
            red();
            printf("playerA moves  : %d\n", plA.moves);
            yellow();
            printf("playerB moves  : %d\n", plB.moves);
            reset();
            blue();
            clockFn(time(NULL)-t);
            reset();
        }
        else {
            plA.moves++;
            red();
            printf("playerA moves  : %d\n", plA.moves);
            yellow();
            printf("playerB moves  : %d\n", plB.moves);
            reset();
            blue();
            clockFn(time(NULL)-t);
            reset();
        }
    }
    else {
        board[row][col] = 'o';
        plB.moves++;
        printBoard();
        row_score(Height, Width, board);
        red();
        printf("playerA moves  : %d\n", plA.moves);
        yellow();
        printf("playerB moves  : %d\n", plB.moves);
        reset();
        blue();
        clockFn(time(NULL)-t);
        reset();
    }

}
void undo(int row, int col, int turn, int comp_undo) {
    static int comp_undo_counter = 0;
    board[row][col] = ' ';
    pop1();
    if (comp_undo == 1) {
        comp_undo_counter++;
    }
    if (comp_undo == 1 && comp_undo_counter % 2 == 0) {
        printBoard();
        plA.moves--;
        plB.moves--;
        row_score(Height, Width, board);
        red();
        printf("playerA moves  : %d\n", plA.moves);
        yellow();
        printf("playerB moves  : %d\n", plB.moves);
        reset();
        pop_flag = 1;
        blue();
        clockFn(time(NULL)-t);
        reset();
    }
    else if (comp_undo == 0) {
        printBoard();
        if (turn % 2 == 1) {
            plA.moves--;
        }
        else {
            plB.moves--;
        }
        row_score(Height, Width, board);
        red();
        printf("playerA moves  : %d\n", plA.moves);
        yellow();
        printf("playerB moves  : %d\n", plB.moves);
        reset();
        blue();
        clockFn(time(NULL)-t);
        reset();
    }
}
int arr[10000] = { 0 };
///function returns the available row to avoid overflow in a certain column
int availableRow(int col, int undo_flag) {
    if (arr[col] < Height && undo_flag == 0) {
        arr[col]++;
        return arr[col] - 1;
    }
    else if (arr[col] <= Height && undo_flag == 1) {
        arr[col]--;
        return arr[col];
    }
    else {
        return -1;
    }
}
///save and load functions
///function saveToReturn is specific to saving the played game while returning to the main menu and this handles that the player may replay his game again
void saveToReturn(){
        FILE* save4;
        save4 = fopen("save4.bin", "wb");
        for (int i = 0; i < Height; i++) {
            fwrite(board[i], sizeof(board[i]), 1, save4);
        }
        fwrite(arr, sizeof(arr), 1, save4);
        fwrite(&plA.moves, sizeof(plA.moves), 1, save4);
        fwrite(&plB.moves, sizeof(plB.moves), 1, save4);
        fwrite(&plA.score, sizeof(plA.moves), 1, save4);
        fwrite(&plB.score, sizeof(plB.moves), 1, save4);
        fwrite(&choose_mode, sizeof(choose_mode), 1, save4);
        fwrite(&top1,sizeof(top1),1,save4);
        for(int j=1;j<=top1;j++){
            fwrite(&stack1[j], sizeof(stack1[j]), 1, save4);
        }
        fclose(save4);
}
///only for saving
void save() {
    char fileno;
    char choice2;
    system("cls");
    lightBlue();
    printf("choose a slot to save game in:\n1)slot1\n2)slot2\n3)slot3");
    reset();
    do{
        scanf(" %c",&fileno);
    }while(fileno!='1' && fileno!='2' && fileno!='3');
    system("cls");
    if (fileno == '1') {
        FILE* save1;
        save1 = fopen("save1.bin", "wb");
        for (int i = 0; i < Height; i++) {
            fwrite(board[i], sizeof(board[i]), 1, save1);
        }
        fwrite(arr, sizeof(arr), 1, save1);
        fwrite(&plA.moves, sizeof(plA.moves), 1, save1);
        fwrite(&plB.moves, sizeof(plB.moves), 1, save1);
        fwrite(&plA.score, sizeof(plA.moves), 1, save1);
        fwrite(&plB.score, sizeof(plB.moves), 1, save1);
        fwrite(&choose_mode, sizeof(choose_mode), 1, save1);
        fwrite(&top1,sizeof(top1),1,save1);
        for(int j=1;j<=top1;j++){
            fwrite(&stack1[j], sizeof(stack1[j]), 1, save1);
        }
        fclose(save1);
        lightBlue();
        printf("saving...\n");
        reset();
        sleep(1);
    }
    else if (fileno == '2') {
        FILE* save2;
        save2 = fopen("save2.bin", "wb");
        for (int i = 0; i < Height; i++) {
            fwrite(board[i], sizeof(board[i]), 1, save2);
        }
        fwrite(arr, sizeof(arr), 1, save2);
        fwrite(&plA.moves, sizeof(plA.moves), 1, save2);
        fwrite(&plB.moves, sizeof(plB.moves), 1, save2);
        fwrite(&plA.score, sizeof(plA.moves), 1, save2);
        fwrite(&plB.score, sizeof(plB.moves), 1, save2);
        fwrite(&choose_mode, sizeof(choose_mode), 1, save2);
        fwrite(&top1,sizeof(top1),1,save2);
        for(int j=1;j<=top1;j++){
            fwrite(&stack1[j], sizeof(stack1[j]), 1, save2);
        }
        fclose(save2);
        lightBlue();
        printf("saving...\n");
        reset();
        sleep(1);
    }
    else if (fileno == '3') {
        FILE* save3;
        save3 = fopen("save3.bin", "wb");
        for (int i = 0; i < Height; i++) {
            fwrite(board[i], sizeof(board[i]), 1, save3);
        }
        fwrite(arr, sizeof(arr), 1, save3);
        fwrite(&plA.moves, sizeof(plA.moves), 1, save3);
        fwrite(&plB.moves, sizeof(plB.moves), 1, save3);
        fwrite(&plA.score, sizeof(plA.moves), 1, save3);
        fwrite(&plB.score, sizeof(plB.moves), 1, save3);
        fwrite(&choose_mode, sizeof(choose_mode), 1, save3);
        fwrite(&top1,sizeof(top1),1,save3);
        for(int j=1;j<=top1;j++){
            fwrite(&stack1[j], sizeof(stack1[j]), 1, save3);
        }
        fclose(save3);
        lightBlue();
        printf("saving...\n");
        reset();
        sleep(1);
    }
    printBoard();
    if (choose_mode == 1) {
        updateBoard(t);
    }
    else {
        vsComputer(t);
    }
}
///loading after the player has chosen to play with the original board and not restarting the game after returning to main menu
void loadToReturn(){
        FILE* save4;
        save4 = fopen("save4.bin", "rb");
        if(save4==NULL){
            lightBlue();
            printf("File does not exist");
            reset();
            mainMenu();
            return;
        }
        for (int i = 0; i < Height; i++) {
            fread(board[i], sizeof(board[i]), 1, save4);
        }
        fread(arr, sizeof(arr), 1, save4);
        fread(&plA.moves, sizeof(plA.moves), 1, save4);
        fread(&plB.moves, sizeof(plB.moves), 1, save4);
        fread(&plA.score, sizeof(plA.moves), 1, save4);
        fread(&plB.score, sizeof(plB.moves), 1, save4);
        fread(&choose_mode, sizeof(choose_mode), 1, save4);
        fread(&top1,sizeof(top1),1,save4);
        for(int j=1;j<=top1;j++){
            fread(&stack1[j], sizeof(stack1[j]), 1, save4);
        }
        fclose(save4);
        if (choose_mode == 1) {
        printBoard();
        updateBoard(t);
    }
    else {
        printBoard();
        vsComputer(t);
    }
}
///only for loading
void load() {
    char loadfile;
    lightBlue();
    printf("Enter the file you want to load(1/2/3)");
    reset();
    do{
        scanf(" %c", &loadfile);
    }while(loadfile!='1' && loadfile !='2' && loadfile!='3');
    start++;
    if (loadfile == '1') {
        lightBlue();
        printf("loading...\n");
        sleep(2);
        reset();
        FILE* save1;
        save1 = fopen("save1.bin", "rb");
        if(save1==NULL){
            lightBlue();
            printf("There is no saved game in this file..");
            reset();
            sleep(3);
            mainMenu();
            return;
        }
        for (int i = 0; i < Height; i++) {
            fread(board[i], sizeof(board[i]), 1, save1);
        }
        fread(arr, sizeof(arr), 1, save1);
        fread(&plA.moves, sizeof(plA.moves), 1, save1);
        fread(&plB.moves, sizeof(plB.moves), 1, save1);
        fread(&plA.score, sizeof(plA.moves), 1, save1);
        fread(&plB.score, sizeof(plB.moves), 1, save1);
        fread(&choose_mode, sizeof(choose_mode), 1, save1);
        fread(&top1,sizeof(top1),1,save1);
        for(int j=1;j<=top1;j++){
            fread(&stack1[j], sizeof(stack1[j-1]), 1, save1);
        }

        //fread(&stack1[top1 - 1], sizeof(stack1[top1 - 1]), 1, save1);
        fclose(save1);
    }
    else if (loadfile == '2') {
        lightBlue();
        printf("loading...\n");
        reset();
        FILE* save2;
        save2 = fopen("save2.bin", "rb");
        if(save2==NULL){
            lightBlue();
            printf("There is no saved game in this file..");
            reset();
            sleep(3);
            mainMenu();
            return;
        }
        for (int i = 0; i < Height; i++) {
            fread(board[i], sizeof(board[i]), 1, save2);
        }
        fread(arr, sizeof(arr), 1, save2);
        fread(&plA.moves, sizeof(plA.moves), 1, save2);
        fread(&plB.moves, sizeof(plB.moves), 1, save2);
        fread(&plA.score, sizeof(plA.moves), 1, save2);
        fread(&plB.score, sizeof(plB.moves), 1, save2);
        fread(&stack1[top1], sizeof(stack1[top1]), 1, save2);
        fread(&stack1[top1 - 1], sizeof(stack1[top1 - 1]), 1, save2);
        fread(&choose_mode, sizeof(choose_mode), 1, save2);
        fclose(save2);
    }
    else if (loadfile == '3') {
        lightBlue();
        printf("loading...\n");
        sleep(1);
        reset();
        FILE* save3;
        save3 = fopen("save3.bin", "rb");
        if(save3==NULL){
            lightBlue();
            printf("There is no saved game in this file..");
            reset();
            sleep(3);
            mainMenu();
            return;
        }
        for (int i = 0; i < Height; i++) {
            fread(board[i], sizeof(board[i]), 1, save3);
        }
        fread(arr, sizeof(arr), 1, save3);
        fread(&plA.moves, sizeof(plA.moves), 1, save3);
        fread(&plB.moves, sizeof(plB.moves), 1, save3);
        fread(&plA.score, sizeof(plA.moves), 1, save3);
        fread(&plB.score, sizeof(plB.moves), 1, save3);
        fread(&stack1[top1], sizeof(stack1[top1]), 1, save3);
        fread(&stack1[top1 - 1], sizeof(stack1[top1 - 1]), 1, save3);
        fread(&choose_mode, sizeof(choose_mode), 1, save3);
        fclose(save3);
    }
    if (choose_mode == 1) {
        printBoard();
        updateBoard(t);
    }
    else {
        printBoard();
        vsComputer(t);
    }
}
char turn1[25], turn2[25];
///the most essential functions in the game which controls the whole playing of 1 vs 1 player
void updateBoard(long long t) {
    static int count1 = 1;
    for (int i = 0; i < Height; i++) {
        for (int j = 1; j <= Width; j++) {
            if (plA.moves + plB.moves == Height * Width) {
                end();
            }
            else {
                if (k % 2) {
                    red();
                    printf("player 1's turn or undo by pressing 'u' or redo by pressing 'r' or save by typing 'save':");///availableRowing the validation if input of player 1
                    reset();
                    do {
                        scanf(" %s", &turn1);
                        temp1 = atoi(turn1);
                        if (temp1 == 0) {
                            if (strcmp(turn1, "0") == 0) {
                                temp1 -= 1;
                            }
                            else if (strcmp(turn1, "yes") == 0) {
                                saveToReturn();
                                startAgain = 0;
                                startNewGame();
                            }
                            else if (strcmp(turn1, "save") == 0) {
                                save();
                            }
                            else if (strcmp(turn1, toupper("u")) == 0) { ///undo Algorithm
                                if (isempty1() == 0) {
                                    pop_flag = 1;///indicator for occurence of undo
                                    res = availableRow(stack1[top1], 1);///row calculating
                                    if (res != -1) {
                                        push2(stack1[top1]); ///the undid move is pushed into another stack
                                        undo(res, stack1[top1], 2, 0);
                                    }
                                    else {
                                        availableRow(temp2, 1);
                                        push2(stack1[top1]);
                                        undo(Height - count1, stack1[top1], 2, 0);///handling undo in overflow
                                        count1++;
                                    }
                                    k++;
                                    updateBoard(t);
                                }
                                else {
                                    lightBlue();
                                    printf("you can't undo an empty grid!!\n");
                                    reset();
                                    updateBoard(t);
                                }
                            }
                            else if (!strcmp(turn1, toupper("r")) && pop_flag == 1) {
                                if (isempty2() == 0) {///if stack2 is empty this means that no undo has happened
                                    res = availableRow(stack2[top2], 0);///so we must not redo
                                    push1(stack2[top2]);
                                    redo(res, stack2[top2], 1, 0);
                                    k++;
                                    updateBoard(t);
                                }
                                else {
                                    lightBlue();
                                    printf("you can't redo without undo\n");
                                    reset();
                                    updateBoard(t);
                                }
                            }
                            else if (!strcmp(turn1, toupper("r")) && pop_flag == 0) {
                                    lightBlue();
                                printf("redo is not available\n");///if there was a move after undo, redo is restricted
                                reset();
                                updateBoard(t);
                            }
                            else {
                                lightBlue();
                                printf("enter a valid input\n");
                                reset();
                                updateBoard(t);
                            }
                        }
                        else {
                            temp1 -= 1;
                        }
                        if (temp1 >= Width || temp1 <0) {
                            lightBlue();
                            printf("the column is not avaliable !\nEnter Again!\n");///availableRowing the validation if input of player 2
                            reset();
                        }
                    } while (temp1 >= Width || temp1 <0);
                    res = availableRow(temp1, 0);///availableRowing the overflow in column
                    if (res != -1) {
                        push1(temp1);
                        board[res][temp1] = 'x';
                        plA.moves++;
                        printBoard();
                        row_score(Height, Width, board);
                        red();
                        printf("playerA moves  : %d\n", plA.moves);
                        yellow();
                        printf("playerB moves  : %d\n", plB.moves);
                        k++;
                        pop_flag = 0;
                        blue();
                        clockFn(time(NULL) - t);
                        reset();
                    }
                    else {  ///overflow in a certain column
                        lightBlue();
                        printf("out of scope!\n");
                        reset();
                        do {
                            red();
                            printf("please enter another column or undo\npress 'u':for undo\nor enter another column");
                            reset();
                            scanf(" %s", &turn1);
                            temp1 = atoi(turn1);
                            if (temp1 == 0) {
                                if (strcmp(turn1, "0") == 0) {
                                    temp1 -= 1;
                                }
                                else if (strcmp(turn1, "yes") == 0) {
                                  saveToReturn();
                                startAgain = 0;
                                startNewGame();
                                }
                                else if (strcmp(turn1, "save") == 0) {
                                    save();
                                }
                                else if (strcmp(turn1, toupper("u")) == 0) {
                                if (isempty1() == 0) {
                                    res = availableRow(stack1[top1], 1);
                                    if (res != -1) {
                                        push2(stack1[top1]);
                                        undo(res, stack1[top1], 2, 0);
                                        pop_flag = 1;
                                    }
                                    else {
                                        availableRow(temp2, 1);
                                        push2(stack1[top1]);
                                        undo(Height - count1, stack1[top1], 2, 0);
                                        count1++;
                                        pop_flag = 1;
                                    }
                                    k++;
                                    updateBoard(t);
                                }
                                else {
                                    lightBlue();
                                    printf("you can't undo an empty grid!!\n");
                                    reset();
                                    updateBoard(t);
                                }
                            }
                            else if(!strcmp(turn1,"r")){
                               lightBlue();
                                printf("You can't redo without undo\n");
                                reset();
                            }
                            else {
                                    lightBlue();
                                    printf("enter a valid input\n");
                                    reset();
                                    updateBoard(t);
                                }
                            }
                            else {
                                temp1 -= 1;
                            }
                            res = availableRow(temp1, 0);
                            if (temp1 >= Width || temp1 < 0) {
                                lightBlue();
                                printf("the column is not avaliable !\nEnter Again!\n");
                                reset();
                            }
                        } while (res == -1 || temp1 >= Width || temp1 < 0);
                        push1(temp1);
                        board[res][temp1] = 'x';
                        plA.moves++;
                        printBoard();
                        row_score(Height, Width, board);
                        red();
                        printf("playerA moves  : %d\n", plA.moves);
                        yellow();
                        printf("playerB moves  : %d\n", plB.moves);
                        reset();
                        k++;
                        pop_flag = 0;
                        blue();
                        clockFn(time(NULL) - t);
                        reset();
                    }
                }
                else if (k % 2 == 0) { ///same handling as we have done in player 1
                    yellow();
                    printf("player 2's turn or undo by pressing 'u' or redo by pressing 'r' or save by typing 'save':");///availableRowing the validation if input of player 1
                    reset();
                    do {
                        scanf(" %s", &turn2);
                        temp2 = atoi(turn2);
                        if (temp2 == 0) {
                            if (strcmp(turn2, "0") == 0) {
                                temp2 -= 1;
                            }
                            else if (strcmp(turn2, "yes") == 0) {
                                saveToReturn();
                                startAgain = 0;
                                startNewGame();
                            }
                            else if (strcmp(turn2, "save") == 0) {

                                save();
                            }
                            else if (strcmp(turn2, toupper("u")) == 0) {
                                if (isempty1() == 0) {
                                    pop_flag = 1;
                                    res = availableRow(stack1[top1], 1);
                                    if (res != -1) {
                                        push2(stack1[top1]);
                                        undo(res, stack1[top1], 1, 0);
                                    }
                                    else {
                                        availableRow(temp1, 1);
                                        undo(Height - count1, stack1[top1], 1, 0);
                                        count1++;
                                    }
                                    k++;
                                    updateBoard(t);
                                }
                                else {
                                    lightBlue();
                                    printf("you can't undo an empty grid!!\n");
                                    reset();
                                    updateBoard(t);
                                }
                            }
                            else if (!strcmp(turn2, toupper("r")) && pop_flag == 1) {
                                if (isempty2() == 0) {
                                    res = availableRow(stack2[top2], 0);
                                    push1(stack2[top2]);
                                    redo(res, stack2[top2], 2, 0);
                                    k++;
                                    updateBoard(t);
                                }
                                else {
                                    lightBlue();
                                    printf("you can't redo without undo\n");
                                    reset();
                                    updateBoard(t);
                                }
                            }
                            else if (!strcmp(turn2, toupper("r")) && pop_flag == 0) {
                                lightBlue();
                                printf("redo is not available\n");
                                reset();
                                updateBoard(t);
                            }
                            else {
                                lightBlue();
                                printf("enter a valid input\n");
                                reset();
                                updateBoard(t);
                            }

                        }
                        else {
                            temp2 -= 1;
                        }
                        if (temp2 >= Width || temp2 < 0) {
                            lightBlue();
                            printf("the column is not avaliable!\nEnter Again!\n");///availableRowing the validation if input of player 2
                            reset();
                        }

                    } while (temp2 >= Width || temp2 < 0);
                    res = availableRow(temp2, 0);
                    if (res != -1) {
                        push1(temp2);
                        board[res][temp2] = 'o';
                        pop_flag = 0;
                        plB.moves++;
                        printBoard();
                        row_score(Height, Width, board);
                        red();
                        printf("playerA moves  : %d\n", plA.moves);
                        yellow();
                        printf("playerB moves  : %d\n", plB.moves);
                        reset();
                        k++;
                        pop_flag = 0;
                        blue();
                        clockFn(time(NULL) - t);
                        reset();
                    }
                    else {
                        lightBlue();
                        printf("out of scope!\n");
                        reset();
                        do {
                            yellow();
                            printf("please enter another column or undo\npress 'u':for undo\nor enter another column");
                            reset();
                            scanf(" %s", &turn2);
                            temp2 = atoi(turn2);
                            if (temp2 == 0) {
                                if (strcmp(turn2, "0") == 0) {
                                    temp2 -= 1;
                                }
                                else if (strcmp(turn2, "yes") == 0) {
                                    saveToReturn();
                                startAgain = 0;
                                startNewGame();
                                }
                                else if (strcmp(turn1, "save") == 0) {
                                    save();
                                }
                                else if(!strcmp(turn1,"r")){
                                    lightBlue();
                                    printf("You can't redo without undo\n");
                                    reset();
                                }
                                else if (!strcmp(turn2,toupper("u"))) {
                                if (isempty1() == 0) {
                                    res = availableRow(stack1[top1], 1);
                                    if (res != -1) {
                                        push2(stack1[top1]);
                                        undo(res, stack1[top1], 1, 0);
                                        pop_flag = 1;
                                    }
                                    else {
                                        availableRow(temp1, 1);
                                        push2(stack1[top1]);
                                        undo(Height - count1, stack1[top1], 1, 0);
                                        count1++;
                                        pop_flag = 1;
                                    }
                                    k++;
                                    updateBoard(t);
                                }
                                else {
                                    lightBlue();
                                    printf("you can't undo an empty grid!!\n");
                                    reset();
                                    updateBoard(t);
                                }
                            }
                                else {
                                    lightBlue();
                                    printf("enter a valid input\n");
                                    reset();
                                    updateBoard(t);
                                }
                            }
                            else {
                                temp2 -= 1;
                            }
                            res = availableRow(temp2, 0);
                            if (temp2 >= Width || temp2 < 0) {
                                lightBlue();
                                printf("the column is not avaliable !\n");
                                printf("Enter Again!\n");
                                reset();
                            }

                        } while (res == -1 || temp2 >= Width || temp2 < 0);
                        push1(temp2);
                        board[res][temp2] = 'o';
                        plB.moves++;
                        printBoard();
                        row_score(Height, Width, board);
                        yellow();
                        printf("playerA moves  : %d\n", plA.moves);
                        yellow();
                        printf("playerB moves  : %d\n", plB.moves);
                        reset();
                        k++;
                        pop_flag = 0;
                        blue();
                        clockFn(time(NULL) - t);
                        reset();
                    }
                }
            }
        }
    }
}
///function randomizes the turn of the computer
int random(int upper)
{
    srand(time(0));
    int num = (rand() % (upper)) ;
    return num;
}
///function controls the whole game flow of player vs computer
void vsComputer(long long t) {
    static int count1 = 1;
    int comp_col, comp_undo = 0, comp_redo = 0, res2;
    for (int i = 0; i < Height; i++) {
        for (int j = 1; j <= Width; j++) {
            if (plA.moves + plB.moves == Height * Width) {
                lastMove=board[res][temp1];
                end();
            }
            else {
                if (k % 2) {
                    red();
                    printf("player 1's turn or undo by pressing 'u' or redo by pressing 'r' or save by typing 'save':");///availableRowing the validation if input of player 1
                    reset();
                    do {
                        scanf(" %s", &turn1);
                        temp1 = atoi(turn1);
                        if (temp1 == 0) {
                            if (strcmp(turn1, "0") == 0) {
                                temp1 -= 1;
                            }
                            else if (strcmp(turn1, "yes") == 0) {
                                saveToReturn();
                                startAgain = 0;
                                startNewGame();
                            }
                            else if (strcmp(turn1, "save") == 0) {
                                save();
                            }
                            else if (!strcmp(turn1, toupper("u"))) {
                                if (!isempty1()) {
                                    res = availableRow(stack1[top1], 1);
                                    push2(stack1[top1 - 1]);
                                    undo(res, stack1[top1], 2, 1);
                                    if(!isempty1()){
                                        res2 = availableRow(stack1[top1], 1);
                                        pop_flag = 1;///availableRowing the last undo
                                        if (res2 != -1) {
                                            undo(res2, stack1[top1], 2, 1);
                                        }
                                        else {
                                            undo(Height - count1, stack1[top1], 2, 1);
                                            count1++;
                                        }
                                    }
                                    k++;
                                }
                                else {
                                    lightBlue();
                                    printf("you can't undo an empty grid\n");
                                    reset();
                                    vsComputer(t);
                                }
                                k++;
                                vsComputer(t);
                            }
                            else if (!strcmp(turn1, toupper("r")) && pop_flag == 1) {
                                if (isempty2() == 0) {
                                    res = availableRow(stack2[top2], 0);
                                    if(res==-1){
                                        lightBlue();
                                        printf("this move can't be redone because of over flow\n");
                                        reset();
                                        vsComputer(t);
                                    }
                                    else{
                                        push1(stack2[top2]);
                                        redo(res, stack2[top2], 1, 1);
                                        k++;
                                        vsComputer(t);
                                    }
                                }
                                else {
                                    lightBlue();
                                    printf("you can't redo without undo\n");
                                    reset();
                                    vsComputer(t);
                                }
                            }
                            else if (!strcmp(turn1, toupper("r")) && pop_flag == 0) {
                                lightBlue();
                                printf("Redo is not available after a new move!\n");
                                reset();
                                vsComputer(t);
                            }
                            else {
                                lightBlue();
                                printf("enter a valid input\n");
                                reset();
                                vsComputer(t);
                            }
                        }
                        else {
                            temp1 -= 1;
                        }
                        if (temp1 >=Width || temp1 < 0) {
                            lightBlue();
                            printf("the column is not avaliable !\nEnter Again!\n");///availableRowing the validation if input of player 2
                            reset();
                        }
                    } while (temp1 >= Width || temp1 < 0);
                    res = availableRow(temp1, 0);///availableRowing the overflow in column
                    if (res != -1) {
                        push1(temp1);
                        board[res][temp1] = 'x';
                        plA.moves++;
                        printBoard();
                        row_score(Height, Width, board);
                        red();
                        printf("playerA moves  : %d\n", plA.moves);
                        yellow();
                        printf("playerB moves  : %d\n", plB.moves);
                        reset();
                        k++;
                        pop_flag = 0;
                        blue();
                        clockFn(time(NULL) - t);
                        reset();
                    }
                    else {  ///overflow in a certain column
                        lightBlue();
                        printf("out of scope!\n");
                        reset();
                        do {
                            red();
                            printf("please enter another column or undo\n press 'u':for undo or enter another column");
                            reset();
                            scanf(" %s", &turn1);
                            temp1 = atoi(turn1);
                            if (temp1 == 0) {
                                if (strcmp(turn1, "0") == 0) {
                                    temp1 -= 1;
                                }
                                else if (strcmp(turn1, "yes") == 0) {
                                    saveToReturn();
                                startAgain = 0;
                                startNewGame();
                                }
                                else if (strcmp(turn1, "save") == 0) {
                                    save();
                                }
                            else if (strcmp(turn1, toupper("u")) == 0) {
                                if (isempty1() == 0) {
                                    res = availableRow(stack1[top1], 1);
                                    if (res != -1) {
                                        push2(stack1[top1]);
                                        undo(res, stack1[top1], 2, 0);
                                        pop_flag = 1;
                                    }
                                    else {
                                        availableRow(temp2, 1);
                                        push2(stack1[top1]);
                                        undo(Height - count1, stack1[top1], 2, 0);
                                        count1++;
                                        pop_flag = 1;
                                    }
                                    k++;
                                    vsComputer(t);
                                }
                                 else {
                                    lightBlue();
                                    printf("you can't undo an empty grid!!\n");
                                    reset();
                                    vsComputer(t);
                                }
                            }
                            else if (strcmp(turn1, toupper("r")) == 0){
                                    lightBlue();
                                    printf("you can't redo without undo\n");
                                    reset();
                                    vsComputer(t);
                            }
                              else {
                                    lightBlue();
                                    printf("enter a valid input\n");
                                    reset();
                                    vsComputer(t);
                                }
                            }
                            else {
                                temp1 -= 1;
                            }
                            res = availableRow(temp1, 0);
                            if (temp1 >= Width || temp1 < 0) {
                                lightBlue();
                                printf("the column is not avaliable !\nEnter Again!\n");
                                reset();
                            }
                        } while (res == -1 || temp1 >= Width || temp1 < 0);
                        push1(temp1);
                        board[res][temp1] = 'x';
                        plA.moves++;
                        printBoard();
                        row_score(Height, Width, board);
                        red();
                        printf("playerA moves  : %d\n", plA.moves);
                        yellow();
                        printf("playerB moves  : %d\n", plB.moves);
                        reset();
                        k++;
                        pop_flag = 0;
                        blue();
                        clockFn(time(NULL) - t);
                        reset();
                    }
                }
                else {
                    do {
                        if (isfull1() == 0) {
                            comp_col = random(Width);
                            res = availableRow(comp_col, 0);
                        }
                        else {
                            end();
                        }
                    } while (res == -1);
                    push1(comp_col);
                    board[res][comp_col] = 'o';
                    printBoard();
                    row_score(Height, Width, board);
                    plB.moves++;
                    red();
                    printf("playerA moves  : %d\n", plA.moves);
                    yellow();
                    printf("computer moves  : %d\n", plB.moves);
                    reset();
                    k++;
                    blue();
                    clockFn(time(NULL) - t);
                    reset();
                }
            }
        }
    }
}
void clockFn(long long sec) {
    int h = (sec / 3600);
    int m = (sec - (3600 * h)) / 60;
    int s = (sec - (3600 * h) - (m * 60));
    printf("H:M:S - %d:%d:%d\n", h, m, s);
}
