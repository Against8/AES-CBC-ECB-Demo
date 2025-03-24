#ifndef _AES_H
#define _AES_H
#include"assist.h"
#include"expandedkey.h"

/**********字节代替**********/
void subBytes(int array[4][4])
{
    int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			array[i][j]=getNumFromSBox(array[i][j]);
		}
	}
}
/**********行移位**********/
void shiftRows(int array[4][4])
{
	int i;
    int rowTwo[4], rowThree[4], rowFour[4];
	for(i=0;i<4;i++)			//赋值给临时数组
	{
		rowTwo[i]=array[1][i];
		rowThree[i]=array[2][i];
		rowFour[i]=array[3][i];
	}
    leftLoop4int(rowTwo, 1);	//第二行循环左移一位
    leftLoop4int(rowThree, 2);	//第三行循环左移两位
    leftLoop4int(rowFour, 3);	//第四行循环左移三位

    for(i = 0; i < 4; i++)		//做完处理后赋回去
    {
        array[1][i] = rowTwo[i];
        array[2][i] = rowThree[i];
        array[3][i] = rowFour[i];
    }
}
/**********列混合(PPT:35)**********/
void mixColumns(int array[4][4])
{
	int i,j;
    int tempArray[4][4];	//设置临时数组，用于后面函数处理
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			tempArray[i][j]=array[i][j];
		}
	}
    for(i=0;i<4;i++)		//列混淆运算
	{
		for(j=0;j<4;j++)
		{
			array[i][j]= GFMul(colM[i][0],tempArray[0][j]) ^ GFMul(colM[i][1],tempArray[1][j])
						^GFMul(colM[i][2],tempArray[2][j]) ^ GFMul(colM[i][3],tempArray[3][j]);
		}
	}
}
/**********轮密钥加(PPT:39)**********/
void addRoundKey(int array[4][4], int round)
{
	int i,j;
    int lmyray[4];		
    for(i=0;i<4;i++)
    {
        splitIntToArray(w[round*4+i],lmyray);//便于后面进行轮密钥加
		/*
			splitIntToArray(w[round * 4 + i], lxmray)
			把一个4字节的数的第一、二、三、四个字节取出，并将其装进一个包含4个元素的整型数组里面。
		如果不拆开，则w[round*4+i]是以32b的形式存在，如果拆开了，则是以4*8b的形式存在。
		（如下，w[0]是以ac7766f3连在一起的形式存在，而拆开后则是以lmyray[0]=ac,lymray[1]=77,
			lymray[2]=66,lymray[3]=f3的形式存在）
		即拆开了才能满足4*4=16，才能更好与array[4][4]进行轮密钥加，以下则是轮密钥加（按位异或）的例子。
			47	40	a3	4c		ac	19	28	57		eb	59	8b	1b
			37	d4	70	9f		77	fa	d1	5c		40	2b	a1	c3
			94	e4	3a	42	⊕	66	dc	29	00	＝	f2	38	13	42
			ed	a5	a6	bc		f3	21	41	6a		le	84	f7	d6
							   w[0]w[1]w[2]w[3]
		eg:{47} ⊕ {AC} = {01000111} ⊕ {10101100} = {11101011} = {EB}
		*/
		for(j=0;j<4;j++)
		{
			array[j][i]=array[j][i]^lmyray[j];	//在这里逐个进行轮密钥按位异或
		}
    }
}

/*
AES加密
参数：
p: 明文的字符串数组
plen: 明文的长度
key: 密钥的字符串数组
*/
void aes(char *p, int plen, char *key)
{
    int keylen = strlen(key);//strlen计算key的长度
    int pArray[4][4];//用于存放4x4的数组
    int k,i;

    extendKey(key);//扩展密钥

    for(k = 0; k < plen; k += 16)
    {
        convertToIntArray(p + k, pArray);
		//把16个字符转变成4X4的数组，该矩阵中字节的排列顺序为从上到下，从左到右依次排列。
        addRoundKey(pArray, 0);//一开始的轮密钥加

        for(i = 1; i < 10; i++)
        {
            subBytes(pArray);//字节代换
            shiftRows(pArray);//行移位
            mixColumns(pArray);//列混合
            addRoundKey(pArray, i);//轮密钥加
        }

        subBytes(pArray);//字节代换
        shiftRows(pArray);//行移位
        addRoundKey(pArray, 10);//轮密钥加
        convertArrayToStr(pArray, p + k);//把4X4数组转回字符串
    }
}
#endif