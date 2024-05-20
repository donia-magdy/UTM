#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int current_state;
    char read_symbol;
    int next_state;
    char write_symbol;
    char action;  // 'L' for left, 'R' for right, 'Y' for accept, 'N' for reject
} Transition;
void printTape(char *tape, int head_position)
{
    for (int i = 0; i < strlen(tape); i++)
    {
        if (i == head_position)
            printf("[%c]", tape[i]);
        else
            printf("%c", tape[i]);
    }
    printf("\n");
}
void executeTM(int initstate,char *tape,Transition *transitions, int num_transitions, int head_position)
{
    int tape_len = strlen(tape);
    int current_state = initstate;

    printf("Initial Tape: %s\n", tape);
    printf("Head Position: %d\n", head_position);

    while (1)
    {
        int found_transition = 0;
        for (int i = 0; i < num_transitions; i++)
        {
            if (transitions[i].current_state == current_state && transitions[i].read_symbol == tape[head_position])
            {
                printf("Step: State: %d, Head: %d, Tape: ", current_state, head_position);
                printTape(tape, head_position);
                printf("\n");
                found_transition = 1;
                current_state = transitions[i].next_state;
                tape[head_position] = transitions[i].write_symbol;


                if (transitions[i].action == 'L')
                {
                    head_position--;
                    if (head_position < 0)
                    {
                        printf("Error: Head moved out of tape bounds!\n");
                        return;
                    }
                }
                else if (transitions[i].action == 'R')
                {
                    head_position++;
                    if (head_position == tape_len)
                    {
                        tape = (char *)realloc(tape, (tape_len + 2) * sizeof(char));
                        if (tape == NULL)
                        {
                            printf("Memory allocation failed.\n");
                            exit(1);
                        }
                        tape[tape_len] = '#';  // Add the blank symbol at the end
                        tape_len++;
                        tape[tape_len] = '\0';
                    }
                }
                else if (transitions[i].action == 'Y')
                {
                    printf("Accept state reached. Halting\n");
                    printf("After execution: ");
                    printTape(tape, head_position);
                    return;
                }
                else if (transitions[i].action == 'N')
                {
                    printf("Reject state reached. Halting\n");
                    printf("After execution: ");
                    printTape(tape, head_position);
                    return;
                }
                break;
            }
        }

        if (!found_transition)
        {
            printf("No transition found. Halting...\n");
            break;
        }
    }

    printf("Final Tape: %s\n", tape);
}
// Convert an integer to a unary string
char* int_to_unary(int n)
{
    char *unary = (char *)malloc((n + 2) * sizeof(char));
    for (int i = 0; i <= n; i++)
    {
        unary[i] = '1';
    }
    unary[n+1] = '\0';
    return unary;
}

// Convert an action to a unary representation
char* action_to_unary(char action)
{
    switch (action)
    {
    case 'L':
        return "1";
    case 'R':
        return "11";
    case 'Y':
        return "111";
    case 'N':
        return "1111";
    default:
        return "";
    }
}


void encodeTM(int num_states, char** alphabet, int num_symbols, char *tape, Transition *transitions, int num_transitions)
{
    printf("Encoded Turing Machine:\n");
    printf("States: %d\n", num_states);
    for(int i=0; i<num_states; i++)
    {
        printf("%d %s \n",i,int_to_unary(i));
    }
    printf("Alphabet: \n");
    for(int i=0; i<num_symbols; i++)
    {
        printf("%s %s \n",alphabet[i],int_to_unary(i));
    }
    printf("Transitions:\n");
    for (int i = 0; i < num_transitions; i++)
    {
        // Find the unary encoding for read and write symbols from the alphabet
        char* read_symbol_unary = NULL;
        char* write_symbol_unary = NULL;

        for (int j = 0; j < num_symbols; j++)
        {
            if (transitions[i].read_symbol == alphabet[j][0])
            {
                read_symbol_unary = int_to_unary(j);
            }
            if (transitions[i].write_symbol == alphabet[j][0])
            {
                write_symbol_unary = int_to_unary(j);
            }
        }

        printf("%s0%s0%s0%s0%s",
               int_to_unary(transitions[i].current_state),
               read_symbol_unary,
               int_to_unary(transitions[i].next_state),
               write_symbol_unary,
               action_to_unary(transitions[i].action));
        if(i!=num_transitions-1)
        {
            printf("00");
        }
        // Free the allocated unary strings after using them
        free(read_symbol_unary);
        free(write_symbol_unary);


    }
    printf("\n");
    /*printf("Tape: ");
    for (int i = 0; i < strlen(tape); i++)
    {
        for (int j = 0; j < num_symbols; j++)
        {
            if (tape[i] == alphabet[j][0])
            {
                printf("%s", int_to_unary(j));
                if(j!=num_symbols-1)
                {
                    printf("0");
                }
                break;
            }

        }
    }*/
}
int main()
{
    printf("Enter the number of states:");
    int k;
    scanf("%d",&k);
    while(k<=0)
    {
        printf("Please enter valid number of states:");
        scanf("%d",&k);
    }
    printf("Enter Alphabet[Gamma]: ");
    char *gamma;
    gamma=(char *)malloc(100 * sizeof(char));


    if (gamma == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }
    scanf("%s",gamma);
    char **alphabet = NULL;
    char* ptr;
    int token_count = 0;
    ptr = strtok(gamma, ",");
    while (ptr != NULL)
    {
        alphabet = (char **)realloc(alphabet, (token_count + 1) * sizeof(char *));
        if (alphabet == NULL)
        {
            printf("Memory allocation failed.\n");
            return 1;
        }

        alphabet[token_count] = (char *)malloc(strlen(ptr) + 1);
        if (alphabet[token_count] == NULL)
        {
            printf("Memory allocation failed.\n");
            return 1;
        }
        strcpy(alphabet[token_count], ptr);
        token_count++;

        ptr = strtok(NULL, ",");
    }

    //printf("%d \n",token_count);
    int initstate;
    printf("Enter the initial state: ");
    scanf("%d",&initstate);
    while(initstate>k)
    {
        printf("Enter valid initial state: ");
        scanf("%d",&initstate);
    }
    printf("Enter the tape: ");
    char *tape;
    tape=(char *)malloc(100 * sizeof(char));


    if (tape == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }
    scanf("%s",tape);
    int head;
    printf("Enter Head position [start from 0]: ");
    scanf("%d",&head);
    /*char transitions[k*token_count][5];
    printf("Enter transitions: \n");
    for (int i = 0; i < k*token_count; i++) {
        scanf(" %c,%c,%c,%c,%c",&transitions[i][0],&transitions[i][1],&transitions[i][2],&transitions[i][3],&transitions[i][4]);
    }*/
    int num_transitions = k * token_count;
    Transition *transitions = (Transition *)malloc(num_transitions * sizeof(Transition));

    printf("Enter transitions (current_state, read_symbol, next_state, write_symbol, action): \n");
    for (int i = 0; i < num_transitions; i++)
    {
        scanf(" %d,%c,%d,%c,%c", &transitions[i].current_state, &transitions[i].read_symbol, &transitions[i].next_state, &transitions[i].write_symbol, &transitions[i].action);
    }
    printf("Before execution: ");
    printTape(tape, head);
    executeTM(initstate, tape, transitions, num_transitions, head);
    encodeTM(k, alphabet, token_count, tape, transitions, num_transitions);
    return 0;
}
