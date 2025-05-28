#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_EXPR_LEN 256
#define MAX_STACK_SIZE 256

typedef enum {INFIX, POSTFIX} Mode;
typedef struct {
    double data[MAX_STACK_SIZE];
    int top;
} DoubleStack;


typedef struct {
    char data[MAX_STACK_SIZE];
    int top;
} CharStack;

void double_stack_init(DoubleStack *s) { s->top = -1; }

int double_stack_empty(DoubleStack *s) { return s->top == -1; }

int double_stack_full(DoubleStack *s) { return s->top == MAX_STACK_SIZE - 1; }

void double_stack_push(DoubleStack *s, double val) {
    if (double_stack_full(s)) {
        printf("Error: Operand stack overflow!\n");
        exit(EXIT_FAILURE);
    }
    s->data[++(s->top)] = val; //stack er position incrementr hoye value insert hocche
}
double double_stack_pop(DoubleStack *s) {
    if (double_stack_empty(s)) {
        printf("Error: Operand stack underflow!\n");
        exit(EXIT_FAILURE);
    }
    return s->data[(s->top)--]; //top er position decrement korbe
}


void char_stack_init(CharStack *s) { s->top = -1; }
int char_stack_empty(CharStack *s) { return s->top == -1; }
int char_stack_full(CharStack *s) { return s->top == MAX_STACK_SIZE - 1; }
void char_stack_push(CharStack *s, char val) {
    if (char_stack_full(s)) {
        printf("Error: Operator stack overflow!\n");
        exit(EXIT_FAILURE);
    }
    s->data[++(s->top)] = val;
}
char char_stack_pop(CharStack *s) {
    if (char_stack_empty(s)) {
        printf("Error: Operator stack underflow!\n");
        exit(EXIT_FAILURE);
    }
    return s->data[(s->top)--];
}
char char_stack_peek(CharStack *s) {
    if (char_stack_empty(s)) return '\0';
    return s->data[s->top];
}



// Operators including all requested: + - * / % ^
int is_operator(char c) {
    return c=='+' || c=='-' || c=='*' || c=='/' || c=='%' || c=='^';
}



