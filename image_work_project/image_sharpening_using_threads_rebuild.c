#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include<sys/types.h>
#include<time.h>
int A[1000][1000][3];
unsigned char buffer[1000][1000][3];
unsigned char out[1000][1000][3];
float filter[3][3]={{0,1,0},{1,1,-1},{0,-1,0}};
void *matrix_multiply(void *arg)
{
int *c=(int*)arg;
int i,j,p,q;
float sum0=0,sum1=0,sum2=0;
//printf("%d-%d\n",c[0],c[1]);
for(i=c[0],p=0;i<c[0]+3;i++,p++)
{
for(j=c[1],q=0;j<c[1]+3;j++,q++)
{
	
	//printf("%d*%f\n",buffer[i][j][0],filter[p][q]);
	sum0=sum0+(float)buffer[i][j][0]*filter[p][q];
	sum1=sum1+(float)buffer[i][j][1]*filter[p][q];
	sum2=sum2+(float)buffer[i][j][2]*filter[p][q];
}
}
out[c[0]+1][c[1]+1][0]=sum0;
out[c[0]+1][c[1]+1][1]=sum1;
out[c[0]+1][c[1]+1][2]=sum2;

//printf("%d-%d-%d\n",out[c[0]+1][c[1]+1][0],out[c[0]+1][c[1]+1][1],out[c[0]+1][c[1]+1][2]);

return NULL;
}
int main(int argc,char *argv[])
{
int i,j,k,l,p,h,q;
FILE *fin,*fout;
time_t t1,t2;
pthread_t t[510][510];

fin=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena_color.bmp","r");
fout=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena_image123.bmp","w+");
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


printf("height=%d\n",height);
printf("width=%d\n",width);

unsigned char colortable[1024];

int size=height*width;

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
time(&t1);
for(i=0;i<height-2;i++)
{
for(j=0;j<width-2;j++)
{
pthread_create(&t[i][j],NULL,matrix_multiply,(void*)&A[i][j]);
pthread_join(t[i][j],NULL);
}
}
time(&t2);
printf("time=%f\n",(float)t2-t1);
//float filter[3][3]={{-1,-1,-1},{-1,9,-1},{-1,-1,-1}};

//float filter[3][3]={{0,1,0},{1,1,-1},{0,-1,0}};

//float filter[3][3]={{v,v,v},{v,v,v},{v,v,v}};

//float filter[3][3]={{1,1,1},{1,-8,1},{1,1,1}};

/*for(i=0;i<height-2;i++)
{
	for(j=0;j<width-2;j++)
	{
		float sum0=0,sum1=0,sum2=0;
		for(k=0;k<3;k++)
		{
			for(l=0;l<3;l++)
			{
				sum0=sum0+(float)buffer[i+k][l+j][0]*filter[k][l];
				sum1=sum1+(float)buffer[i+k][l+j][1]*filter[k][l];
				sum2=sum2+(float)buffer[i+k][l+j][2]*filter[k][l];
			}
		}
		out[i+1][j+1][0]=sum0;
		out[i+1][j+1][1]=sum1;
		out[i+1][j+1][2]=sum2;
	}
}*/
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
