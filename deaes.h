#ifndef _DEAES_H
#define _DEAES_H
#include"assist.h"

/**********���ֽڴ���**********/
static void deSubBytes(int array[4][4])
{
    int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			array[i][j]=getNumFromS1Box(array[i][j]);
		}
	}
}


/**********������λ**********/
static void deShiftRows(int array[4][4])
{
    int rowTwo[4], rowThree[4], rowFour[4];
    int i;
    for(i = 0; i < 4; i++)		//��ֵ����ʱ����
    {
        rowTwo[i] = array[1][i];	
        rowThree[i] = array[2][i];
        rowFour[i] = array[3][i];
    }

    rightLoop4int(rowTwo, 1);	//�ڶ���ѭ������һλ
    rightLoop4int(rowThree, 2);	//������ѭ��������λ
    rightLoop4int(rowFour, 3);	//������ѭ��������λ

    for(i = 0; i < 4; i++)		//���괦��󸳻�ȥ
    {
        array[1][i] = rowTwo[i];
        array[2][i] = rowThree[i];
        array[3][i] = rowFour[i];
    }
}

/**********���л��**********/
static void deMixColumns(int array[4][4])
{
    int i,j;
    int tempArray[4][4];	//������ʱ���飬���ں��溯������
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			tempArray[i][j]=array[i][j];
		}
	}
    for(i = 0; i < 4; i++)	//���л�������
	{
        for(j = 0; j < 4; j++)
        {
            array[i][j] = GFMul(deColM[i][0],tempArray[0][j]) ^ GFMul(deColM[i][1],tempArray[1][j])
                          ^ GFMul(deColM[i][2],tempArray[2][j]) ^ GFMul(deColM[i][3], tempArray[3][j]);
        }
	}
}

/*
AES����
������
c:���ĵ��ַ�������
clen: ���ĵĳ���
key: ��Կ���ַ�������
*/
void deAes(char *c, int clen, char *key)
{
    int cArray[4][4];
    int keylen,k;
    keylen = strlen(key);
    extendKey(key);//��չ��Կ

    for(k = 0; k < clen; k += 16)
    {
        int i;
        int wArray[4][4];
        convertToIntArray(c + k, cArray);
		//��16���ַ�ת���4X4�����飬�þ������ֽڵ�����˳��Ϊ���ϵ��£��������������С�
        addRoundKey(cArray, 10);

        for(i = 9; i >= 1; i--)
        {
            deSubBytes(cArray);//���ֽڴ���
            deShiftRows(cArray);//������λ
			//���������д�����Ϊ�˻�ԭ�������ʱ�л�Ϻ�����Կ�ӵ�Ӱ��
            deMixColumns(cArray);//���л��
            getArrayFrom4W(i, wArray);//��4��32λ����Կ���л��4X4���飬���ڽ������л��
            deMixColumns(wArray);//���л��w����
            addRoundTowArray(cArray, wArray);//������4x4������������
        }

        deSubBytes(cArray);//���ֽڴ���
        deShiftRows(cArray);//������λ
        addRoundKey(cArray, 0);//����Կ��
        convertArrayToStr(cArray, c + k);//��4X4����ת���ַ���
    }
}


#endif