#include <stdio.h>
#include <ctype.h>
#pragma warning (disable:4996)
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 100		//스택 최대 크기



char *OperatorStack;
double *OperandStack;
int Size1, Size2;
int top1, top2;

//OperatorStack에 대한 함수
void OperatorStackfree()             
{
	free(OperatorStack);
}

char OperatorStackpop()          
{
	if (top1 >= 0)
	{
		return OperatorStack[top1--];
	}
	else
	{
		return -1;
	}
}

int OperatorStackPush(char inputvalue)        
{
	if (top1 < Size1 - 1)
	{
		top1++;
		OperatorStack[top1] = inputvalue;

		return 1;
	}

	else {
		return 0;
	}
}

void OperatorStackInit(int stacksize)          
{
	Size1 = stacksize;
	OperatorStack = (char *)malloc(Size1 * sizeof(char));
	top1 = -1;
}

//OperandStack에 대한 함수

void OperandStackfree()
{
	free(OperandStack);
}

double OperandStackpop()
{
	if (top2 >= 0) {
		return OperandStack[top2--];
	}
	else {
		return -1;
	}
}

int OperandStackPush(double inputvalue)
{
	if (top2 < Size2 - 1) {
		top2++;
		OperandStack[top2] = inputvalue;
		return 1;
	}
	else {
		return 0;
	}
}

void OperandStackInit(int stacksize)
{
	Size2 = stacksize;
	OperandStack = (double *)malloc(Size2 * sizeof(double));
	top2 = -1;
}

int getOperatorPriority(int Operator)  
{
	switch (Operator) {
	case '(':
		return 0;
	case '+': case '-':
		return 1;
	case '*': case '/':
		return 2;
	case '^':
		return 3;
	}
	return 200;
}

void InfixtoPostfix(char *infixform, char *postfixform )  //중위표기 ->후위표기
{
	char *start = infixform;
	char *end = postfixform;
	OperatorStackInit(MAX);

	while (*start) {
		// isdigit(): 숫자면 true 반환
		// postfix 에서는 operand가 연이어 나올 수 있기 때문에 공백추가 필요
		if (isdigit(*start))
		{
			while (isdigit(*start) ) *end++ = *start++;
			*end++ = ' ';
		}
		

		else if (strchr("^*/+-", *start))
        //strchr(str,c)
        //첫번재 매개변수 str 의 문자열에서 두번째 매개변수로 들어온 c가 존재하는지 검사하고, 
        //문자가 존재하면 해당 문자가 존재하는 곳의 포인터를 반환하고,존재하지 않으면 널포인터 반환 
		{
			while (top1 != -1 && getOperatorPriority(OperatorStack[top1]) >= getOperatorPriority(*start))
			{
				*end++ = OperatorStackpop();
			}
            //비어있지 않고, 이미 pop에 있는게 들어갈거 보다 우선순위 높으면 그렇지 않을 때 까지 pop


			OperatorStackPush(*start++);
		}
		else
			//  ( 괄호는 가장 우선순위가 높다.
			if (*start == '(')
			{
				OperatorStackPush(*start++);
			}

			else if (*start == ')')
			{
		        // ) 괄호의 경우 (괄호가 나올때 까지 OperatorStack에 있는 연산자를 pop한다. 
				while(1)
				{
	               
					char foo = OperatorStackpop();
					if (foo == '('){
                    break;
                    }

					*end++ = foo;
                    
						
				}
				*start++;
			}
			else
			{
				*start++;
			}
	}


	// 모든 계산이 끝난 후에는 스택에 남아있는 연산자들을 차례대로 꺼낸다.
	while (top1 != -1)
	{
		*end++ = OperatorStackpop();
	}
	*end = 0;
	OperatorStackfree();
}

double resultfunction(const char *postfix_array)
{   
    printf("Evaluation starts…\n");
	const char *pfa_p = postfix_array;
	double result;
	double operand1, operand2;

	OperandStackInit(MAX);
	while (*pfa_p) {

		
		if (isdigit(*pfa_p)) {
			result = atof(pfa_p);
            //double atof(const char* _String);
            //문자열(char*)을 실수(double)로 변환하기 위해 사용된다.

			OperandStackPush(result);
            if(isdigit(*pfa_p)){pfa_p++;}
		}
		else {
			// 연산자가 나올 시 위의 두 operand을 계산한다.
			if (strchr("^*/+-", *pfa_p)) {
				operand2 = OperandStackpop();
				operand1 = OperandStackpop();
				
				switch (*pfa_p) {
				case '+': 
					OperandStackPush(operand1 + operand2);
                    printf("%f + %f = %f\n",operand1,operand2,operand1 + operand2);
					break;
				case '-':
					OperandStackPush(operand1 - operand2);
                    printf("%f - %f = %f\n",operand1,operand2,operand1 - operand2);

					break;
				case '*':
					OperandStackPush(operand1*operand2);
                    printf("%f * %f = %f\n",operand1,operand2,operand1 * operand2);

					break;
				case '/':
					OperandStackPush(operand1 / operand2);
                    printf("%f / %f = %f\n",operand1,operand2,operand1 / operand2);

					break;
				case '^':
					OperandStackPush(pow(operand1, operand2));
                    printf("%f ^ %f = %f\n",operand1,operand2,pow(operand1, operand2));

					break;
				}
			}
			
			
			// 연산자 안나오면 이동
			pfa_p++;
		}
	}
	if (top2 != -1) {
		result = OperandStackpop();
	}

	OperandStackfree();
	return result;
}


int main(void) {
	char infixform[MAX];
	char postfixform[MAX];
	double result;

	while(1)
	{

		printf("Please enter a formula : ");
        scanf("%s", &infixform);

		printf("Input expression(infixform): %s", infixform);
		printf("\n");


		InfixtoPostfix(infixform, postfixform );
		printf("Input expression(postfixform) : %s", postfixform);
		printf("\n");


		result = resultfunction(postfixform);

		printf("\n");

		printf(" Evaluation finishes. The result is : %f\n", result);

	}
}