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
int rhs_len[] = { 0, 3, 1, 3, 1, 3, 1 }; //non-terminal�� terminal�� ���� ����
char token[] = { 'd', '+', '*', '(', ')', '$' };
char NT[] = { ' ', 'E', 'T', 'F' };

int stack[MAX];
int sp = -1;//���� ������ �ʱ�ȭ

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

// �־��� ���ڿ����� Ư�� ������ �ε����� ã�� �Լ�
int find_index(char* arr, char c) {
    for (int i = 0; i < strlen(arr); i++) {
        if (arr[i] == c)
            return i;
    }
    return -1;
}

// LR parser
void LR_Parser(char* input) {
    char* ptr = input; //�Է� ���ڿ� ���
    int state = 0; //�ʱ� ����
    int count = 1; // �Ľ� �ܰ�
    push_stack(state); // �ʱ� ���� push
    printf("(%d)", count);
    printf("initial  :\t");
    print_stack();
    printf("\t%s\n", input);

    while (1) {
        state = stack[sp]; // ���� ����
        int action = action_tbl[state][find_index(token, *ptr)]; // �ļ� ����
        count++;
        printf("(%d)", count); //�Ľ� �ܰ�
        if (action == 999) {//accept
            printf("Accepted\n");
            while (1) { //���� �ʱ�ȭ
                if (sp == -1)
                    break;
                else
                    pop_stack();
            }
            break;

        }
        else if (action > 0) { // shift ����            
            push_stack(*ptr);
            push_stack(action); //�Է� ����� ���� ���� ���ÿ� �Է�
            printf("shift  %d :\t", action);
            print_stack();
            ptr++; // ���� �Է� ���ڷ� �̵�
            printf("\t%s\n", ptr);
        }

        else if (action < 0) { // reduce ����
            int rule = -action; // action�� -���̱� ������
            printf("reduce %d :\t", rule);
            for (int i = 0; i < rhs_len[rule] * 2; i++) { //*2�� ������ ���� �����̱� ������
                pop_stack();
            }
            int newState = goto_tbl[stack[sp]][find_index(NT, lhs[rule])]; //��ҵ� ���ο� ���� ���
            push_stack(lhs[rule]);
            push_stack(newState); // ���ο� ���¸� ���ÿ� �߰�
            print_stack();
            printf("\t%s\n", ptr);
        }

        else {
            if (find_index(token, *ptr) == -1) { //���� ��ū�� �Է��� ���
                printf("Invalid token (%c) error\n", *ptr);
                while (1) {//���� �ʱ�ȭ
                    if (sp == -1)
                        break;
                    else
                        pop_stack();
                }
                break;
            }
            else {  //�߸��� ������ �Է��� ���
                printf("error\n");
                while (1) {//���� �ʱ�ȭ
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

// ���� �Լ�
int main() {
    char input[MAX];
    while (1) {
        printf("\nInput: ");
        scanf("%s", input);
        if (input[0] == '$')
            break;
        LR_Parser(input); // LR �ļ� ȣ��
    }
    return 0;
}
