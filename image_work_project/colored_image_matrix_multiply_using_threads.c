#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
int A[510][510][2];
/*void *matrix_multiply(void *arg)
{

return NULL;
}*/
int main(int argc,char *argv[])
{
int i,j,k,l;
FILE *fin,*fout;
fin=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena_color.bmp","r");
fout=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena_col_blur3.bmp","w+");
unsigned char byte[54];
if(fin==NULL)
{
printf("file not found\n");
}
for(i=0;i<54;i++)
{
byte[i]=getc(fin);
}
fwrite(byte,sizeof(unsigned char),54,fout);

int height=*(int*)&byte[18];
int width=*(int*)&byte[22];
int bitdepth=*(int*)&byte[28];

unsigned char colortable[1024];

int size=height*width;

unsigned char buffer[height][width][3];
unsigned char out[height][width][3];


if(bitdepth<=8)
{
fread(colortable,sizeof(unsigned char),1024,fin);
fwrite(colortable,sizeof(unsigned char),1024,fout);
}

for(i=0;i<width;i++)
{
for(j=0;j<height;j++)
{
buffer[i][j][0]=getc(fin);
buffer[i][j][1]=getc(fin);
buffer[i][j][2]=getc(fin);
}
}
for(i=0;i<height-2;i++)
{
for(j=0;j<width-2;j++)
{
	A[i][j][0]=i;
	A[i][j][1]=j;
}
}
for(i=0;i<height;i++)
{
	for(j=0;j<width;j++)
	{
		out[i][j][0]=buffer[i][j][0];
		out[i][j][1]=buffer[i][j][1];
		out[i][j][2]=buffer[i][j][2];
	}
}
float v=1.0/9.0;
int filter[3][3]={{v,v,v},{v,v,v},{v,v,v}};
for(i=0;i<height-2;i++)
{
	for(j=0;j<width-2;j++)
	{
	   float sum0=0.0,sum1=0.0,sum2=0.0;
	   for(k=i;k<=i+2;k++)
	   {
		for(l=j;l<=j+2;l++)
		{
		   sum0=sum0+buffer[k][l][0];
		   sum1=sum1+buffer[k][l][1];
		   sum2=sum2+buffer[k][l][2];
		}
	   }
	   out[i+1][j+1][0]=sum0/9;
	   out[i+1][j+1][1]=sum1/9;
	   out[i+1][j+1][2]=sum2/9;
	   //printf("%d-%d-%d\n",out[i+1][j+1][0],out[i+1][j+1][1],out[i+1][j+1][2]);
	}
}
for(i=0;i<height;i++)
{
for(j=0;j<width;j++)
{

putc(out[i][j][0],fout);
putc(out[i][j][1],fout);
putc(out[i][j][2],fout);
}
}
fclose(fin);
fclose(fout);
return 0;
}
