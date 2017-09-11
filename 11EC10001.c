//Made by
//11EC10001

#include <stdio.h>
#include <stdlib.h>

#define MAXCHILD 9
#define NULL 0

//here MAXLEVEL=100 means that the code will form as many nodes as it can, till the board is full or someone has won
//if you want upto certain level, say level = 4 then set MAXLEVEL=4
#define MAXLEVEL 100

struct nodetype
{
    char board[3][3];
    struct nodetype *child[MAXCHILD];
};
typedef struct nodetype *NODEPTR;

NODEPTR getnode();
void initialize_board(char board[3][3]);
void printboard(char board[3][3]);
void copyboard(char dest_board[3][3],char source_board[3][3]);
int computer_play(NODEPTR root,char board[3][3],int level,char player);
void create_all_child(NODEPTR root,char symbol);
int evaluate_function(NODEPTR node,char player);
int game_is_not_over(char board[3][3]);
int someone_has_won(char board[3][3]);

int main()
{
    int i,j,flag;
    char main_board[3][3];
    NODEPTR root;
    initialize_board(main_board);
    printboard(main_board);
        do
        {
            do
            {
                flag = 0;
                printf("User move. Enter the coordinates of your move. The coordinate should be separated by space and then hit enter\n");
                scanf("%d",&i);
                scanf("%d",&j);
                printf("i = %d j = %d\n",i,j);
                if (main_board[i][j] == 'e')
                {
                    main_board[i][j] = 'X';
                    printboard(main_board);
                    flag = 1;
                }
                else
                {
                    printf("Error. Target area already occupied. Try again\n");
                }
            }
            while(flag == 0);
            if(game_is_not_over(main_board))
            {
                root = getnode();
                copyboard(root->board,main_board);
                computer_play(root,main_board,1,'O'); //level = 1,computer move = 'O'
                printf("Computer's move\n");
                printboard(main_board);
            }
        }
        while(game_is_not_over(main_board));

        return 0;
}

NODEPTR getnode()
{
    NODEPTR p;
    p = (NODEPTR)malloc(sizeof(struct nodetype));
    return(p);
}

void initialize_board(char board[3][3])
{
    int i,j;
    for(i = 0; i<3; i++)
    {
        for(j = 0; j<3; j++)
        {
            board[i][j] = 'e'; //'e' represents empty
        }
    }
}

void printboard(char board[3][3])
{
    int i,j;
    printf("+------+\n");
    for(i = 0; i<3; i++)
    {
        printf("|");
        for(j = 0; j<3; j++)
        {
            if (board[i][j] == 'e')
            {
               printf(" |");
            }
            else
            {
                printf("%c|",board[i][j]);
            }
        }
        printf("\n");
        printf("+------+\n");
    }
}

void copyboard(char dest_board[3][3],char source_board[3][3])
{
    int i,j;
    for(i = 0; i<3; i++)
    {
        for(j = 0; j<3 ; j++)
        {
            dest_board[i][j] = source_board[i][j];
        }
    }
}

int computer_play(NODEPTR root,char main_board[3][3],int level,char player)
{
    if(level==(MAXLEVEL+1))
    {
        return 200;
    }
    int k = 0;
    int worst_case,best_case,val,num;
    char sec_player = 'X';
    if(player=='X')
    {
        sec_player = 'O';
    }
    NODEPTR saved_node;
    worst_case = 1000;
    best_case = -1000;
    if(someone_has_won(root->board)) //no child should be created after this
    {
        return 200;
    }
    create_all_child(root,player);//create child with player as the new entry
    while(root->child[k]!=NULL)
    {
        num = computer_play(root->child[k],main_board,level+1,sec_player);
        if(num==200 && level%2==1) //if no more child can be created
        {
            num = evaluate_function(root->child[k],player);//if level is odd then we will do best case
        }
        if(num==200 && level%2==0) //if no more child can be created
        {
            num = evaluate_function(root->child[k],sec_player);//if level is even then we will do worst case
        }
        if (level%2==1)   //odd level so best case
        {
            if(num>best_case)
            {
                best_case = num;
                if(level==1)
                {
                    saved_node = root->child[k]; //save the node to be displayed from the first level
                }
            }
        }
        else  //even level so worst case
        {
            if(num<worst_case)
            {
                worst_case = num;
                best_case = num;
            }
        }
    k++;
    }
    if(level==1)
    {
        copyboard(main_board,saved_node->board); //copy the computer move to main_board
    }
    if(k==0)
    {
        return(200); //this represents that we have reached the leaf, no more child can be created
    }
    else
    {
        return(best_case);
    }
}

void create_all_child(NODEPTR root,char symbol)
{
    int i,j,k;
    k = 0;
    for(i = 0; i<3; i++)
    {
        for(j = 0; j<3; j++)
        {
            if(root->board[i][j] == 'e')
            {
                root->child[k] = getnode();
                copyboard(root->child[k]->board,root->board);
                root->child[k]->board[i][j] = symbol;
                k = k + 1;
            }
        }
    }
    root->child[k] = NULL;

}

