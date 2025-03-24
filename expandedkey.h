#ifndef _EXPANDEDKEY_H
#define _EXPANDEDKEY_H
#include"assist.h"

/*
Rcon常量轮值表
在函数G()完成按字节S盒代换后，下一步就是与轮系数Rcon进行异或运算（PPT44）
*/
const int Rcon[10] = {0x01000000, 0x02000000,
                      0x04000000, 0x08000000,
                      0x10000000, 0x20000000,
                      0x40000000, 0x80000000,
                      0x1b000000, 0x36000000};

//密钥扩展中的G()函数(PPT:44)
int G(int num, int round)
{
    int numArray[4];
    int i;
    int result;
    splitIntToArray(num, numArray);//四字节分开，这一步就是为了便于下一步行移位
	/*
		splitIntToArray(num, numArray)
		把一个4字节的数的第一、二、三、四个字节取出，并将其装进一个包含4个元素的整型数组里面。
		numArray[0] numArray[1] numArray[2] numArray[3]
		    A			B			C			D
		经过下面的循环左移以后，就变成了：
			B			C			D			A	
	*/
    leftLoop4int(numArray, 1);//循环左移一个字节

    //S盒字节代换(S盒：PPT24)
	for(i=0;i<4;i++)
	{
		numArray[i]=getNumFromSBox(numArray[i]);
	}

    result = mergeArrayToInt(numArray);	//四字节合并，合并成一个4字节整型
    return result ^ Rcon[round];		//用result与轮系数Rcon进行异或运算
}

//密钥对应的扩展数组
int w[44];

//扩展密钥，结果是把w[44]中的每个元素初始化
void extendKey(char *key)
{
    int i,j;
	for(i=0;i<4;i++)	//求得w[0],w[1],w[2],w[3]
	{
		w[i]=getWordFromStr(key+i*4);//把连续的4个字符合并成一个4字节的整型
		/*举例:如果输入的是1234567890123456
		则首先分组1234 5678 9012 3456,然后赋值
		w[0]=1234,w[1]=5678,w[2]=9012,w[3]=3456
		*/
	}
	for(i=4,j=0;i<44;i++)
	{
		if(i%4==0)//如果是w[4]、w[8]...,则将w[i-1]进行G函数处理后再与w[i-4]进行异或运算
		{
			w[i]=w[i-4]^G(w[i-1],j);		// ^ 表示按位异或
			j++;//下一轮
		}
		else//如果不是，则直接将w[i-1]与w[i-4]进行异或运算
		{
			w[i]=w[i-4]^w[i-1];
		}
	}
}

//打印W数组
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

//从4个32位的密钥字中获得4X4数组，用于进行逆列混合
void getArrayFrom4W(int i, int array[4][4])
{
    int index,j;
    int colOne[4], colTwo[4], colThree[4], colFour[4];
    index = i * 4;
	//把一个4字节的数的第一、二、三、四个字节取出，并将其装进一个包含4个元素的整型数组里面。
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