// Precedence: ^ highest (3, right assoc), * / % next(2), + - lowest(1)
int precedence(char op) {
    switch(op) {
        case '^': return 3;
        case '*': case '/': case '%': return 2;
        case '+': case '-': return 1;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////

// For associativity: return 1 if right-associative else 0 (left-associative)
int is_right_associative(char op) {
    return op == '^';
}


int is_open_paren(char c) {
    return c=='(' || c=='{' || c=='[';
}


int is_close_paren(char c) {
    return c==')' || c=='}' || c==']';
}



int is_matching_pair(char open, char close) {
    return (open=='(' && close==')') ||
           (open=='{' && close=='}') ||
           (open=='[' && close==']');
}



// Convert infix expression to postfix with all parentheses and operators
int infix_to_postfix(const char *infix, char *postfix) {
    CharStack op_stack;
    char_stack_init(&op_stack);
    int i=0, k=0;
    while(infix[i]) {
        char c = infix[i];
        if (isspace(c)) { i++; continue; }
        if (isdigit(c) || c=='.') {
            while (isdigit(infix[i]) || infix[i]=='.') {
                postfix[k++] = infix[i++];
            }
            postfix[k++] = ' ';
            continue;
        }
        else if (is_open_paren(c)) {
            char_stack_push(&op_stack, c);
        }
        else if (is_close_paren(c)) {
            int found_match = 0;
            while(!char_stack_empty(&op_stack)) {
                char top = char_stack_pop(&op_stack);
                if (is_open_paren(top)) {
                    if (!is_matching_pair(top, c)) {
                        printf("Error: Mismatched parentheses '%c' and '%c'\n", top, c);
                        return 0;
                    }
                    found_match = 1;
                    break;
                }
                postfix[k++] = top;
                postfix[k++] = ' ';
            }
            if (!found_match) {
                printf("Error: Unmatched closing parenthesis '%c'\n", c);
                return 0;
            }
        }
        else if (is_operator(c)) {
            while (!char_stack_empty(&op_stack)) {
                char top = char_stack_peek(&op_stack);
                if (is_open_paren(top)) break;
                int top_prec = precedence(top);
                int cur_prec = precedence(c);
                if ( (top_prec > cur_prec) || (top_prec == cur_prec && !is_right_associative(c)) ) {
                    postfix[k++] = char_stack_pop(&op_stack);
                    postfix[k++] = ' ';
                } else break;
            }
            char_stack_push(&op_stack, c);
        }
        else {
            printf("Error: Invalid character '%c' in expression\n", c);
            return 0;
        }
        i++;
    }
    while(!char_stack_empty(&op_stack)) {
        char top = char_stack_pop(&op_stack);
        if (is_open_paren(top)) {
            printf("Error: Unmatched opening parenthesis '%c'\n", top);
            return 0;
        }
        postfix[k++] = top;
        postfix[k++] = ' ';
    }
    postfix[k] = '\0';
    return 1;
}



// Evaluate postfix expression with all operators including modulus and power
int evaluate_postfix(const char *postfix, double *result) {
    DoubleStack val_stack;
    double_stack_init(&val_stack);
    int i=0;
    while(postfix[i]) {
        if (isspace(postfix[i])) { i++; continue; }
        if (isdigit(postfix[i]) || postfix[i]=='.') {
            char number_buffer[64];
            int nb_i=0;
            while(isdigit(postfix[i])||postfix[i]=='.') {
                if(nb_i<63) number_buffer[nb_i++] = postfix[i];
                i++;
            }
            number_buffer[nb_i] = '\0';
            double val = atof(number_buffer);
            double_stack_push(&val_stack, val);
        }
        else if (is_operator(postfix[i])) {
            if(val_stack.top<1) {
                printf("Error: Not enough operands for operator '%c'\n", postfix[i]);
                return 0;
            }
            double b = double_stack_pop(&val_stack);
            double a = double_stack_pop(&val_stack);
            double res = 0;
            switch(postfix[i]) {
                case '+': res = a + b; break;
                case '-': res = a - b; break;
                case '*': res = a * b; break;
                case '/':
                    if (b == 0) {
                        printf("Error: Division by zero\n");
                        return 0;
                    }
                    res = a / b;
                    break;
                case '%':
                    {
                        int ia = (int)a;
                        int ib = (int)b;
                        if (ib == 0) {
                            printf("Error: Modulus by zero\n");
                            return 0;
                        }
                        res = ia % ib;
                    }
                    break;
                case '^':
                    res = pow(a,b);
                    break;
                default:
                    printf("Error: Unknown operator '%c'\n", postfix[i]);
                    return 0;
            }
            double_stack_push(&val_stack,res);
            i++;
        }
        else {
            printf("Error: Invalid character '%c' in postfix expression\n", postfix[i]);
            return 0;
        }
    }
    if(val_stack.top != 0) {
        printf("Error: Invalid postfix expression. Too many operands.\n");
        return 0;
    }
    *result = double_stack_pop(&val_stack);
    return 1;
}



// Evaluate infix expression (convert to postfix first then evaluate)
int evaluate_infix(const char *infix, double *result) {
    char postfix[MAX_EXPR_LEN * 2];
    if(!infix_to_postfix(infix, postfix)) return 0;
    return evaluate_postfix(postfix, result);
}



// Evaluate postfix input expression string directly
int evaluate_postfix_input(const char *postfix, double *result) {
    // Just evaluate postfix directly
    return evaluate_postfix(postfix, result);
}



void flush_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}



int main() {
    Mode mode = INFIX;
    char input[MAX_EXPR_LEN];
    printf("Welcome to the C Calculator!\n");
    printf("Supports operators + - * / %% ^ (power) and parentheses (), {}, []\n");
    printf("Two modes available: INFIX (default) and POSTFIX.\n");
    printf("Commands: 'mode' to switch mode, 'exit' to quit.\n\n");



    while(1) {
        printf("[Mode: %s] Enter expression (or 'mode'/'exit'): ", mode == INFIX ? "INFIX" : "POSTFIX");
        if (!fgets(input, sizeof(input), stdin)) {
            printf("Input error\n");
            break;
        }
        // Remove trailing newline
        size_t len = strlen(input);
        if (len>0 && input[len-1]=='\n') input[len-1]='\0';



        if (strcmp(input, "exit") == 0) {
            printf("Exiting calculator. Goodbye!\n");
            break;
        }
        if (strcmp(input, "mode") == 0) {
            if (mode == INFIX) {
                mode = POSTFIX;
            } else {
                mode = INFIX;
            }
            printf("Switched mode to %s\n", mode == INFIX ? "INFIX" : "POSTFIX");
            continue;
        }
        if(strlen(input)==0) continue;


        double result = 0;
        int success = 0;
        if(mode == INFIX) {
            success = evaluate_infix(input, &result);
        } else {
            success = evaluate_postfix_input(input, &result);
        }
        if(success) {
            printf("Result: %.10g\n\n", result);
        } else {
            printf("Calculation failed. Please check your expression and try again.\n\n");
        }
    }



    return 0;
}