int evaluate_function(NODEPTR node,char player)
{
    int i,j,sum_player,sum_sec_player;//sum_player stores the number of lines for player and sec_player for second player
    int count_player,count_sec_player;
    count_player = 0;
    count_sec_player = 0;
    sum_player = 0;
    sum_sec_player = 0;
    char second_player = 'O';
    if(player == 'O')
    {
        second_player = 'X';
    }
    //horizontal and vertical scan
    for(i = 0; i<3; i++)
    {
        for(j = 0; j<3; j++)
        {
            if(node->board[i][j] == player)
            {
                if((node->board[i][(j+1)%3]!=second_player) && (node->board[i][(j+2)%3]!=second_player))
                {
                    sum_player = sum_player + 1;
                }
                if((node->board[(i+1)%3][j]!=second_player) && (node->board[(i+2)%3][j]!=second_player))
                {
                    sum_player = sum_player + 1;
                }
            }
            if(node->board[i][j] == second_player)
            {
                if((node->board[i][(j+1)%3]!=player) && (node->board[i][(j+2)%3]!=player))
                {
                    sum_sec_player = sum_sec_player + 1;
                }
                if((node->board[(i+1)%3][j]!=player) && (node->board[(i+2)%3][j]!=player))
                {
                    sum_sec_player = sum_sec_player + 1;
                }
            }
        }
    }
    //diagonal scan
    if(node->board[0][0] == player)
    {
        if((node->board[1][1]!=second_player) && (node->board[2][2]!=second_player))
        {
            sum_player = sum_player + 1;
        }
    }
    else if(node->board[1][1] == player)
    {
        if((node->board[0][0]!=second_player) && (node->board[2][2]!=second_player))
        {
            sum_player = sum_player + 1;
        }
    }
    else if(node->board[2][2] == player)
    {
        if((node->board[0][0]!=second_player) && (node->board[1][1]!=second_player))
        {
            sum_player = sum_player + 1;
        }
    }
    else if(node->board[0][0] == second_player)
    {
        if((node->board[1][1]!=player) && (node->board[2][2]!=player))
        {
            sum_sec_player = sum_sec_player + 1;
        }
    }
    else if(node->board[1][1] == second_player)
    {
        if((node->board[0][0]!=player) && (node->board[2][2]!=player))
        {
            sum_sec_player = sum_sec_player + 1;
        }
    }
    else if(node->board[2][2] == second_player)
    {
        if((node->board[0][0]!=player) && (node->board[1][1]!=player))
        {
            sum_sec_player = sum_sec_player + 1;
        }
    }

    if(node->board[0][2] == player)
    {
        if((node->board[1][1]!=second_player) && (node->board[2][0]!=second_player))
        {
            sum_player = sum_player + 1;
        }
    }
    else if(node->board[1][1] == player)
    {
        if((node->board[2][0]!=second_player) && (node->board[0][2]!=second_player))
        {
            sum_player = sum_player + 1;
        }
    }
    else if(node->board[2][0] == player)
    {
        if((node->board[1][1]!=second_player) && (node->board[0][2]!=second_player))
        {
            sum_player = sum_player + 1;
        }
    }
    else if(node->board[0][2] == second_player)
    {
        if((node->board[1][1]!=player) && (node->board[2][0]!=player))
        {
            sum_sec_player = sum_sec_player + 1;
        }
    }
    else if(node->board[1][1] == second_player)
    {
        if((node->board[0][2]!=player) && (node->board[2][0]!=player))
        {
            sum_sec_player = sum_sec_player + 1;
        }
    }
    else if(node->board[2][0] == second_player)
    {
        if((node->board[0][2]!=player) && (node->board[1][1]!=player))
        {
            sum_sec_player = sum_sec_player + 1;
        }
    }
    //remove extra occurences during horizontal and vertical scan
    for(i = 0; i<3; i++)
    {
        count_player = 0;
        count_sec_player = 0;
        for(j = 0; j<3; j++)
        {
           if(node->board[i][j]==player)
           {
               count_player = count_player + 1;
           }
           if(node->board[i][j]==second_player)
           {
               count_sec_player = count_sec_player + 1;
           }
        }
        if(count_sec_player==0 && count_player==3)
        {
            sum_player = sum_player - 2;
        }
        if(count_sec_player==0 && count_player==2)
        {
            sum_player = sum_player - 1;
        }
        if(count_player==0 && count_sec_player==3)
        {
            sum_sec_player = sum_sec_player - 2;
        }
        if(count_player==0 && count_sec_player==2)
        {
            sum_sec_player = sum_sec_player - 1;
        }
    }

    for(j = 0; j<3; j++)
    {
        count_player = 0;
        count_sec_player = 0;
        for(i = 0; i<3; i++)
        {
           if(node->board[i][j]==player)
           {
               count_player = count_player + 1;
           }
           if(node->board[i][j]==second_player)
           {
               count_sec_player = count_sec_player + 1;
           }
        }
        if(count_sec_player==0 && count_player==3)
        {
            sum_player = sum_player - 2;
        }
        if(count_sec_player==0 && count_player==2)
        {
            sum_player = sum_player - 1;
        }
        if(count_player==0 && count_sec_player==3)
        {
            sum_sec_player = sum_sec_player - 2;
        }
        if(count_player==0 && count_sec_player==2)
        {
            sum_sec_player = sum_sec_player - 1;
        }
    }
    //if the game is over then assign most positive or most negative
    for(i = 0; i<3; i++)
    {
        if((node->board[i][0]==node->board[i][1]) && (node->board[i][0] == node->board[i][2]) && (node->board[i][0]==player))
        {
            return 10;
        }
        if((node->board[i][0]==node->board[i][1]) && (node->board[i][0] == node->board[i][2]) && (node->board[i][0]==second_player))
        {
            return -10;
        }
    }
    for(j = 0; j<3; j++)
    {
        if((node->board[0][j]==node->board[1][j]) && (node->board[0][j]==node->board[2][j]) && (node->board[0][j]==player))
        {
            return 10;
        }
        if((node->board[0][j]==node->board[1][j]) && (node->board[0][j]==node->board[2][j]) && (node->board[0][j]==second_player))
        {
            return -10;
        }
    }
    if((node->board[0][0]==node->board[1][1]) && (node->board[0][0]==node->board[2][2]) && (node->board[0][0]==player))
    {
        return 10;
    }
    if((node->board[0][0]==node->board[1][1]) && (node->board[0][0]==node->board[2][2]) && (node->board[0][0]==second_player))
    {
        return -10;
    }
    if((node->board[0][2]==node->board[1][1]) && (node->board[0][2]==node->board[2][0]) && (node->board[0][2]==player))
    {
        return 10;
    }
    if((node->board[0][2]==node->board[1][1]) && (node->board[0][2]==node->board[2][0]) && (node->board[0][2]==second_player))
    {
        return -10;
    }


    return(sum_player-sum_sec_player);
}

