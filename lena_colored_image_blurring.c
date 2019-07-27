#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>

int main()
{
int i,j,k,l;
FILE *fin,*fout;
fin=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena_color.bmp","r");
fout=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena_col_blur1.bmp","w+");
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

unsigned char buffer[size][3];
unsigned char out[size][3];


if(bitdepth<=8)
{
fread(colortable,sizeof(unsigned char),1024,fin);
fwrite(colortable,sizeof(unsigned char),1024,fout);
}

for(i=0;i<size;i++)
{
buffer[i][2]=getc(fin);
buffer[i][1]=getc(fin);
buffer[i][0]=getc(fin);
}

float a =1.0/9;

for(i=0;i<size;i++)
{
out[i][0]=buffer[i][0];
out[i][1]=buffer[i][1];
out[i][2]=buffer[i][2];
}
float filter[3][3]={{a,a,a},{a,a,a},{a,a,a}};
for(i=1;i<height-1;i++)
{
for(j=1;j<width-1;j++)
   {
	float sum0=0.0,sum1=0.0,sum2=0.0;
	for(k=-1;k<=1;k++)
	{
		for(l=-1;l<=1;l++)
		{
			sum0=sum0+(float)filter[k+1][l+1]*buffer[(i+k)*width+(j+l)][0];
			sum1=sum1+(float)filter[k+1][l+1]*buffer[(i+k)*width+(j+l)][1];
			sum2=sum2+(float)filter[k+1][l+1]*buffer[(i+k)*width+(j+l)][2];	
		}
		
	}
	out[(i)*width+(j)][0]=sum0;
	out[(i)*width+(j)][1]=sum1;
	out[(i)*width+(j)][2]=sum2;
    }
}
for(i=0;i<size;i++)
{
putc(out[i][2],fout);
putc(out[i][1],fout);
putc(out[i][0],fout);
}
fclose(fin);
fclose(fout);
return 0;
}
