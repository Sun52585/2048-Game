// 2048.cpp : Defines the entry point for the console application.
//

#include <stdafx.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

int rst[4][4]={2,4,4,2,8,0,0,2,0,4,2,0,8,2,0,2};


//搜索数组中为零的位置  可以优化
char find_zero(int a[][4],int z0x[],int z0y[])
{
	char i=0,j=0,k=0;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
		{
			if(a[i][j]==0)
			{
				z0x[k]=i;
				z0y[k]=j;
				k++;
			}
		}
	return k-1; 
}
//随机在剩下区域产生2
void remain_rand2(int a[][4],int z0x[],int z0y[],char k)
{
       int i;
       time_t t;
	   int site;
       srand((unsigned) time(&t));
	   site=(int)((rand()%100)/100.0*k);
	  a[z0x[site]][z0y[site]]=2;
		for(k=0;k<4;k++)	
		{
			for(i=0;i<4;i++)
				printf("%d    ",a[i][k]);
			printf("\n");
		}
}


/*四种转向函数*/

void slide_left(int a[][4])
{
	int b[4][4]={0};
	int i=0,j=0,k=0;
	int z0x[17]={0};
	int z0y[17]={0};
		for(k=0;k<4;k++)
		{
			for(i=0;i<4;i++)
			if(a[i][k]!=0) 
			{
				b[j][k]=a[i][k];
				j+=1;
			}
			for(i=0;i<4-1;i++)
			{
				if(b[i][k]==b[i+1][k]){
					b[i][k]=b[i][k]<<1;
					b[i+1][k]=0;
				}
				else 
				{
					b[i][k]=b[i][k];
					b[i+1][k]=b[i+1][k];		
				}

			}
			for(i=0;i<4;i++)a[i][k]=0;
			j=0;
			for(i=0;i<4;i++)
			{
				if(b[i][k]!=0) {a[j][k]=b[i][k];j+=1;}
				
			}
			j=0;
			for(i=0;i<4;i++)
			{
				b[i][k]=0;
			}
		}
		k=find_zero(a,z0x,z0y);
		remain_rand2(a,z0x,z0y,k);

}
void slide_right(int a[][4])
{
	int b[4][4]={0};
	int i=0,j=0,k=0;
	int z0x[17]={0};
	int z0y[17]={0};
		for(k=0;k<4;k++)
		{
			j=3;
			for(i=3;i>=0;i--)
			if(a[i][k]!=0) 
			{
				b[j][k]=a[i][k];
				j-=1;
			}
			for(i=3;i>=1;i--)
			{
				if(b[i][k]==b[i-1][k]){
					b[i][k]=b[i][k]<<1;
					b[i-1][k]=0;
				}

			}
			for(i=3;i>=0;i--)a[i][k]=0;
			j=3;
			for(i=3;i>=0;i--)
			{
				if(b[i][k]!=0) {a[j][k]=b[i][k];j-=1;}
				
			}
			j=3;
		}
		k=find_zero(a,z0x,z0y);
		remain_rand2(a,z0x,z0y,k);
}
void slide_up(int a[][4])
{
	int b[4][4]={0};
	int i=0,j=0,k=0;
	int z0x[17]={0};
	int z0y[17]={0};
		for(k=0;k<4;k++)
		{
			for(i=0;i<4;i++)
			if(a[k][i]!=0) 
			{
				b[k][j]=a[k][i];
				j+=1;
			}
			for(i=0;i<4-1;i++)
			{
				if(b[k][i]==b[k][i+1]){
					b[k][i]=b[k][i]<<1;
					b[k][i+1]=0;
				}

			}
			for(i=0;i<4;i++)a[k][i]=0;
			j=0;
			for(i=0;i<4;i++)
			{
				if(b[k][i]!=0) 
				{
					a[k][j]=b[k][i];
					j+=1;
				}
				
			}
			j=0;
			for(i=0;i<4;i++)
			{

				b[k][i]=0;
			}
		}
		k=find_zero(a,z0x,z0y);
		remain_rand2(a,z0x,z0y,k);
}
void slide_down( int a[][4])
{
	int b[4][4]={0};
	int i=0,j=0,k=0;
	int z0x[17]={0};
	int z0y[17]={0};
		for(k=0;k<4;k++)
		{
			j=3;
			for(i=3;i>=0;i--)
			if(a[k][i]!=0) 
			{
				b[k][j]=a[k][i];
				j-=1;
			}
			for(i=3;i>=1;i--)
			{
				if(b[k][i]==b[k][i-1]){
					b[k][i]=b[k][i]<<1;
					b[k][i-1]=0;
				}

			}
			for(i=3;i>=0;i--)a[k][i]=0;
			j=3;
			for(i=3;i>=0;i--)
			{
				if(b[k][i]!=0) {a[k][j]=b[k][i];j-=1;}
				
			}
			j=3;
			for(i=0;i<4;i++)
			{
				b[k][i]=0;
			}
		}
		k=find_zero(a,z0x,z0y);
		remain_rand2(a,z0x,z0y,k);
}
//清屏函数 lcd 


//清盘复原
void reset(int a[][4])
{
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			a[i][j]=rst[i][j];
}
void intial_2048(int a[][4])
{
	int z0x[17]={0},z0y[17]={0},k=0;
		k=find_zero(a,z0x,z0y);
		remain_rand2(a,z0x,z0y,k);
		for(k=0;k<17;k++)z0x[k]=0;
		for(k=0;k<17;k++)z0y[k]=0;

		k=find_zero(a,z0x,z0y);
		remain_rand2(a,z0x,z0y,k);
		for(k=0;k<17;k++)z0x[k]=0;
		for(k=0;k<17;k++)z0y[k]=0;

		k=find_zero(a,z0x,z0y);
		remain_rand2(a,z0x,z0y,k);
		for(k=0;k<17;k++)z0x[k]=0;
		for(k=0;k<17;k++)z0y[k]=0;		
}
int main(int argc, char* argv[])
{
	char i=0,j=0;
	int dir=0,k=0;
	int a[4][4]={0};
	intial_2048(a);
	//0.设置方向 左右上下
	//1.搜索非零元素有序提取保存
	//2.对相连相同者合并，下标小号值《1，大号着清理
	//3.重新搜索非零元素，在提取保存得到最终结果
		printf("initial platform：\n");	
		for(k=0;k<4;k++)
		{
			for(i=0;i<4;i++)	
			printf("%d    ",a[i][k]);
			printf("\n");
		}
		while(1)
		{
		scanf("%c",&dir);
		switch(dir)
		{
		case 'w': slide_up(a);break;
		case 'z': slide_down(a);break;
		case 'a': slide_left(a);break;
		case 'd': slide_right(a);break;
		case 'e': exit(2);break;
		case 'r': reset(a);break;
		}
		}
		getch();
	return 0;
}

