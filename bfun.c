/*
	NAME
       open and possibly create a file or device

SYNOPSIS
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>

       int open(const char *pathname, int flags);
       int open(const char *pathname, int flags, mode_t mode);

	   file  descriptor:文件描述符
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include "hanziku.h"
#define reset 0
#define up 1
#define down 2
#define left 3
#define right 4 
int *plcd = NULL;

char wo[] = {
	0x04,0x40,0x0E,0x50,0x78,0x48,0x08,0x48,0x08,0x40,0xFF,0xFE,0x08,0x40,0x08,0x44,
0x0A,0x44,0x0C,0x48,0x18,0x30,0x68,0x22,0x08,0x52,0x08,0x8A,0x2B,0x06,0x10,0x02
};
#include "stdlib.h"
#include "time.h"
int rst[4][4]={2,4,4,2,8,0,0,2,0,4,2,0,8,2,0,2};
int key=0;

/*
enum button: unsigned int
{
     up=1,
     down=2,
     left=3,
     right=4
}key;
*/
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

int init_serial(const char *file, int baudrate)
{


#if 0
	struct termios
	{
		tcflag_t c_iflag;      
		tcflag_t c_oflag;       /* output mode flags */
		tcflag_t c_cflag;       /* control mode flags */
		tcflag_t c_lflag;       /* local mode flags */
		cc_t c_line;            /* line discipline */
		cc_t c_cc[NCCS];        /* control characters */
		speed_t c_ispeed;       /* input speed */
		speed_t c_ospeed;       /* output speed */
#define _HAVE_STRUCT_TERMIOS_C_ISPEED 1
#define _HAVE_STRUCT_TERMIOS_C_OSPEED 1
	};
#endif
	
	int fd;
	
	fd = open(file, O_RDWR);
	if (fd == -1)
	{
		perror("open device error:");
		return -1;
	}

	struct termios myserial;
	//清空结构体
	memset(&myserial, 0, sizeof (myserial));
	//O_RDWR               
	myserial.c_cflag |= (CLOCAL | CREAD);
	//设置控制模式状态，本地连接，接受使能
	//设置 数据位
	myserial.c_cflag &= ~CSIZE;   //清空数据位
	myserial.c_cflag &= ~CRTSCTS; //无硬件流控制
	myserial.c_cflag |= CS8;      //数据位:8

	myserial.c_cflag &= ~CSTOPB;//   //1位停止位
	myserial.c_cflag &= ~PARENB;  //不要校验
	//myserial.c_iflag |= IGNPAR;   //不要校验
	//myserial.c_oflag = 0;  //输入模式
	//myserial.c_lflag = 0;  //不激活终端模式

	switch (baudrate)
	{
		case 9600:
			cfsetospeed(&myserial, B9600);  //设置波特率
			cfsetispeed(&myserial, B9600);
			break;
		case 115200:
			cfsetospeed(&myserial, B115200);  //设置波特率
			cfsetispeed(&myserial, B115200);
			break;
		case 19200:
			cfsetospeed(&myserial, B19200);  //设置波特率
			cfsetispeed(&myserial, B19200);
			break;
	}
		/* 刷新输出队列,清楚正接受的数据 */
	tcflush(fd, TCIFLUSH);

	/* 改变配置 */
	tcsetattr(fd, TCSANOW, &myserial);

	return fd;
}

void lcd_draw_point(int x0, int y0, int color)
{
	*(plcd + y0*800 + x0) = color;
}

//在顶点为x0,y0的位置开始显示一个颜色矩形，矩形的宽//度为w, 高度为h,颜色为color
void lcd_draw_rect(int x0, int y0, int w, int h, int color)
{
	int x, y;
	for (y = y0; y < y0+h; y++)
	{
		for (x = x0; x < x0+w; x++)
			lcd_draw_point(x, y, color);
	}
}

