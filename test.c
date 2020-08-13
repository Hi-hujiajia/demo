
#include "stdio.h"
#include <string.h>
#include<math.h>    //pow()
#include<windows.h>  
#include<time.h>
#define PY_SSIZE_T_CLEAN
#include<Python.h> 


void Trim(char *src);
int vChange(int num);    //v�ű仯
int v1Change(int num);   // v+ �ű仯

#define MAXLEN 1000
#define QLEN 200              //������������
char ln[MAXLEN];             // ����������
char Circuit[MAXLEN][QLEN];  // �洢����

char Circuit01[MAXLEN][QLEN];  //�������Ƶ�洢  1Ϊ1���Ƶ� 0Ϊ0 ���Ƶ�

int v[QLEN];                 //ֵ
int a[QLEN];                 //��Ӧ���ֵ
int qnum;                   //��ǰ����������

int temp[QLEN];            //temp ����1->01   0->10
int UM[QLEN][QLEN];        //���վ���
int UM1[QLEN][QLEN];       //�����Ӧ�еľ���

int num[QLEN];               // count����   ��v,v+����ڿ���λ�Ĳ��� 

double total_t;
clock_t start_t, finish_t;
void test_func(char *str)
{
	
	FILE *fp, *fcout;
	int ilen, idx = 0;
	int itop = 1, ibegin = 0, iend = 0;
	fp = fopen(str, "r");// ham3tc
	if (fp == NULL) return 1;
	int i, j;

	while (1)
	{                                  // ��ȡ��·�洢��circuit
		if (NULL == fgets(ln, MAXLEN, fp)) break;   // fgets ��ȡһ������
		Trim(ln);                              // Trim ȥ��һ��ǰ�˺�˵Ŀո� ���з�
		ilen = strlen(ln);
		if (ilen == 0) continue;                 // ��������

		if (itop == 1)                          // ��һ��  qnum
		{
			itop = 0;
			qnum = ln[ilen - 1] - 'a' + 1;           // ȡqnum ֵ
			//printf("qnum=%d\n",qnum);     
		}
		//printf("%s",ln);
		//printf("\n");
		if (strcmp(ln, "BEGIN") == 0)           // ��� ΪBEGIN�� �����һ��
		{
			ibegin = 1;
			continue;
		}
		if (!ibegin)                          // ��� ibegin Ϊ0 
			continue;
		if (strcmp(ln, "END") == 0)
			iend = 1;
		if (iend) break;
		//dui t v ��ͷ�ֱ���
		int count = 0;//���㣬����
		if (ln[0] == 'T' || ln[0] == 't')  //��ͷΪt
		{
			j = 0;
			for (i = 0; i < ilen; i++)
			{
				if (ln[i] == ' ');            //��ֵ����ln[]  ln[i] == ','
				else if (ln[i] == ',') count++;
				else if (ln[i] == '\'')Circuit01[idx][j - 1] = 0;//�ж��Ƿ���� ������'
				else{
					Circuit01[idx][j] = 1;
					Circuit[idx][j++] = ln[i];
				}
			}
			num[idx] = count + 1;
		}
		//��ͷΪv  ��cc[][1]���д���
		else
		{
			Circuit[idx][0] = 'V';
			j = 2;  //��3λ���и�ֵ���ֲ�v  v+û�и����Ŀ�λ
			if (ln[1] == '+') //�ڶ�λΪ"+"  V+    cc[][1]='+'
			{
				Circuit[idx][1] = '+';
				for (i = 2; i < ilen; i++)
				{
					if (ln[i] == ' ');
					else if (ln[i] == ',') count++;
					else if (ln[i] == '\'') Circuit01[idx][j - 1] = 0;  //�ж��Ƿ���� ������'
					else{
						Circuit01[idx][j] = 1;
						Circuit[idx][j++] = ln[i];
					}
				}
			}
			else{  //v������cc[][1]���и�0����
				for (i = 2; i < ilen; i++)
				{
					Circuit[idx][1] = '0';
					if (ln[i] == ' ');
					else if (ln[i] == ',') count++;
					else if (ln[i] == '\'')  Circuit01[idx][j - 1] = 0; //�ж��Ƿ���� ������'
					else{
						Circuit01[idx][j] = 1;
						Circuit[idx][j++] = ln[i];
					}
				}
			}
			num[idx] = count + 1;
		}
		idx++;

	}
	fclose(fp);
	//idx Ϊ��·�ĳ��� ���ٸ���
	// ��� circuit
	for(i = 0; i<idx; i++)
	{
		if (Circuit[i][0] == 'T')
		{
			Circuit[i][1] = Circuit[i][1] - '0';
		//	printf("%c %d ", Circuit[i][0], Circuit[i][1]);
		}
	/*	else
		{
			printf("%c %c ", Circuit[i][0], Circuit[i][1]);
		}*/

		for (j = 0; j<num[i]; j++)
		{
		Circuit[i][j + 2] = qnum - 1 + 'a' - Circuit[i][j + 2];
	//printf("%d ", Circuit[i][j + 2]);
		}
	//printf("\n");
	}

	// ������
	int n, n2, n3;                       //n Ϊ2^n  
	for(n = 0; n < pow(2.0, qnum); n++)   //2^n ѭ��   //2^n ����
	{
		int n3 = n;
		for (n2 = 0; n2 < qnum; n2++)        //��Ϊ2���� ��������a��
		{
			a[n2] = n3 % 2;
			n3 /= 2;
		}

		for (i = 0; i < idx; i++)  //������·�� ����  //��ʼʵ��v t��
		{
			int jump = 1;    //t v ʱ�Ƿ�ת
			if (Circuit[i][0] == 'T' || Circuit[i][0] == 't')
			{
				for (j = 0; j < num[i] - 1; j++)
				{
					if (a[Circuit[i][j + 2]] != Circuit01[i][j + 2])
					{
						jump = 0;
						break;
					}
				}
				if (jump == 1)
					a[Circuit[i][num[i] + 1]] = 1 - a[Circuit[i][num[i] + 1]];
			}
			else  //��ͷΪv  ���沢û�п���v0 ��not�ŵ����
			{
				// �Ƿ���c^2�����
				if (Circuit[i][1] == '+')// Ϊv+�����
				{
					if (num[i] == 1){ //ֻ����v+����
						a[Circuit[i][2]] = v1Change(a[Circuit[i][2]]);
					}
					else{  //�ж��Ƿ����v+���� 
						for (j = 0; j < num[i] - 1; j++)
						{
							if (a[Circuit[i][j + 2]] != Circuit01[i][j + 2])
							{
								jump = 0;
								break;
							}
						}
						if (jump == 1)
							a[Circuit[i][num[i] + 1]] = v1Change(a[Circuit[i][num[i] + 1]]);
					}
				}
				else //Ϊv�����
				{
					if (num[i] == 1){ //ֻ����v����
						a[Circuit[i][2]] = vChange(a[Circuit[i][2]]);
					}
					else{  //�ж��Ƿ����v���� 
						for (j = 0; j < num[i] - 1; j++)
						{
							if (a[Circuit[i][j + 2]] != Circuit01[i][j + 2])
							{
								jump = 0;
								break;
							}
						}
						if (jump == 1)
							a[Circuit[i][num[i] + 1]] = vChange(a[Circuit[i][num[i] + 1]]);
					}
				}
			}
		}
		int end = 0;
		for (n2 = qnum - 1; n2 >= 0; n2--)
		{
			end = end * 2 + a[n2];

		}
		// 2 ֱ�Ӹ�ֵ
		UM[end][n] = 1;
	}

	//���UM
	fcout = fopen("cout.txt", "w");
	for (i = 0; i < pow(2.0, qnum); i++)
	{
		for (j = 0; j < pow(2.0, qnum); j++)
		{
			fprintf(fcout,"%d  ",UM[i][j]);
		}
		fprintf(fcout, "\n");   
	}
	fclose(fcout);
}


