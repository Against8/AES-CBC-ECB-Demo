#ifndef _EXPANDEDKEY_H
#define _EXPANDEDKEY_H
#include"assist.h"

/*
Rcon������ֵ��
�ں���G()��ɰ��ֽ�S�д�������һ����������ϵ��Rcon����������㣨PPT44��
*/
const int Rcon[10] = {0x01000000, 0x02000000,
                      0x04000000, 0x08000000,
                      0x10000000, 0x20000000,
                      0x40000000, 0x80000000,
                      0x1b000000, 0x36000000};

//��Կ��չ�е�G()����(PPT:44)
int G(int num, int round)
{
    int numArray[4];
    int i;
    int result;
    splitIntToArray(num, numArray);//���ֽڷֿ�����һ������Ϊ�˱�����һ������λ
	/*
		splitIntToArray(num, numArray)
		��һ��4�ֽڵ����ĵ�һ�����������ĸ��ֽ�ȡ����������װ��һ������4��Ԫ�ص������������档
		numArray[0] numArray[1] numArray[2] numArray[3]
		    A			B			C			D
		���������ѭ�������Ժ󣬾ͱ���ˣ�
			B			C			D			A	
	*/
    leftLoop4int(numArray, 1);//ѭ������һ���ֽ�

    //S���ֽڴ���(S�У�PPT24)
	for(i=0;i<4;i++)
	{
		numArray[i]=getNumFromSBox(numArray[i]);
	}

    result = mergeArrayToInt(numArray);	//���ֽںϲ����ϲ���һ��4�ֽ�����
    return result ^ Rcon[round];		//��result����ϵ��Rcon�����������
}

//��Կ��Ӧ����չ����
int w[44];

//��չ��Կ������ǰ�w[44]�е�ÿ��Ԫ�س�ʼ��
void extendKey(char *key)
{
    int i,j;
	for(i=0;i<4;i++)	//���w[0],w[1],w[2],w[3]
	{
		w[i]=getWordFromStr(key+i*4);//��������4���ַ��ϲ���һ��4�ֽڵ�����
		/*����:����������1234567890123456
		�����ȷ���1234 5678 9012 3456,Ȼ��ֵ
		w[0]=1234,w[1]=5678,w[2]=9012,w[3]=3456
		*/
	}
	for(i=4,j=0;i<44;i++)
	{
		if(i%4==0)//�����w[4]��w[8]...,��w[i-1]����G�������������w[i-4]�����������
		{
			w[i]=w[i-4]^G(w[i-1],j);		// ^ ��ʾ��λ���
			j++;//��һ��
		}
		else//������ǣ���ֱ�ӽ�w[i-1]��w[i-4]�����������
		{
			w[i]=w[i-4]^w[i-1];
		}
	}
}

//��ӡW����
static void printW()
{
    int i, j;
    for(i = 0, j = 1; i < 44; i++,j++)
    {
        printf("w[%d] = 0x%x ", i, w[i]);
        if(j % 4 == 0) printf("\n");
    }
    printf("\n");
}

//��4��32λ����Կ���л��4X4���飬���ڽ������л��
void getArrayFrom4W(int i, int array[4][4])
{
    int index,j;
    int colOne[4], colTwo[4], colThree[4], colFour[4];
    index = i * 4;
	//��һ��4�ֽڵ����ĵ�һ�����������ĸ��ֽ�ȡ����������װ��һ������4��Ԫ�ص������������档
    splitIntToArray(w[index], colOne);
    splitIntToArray(w[index + 1], colTwo);
    splitIntToArray(w[index + 2], colThree);
    splitIntToArray(w[index + 3], colFour);

    for(j = 0; j < 4; j++)
    {
        array[j][0] = colOne[j];
        array[j][1] = colTwo[j];
        array[j][2] = colThree[j];
        array[j][3] = colFour[j];
    }
}

#endif
