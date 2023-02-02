#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

#include <windows.h>

typedef struct
{
    char name[10000];
    int moves, score, rank, symbol;
}player;
int score_row_A = 0, score_row_B = 0, score_col_A = 0, score_col_B = 0, diag_score_A = 0, diag_score_B = 0, rev_diag_score_A = 0, rev_diag_score_B = 0;
player plA, plB;
int res,k = 1,pop_flag,start;
int choose_mode;
typedef struct
{
char name[10000];
int score ;
} information;
information top_players[10000];
void red()
{
    HANDLE console_color =GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleTextAttribute(console_color,4);
}
void blue()
{
    HANDLE console_color =GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleTextAttribute(console_color,2);
}
void yellow()
{
   HANDLE console_color =GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleTextAttribute(console_color,6);
}
void reset()
{
    HANDLE console_color =GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleTextAttribute(console_color,15);
}
void end_of_game(int score1,int score2)
{
    int endF;
    printf("the board is full!! do you want to\nend(1)\nundo(2)\n");
    scanf("%d",&endF);
    if(endF==1){
        information player;
        if (score1==score2){
            blue();
            printf("Ooops A Draw,No Winner,NO Loser!!");
            reset();
            return ;
        }
        else if(score1>score2)
        {
            red();
            printf("winner is player1!!\n please enter your name : ");
            fgets(player.name,30,stdin);
            reset();
            conver_to_lower(player.name);
            player.score=score1;
        }
        else {
            yellow();
            printf("Winner Winner Chicken Dinner , congratulation player2!!\n please enter your name : ");
            fgets(player.name,30,stdin);
            reset();
            conver_to_lower(player.name);
            player.score=score2;
        }
    FILE *file;

       int number_of_saves=0,identically=0;
    if((file=fopen("Topscores.binary","rb"))== NULL)
    {
        red();
        printf("Error in top score file!!!");
        reset();
        return ;
    }

    while ( fread(&top_players[number_of_saves],sizeof(information),1,file))
    {

        if(!(strcmp(player.name,top_players[number_of_saves].name)))
           {
               if (player.score>=top_players[number_of_saves].score)
                   top_players[number_of_saves].score=player.score;
                identically=1;
           }
           number_of_saves++;

    }
    if(identically==0)
    {
        strcpy(top_players[number_of_saves].name,player.name);
        top_players[number_of_saves].score=player.score;
        number_of_saves++;
   }

    fclose(file);
     mergeSort(top_players,0,number_of_saves-1);

   FILE *file2;
   file2=fopen("Topscores.binary","wb");
    for(int i=0;i<number_of_saves;++i)
            fwrite(&top_players[i],sizeof(information),1,file2);
            fclose(file2);
    }
}
 void conver_to_lower(char name [])
 {
     int i=0;
     while(name[i]!='\0')
     {
         name[i]=tolower(name[i]);
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
void Top_players(int number_of_top)
{

     FILE * file3;
     file3 = fopen("Topscores.binary","r");
        while (fread(&top_players[Index],sizeof(information),1,file3))
            Index++;
        blue();
        printf(" %-5s %-30s\n","Score","Name");
        reset();
     if(Index-1>number_of_top)
    {
      for(int i = Index-1; i >Index - 1 - number_of_top ; --i)
          printf(" %-5d %-30s\n",top_players[i].score,top_players[i].name);}
    else{
      for(int i=Index-1;i>=0;--i)
          printf(" %-5d %-30s\n",top_players[i].score,top_players[i].name);
    }
    fclose(file3);
}
bool FlagIn = false;
int ErrorFlag = 0;
int Width,Height,Highscore;
void xml()
{
    static int PathError = 0 ; char FilePath[200] ;
    if(!PathError)
       strcpy(FilePath,"mytesterfile.xml");
       else
       {
           if(PathError < 4)
           {
                printf("Error in configuration file , Enter the path to try again :");
                gets(FilePath);
           }
           else
            {
              Width = 7;
              Height = 6;
              Highscore = 10;
              return ;
            }
       }
    char ch , MyFile[500] ,conf_1[] = "<Configurations>" , width_1[] = "<Width>" , height_1[] = "<Height>" , highscores_1[]="<Highscores>";
    char conf_2[] = "</Configurations>" , width_2[] = "</Width>" , height_2[] = "</Height>" , highscores_2[] = "</Highscores>";
    int i = 0 ,ConfigStart,ConfigEnd,WidthStart,WidthEnd,HeightStart,HeightEnd,HighScoreStart,HighScoreEnd;
    FILE *file;
    if((file= fopen(FilePath, "r")) == NULL){
        PathError++;
        xml();
        return;
    }
    while((ch = fgetc(file)) != EOF){
            if(ch == '<'){
              FlagIn = true;
            }
            else if(ch == '>'){
              FlagIn = true;
            }
        if(ch!=' '&& ch != '\n' && ch != '\t'){
                FlagIn = false;
           MyFile[i] = ch;
           i++;
      }
      else if(!FlagIn)
          MyFile[i++] = ch;
    }

    MyFile[i] = '\0';
    fclose(file);
    ConfigStart = SearchForString(conf_1,MyFile);
    ConfigEnd = SearchForString(conf_2,MyFile);
    int Range = ConfigEnd-ConfigStart + 1;

    if(ConfigStart == -1 || ConfigEnd == -1 || ConfigEnd < ConfigStart){
            PathError++;
            return xml() ;
    }

    WidthStart= SearchForString(width_1,MyFile);
    WidthEnd = SearchForString(width_2,MyFile);
    int RangeA = WidthEnd - WidthStart + 1;

        if(WidthStart == -1 || WidthEnd == -1 || WidthEnd < WidthStart){
            PathError++;
            return xml() ;
        }
    char width[RangeA];
    for(int p = 0, j = WidthStart + 7 ; j < WidthEnd ; j++ , p++)
    {
        ErrorFlag = 0;
        if(isdigit(MyFile[j]))
        {
            width[p] = MyFile[j];
        }
        else
        {
          ErrorFlag = 0 ;
        PathError++;
        break;
        }
    }
    if(ErrorFlag)
    {
        return xml();
    }
    width[WidthEnd]='\0';
    HeightStart = SearchForString(height_1,MyFile);
    HeightEnd = SearchForString(height_2,MyFile);
    int RangeB = HeightEnd - HeightStart + 1;

        if(HeightStart == -1 || HeightEnd == -1 || HeightEnd < HeightStart){
          PathError++;
            return xml() ;
        }
    char height[RangeB];
       for(int h = 0 , v = HeightStart + 8 ; v < HeightEnd ; v++,h++)
       {
        ErrorFlag = 0;
        if(isdigit(MyFile[v]))
        {
            height[h] = MyFile[v];
        }
        else
        {
          ErrorFlag = 0 ;
        PathError++;
        break;
        }
    }
    if(ErrorFlag)
    {
        return xml();
    }
    height[HeightEnd]='\0';
    HighScoreStart = SearchForString(highscores_1,MyFile);
    HighScoreEnd = SearchForString(highscores_2,MyFile);
    int RangeC = HighScoreEnd - HighScoreStart + 1;

        if(HighScoreStart == -1 || HighScoreEnd == -1 || HighScoreEnd < HighScoreEnd){
            PathError ++;
            return xml() ;
        }
    char highscores[RangeC];
       for(int p = 0 ,j = HighScoreStart + 12 ; j < HighScoreEnd ; j++ , p++)
         {
        ErrorFlag = 0;
        if(isdigit(MyFile[j]))
        {
            highscores[p] = MyFile[j];
        }
        else
        {
          ErrorFlag = 0 ;
        PathError++;
        break;
        }
    }
    if(ErrorFlag)
    {
        return xml();
    }
    highscores[HighScoreEnd]='\0';

   if(atoi(height) < 4 || atoi(width) < 4 || atoi(highscores) < 4)
    {
            PathError ++ ;
            return xml();
    }
     Width = atoi(width);
     Height = atoi(height);
     Highscore = atoi(highscores);
}
char board[6][7];
int SearchForString(char s1[],char s2[]){
    int counter = -1 , i = 0;
    while(s2[i]!='\0'){
        int j=0;
        if(s2[i]==s1[j]){
            int k=i;
            while(s2[k]==s1[j] && s1[j]!='\0'){
                k++;
                j++;
            }
            if(s1[j]=='\0'){
                counter=i;
            }
        }
        i++;
    }
    return counter;
}
///creating a stack with its functions
///stack which involves columns indexes in which undo has been done
///which will help us undo and redo 2 stacks the first stores all moves played
///and the second will store the undid moves so we can redo

int MAXSIZE = 6 * 7;
int stack2[6 * 7];
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
int stack1[6 * 7];
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
    if (!isfull1()) {
        top1 = top1 + 1;
        stack1[top1] = data1;
    }
    else {
        printf("Could not insert data, Stack is full.\n");
    }
}
char mainMenu()
{
    char v,choose;
    printf("welcome to connect four\n\n\n\nChoose:\nA.Start Game\nB.Load a game\nC.Top scores\nD.Quit\n");
    printf("What's your choice\n");
    do {
        scanf("%c", &choose);
    } while (choose != 'A' && choose != 'B' && choose != 'C' && choose != 'D');
    switch(choose){
        case 'A':  printf("Choose your mode\nA.Play with player\nB.play with computer");
                   scanf("%c",&v);
                   return v;
                   break;
        case 'B':printf("Choose your mode\n1.Play with player\n1.play with computer");
                scanf("%d",&choose_mode);
                load();
                 break;
        case 'C':Top_players(10);
                break;
        case 'D':Quit();
                break;
        default:
            printf("Enter a valid input");
    }
}

int main(void)
{
    xml();
    time_t t=time(NULL);
    char choice;
    choice=mainMenu();
    choice=getchar();
    if (choice=='A'){
        printBoard();
        printf("\n\n\n");
        choose_mode=1;
        updateBoard(t);
        end();
    }
    else if (choice=='B'){
        printBoard();
        printf("\n\n\n");
        choose_mode=2;
        vsComputer(t);
        end();
    }
    return 0;
}
void rev_diag_score(int DRC_A, int DRC_B, int Height, int Width, char board[][Width]) {
    static total_score_A = 0, total_score_B = 0;
    int i, k;
    for (i = Height - 1; i >= 0; i--) {
        for (k = 0; k < Width; k++) {
            if (board[i][k] == 'x') {
                if (board[i][k] == board[i + 1][k + 1] && board[i + 1][k + 1] == board[i + 2][k + 2] && board[i + 2][k + 2] == board[i + 3][k + 3]) {
                    rev_diag_score_A++;
                }

            }
            else if (board[i][k] == 'o') {
                if (board[i][k] == board[i + 1][k + 1] && board[i + 1][k + 1] == board[i + 2][k + 2] && board[i + 2][k + 2] == board[i + 3][k + 3]) {
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
    printf("Player A Score : %d\n",total_score_A);
    yellow();
    printf("Player B Score : %d\n",total_score_B);
    reset();
    score_row_A = 0;
    score_row_B = 0;
    score_col_A = 0;
    score_col_B = 0;
    diag_score_A = 0;
    diag_score_B = 0;
    rev_diag_score_A = 0;
    rev_diag_score_B = 0;
    plA.score=total_score_A;
    plB.score=total_score_B;
}
void diag_score(int sum_RC_A, int sum_RC_B, int Height, int Width, char board[][Width]) {
    static sum_DRC_A = 0, sum_DRC_B = 0;
    int i, k;
    for (i = Height - 1; i >= 0; i--) {
        for (k = 0; k < Width; k++) {
            if (board[i][k] == 'x') {
                if (board[i][k] == board[i - 1][k + 1] && board[i - 1][k + 1] == board[i - 2][k + 2] && board[i - 2][k + 2] == board[i - 3][k + 3]) {
                    diag_score_A++;
                }

            }
            else if (board[i][k] == 'o') {
                if (board[i][k] == board[i - 1][k + 1] && board[i - 1][k + 1] == board[i - 2][k + 2] && board[i - 2][k + 2] == board[i - 3][k + 3]) {
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
    rev_diag_score(sum_DRC_A, sum_DRC_B, Height, Width, board);

}
void col_score(int row_score_A, int row_score_B, int Height, int Width, char board[][Width]) {
    int i, j, k;
    static int sum_A = 0, sum_B = 0;
    for (i = Height - 1; i >= 0; i--) {
        for (k = 0; k < Width; k++) {
            if (board[i][k] == 'x') {
                if (board[i][k] == board[i + 1][k] && board[i + 1][k] == board[i + 2][k] && board[i + 2][k] == board[i + 3][k]) {
                    score_col_A++;
                }
            }
            else if (board[i][k] == 'o') {
                if (board[i][k] == board[i + 1][k] && board[i + 1][k] == board[i + 2][k] && board[i + 2][k] == board[i + 3][k]) {
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
    diag_score(sum_A, sum_B, Height, Width, board);
}
void row_score(int Height, int Width, char board[][Width]) {

    int i, j, k;
    for (i = Height- 1; i >= 0; i--) {
        for (k = 0; k < Width; k++) {
            if (board[i][k] == 'x') {
                if (board[i][k] == board[i][k + 1] && board[i][k + 1] == board[i][k + 2] && board[i][k + 2] == board[i][k + 3]) {
                    score_row_A++;
                }

            }
            else if (board[i][k] == 'o') {
                if (board[i][k] == board[i][k + 1] && board[i][k + 1] == board[i][k + 2] && board[i][k + 2] == board[i][k + 3]) {
                    score_row_B++;
                }

            }
            else {
                continue;
            }
        }
    }

    col_score(score_row_A, score_row_B, Height, Width, board);
}
void end(){
        time_t t=time(NULL);
        int answer;
        char winnername[30];
        static int undo_count=1;
        do{
        if(undo_count%2==0){
            answer=0;
        }
        else{
            printf("Warning! the grid is full\nDo you want to\nend:press 1\nundo:press 0\n");
            scanf("%d",&answer);
        }
        if(choose_mode==1  && answer==1){
            if(plA.score>plB.score){
                end_of_game(plA.score,plB.score);
                mainMenu();
            }
            else if(plA.score<plB.score){
                end_of_game(plA.score,plB.score);
                mainMenu();
            }
            else if (plA.score==plB.score){
                printf("\n\nno winner\n");
                end_of_game(plA.score,plB.score);
                mainMenu();
            }
        }
        else if(choose_mode==2 && answer==1){
            if(plA.score>plB.score){
                end_of_game(plA.score,plB.score);
                mainMenu();
            }
            else if(plA.score<plB.score){
                printf("\n\nWinner is the computer!!");
                end_of_game(plA.score,plB.score);
                mainMenu();
            }
            else if (plA.score==plB.score){
                printf("\n\nno winner");
                mainMenu();
            }
        }
        else if(answer==0 && choose_mode==1){
            res=check(stack1[top1],1);
            push2(stack1[top1]);
            k++;
            undo(res,stack1[top1],k,0);
            pop_flag=1;
            updateBoard(t);
        }
        else if(answer==0&&choose_mode==2){
            res=check(stack1[top1],1);
            push2(stack1[top1]);
            undo_count++;
            undo(res,stack1[top1],k,1);
            pop_flag=1;
            k=1;
            vsComputer(t);
        }
    }while(answer!=1 && answer!=0);
}
void Quit()
{
    printf("Good bye\n");
    exit(0);
}
void printBoard() {
    if (!start) {
        for (int i = Height-1; i >= 0; i--)
        {
            for (int j = Width; j >= 0; j--)
            {
                printf(" | ");
            }
            printf(" | \n");
        }
    }
    else {
        for (int i = Height-1; i >= 0; i--)
        {
            for (int j = 0; j < Width; j++)
            {
                printf("|");
                if(board[i][j] == 'x'){
                    red();
                }
                else if (board[i][j] == 'o')
                {
                    yellow();
                }
                printf(" %c ", board[i][j]);
                reset();
            }
            printf(" |\n");
        }
    }
    start++;
}
void redo(int row,int col,int turn,int comp_redo){
    pop2();
    if (turn%2==1){
        board[row][col]='x';
        printBoard();
        row_score(Height, Width, board);
        if(comp_redo==1){
            plA.moves++;
           red();
           printf("playerA moves  : %d\n",plA.moves);
           yellow();
           printf("playerB moves  : %d\n",plB.moves);
           reset();


        }
        else{
            plA.moves++;
          red();
           printf("playerA moves  : %d\n",plA.moves);
           yellow();
           printf("playerB moves  : %d\n",plB.moves);
           reset();
        }
    }
    else{
        board[row][col]='o';
        plB.moves++;
        printBoard();
        row_score(Height, Width, board);
        red();
           printf("playerA moves  : %d\n",plA.moves);
           yellow();
           printf("playerB moves  : %d\n",plB.moves);
           reset();
    }

}
void undo(int row, int col, int turn,int comp_undo) {
    static int comp_undo_counter=0;
    board[row][col] = ' ';
    pop1();
    if (comp_undo==1){
        comp_undo_counter++;
    }
    if(comp_undo==1&&comp_undo_counter%2==0){
        printBoard();
        plA.moves--;
        plB.moves--;
        row_score(Height, Width, board);
       red();
           printf("playerA moves  : %d\n",plA.moves);
           yellow();
           printf("playerB moves  : %d\n",plB.moves);
           reset();
           pop_flag=1;
    }
    else if(comp_undo==0){
        printBoard();
        if(turn%2==1){
            plA.moves--;
        }
        else{
            plB.moves--;
        }
        row_score(Height, Width, board);
        red();
           printf("playerA moves  : %d\n",plA.moves);
           yellow();
           printf("playerB moves  : %d\n",plB.moves);
           reset();
    }
}
int arr[10000] = { 0 };
int check(int col, int undo_flag) {
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
void save(){
    time_t t=time(NULL);
    int fileno;
    char choice2;
    system("cls");
    printf("choose a slot to save game in:slot1\nslot2\nslot3");
    scanf("%d",&fileno);
    system("cls");
    if(fileno==1){
        FILE *save1;
        save1=fopen("save1.bin","wb");
        for(int i=0;i<Height;i++){
            fwrite(board[i],sizeof(board[i]),1,save1);
        }
        fwrite(arr,sizeof(arr),1,save1);
        fwrite(&plA.moves,sizeof(plA.moves),1,save1);
        fwrite(&plB.moves,sizeof(plB.moves),1,save1);
        fwrite(&plA.score,sizeof(plA.moves),1,save1);
        fwrite(&plB.score,sizeof(plB.moves),1,save1);
        fwrite(&stack1[top1],sizeof(stack1[top1]),1,save1);
        fwrite(&stack1[top1-1],sizeof(stack1[top1-1]),1,save1);
        fwrite(&choose_mode,sizeof(choose_mode),1,save1);
        fclose(save1);
    }
    else if (fileno==2){
        FILE *save2;
        save2=fopen("save2.bin","wb");
        for(int i=0;i<6;i++){
            fwrite(board[i],sizeof(board[i]),1,save2);
        }
        fwrite(arr,sizeof(arr),1,save2);
        fwrite(&plA.moves,sizeof(plA.moves),1,save2);
        fwrite(&plB.moves,sizeof(plB.moves),1,save2);
        fwrite(&plA.score,sizeof(plA.moves),1,save2);
        fwrite(&plB.score,sizeof(plB.moves),1,save2);
        fwrite(&stack1[top1],sizeof(stack1[top1]),1,save2);
        fwrite(&stack1[top1-1],sizeof(stack1[top1-1]),1,save2);
        fwrite(&choose_mode,sizeof(choose_mode),1,save2);
        fclose(save2);

    }
     else if (fileno==3){
        FILE *save3;
        save3=fopen("save3.bin","wb");
        for(int i=0;i<Height;i++){
            fwrite(board[i],sizeof(board[i]),1,save3);
        }
        fwrite(arr,sizeof(arr),1,save3);
        fwrite(&plA.moves,sizeof(plA.moves),1,save3);
        fwrite(&plB.moves,sizeof(plB.moves),1,save3);
        fwrite(&plA.score,sizeof(plA.moves),1,save3);
        fwrite(&plB.score,sizeof(plB.moves),1,save3);
        fwrite(&stack1[top1],sizeof(stack1[top1]),1,save3);
        fwrite(&stack1[top1-1],sizeof(stack1[top1-1]),1,save3);
        fwrite(&choose_mode,sizeof(choose_mode),1,save3);
        fclose(save3);

    }
        printf("do you want to continue?y or n");
        do{
            scanf("%c",&choice2);
        }while(choice2!='y' && choice2!='n');
        if(choice2=='y'){
            printBoard();
            updateBoard(t);
        }
        else{
            mainMenu();
        }
}
void load(){
    time_t t=time(NULL);
    int loadfile;
    printf("enter load file");
    scanf("%d",&loadfile);
     if(loadfile==1){
        FILE *save1;
        save1=fopen("save1.bin","rb");
        for(int i=0;i<Height;i++){
            fread(board[i],sizeof(board[i]),1,save1);
        }
        fread(arr,sizeof(arr),1,save1);
        fread(&plA.moves,sizeof(plA.moves),1,save1);
        fread(&plB.moves,sizeof(plB.moves),1,save1);
        fread(&plA.score,sizeof(plA.moves),1,save1);
        fread(&plB.score,sizeof(plB.moves),1,save1);
        fread(&choose_mode,sizeof(choose_mode),1,save1);
        fclose(save1);

    }
    else if (loadfile==2){
        FILE *save2;
        save2=fopen("save2.bin","rb");
        for(int i=0;i<Height;i++){
            fread(board[i],sizeof(board[i]),1,save2);
        }
        fread(&plA.moves,sizeof(plA.moves),1,save2);
        fread(&plB.moves,sizeof(plB.moves),1,save2);
        fread(&plA.score,sizeof(plA.moves),1,save2);
        fread(&plB.score,sizeof(plB.moves),1,save2);
        fread(&stack1[top1],sizeof(stack1[top1]),1,save2);
        fread(&stack1[top1-1],sizeof(stack1[top1-1]),1,save2);
        fread(&choose_mode,sizeof(choose_mode),1,save2);
        fclose(save2);
    }
       else if (loadfile==3){
        FILE *save3;
        save3=fopen("save3.bin","rb");
        for(int i=0;i<Height;i++){
            fread(board[i],sizeof(board[i]),1,save3);
        }
        fread(arr,sizeof(arr),1,save3);
        fread(&plA.moves,sizeof(plA.moves),1,save3);
        fread(&plB.moves,sizeof(plB.moves),1,save3);
        fread(&plA.score,sizeof(plA.moves),1,save3);
        fread(&plB.score,sizeof(plB.moves),1,save3);
        fread(&stack1[top1],sizeof(stack1[top1]),1,save3);
        fread(&stack1[top1-1],sizeof(stack1[top1-1]),1,save3);
        fread(&choose_mode,sizeof(choose_mode),1,save3);
        fclose(save3);
    }
    if(choose_mode==1){
        printBoard();
        row_score(Height,Width,board);
        printf("player 1 moves=%d,player 2 moves=%d",plA.moves,plB.moves);
        clockFn(time(NULL)-t);
        updateBoard(t);
    }
    else{
        printBoard();
        row_score(Height,Width,board);
        printf("player 1 moves=%d,player 2 moves=%d",plA.moves,plB.moves);
        clockFn(time(NULL)-t);
        vsComputer(t);
    }
}
char turn1[25], turn2[25], str[] = "undo";
int temp1, temp2;
void updateBoard(long long t) {
    static int count1 = 1;
    for (int i = 0; i < Height; i++) {
        for (int j = 1; j <= Width; j++) {
            if (plA.moves + plB.moves == Height*Width) {
                end();
            }
            else {
                if (k % 2) {
                    printf("player 1's turn or undo by pressing 0 or redo by pressing -1 or save by pressing 8:");///checking the validation if input of player 1
                    do {
                        scanf("%s",&turn1);
                        temp1=atoi(turn1);
                        if(temp1==0){
                            if(strcmp(turn1,"0")==0){
                                temp1-=1;
                            }
                            else if(strcmp(turn1,"yes")==0){
                                mainMenu();
                            }
                            else{
                                printf("enter a valid input\n");
                                updateBoard(t);
                            }
                        }
                        else{
                            temp1-=1;
                        }
                        if (temp1 > 8 || temp1 < 0 && temp1 != -1 && temp1 != -2) {
                            printf("the column is not avaliable !\nEnter Again!\n");///checking the validation if input of player 2
                        }
                        else if (temp1 == -1) { ///undo Algorithm
                            if (isempty1() == 0) {
                                pop_flag = 1;///indicator for occurence of undo
                                res = check(stack1[top1], 1);///row calculating
                                if (res != -1) {
                                    push2(stack1[top1]); ///the undid move is pushed into another stack
                                    undo(res, stack1[top1], 2, 0);
                                }
                                else {
                                    check(temp2, 1);
                                    push2(stack1[top1]);
                                    undo(Height - count1, stack1[top1], 2, 0);///handling undo in overflow
                                    count1++;
                                }
                                k++;
                                updateBoard(t);
                            }
                            else {
                                printf("you can't undo an empty grid!!\n");
                                updateBoard(t);
                            }
                        }
                        else if (temp1 == -2 && pop_flag == 1) {
                            if (isempty2() == 0) {///if stack2 is empty this means that no undo has happened
                                res = check(stack2[top2], 0);///so we must not redo
                                push1(stack2[top2]);
                                redo(res, stack2[top2], 1, 0);
                                k++;
                                updateBoard(t);
                            }
                            else {
                                printf("you can't redo without undo\n");
                                updateBoard(t);
                            }
                        }
                        else if (temp1 == -2 && pop_flag == 0) {
                            printf("redo is not available\n");///if there was a move after undo, redo is restricted
                        }
                        else if (temp1==7){
                            save();
                        }
                    } while (temp1 > 8 || temp1 < 0);
                    res = check(temp1, 0);///checking the overflow in column
                    if (res != -1) {
                        push1(temp1);
                        board[res][temp1] = 'x';
                        plA.moves++;
                        printBoard();
                        row_score(Height, Width, board);
                        red();
                        printf("playerA moves  : %d\n",plA.moves);
                        yellow();
                        printf("playerB moves  : %d\n",plB.moves);
                        reset();
                        k++;
                        pop_flag=0;
                        clockFn(time(NULL)-t);
                    }
                    else {  ///overflow in a certain column
                        printf("out of scope!\n");
                        do {
                            printf("please enter another column or undo\n press 0:for undo or enter another column");
                            scanf("%s",&turn1);
                            temp1=atoi(turn1);
                            if(temp1==0){
                                if(strcmp(turn1,"0")==0){
                                    temp1-=1;
                                }
                                else if(strcmp(turn1,"yes")==0){
                                    mainMenu();
                                }
                                else{
                                    printf("enter a valid input\n");
                                    updateBoard(t);
                                }
                            }
                            else{
                                temp1 -= 1;
                            }
                            res = check(temp1, 0);
                            if (temp1 > 6 || temp1 < 0 && temp1 != -1 && temp1 != -2) {
                                printf("the column is not avaliable !\nEnter Again!\n");
                            }
                            else if (temp1 == -1) {
                                if (isempty1() == 0) {
                                    res = check(stack1[top1], 1);
                                    if (res != -1) {
                                        push2(stack1[top1]);
                                        undo(res, stack1[top1], 2, 0);
                                        pop_flag = 1;
                                    }
                                    else {
                                        check(temp2, 1);
                                        push2(stack1[top1]);
                                        undo(6 - count1, stack1[top1], 2, 0);
                                        count1++;
                                        pop_flag = 1;
                                    }
                                    k++;
                                    updateBoard(t);
                                }
                                else {
                                    printf("you can't undo an empty grid!!\n");
                                    updateBoard(t);
                                }
                            }
                            else if (temp1 == -2 && pop_flag == 1) {
                                if (isempty2() == 0) {
                                    res = check(stack2[top2], 0);
                                    push1(stack2[top2]);
                                    redo(res, stack2[top2], 1, 0);
                                    k++;
                                    updateBoard(t);
                                }
                                else {
                                    printf("you can't redo without undo\n");
                                    updateBoard(t);
                                }
                            }
                            else if(temp1==7){
                                save();
                            }
                        } while (res == -1 || temp1 > 8 || temp1 < 0);
                        push1(temp1);
                        board[res][temp1] = 'x';
                        plA.moves++;
                        printBoard();
                        row_score(Height, Width, board);
                        red();
                        printf("playerA moves  : %d\n",plA.moves);
                        yellow();
                        printf("playerB moves  : %d\n",plB.moves);
                        reset();
                        k++;
                        pop_flag=0;
                        clockFn(time(NULL)-t);
                    }
                }
                else if (k % 2 == 0) { ///same handling as we have done in player 1
                    printf("player 2's turn or undo by pressing 0 or redo by pressing -1:");///checking the validation if input of player 1
                    do {
                        scanf("%s",&turn2);
                        temp2=atoi(turn2);
                        if(temp2==0){
                            if(strcmp(turn2,"0")==0){
                                temp2-=1;
                            }
                            else if(strcmp(turn2,"yes")==0){
                                mainMenu();
                            }
                            else{
                                printf("enter a valid input\n");
                                updateBoard(t);
                            }
                        }
                        else{
                            temp2-=1;
                        }
                        if (temp2 > 8 || temp2 < 0 && temp2 != -1 && temp2 != -2) {
                            printf("the column is not avaliable!\nEnter Again!\n");///checking the validation if input of player 2
                        }
                        else if (temp2 == -1) {
                            if (isempty1() == 0) {
                                pop_flag = 1;
                                res = check(stack1[top1], 1);
                                if (res != -1) {
                                    push2(stack1[top1]);
                                    undo(res, stack1[top1], 1, 0);
                                }
                                else {
                                    check(temp1, 1);
                                    undo(6 - count1, stack1[top1], 1, 0);
                                    count1++;
                                }
                                k++;
                                updateBoard(t);
                            }
                            else {
                                printf("you can't undo an empty grid!!\n");
                                updateBoard(t);
                            }
                        }
                        else if (temp2 == -2 && pop_flag == 1) {
                            if (isempty2() == 0) {
                                res = check(stack2[top2], 0);
                                push1(stack2[top2]);
                                redo(res, stack2[top2], 2, 0);
                                k++;
                                updateBoard(t);
                            }
                            else {
                                printf("you can't redo without undo\n");
                            }
                        }
                        else if (temp2 == -2 && pop_flag == 0) {
                            printf("redo is not available\n");
                        }
                        else if (temp2==7){
                            save();
                        }
                    } while (temp2 > 6 || temp2 < 0);
                    res = check(temp2, 0);
                    if (res != -1) {
                        push1(temp2);
                        board[res][temp2] = 'o';
                        pop_flag = 0;
                        plB.moves++;
                        printBoard();
                        row_score(Height, Width, board);
                       red();
                        printf("playerA moves  : %d\n",plA.moves);
                        yellow();
                        printf("playerB moves  : %d\n",plB.moves);
                        reset();
                        k++;
                        pop_flag=0;
                        clockFn(time(NULL)-t);
                    }
                    else {
                        printf("out of scope!\n");
                        do {
                            printf("please enter another column or undo or redo\npress 0:for undo\npress -1:for redo\nor enter another column");
                            scanf("%s",&turn2);
                            temp2=atoi(turn2);
                            if(temp2==0){
                            if(strcmp(turn2,"0")==0){
                                temp2-=1;
                            }
                            else if(strcmp(turn2,"yes")==0){
                                mainMenu();
                            }
                            else{
                                printf("enter a valid input\n");
                                updateBoard(t);
                                }
                            }
                            else{
                                temp2-=1;
                            }
                            res = check(temp2, 0);
                            if (temp2 > 6 || temp2 < 0 && temp2 != -1 && temp2 != -2) {
                                printf("the column is not avaliable !\n");
                                printf("Enter Again!\n");
                            }
                            else if (temp2 == -1) {
                                if (isempty1() == 0) {
                                    res = check(stack1[top1], 1);
                                    if (res != -1) {
                                        push2(stack1[top1]);
                                        undo(res, stack1[top1], 1, 0);
                                        pop_flag = 1;
                                    }
                                    else {
                                        check(temp1, 1);
                                        push2(stack1[top1]);
                                        undo(6 - count1, stack1[top1], 1, 0);
                                        count1++;
                                        pop_flag = 1;
                                    }
                                    k++;
                                    updateBoard(t);
                                }
                                else {
                                    printf("you can't undo an empty grid!!\n");
                                    updateBoard(t);
                                }
                            }
                            else if (temp2 == -2 && pop_flag == 1) {
                                if (isempty2() == 0) {
                                    res = check(stack2[top2], 0);
                                    push1(stack2[top2]);
                                    redo(res, stack2[top2], 1, 0);
                                    k++;
                                    updateBoard(t);
                                }
                                else {
                                    printf("you can't redo without undo\n");
                                    updateBoard(t);
                                }
                            }
                        } while (res == -1 || temp2 > 6 || temp2 < 0);
                        push1(temp2);
                        board[res][temp2] = 'o';
                        plB.moves++;
                        printBoard();
                       row_score(Height, Width, board);
                       red();
                        printf("playerA moves  : %d\n",plA.moves);
                        yellow();
                        printf("playerB moves  : %d\n",plB.moves);
                        reset();
                        k++;
                        pop_flag=0;
                        clockFn(time(NULL-t));
                    }
                }
            }
        }
    }
}
int random(int upper)
{
    srand(time(0));
    int num = (rand() %(upper)) + 1;
      return num;
}
void vsComputer(long long t) {
    static int count1 = 1;
    int comp_col, comp_undo = 0, comp_redo = 0,res2;
    for (int i = 0; i < Height; i++) {
        for (int j = 1; j <= Width; j++) {
            if (plA.moves + plB.moves == Height*Width) {
                end();
            }
            else {
                if (k % 2) {
                    printf("player's 1's turn[enter column number or undo(0) or redo(-1) or save(8)");
                    do {
                        scanf("%d",&temp1);
                        temp1--;
                        if (temp1 > 8 || temp1 < 0 && temp1 != -1 && temp1 != -2) {
                            printf("the column is not avaliable !\nEnter Again!\n");///checking the validation if input of player 2
                        }
                        else if (temp1 == -1) {
                            if (isempty1() == 0) {
                                pop_flag = 1;///indicator for occurence of undo
                                res = check(stack1[top1], 1);///row calculating
                                res2 = check(stack1[top1 - 1], 1);
                                if (res != -1) {
                                    push2(stack1[top1-1]); ///the undid move is pushed into another stack
                                    undo(res, stack1[top1], 2, 1);
                                    undo(res2, stack1[top1], 1, 1);
                                    k++;
                                }
                            else {
                                    check(comp_col, 1);
                                    push2(stack1[top1]);
                                    undo(Height - count1, stack1[top1], 1, 1);///handling undo in overflow
                                    check(stack1[top1], 1);
                                    undo(Height - count1 - 1, stack1[top1], 1, 1);
                                    count1++;
                                    pop_flag = 1;
                                    k++;
                                }
                                k++;
                                vsComputer(t);
                            }
                            else {
                                printf("you can't undo an empty grid!!\n");
                                vsComputer(t);
                            }
                        }
                        else if (temp1 == -2 && pop_flag == 1) {
                            if (isempty2() == 0) {///if stack2 is empty this means that no undo has happened
                                res = check(stack2[top2], 0);///so we must not redo
                                push1(stack2[top2]);
                                redo(res, stack2[top2], 1, 1);
                                k++;
                                vsComputer(t);
                            }
                            else {
                                printf("you can't redo without undo\n");
                            }
                        }
                        else if (temp1 == -2 && pop_flag==0) {
                            printf("redo is not available after new move\n");
                        }
                        else if (temp1==7){
                            save();
                        }
                    } while (temp1 > 8 || temp1 < 0);
                        res = check(temp1, 0);
                        if (res != -1) {
                        push1(temp1);
                        board[res][temp1] = 'x';
                        plA.moves++;
                        printBoard();
                        row_score(Height, Width, board);
                        red();
                        printf("playerA moves  : %d\n",plA.moves);
                        yellow();
                        printf("computer moves  : %d\n",plB.moves);
                        reset();
                        k++;
                        pop_flag=0;
                    }
                    else if (res == -1) {
                        printf("out of scope!\n");
                        do {
                            printf("please enter another column or undo\n press 0:for undo or enter another column");
                            scanf("%d", &temp1);
                            temp1 -= 1;
                            res = check(temp1, 0);
                            if (temp1 > 6 || temp1 < 0 && temp1 != -1) {
                                printf("the column is not avaliable !\nEnter Again!\n");
                            }
                            else if (temp1 == -1) {
                                if (isempty1() == 0) {
                                    res = check(stack1[top1], 1);
                                    push2(stack1[top1-1]);
                                    undo(res, stack1[top1], 2, 1);
                                    res2 = check(stack1[top1], 1);
                                    pop_flag = 1;
                                    if (res2 != -1) {
                                        undo(res2, stack1[top1], 2, 1);
                                    }
                                    else {
                                        undo(Height - count1, stack1[top1], 2, 1);
                                        count1++;
                                    }
                                    k++;
                                }
                                else {
                                    printf("you can't undo an empty grid");
                                    vsComputer(t);
                                }
                                k++;
                                vsComputer(t);
                            }
                            else if (temp1 == -2 && pop_flag == 1) {
                                if (isempty2() == 0) {
                                    res = check(stack2[top2], 0);
                                    push1(stack2[top2]);
                                    redo(res, stack2[top2], 1, 1);
                                    k++;
                                    vsComputer(t);
                                }
                                else {
                                    printf("you can't redo without undo\n");
                                    vsComputer(t);
                                }
                            }
                            else if (temp1 == -2 && pop_flag == 0){
                                printf("Redo is not available after a new move!");
                            }
                            else if (temp1==7){
                                save();
                            }
                        } while (res == -1 || temp1 > 8 || temp1 < 0);
                        push1(temp1);
                        plA.moves++;
                        board[res][temp1] = 'x';
                        printBoard();
                        row_score(Height, Width, board);
                        red();
                        printf("playerA moves  : %d\n",plA.moves);
                        yellow();
                        printf("playerB moves  : %d\n",plB.moves);
                        reset();
                        k++;
                        pop_flag=0;
                    }
                }
                else {
                    do {
                        if (isfull1() == 0) {
                            comp_col = random(Height);
                            res = check(comp_col, 0);
                        }
                        else {
                            return 0;
                        }
                    } while (res == -1);
                    push1(comp_col);
                    board[res][comp_col] = 'o';
                    printBoard();
                    row_score(Height, Width, board);
                    plB.moves++;
                    red();
                    printf("playerA moves  : %d\n",plA.moves);
                    yellow();
                    printf("computer moves  : %d\n",plB.moves);
                    reset();
                    k++;
                }
            }
        }
    }
}

void clockFn(long long sec){
int h = (sec/3600);
int m = (sec -(3600*h))/60;
int s = (sec -(3600*h)-(m*60));
printf("H:M:S - %d:%d:%d\n",h,m,s);
}




