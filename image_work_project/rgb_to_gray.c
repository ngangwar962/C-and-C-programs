#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
unsigned char buffer[1000][1000][3];
unsigned char out_buffer[1000][1000];
/*void *rgb_to_gray(void *arg)
{
return NULL;
}*/
int main(int argc,char *argv[])
{

int i,j,k,l;
FILE *fin,*fout;
fin=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena_color.bmp","r");
fout=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena_color_rgb_to_gray.bmp","w+");

unsigned char byte[54];
for(i=0;i<54;i++)
{
	byte[i]=getc(fin);
}
fwrite(byte,sizeof(unsigned char),54,fout);

int height=*(int*)&byte[18];
int width=*(int*)&byte[22];
int bitdepth=*(int*)&byte[28];

int size=height*width;
unsigned char colortable[1024];

if(bitdepth<=8)
{
fread(colortable,sizeof(unsigned char),1024,fin);
fwrite(colortable,sizeof(unsigned char),1024,fout);
}


for(i=0;i<height;i++)
{
	for(j=0;j<width;j++)
	{
		buffer[i][j][0]=getc(fin);
		buffer[i][j][1]=getc(fin);
		buffer[i][j][2]=getc(fin);
	}
}
//fread(buffer,sizeof(unsigned char),size,fin);
for(i=0;i<height;i++)
{
	for(j=0;j<width;j++)
	{
		float sum0=buffer[i][j][0]+buffer[i][j][1]+buffer[i][j][2];
		out_buffer[i][j]=sum0/3.0;
	}
}


for(i=0;i<height;i++)
{
	for(j=0;j<width;j++)
	{
		putc(out_buffer[i][j],fout);
	}
}
return 0;
}