//把lcd清成color颜色
void lcd_clean_screen(int color)
{
	int x, y;
	for (y = 0; y < 480; y++)
	{
		for (x = 0; x < 800; x++)
			lcd_draw_point(x, y, color);
	}
}
void lcd_clean_qipan(int color)
{
	int x, y;
	for (y = 0; y < 480; y++)
	{
		for (x = 0; x < 480; x++)
			lcd_draw_point(x, y, color);
	}
}

//显示一张24位的bmp图片，图片的宽度必须为4的倍数 
void lcd_draw_bmp(char *name, int x0, int y0, int w, int h){
	int fd;
	fd = open(name, O_RDWR);
	if (-1 == fd)
	{
		printf("open %s error!\n", name);
		
		return ;
	}
	
	//1、跳过前54个byte
	lseek(fd, 54, SEEK_SET);
	char ch[w*h*3];
	read(fd, ch, sizeof(ch));
	close(fd);
	
	//lcd 的像素点占4byte， 24位bmp 3byte
	int color;
	char r, g, b;
	//位运算
	int x, y, i = 0;
	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
		{
			b = ch[i];
			g = ch[i+1];
			r = ch[i+2];
			i += 3;
			color = (r << 16) | (g << 8) | b;
			lcd_draw_point(x+x0, h-y+y0, color);
		}
	}
}
//显示一个字
void lcd_draw_word(unsigned char *ch, int color, int len,  int w, int x, int y)
{
	int i, j;
	
	int high;
	
	//32 * 64
	// 一个字的显示：画点（一个一个像素点）
	// 八个点用一个 unsigned char 
	// 已知：数组长度：len => 像素点的个数：len * 8
	// 已知：字的宽度=> 高度：len*8/w 
	high = len*8 / w; //64 
	
	int flag;
	flag = w/8; // 4
	
	for (i = 0; i < len; i++)
	{
		for (j = 7; j >= 0; j--)
		{
			if ((ch[i] >> j) & 1 == 1)
			{
				if (i%flag == 0)
					lcd_draw_point(7-j+x, i/flag+y, color);
				else
					lcd_draw_point(8*(i%flag)+7-j+x, i/flag+y, color);
			}
		}
	}
}
void lcd_show_2048(int a[][4])
{
	int i=0,j=0,k=0;
	int nzx[17]={0},nzy[17]={0};
	//刷新界面
	lcd_clean_qipan(0xFFFFFFFF);

//	lcd_draw_word(zhou, 0x0000ff00, 56*56, 56, 200, 200);
	// lcd_show_2048(a_2048);
	//如何显示一个矩形
	lcd_draw_rect(30, 30, 420,420, 0xffffff);
	
//	lcd_draw_rect(480,0,319,480,0x000000ff);
//	draw a line
	for(j=0;j<=4;j++)
	for(i=30;i<=420+30;i++)
		lcd_draw_point(30+j*105,i,0x00ff0000);
	for(i=0;i<=4;i++)
	for(j=30;j<=420+30;j++)
		lcd_draw_point(j,30+i*105,0x00ff0000);			
//	char cp1[56*56+1]={0},cp2[];
	char *p1=NULL,*p2=NULL;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
		{
			// if(a[i][j]!=0)
			// {

			// lcd_draw_word(zhou,0xff000000,32,16,i*105+75,j*105+75);
			// }
			switch(a[i][j])
			{
				case 0: 
				lcd_draw_rect(i*105+30, j*105+30, 105,105, 0xeeeeeeee);				
				lcd_draw_word(man,0xf000000,32,16,i*105+75,j*105+75);break;
				
				case 2:	
				lcd_draw_rect(i*105+30, j*105+30, 105,105, 0x0000a0a0);				
				lcd_draw_word(zhou,0x00ff0000,32,16,i*105+75,j*105+75);break;
				case 4:	 
				lcd_draw_rect(i*105+30, j*105+30, 105,105, 0x0000ff00);
				lcd_draw_word(chu,0x00ff0000,32,16,i*105+75,j*105+75);break;
				case 8:	 
				lcd_draw_rect(i*105+30, j*105+30, 105,105, 0x000000ff);
				lcd_draw_word(qin,0x00ff0000,32,16,i*105+75,j*105+75);break;
				case 16:	 
				lcd_draw_rect(i*105+30, j*105+30, 105,105, 0x00cc5533);
				lcd_draw_word(qi,0x00ff0000,32,16,i*105+75,j*105+75);break;
				case 32:	 
				lcd_draw_rect(i*105+30, j*105+30, 105,105, 0x0066666);
				lcd_draw_word(yan,0x00ff0000,32,16,i*105+75,j*105+75);break;
				case 64:	 
				lcd_draw_rect(i*105+30, j*105+30, 105,105, 0x00EEDDCC);
				lcd_draw_word(wei,0x00ff0000,32,16,i*105+75,j*105+75);break;
				case 128:	 
				lcd_draw_rect(i*105+30, j*105+30, 105,105, 0x00CCBBAA);
				lcd_draw_word(han,0x00ff0000,32,16,i*105+75,j*105+75);break;
				case 256:	 
				lcd_draw_rect(i*105+30, j*105+30, 105,105, 0x0088CC99);
				lcd_draw_word(zhao,0x00ff0000,32,16,i*105+75,j*105+75);break;
				case 512:	 
				lcd_draw_rect(i*105+30, j*105+30, 105,105, 0x00bbaa66);
				lcd_draw_word(wu,0x00ff0000,32,16,i*105+75,j*105+75);break;
				case 1024:	 
				lcd_draw_rect(i*105+30, j*105+30, 105,105, 0x00558877);
				lcd_draw_word(yue,0x00ff0000,32,16,i*105+75,j*105+75);break;
				case 2048:	 
				lcd_draw_rect(i*105+30, j*105+30, 105,105,0x00ff0000);
				lcd_draw_word(xihan,0x00ff0000,32,16,i*105+75,j*105+75);break;
			}	
		}
		

}
//出事化为3个2,复位
void initial_2048(int a[][4])
{
	int j=0,z0x[17]={0},z0y[17]={0},k=0;
	//期盼清零
	for(k=0;k<4;k++)
		for(j=0;j<4;j++)a[k][j]=0;
		//随机化棋盘
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
void monitor_touch(int a_2048[ ][4])
{
	int fd,i=0,j=0;
	int k=0;int endx=0,endy=0;
	int kx=0,ky=0;
	fd = open("/dev/event0", O_RDWR);
	if (-1 == fd)
		return ;
	
	struct input_event ev;
	int x, y;
	int touchx[1000]={0};
	int touchy[1000]={0};
	while (1)
	{
		read(fd, &ev, sizeof(ev));
		//如何判断该事件是一个触摸事件
		if ((ev.type == EV_ABS) && (ev.code == ABS_X))
		{
			x = ev.value;
			touchx[i]=x;
			i++;
		}
		else if ((ev.type == EV_ABS) && (ev.code == ABS_Y))
		{
			y = ev.value;
			touchy[j]=y;
			j++;
		}
		
		
		//手指弹起
	if ((ev.type == EV_ABS) && (ev.code == ABS_PRESSURE) && (ev.value == 0))
		{
			printf("x: %d, y: %d\n", x, y);
			//calculate tanh
			endx =i-1; //get back
			endy =j-1;
			printf("a[0]=%d,b[0]=%d\n",touchx[0],touchy[0]);
			printf("a[end]=%d,b[end]=%d\n",touchx[endx],touchy[endy]);
			kx=(touchx[endx]-touchx[0]);
			ky=(touchy[endy]-touchy[0]);
			//先判断是否复位
			if((touchx[endx]>=580)&&(touchx[endx]<=700)&&(touchy[endy]>=360)&&(touchy[endy]<=480))
			{
				key=reset;
				initial_2048(a_2048);
				lcd_show_2048(a_2048);
			}
			if((kx>0)&&(ky>0))  // N1
			{
				if(kx>=ky)
				{
					key=right;
					printf("right\n");
					slide_right(a_2048);
					lcd_show_2048(a_2048);
				}
				else{
					key=down;
					printf("down\n");
					slide_down(a_2048);
					lcd_show_2048(a_2048);
				}
			}
			if((kx<0)&&(ky>0))   //N2
			{
				if(kx+ky<=0){
					key=left;
					printf("left\n");
					slide_left(a_2048);
					lcd_show_2048(a_2048);
				}
				else {
					key=down;
					printf("down\n");
					slide_down(a_2048);
					lcd_show_2048(a_2048);
				}					
			}				
			if((kx<0)&&(ky<0))  // N3
			{
				if(kx<=ky){ 
				key=left;
				printf("left\n");
					slide_left(a_2048);
					lcd_show_2048(a_2048);
				}
				else {
					key=up;
					printf("up\n");
					slide_up(a_2048);
					lcd_show_2048(a_2048);
				}					
			}				
			if((kx>0)&&(ky<0))   //N4
			{
				if(kx+ky>=0) {
					key=right;
					printf("right\n");
					slide_right(a_2048);
					lcd_show_2048(a_2048);
				}
				else {
					key=up;
					printf("up\n");
					slide_up(a_2048);
					lcd_show_2048(a_2048);
				}
			}	
			//触摸值清零复位
			for(i=0;i<=endx;i++)touchx[i]=0;
			for(j=0;j<=endy;j++)touchy[j]=0;
				i=0;j=0;
		}			
		
}
	close(fd);
}

int main(int argc, char *argv[])
{
	int fd;
	int i,j;
	int a_2048[4][4]={0};
	initial_2048(a_2048);
//	lcd_show_2048(a_2048);
	//O_RDWR: 可读可写
	fd = open("/dev/fb0", O_RDWR);
	// if(fd == -1)
	//if (fd = -1)
	if (-1 == fd)
	{
		printf("open dev/fb0 error !\n");
		
		return 0;
	}
	
	//内存映射，吧一个文件或者设备映射到一个进程的///地址空间中（内存），在进程中操作这块映射的内///存就相当于操作 被映射的文件
	//plcd 存储了映射的地址空间的首地址
	plcd = mmap(NULL, 800*480*4, 
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			fd,
			0);
	
	//如何把lcd上的第n个像素点 显示成红色？
	//*(plcd + n) = 0x00ff0000;
	
	//如何把坐标为(x, y)的像素点，显示成红色？
	//*(plcd + y*800+x) = 0x00ff0000;
	
	//如何把整个屏幕全部显示白色？
	//循环
	lcd_clean_screen(0xFFFFFFFF);
	//lcd_draw_word(chun, 0x00ffff00, 32*29, 32, 550+100, 30);
	// lcd_draw_word(qiu, 0x00ffff00, 32*29, 32,  550+100, 135);
	// lcd_draw_word(luan, 0x00ffff00, 32*29, 32, 550+100, 240);
	// lcd_draw_word(shi, 0x00ffff00, 32*29, 32, 550+100, 345);
	lcd_draw_rect(480+100, 360, 80,50, 0x00ff0000);
	//lcd_draw_word(fu, 0x00000000, 24*20, 24, 500+100, 375);
	// lcd_draw_word(wei1, 0x00000000, 24*20, 24,530+100 , 375);
//	lcd_draw_word(zhou, 0x0000ff00, 56*56, 56, 200, 200);
//	lcd_show_2048(a_2048);
//	如何显示一个矩形
	// lcd_draw_rect(30, 30, 420,420, 0x00ff0000);
//	lcd_draw_rect(480,0,319,480,0x000000ff);
//	draw a line
	// for(j=0;j<4;j++)
	// for(i=30;i<420+30;i++)
		// lcd_draw_point(30+j*105,i,0xff000000);
	// for(i=0;i<4;i++)
	// for(j=30;j<420+30;j++)
		// lcd_draw_point(j,30+i*105,0xff000000);		
	lcd_show_2048(a_2048);

	//显示BMP图片
//	lcd_draw_bmp("music.bmp", 100, 100, 240, 240);
	
	//初始化串口
	int com_fd = init_serial("/dev/s3c2410_serial3", 9600);
	
	monitor_touch(a_2048);
	//注意及时key清零
	close(fd);
	while(1);
}
