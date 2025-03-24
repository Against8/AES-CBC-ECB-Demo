#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"assist.h"
#include"aes.h"
#include"expandedkey.h"
#include"deaes.h"
int flag=1;
int getStringCipher(char *str)
{
	int i=0;
	int slen;
	int len;
	scanf("%s",str);
	slen=strlen(str);
	if((slen+i)<16)
	{
		printf("您输入的字符串小于16位，将自动填充0\n");
		while((slen+i)<16)
		{
		str[slen+i]='0';
		i++;
		}
		str[slen+i]='\0';
	}
	len=strlen(str);
	return len;
}
int getStringPlain(char *str)
{
	int i=0;
	int slen;
	int len;
	int count;
	scanf("%s",str);
	slen=strlen(str);
	if((slen+i)%16!=0)
	{
		printf("您输入的字符串长度不是16的倍数，将自动填充0，达到16的倍数\n");
		while((slen+i)%16!=0)
		{
			str[slen+i]='0';
			i++;
		}
		str[slen+i]='\0';
	}
	len=strlen(str);
	printf("您输入的明文为:");
	for(i=0;i<len;i++)
	{
		printf("%c",str[i]);
	}
	printf("\n");
	return len;
}
void writeStrToFile(char *str, int len, char *fileName)
{
	int i;
    FILE *fp;							
    fp = fopen(fileName, "wb");
	for(i=0;i<len;i++)
	{
		putc(str[i],fp);
	}
    fclose(fp);
}
void aesStrToFile(char *key)
{
    char p[MAXLEN];
    int plen;
	char fileName[64];
    printf("请输入明文:");
	plen=getStringPlain(p);
    printf("正在AES加密中..................\n");
    aes(p, plen, key);
    printf("加密完后的明文的ASCCI为：\n");
    printASCCI(p, plen);
    printf("请输入你想要写进的文件名，比如'test1.txt'、'test2.doc':");
    if(scanf("%s", fileName) == 1)
    {
        writeStrToFile(p, plen, fileName);
		printf("成功写入密文，您可以在运行该程序的当前目录中找到名为%s的密文文件。\n", fileName);
    }
}


int readStrFromFile(char *fileName, char *str)
{
	int i;
    FILE *fp = fopen(fileName, "rb");
    if(fp == NULL)
    {
        printf("打开文件出错，请确认文件存在当前目录下！\n");
        exit(0);
    }
    for(i = 0; i < MAXLEN && (str[i] = getc(fp)) != EOF; i++);
    if(i >= MAXLEN)
    {
        printf("解密文件过大！\n");
        exit(0);
    }
    str[i] = '\0';
    fclose(fp);
    return i;
}
void deAesFile(char *key)
{
    char fileName[64];
    char c[MAXLEN];
    printf("请输入要解密的文件名，该文件必须和本程序在同一个目录:");
    if(scanf("%s", fileName) == 1)
    {
        int clen = readStrFromFile(fileName, c);
        printf("正在解密中.........\n");
        deAes(c, clen, key);
        printf("解密后的明文ASCII为：\n");
        printASCCI(c, clen);
        printf("明文为：%s\n", c);
        writeStrToFile(c,clen,fileName);
		printf("您可以在与本程序同一个目录下的%s文件中查看解密后的密文\n",fileName);
    }
}
void aesFile(char *key)
{
    char fileName[64];
    char fileP[MAXLEN];
    printf("请输入要加密的文件名，该文件必须和本程序在同一个目录\n");
    if(scanf("%s", fileName) == 1)
    {
        readStrFromFile(fileName, fileP);
        int plen = strlen(fileP);
        printf("正在加密中.........\n");
        printf("加密前文件中字符的ASCII为：\n");
        printASCCI(fileP, plen);
        aes(fileP, plen, key);
        printf("加密后的密文ASCII为：\n");
        printASCCI(fileP, plen);
        writeStrToFile(fileP,plen,fileName);
        printf("已经将加密后的密文写进%s中了\n",fileName);
    }
}
int main(int argc, char const *argv[])
{
    char key[17];
	int klen;
	printf("******************************欢迎进入AES加密解密系统******************************\n");
	printf("本系统采用AES对称密码算法，密钥长度为128bit，采用ECB分组密码模式\n");
	printf("*******************************您可以进行以下几种操作******************************\n");
	printf("\t\t\t\t 1.加密字符串\n\n");
	printf("\t\t\t\t 2.加密文件\n\n");
	printf("\t\t\t\t 3.解密文件\n\n");
	printf("\t\t\t\t 4.退出\n\n");
	while(flag==1)
	{
		printf("请输入一串不大于16个字符的密钥：");
		getStringCipher(key);
		klen = strlen(key);
		printf("你输入的密钥为：%s\n",key);
		printf("注意：解密时，输入的密钥必须要与加密用的密钥完全一致!\n");
		printf("请输入数字选择你要进行的操作(1表示加密字符串,2表示加密文件,3表示解密文件,4表示退出):");
		int x;
		scanf("%d",&x);
		if(x == 1)
		{
			printf("您选择了加密字符串\n");
			aesStrToFile(key);
		}
		else if(x == 2)
		{
			printf("您选择了加密文件\n");
			aesFile(key);
		}
		else if(x == 3)
		{
			printf("您选择了解密文件\n");
			deAesFile(key);
		}
		else if(x == 4)
			exit(0);
		else
			printf("\n\n所选操作不合法!\n");
		printf("\n\n\n");
		printf("************************************本次操作完成************************************\n");
		printf("请问您是否还要进行操作，输入数字1表示继续，输入数字0表示结束:");
		scanf("%d",&flag);
	}

    return 0;
}