void Trim(char *src)
{
	char *begin = src;
	char *end = src;

	while (*end && *end != '\n') {
		end++; // Statement is null  
	}

	if (begin == end)
	{
		*begin = 0;
		return;
	}

	while (*begin == ' ' || *begin == '\t')
		++begin;
	while ((*end) == '\0' || *end == ' ' || *end == '\t' || *end == '\n')
		--end;

	if (begin > end) {
		*src = '\0';  return;
	}
	//printf("begin=%1.1s; end=%1.1s\n", begin, end);
	while (begin != end) {
		*src++ = *begin++;
	}

	*src++ = *end;
	*src = '\0';

	return;
}
int vChange(int num)// v0 == 2  v1 ==3
{
	if (num == 0)
	{
		return 2;
	}
	else if (num == 1)
	{
		return 3;
	}
	else if (num == 2)
	{
		return 1;
	}
	else{
		return 0;
	}
}
int v1Change(int num)// v0 == 2  v1 ==3  v+�仯����
{
	switch (num)
	{
	case 0:
		return 3;
		break;
	case 1:
		return 2;
		break;
	case 2:
		return 0;
		break;
	case 3:
		return 1;
		break;
	default:
	    break;
	}
}

static PyObject *PyTestFunc(PyObject *self, PyObject *args)
{
	char *str=NULL;
	
	if(!PyArg_ParseTuple(args, "s", &str))
		return NULL;
		
	test_func(str);
	return (PyObject *)Py_BuildValue(""); 	
}


static PyMethodDef  testMethods[] = {
    {"test",(PyCFunction)PyTestFunc, METH_VARARGS | METH_KEYWORDS, ""},
    {NULL,NULL,0, NULL}
};

static struct PyModuleDef testModule = {
    PyModuleDef_HEAD_INIT,
    "test_module",
    "Python interface for the fputs C library function",
    -1,
    testMethods
};

PyMODINIT_FUNC PyInit_test_module(void)
{
    return PyModule_Create(&testModule);
}
