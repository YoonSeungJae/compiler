#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#define MAX 100

int action_tbl[12][6] = {
    { 5, 0, 0, 4, 0, 0 },
    { 0, 6, 0, 0, 0, 999 },
    { 0, -2, 7, 0, -2, -2 },
    { 0, -4, -4, 0, -4, -4 },
    { 5, 0, 0, 4, 0, 0 },
    { 0, -6, -6, 0, -6, -6 },
    { 5, 0, 0, 4, 0, 0 },
    { 5, 0, 0, 4, 0, 0 },
    { 0, 6, 0, 0, 11, 0 },
    { 0, -1, 7, 0, -1, -1 },
    { 0, -3, -3, 0, -3, -3 },
    { 0, -5, -5, 0, -5, -5 }
};

int goto_tbl[12][4] = {
    { 0, 1, 2, 3 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 8, 2, 3 },
    { 0, 0, 0, 0 },
    { 0, 0, 9, 3 },
    { 0, 0, 0, 10 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
};

char lhs[] = { ' ', 'E', 'E', 'T', 'T', 'F', 'F' };
int rhs_len[] = { 0, 3, 1, 3, 1, 3, 1 }; //non-terminal과 terminal의 쌍의 개수
char token[] = { 'd', '+', '*', '(', ')', '$' };
char NT[] = { ' ', 'E', 'T', 'F' };

int stack[MAX];
int sp = -1;//스택 포인터 초기화

int isEmpty(int sp) {
    if (sp == -1)
        return 1;
    else
        return 0;
}

int isFull(int sp) {
    if (sp == MAX - 1)
        return 1;
    else
        return 0;
}

int push_stack(int data) {
    if (isFull(sp)) {
        printf("stack full\n");
        return 0;
    }
    else {
        sp++;
        stack[sp] = data;
    }
}

int pop_stack() {
    if (isEmpty(sp)) {
        printf("stack empty\n");
        return 0;
    }
    else {        
        sp--;
    }
}

void print_stack() {
    for (int i = 0;i < sp + 1;i++) {
        if (i % 2 == 0)
            printf("%d", stack[i]);
        else
            printf("%c", stack[i]);
    }
}

// 주어진 문자열에서 특정 문자의 인덱스를 찾는 함수
int find_index(char* arr, char c) {
    for (int i = 0; i < strlen(arr); i++) {
        if (arr[i] == c)
            return i;
    }
    return -1;
}

// LR parser
void LR_Parser(char* input) {
    char* ptr = input; //입력 문자열 출력
    int state = 0; //초기 상태
    int count = 1; // 파싱 단계
    push_stack(state); // 초기 상태 push
    printf("(%d)", count);
    printf("initial  :\t");
    print_stack();
    printf("\t%s\n", input);

    while (1) {
        state = stack[sp]; // 현재 상태
        int action = action_tbl[state][find_index(token, *ptr)]; // 파서 동작
        count++;
        printf("(%d)", count); //파싱 단계
        if (action == 999) {//accept
            printf("Accepted\n");
            while (1) { //스택 초기화
                if (sp == -1)
                    break;
                else
                    pop_stack();
            }
            break;

        }
        else if (action > 0) { // shift 동작            
            push_stack(*ptr);
            push_stack(action); //입력 내용과 현재 동작 스택에 입력
            printf("shift  %d :\t", action);
            print_stack();
            ptr++; // 다음 입력 문자로 이동
            printf("\t%s\n", ptr);
        }

        else if (action < 0) { // reduce 동작
            int rule = -action; // action이 -값이기 때문에
            printf("reduce %d :\t", rule);
            for (int i = 0; i < rhs_len[rule] * 2; i++) { //*2인 이유는 쌍의 개수이기 때문에
                pop_stack();
            }
            int newState = goto_tbl[stack[sp]][find_index(NT, lhs[rule])]; //축소된 새로운 상태 계산
            push_stack(lhs[rule]);
            push_stack(newState); // 새로운 상태를 스택에 추가
            print_stack();
            printf("\t%s\n", ptr);
        }

        else {
            if (find_index(token, *ptr) == -1) { //없는 토큰을 입력한 경우
                printf("Invalid token (%c) error\n", *ptr);
                while (1) {//스택 초기화
                    if (sp == -1)
                        break;
                    else
                        pop_stack();
                }
                break;
            }
            else {  //잘못된 문법을 입력한 경우
                printf("error\n");
                while (1) {//스택 초기화
                    if (sp == -1)
                        break;
                    else
                        pop_stack();
                }
                break;
            }
        }
    }
}

// 메인 함수
int main() {
    char input[MAX];
    while (1) {
        printf("\nInput: ");
        scanf("%s", input);
        if (input[0] == '$')
            break;
        LR_Parser(input); // LR 파서 호출
    }
    return 0;
}
