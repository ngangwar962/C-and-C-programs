#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>

unsigned char buffer[512][512];
unsigned char out_buffer[512][512];

void *rotate_right(void *arg)/*thread function to rotate the complete image to right*/
{
int *c=(int*)arg;
int i;
for(i=0;i<512;i++)
{
	out_buffer[c[0]][i]=buffer[i][511-c[0]];
}
return NULL;
}
void *rotate_left(void *arg)/*thread function to rotate the complete image to left*/
{
int i;
int *c=(int*)arg;
for(i=0;i<512;i++)
{
	out_buffer[c[0]][i]=buffer[511-i][c[0]];
}
return NULL;
}
void *rotate_invert(void *arg)/*thread function to imvert the complete image*/
{
	int i;
	int *c=(int*)arg;
	for(i=0;i<512;i++)
	{
		out_buffer[c[0]][i]=buffer[511-c[0]][i];
	}
	return NULL;
}
void *part_rotation(void *arg)/*thread function to rotate the portions of the images*/
{
	int i,j;
	int *c=(int*)arg;
	int ch;
	printf("press 1 to rotate this part to right\npress 2 to rotate this part to left\npress 3 to invert this part\n");
	scanf("%d",&ch);
	switch(ch)
	{
	case 1:/*rotating the part of the image to the right*/
	{
		for(i=c[0];i<c[3];i++)
		{
			for(j=c[0];j<c[3];j++)
			{
				out_buffer[i][j]=buffer[j][c[3]-i];
			}
		}
		break;	
	}
	case 2:/*rotating this part of the image to the left*/
	{
		for(i=c[0];i<c[3];i++)
		{
			for(j=c[0];j<c[3];j++)
			{
				out_buffer[i][j]=buffer[c[3]-j][i];
			}
		}
		break;
	}
	case 3:/*inverting the part of the image*/
	{
		for(i=c[0];i<c[3];i++)
		{
			for(j=c[0];j<c[3];j++)
			{
				out_buffer[i][j]=buffer[c[3]-i][j];	
			}
		}
		break;
	}
	}
return NULL;
}
int main()
{
	int i,j,k,l,ch;
	pthread_t t[512];
	unsigned char byte[54],colorTable[1024];
	FILE *fin,*fout;
	fin=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena.bmp","r");
	fout=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena_rotate_threads.bmp","w+");
	if(fin==NULL)
	{
		printf("file not found at specified position\n");
	}
	for(i=0;i<54;i++)										//read the 54 byte header from fIn
	{
		byte[i] = getc(fin);
	}
	fwrite(byte,sizeof(unsigned char),54,fout);	
	int height = *(int*)&byte[18];
	int width = *(int*)&byte[22];
	int bitDepth = *(int*)&byte[28];
	
	printf("width: %d\n",width);
	printf("height: %d\n",height );
	printf("bitdepth: %d\n",bitDepth);

	int size = height*width;
	
	if(bitDepth <= 8)										//if ColorTable present, extract it.
	{
		fread(colorTable,sizeof(unsigned char),1024,fin);
		fwrite(colorTable,sizeof(unsigned char),1024,fout);
	}

	fread(buffer,sizeof(unsigned char),size,fin);
	int A[height];
	for(i=0;i<height;i++)
	{
		A[i]=i;
	}
	printf("press 1 to rotate the image to right\npress 2 to rotate the image to left\npress 3 to rotate the image by 180\npress 4 if wants to rotate the particulat portion of the image\n");
	scanf("%d",&ch);
	switch(ch)
	{
		case 1:/*rotating the complete image to the right*/
		{
			for(i=0;i<height;i++)
			{
				pthread_create(&t[i],NULL,rotate_right,(void*)&A[i]);
				pthread_join(t[i],NULL);
			}
			break;
		}
		case 2:/*rotating the complete image to the left*/
		{
			for(i=0;i<height;i++)
			{
				pthread_create(&t[i],NULL,rotate_left,(void*)&A[i]);
				pthread_join(t[i],NULL);
			}
			break;
		}
		case 3:/*inverting the complete image*/
		{	
			for(i=0;i<height;i++)
			{
				pthread_create(&t[i],NULL,rotate_invert,(void*)&A[i]);
				pthread_join(t[i],NULL);
			}	
			break;
		}
		case 4:/*code for rotating some part of the image*/
		{
			int x1,x2,y2,y1;
			printf("enter the leftmost coordinates as x and y\n");	
			scanf("%d%d",&x1,&y1);
			printf("enter the rightmost coordinates as x and y such that difference should be equal\n");
			scanf("%d%d",&x2,&y2);
			for(i=0;i<height;i++)
			{
			for(j=0;j<width;j++)
			{
			out_buffer[i][j]=buffer[i][j];
			}
			}
			int arr[1][4]={x1,y1,x2,y2};
			pthread_create(&t[0],NULL,part_rotation,(void*)&arr[0]);
			pthread_join(t[0],NULL);
			break;
		}
	}
	fwrite(out_buffer,sizeof(unsigned char),size,fout);
	return 0;
}
