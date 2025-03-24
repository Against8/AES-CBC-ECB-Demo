#ifndef _DEAES_H
#define _DEAES_H
#include"assist.h"

/**********逆字节代替**********/
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


/**********逆行移位**********/
static void deShiftRows(int array[4][4])
{
    int rowTwo[4], rowThree[4], rowFour[4];
    int i;
    for(i = 0; i < 4; i++)		//赋值给临时数组
    {
        rowTwo[i] = array[1][i];	
        rowThree[i] = array[2][i];
        rowFour[i] = array[3][i];
    }

    rightLoop4int(rowTwo, 1);	//第二行循环右移一位
    rightLoop4int(rowThree, 2);	//第三行循环右移两位
    rightLoop4int(rowFour, 3);	//第四行循环右移三位

    for(i = 0; i < 4; i++)		//做完处理后赋回去
    {
        array[1][i] = rowTwo[i];
        array[2][i] = rowThree[i];
        array[3][i] = rowFour[i];
    }
}

/**********逆列混合**********/
static void deMixColumns(int array[4][4])
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
    for(i = 0; i < 4; i++)	//逆列混淆运算
	{
        for(j = 0; j < 4; j++)
        {
            array[i][j] = GFMul(deColM[i][0],tempArray[0][j]) ^ GFMul(deColM[i][1],tempArray[1][j])
                          ^ GFMul(deColM[i][2],tempArray[2][j]) ^ GFMul(deColM[i][3], tempArray[3][j]);
        }
	}
}

/*
AES解密
参数：
c:密文的字符串数组
clen: 密文的长度
key: 密钥的字符串数组
*/
void deAes(char *c, int clen, char *key)
{
    int cArray[4][4];
    int keylen,k;
    keylen = strlen(key);
    extendKey(key);//扩展密钥

    for(k = 0; k < clen; k += 16)
    {
        int i;
        int wArray[4][4];
        convertToIntArray(c + k, cArray);
		//把16个字符转变成4X4的数组，该矩阵中字节的排列顺序为从上到下，从左到右依次排列。
        addRoundKey(cArray, 10);

        for(i = 9; i >= 1; i--)
        {
            deSubBytes(cArray);//逆字节代换
            deShiftRows(cArray);//逆行移位
			//下面这四行代码是为了还原正向加密时列混合和轮密钥加的影响
            deMixColumns(cArray);//逆列混合
            getArrayFrom4W(i, wArray);//从4个32位的密钥字中获得4X4数组，用于进行逆列混合
            deMixColumns(wArray);//逆列混合w数组
            addRoundTowArray(cArray, wArray);//把两个4x4的数组进行异或
        }

        deSubBytes(cArray);//逆字节代换
        deShiftRows(cArray);//逆行移位
        addRoundKey(cArray, 0);//轮密钥加
        convertArrayToStr(cArray, c + k);//把4X4数组转回字符串
    }
}


#endif