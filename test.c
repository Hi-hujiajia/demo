
#include "stdio.h"
#include <string.h>
#include<math.h>    //pow()
#include<windows.h>  
#include<time.h>
#define PY_SSIZE_T_CLEAN
#include<Python.h> 


void Trim(char *src);
int vChange(int num);    //v门变化
int v1Change(int num);   // v+ 门变化

#define MAXLEN 1000
#define QLEN 200              //最大的量子线数
char ln[MAXLEN];             // 存贮行数据
char Circuit[MAXLEN][QLEN];  // 存储数据

char Circuit01[MAXLEN][QLEN];  //正负控制点存储  1为1控制点 0为0 控制点

int v[QLEN];                 //值
int a[QLEN];                 //对应输出值
int qnum;                   //当前的量子线数

int temp[QLEN];            //temp 存贮1->01   0->10
int UM[QLEN][QLEN];        //最终矩阵
int UM1[QLEN][QLEN];       //计算对应列的矩阵

int num[QLEN];               // count存贮   是v,v+否存在控制位的参数 

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
	{                                  // 读取电路存储到circuit
		if (NULL == fgets(ln, MAXLEN, fp)) break;   // fgets 读取一行数据
		Trim(ln);                              // Trim 去除一行前端后端的空格 换行符
		ilen = strlen(ln);
		if (ilen == 0) continue;                 // 空行跳过

		if (itop == 1)                          // 第一行  qnum
		{
			itop = 0;
			qnum = ln[ilen - 1] - 'a' + 1;           // 取qnum 值
			//printf("qnum=%d\n",qnum);     
		}
		//printf("%s",ln);
		//printf("\n");
		if (strcmp(ln, "BEGIN") == 0)           // 如果 为BEGIN行 则读下一行
		{
			ibegin = 1;
			continue;
		}
		if (!ibegin)                          // 如果 ibegin 为0 
			continue;
		if (strcmp(ln, "END") == 0)
			iend = 1;
		if (iend) break;
		//dui t v 开头分别处理
		int count = 0;//计算，个数
		if (ln[0] == 'T' || ln[0] == 't')  //开头为t
		{
			j = 0;
			for (i = 0; i < ilen; i++)
			{
				if (ln[i] == ' ');            //传值进入ln[]  ln[i] == ','
				else if (ln[i] == ',') count++;
				else if (ln[i] == '\'')Circuit01[idx][j - 1] = 0;//判断是否包含 单引号'
				else{
					Circuit01[idx][j] = 1;
					Circuit[idx][j++] = ln[i];
				}
			}
			num[idx] = count + 1;
		}
		//开头为v  对cc[][1]进行处理
		else
		{
			Circuit[idx][0] = 'V';
			j = 2;  //从3位进行赋值，弥补v  v+没有个数的空位
			if (ln[1] == '+') //第二位为"+"  V+    cc[][1]='+'
			{
				Circuit[idx][1] = '+';
				for (i = 2; i < ilen; i++)
				{
					if (ln[i] == ' ');
					else if (ln[i] == ',') count++;
					else if (ln[i] == '\'') Circuit01[idx][j - 1] = 0;  //判断是否包含 单引号'
					else{
						Circuit01[idx][j] = 1;
						Circuit[idx][j++] = ln[i];
					}
				}
			}
			else{  //v操作对cc[][1]进行赋0操作
				for (i = 2; i < ilen; i++)
				{
					Circuit[idx][1] = '0';
					if (ln[i] == ' ');
					else if (ln[i] == ',') count++;
					else if (ln[i] == '\'')  Circuit01[idx][j - 1] = 0; //判断是否包含 单引号'
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
	//idx 为电路的长度 多少个门
	// 输出 circuit
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

	// 二进制
	int n, n2, n3;                       //n 为2^n  
	for(n = 0; n < pow(2.0, qnum); n++)   //2^n 循环   //2^n 输入
	{
		int n3 = n;
		for (n2 = 0; n2 < qnum; n2++)        //变为2进制 存在数组a中
		{
			a[n2] = n3 % 2;
			n3 /= 2;
		}

		for (i = 0; i < idx; i++)  //经过电路门 计算  //开始实行v t门
		{
			int jump = 1;    //t v 时是否翻转
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
			else  //开头为v  上面并没有考虑v0 过not门的情况
			{
				// 是否考虑c^2的情况
				if (Circuit[i][1] == '+')// 为v+的情况
				{
					if (num[i] == 1){ //只进行v+操作
						a[Circuit[i][2]] = v1Change(a[Circuit[i][2]]);
					}
					else{  //判断是否进行v+操作 
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
				else //为v的情况
				{
					if (num[i] == 1){ //只进行v操作
						a[Circuit[i][2]] = vChange(a[Circuit[i][2]]);
					}
					else{  //判断是否进行v操作 
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
		// 2 直接改值
		UM[end][n] = 1;
	}

	//输出UM
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
int v1Change(int num)// v0 == 2  v1 ==3  v+变化函数
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
