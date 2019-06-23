#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#define BRIGHT 8
#define MAXCOLOR 255
#define THRESHOLD 20
int main()
{
int i,j,k,l;

FILE *fin,*fout;
fin=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena.bmp","r");
fout=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena_darking.bmp","wb");
if(fin==NULL)
{
	printf("file not found at the specified position\n");
	exit(0);
}
unsigned char header[54],colortable[1024];
for(i=0;i<54;i++)
{
	header[i]=getc(fin);
}
int height=*(int*)&header[22];
int width=*(int*)&header[18];
int bitdepth=*(int*)&header[28];
if(bitdepth<=8)
{
	fread(colortable,sizeof(unsigned char),1024,fin);
}
printf("width=%d\n",width);
printf("height=%d\n",height);
int size=width*height;
fwrite(header,sizeof(unsigned char),54,fout);
unsigned char buffer[height*width];
fread(buffer,sizeof(unsigned char),(height*width),fin);

if(bitdepth<=8)
	fwrite(colortable,sizeof(unsigned char),1024,fout);
unsigned char output[width][height];
int count=0;
for(i=0;i<size;i++)
{
if(buffer[i]>THRESHOLD)
{
buffer[i]=buffer[i]-2;
}
}
printf("count=%d\n",count);
fwrite(buffer,sizeof(unsigned char),(height*width),fout);

fclose(fout);
fclose(fin);
return 0;
}