int game_is_not_over(char board[3][3])
{
    int i,j;

    //horizontal check
    for(i = 0; i<3; i++)
    {
        if((board[i][0]==board[i][1]) && (board[i][0] == board[i][2]) && (board[i][0]=='X'))
        {
            printf("Player wins\n\n");
            return 0;
        }
        if((board[i][0]==board[i][1]) && (board[i][0] == board[i][2]) && (board[i][0]=='O'))
        {
            printf("Computer wins\n\n");
            return 0;
        }
    }
    //vertical check
    for(j = 0; j<3; j++)
    {
        if((board[0][j]==board[1][j]) && (board[0][j]==board[2][j]) && (board[0][j]=='X'))
        {
            printf("Player wins\n\n");
            return 0;
        }
        if((board[0][j]==board[1][j]) && (board[0][j]==board[2][j]) && (board[0][j]=='O'))
        {
            printf("Computer wins\n\n");
            return 0;
        }
    }
    //diagonal check
    if((board[0][0]==board[1][1]) && (board[0][0]==board[2][2]) && (board[0][0]=='X'))
    {
        printf("Player wins\n\n");
        return 0;
    }
    if((board[0][0]==board[1][1]) && (board[0][0]==board[2][2]) && (board[0][0]=='O'))
    {
        printf("Computer wins\n\n");
        return 0;
    }
    if((board[0][2]==board[1][1]) && (board[0][2]==board[2][0]) && (board[0][2]=='X'))
    {
        printf("Player wins\n\n");
        return 0;
    }
    if((board[0][2]==board[1][1]) && (board[0][2]==board[2][0]) && (board[0][2]=='O'))
    {
        printf("Computer wins\n\n");
        return 0;
    }
    //check if the game can still continue
    for(i = 0; i<3; i++)
    {
        for(j = 0; j<3; j++)
        {
            if(board[i][j]=='e') //'e' represent empty
            {
                return 1;
            }
        }
    }
    printf("Game Draw\n\n");
    return 0;
}

int someone_has_won(char board[3][3])
{
    int i,j;
    //horizontal check
    for(i = 0; i<3; i++)
    {
        if((board[i][0]==board[i][1]) && (board[i][0] == board[i][2]) && (board[i][0]!='e'))
        {
            return 1;
        }
    }
    //vertical check
    for(j = 0; j<3; j++)
    {
        if((board[0][j]==board[1][j]) && (board[0][j]==board[2][j]) && (board[0][j]!='e'))
        {
            return 1;
        }
    }
    //diagonal check
    if((board[0][0]==board[1][1]) && (board[0][0]==board[2][2]) && (board[0][0]!='e'))
    {
        return 1;
    }
    if((board[0][2]==board[1][1]) && (board[0][2]==board[2][0]) && (board[0][2]!='e'))
    {
        return 1;
    }
    return 0;
}
