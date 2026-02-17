#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define EPS 'e'

#define MAX 100

int state = 0;

typedef struct NFA {
    int start;
    int end;
} NFA;

struct transition {
    int from;
    char symbol;
    int to;
} trans[MAX];

int t_index = 0;

NFA stack[MAX];
int top = -1;

void push(NFA n) {
    stack[++top] = n;
}

NFA pop() {
    return stack[top--];
}

void add_transition(int from, char symbol, int to) {
    trans[t_index].from = from;
    trans[t_index].symbol = symbol;
    trans[t_index].to = to;
    t_index++;
}

void symbol_nfa(char symbol) {
    NFA n;
    n.start = state++;
    n.end = state++;
    add_transition(n.start, symbol, n.end);
    push(n);
}

void concat_nfa() {
    NFA n2 = pop();
    NFA n1 = pop();

    add_transition(n1.end, EPS, n2.start);

    NFA n;
    n.start = n1.start;
    n.end = n2.end;

    push(n);
}

void union_nfa() {
    NFA n2 = pop();
    NFA n1 = pop();

    NFA n;
    n.start = state++;
    n.end = state++;

    add_transition(n.start, 'ε', n1.start);
    add_transition(n.start, 'ε', n2.start);
    add_transition(n1.end, 'ε', n.end);
    add_transition(n2.end, 'ε', n.end);

    push(n);
}

void star_nfa() {
    NFA n1 = pop();

    NFA n;
    n.start = state++;
    n.end = state++;

    add_transition(n.start, 'ε', n1.start);
    add_transition(n.start, 'ε', n.end);
    add_transition(n1.end, 'ε', n1.start);
    add_transition(n1.end, 'ε', n.end);

    push(n);
}

int main() {
    char postfix[50];
    printf("Enter postfix regular expression: ");
    scanf("%s", postfix);

    for(int i=0; i<strlen(postfix); i++) {
        char ch = postfix[i];

        if(ch == '*')
            star_nfa();
        else if(ch == '.')
            concat_nfa();
        else if(ch == '|')
            union_nfa();
        else
            symbol_nfa(ch);
    }

    NFA result = pop();

    printf("\nStart State: %d\n", result.start);
    printf("Final State: %d\n", result.end);

    printf("\nTransitions:\n");
    for(int i=0; i<t_index; i++) {
        printf("%d -- %c --> %d\n",
               trans[i].from,
               trans[i].symbol,
               trans[i].to);
    }

    return 0;
}