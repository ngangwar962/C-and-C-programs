#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>

unsigned char buffer[512][512];
unsigned char out_buffer[512][512];
unsigned char out_buffer1[512][512][3];
unsigned char buffer_color[512][512][3];
unsigned char out_buffer_color[512][512][3];
int A1[1000][1000][2];
float filter[3][3]={{0,-1,0},{1,1,1},{0,-1,0}};
void *rotate_right_color(void *arg) /*thread function to rotate the complete image to right*/
{
int *c=(int*)arg;
int i;
for(i=0;i<512;i++)
{
	out_buffer_color[c[0]][i][0]=buffer_color[i][511-c[0]][0];
	out_buffer_color[c[0]][i][1]=buffer_color[i][511-c[0]][1];
	out_buffer_color[c[0]][i][2]=buffer_color[i][511-c[0]][2];
}
return NULL;
}
void *rotate_left_color(void *arg) /*thread function to rotate the complete image to left*/
{
int i;
int *c=(int*)arg;
for(i=0;i<512;i++)
{
	out_buffer_color[c[0]][i][0]=buffer_color[511-i][c[0]][0];
	out_buffer_color[c[0]][i][1]=buffer_color[511-i][c[0]][1];
	out_buffer_color[c[0]][i][2]=buffer_color[511-i][c[0]][2];
}
return NULL;
}
void *rotate_invert_color(void *arg) /*thread function to imvert the complete image*/
{
	int i;
	int *c=(int*)arg;
	for(i=0;i<512;i++)
	{
		out_buffer_color[c[0]][i][0]=buffer_color[511-c[0]][i][0];
		out_buffer_color[c[0]][i][1]=buffer_color[511-c[0]][i][1];
		out_buffer_color[c[0]][i][2]=buffer_color[511-c[0]][i][2];
	}
	return NULL;
}
void *part_rotation_color(void *arg) /*thread function to rotate the portions of the images*/
{
	int i,j;
	int *c=(int*)arg;
	int ch;
	printf("press 1 to rotate this part to right\npress 2 to rotate this part to left\npress 3 to invert this part\n");
	scanf("%d",&ch);
	switch(ch)
	{
	case 1: /*rotating the part of the image to the right*/
	{
		for(i=c[0];i<c[3];i++)
		{
			for(j=c[0];j<c[3];j++)
			{
				out_buffer_color[i][j][0]=buffer_color[j][c[3]-i][0];
				out_buffer_color[i][j][1]=buffer_color[j][c[3]-i][1];
				out_buffer_color[i][j][2]=buffer_color[j][c[3]-i][2];
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
				out_buffer_color[i][j][0]=buffer_color[c[3]-j][i][0];
				out_buffer_color[i][j][1]=buffer_color[c[3]-j][i][1];
				out_buffer_color[i][j][2]=buffer_color[c[3]-j][i][2];
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
				out_buffer_color[i][j][0]=buffer_color[c[3]-i][j][0];	
				out_buffer_color[i][j][1]=buffer_color[c[3]-i][j][1];
				out_buffer_color[i][j][2]=buffer_color[c[3]-i][j][2];
			}
		}
		break;
	}
	}
return NULL;
}

void *matrix_multiply_coloured_image(void *arg)
{
	int *c=(int*)arg;
	int i,j,k,l,p,q;
	float sum0=0;
	float sum1=0;
	float sum2=0;
	for(i=c[0],p=0;i<c[0]+3;i++,p++)
	{
		for(j=c[1],q=0;j<c[1]+3;j++,q++)
		{
			/*out_buffer_color[i][j][0]=buffer_color[i][j][0];
			out_buffer_color[i][j][1]=buffer_color[i][j][1];
			out_buffer_color[i][j][2]=buffer_color[i][j][2];*/
			sum0=sum0+(float)buffer_color[i][j][0]*filter[p][q];
			sum1=sum1+(float)buffer_color[i][j][1]*filter[p][q];
			sum2=sum2+(float)buffer_color[i][j][2]*filter[p][q];
		}
	}
	out_buffer_color[c[0]+1][c[1]+1][0]=sum0;
	out_buffer_color[c[0]+1][c[1]+1][1]=sum1;
	out_buffer_color[c[0]+1][c[1]+1][2]=sum2;

	return NULL;
}



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

void *matrix_multiply(void *arg)  /*code for blurring the grayscale image*/
{
	int *c=(int*)arg;
	int i,j,p,q;
	float sum0=0;
	for(i=c[0],p=0;i<c[0]+3;i++,p++)
	{
		for(j=c[1],q=0;j<c[1]+3;j++,q++)
		{
			sum0=sum0+(float)buffer[i][j]*filter[p][q];
		}
	}
	out_buffer[c[0]+1][c[1]+1]=sum0;
	return NULL;
}
void *rgb_to_gray(void *arg)
{
	int *c=(int*)arg;
	int i,j;
	float y=0;
	y=(buffer_color[c[0]][c[1]][0]*0.3) + (buffer_color[c[0]][c[1]][1]*0.59)	+ (buffer_color[c[0]][c[1]][2]*0.11);
	out_buffer1[c[0]][c[1]][0]=y;
	out_buffer1[c[0]][c[1]][1]=y;
	out_buffer1[c[0]][c[1]][2]=y;
	return NULL;
}
void *crop_from_side(void *arg)
{
int i,j;
int *c=(int*)arg;
out_buffer[c[0]][c[1]]=buffer[c[0]][c[1]];
return NULL;
}
void *part_rotation(void *arg)   /*thread function to rotate the portions of the images*/
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
int main(int argc,char *argv[])
{
	int i,j,k,l,ch;
	pthread_t t[1000],t1[1000][1000];
	unsigned char byte[54],colorTable[1024];
	FILE *fin,*fout;
	int choice1;
	printf("press 1 for gray scale image\npress 2 for coloured image\n");
	scanf("%d",&choice1);
	switch(choice1)
	{
	case 1:
	{
		fin=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena.bmp","r");
		fout=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena_rotate_threads.bmp","w+");
		if(fin==NULL)
		{
			printf("file not found at specified position\n");
		}
		for(i=0;i<54;i++)	
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
	
		if(bitDepth <= 8)			
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
		for(i=0;i<height-2;i++)
		{
			for(j=0;j<width-2;j++)
			{
				A1[i][j][0]=i;
				A1[i][j][1]=j;
			}
		}
		printf("press 1 to rotate the image to right\npress 2 to rotate the image to left\npress 3 to rotate the image by 180\npress 4 if wants to rotate the particulat portion of the image\npress 5 to blurr the image\npress 6 to crop image form each side\nenter your choice\n");
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
			case 5:
			{
				for(i=0;i<height-2;i++)
				{
					for(j=0;j<width-2;j++)
					{
						pthread_create(&t1[i][j],NULL,matrix_multiply,(void*)&A1[i][j]);
						pthread_join(t1[i][j],NULL);
					}
				}
				break;
			}
			case 6:
			{
				int side,k,l;
				printf("enter the size to be cropped from each sides\n");
				scanf("%d",&side);
				printf("cropped amount will be %d\n",side);
				for(k=side;k<(height-side);k++)
				{
					for(l=side;l<(width-side);l++)
					{
						pthread_create(&t1[k][l],NULL,crop_from_side,(void*)&A1[k][l]);
						pthread_join(t1[k][l],NULL);
					}				
				}
				break;
			}
		}
		fwrite(out_buffer,sizeof(unsigned char),size,fout);
		break;
	  }
	case 2:
	{
		fin=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena_color.bmp","r");
		fout=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena_rotate_threads.bmp","w+");
		if(fin==NULL)
		{
			printf("file not found at specified position\n");
		}
		for(i=0;i<54;i++)	
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
	
		if(bitDepth <= 8)			
		{
			fread(colorTable,sizeof(unsigned char),1024,fin);
			fwrite(colorTable,sizeof(unsigned char),1024,fout);
		}

		for(i=0;i<width;i++)
		{
			for(j=0;j<height;j++)
			{
				buffer_color[i][j][0]=getc(fin);
				buffer_color[i][j][1]=getc(fin);
				buffer_color[i][j][2]=getc(fin);
			}
		}
		int A[height];
		for(i=0;i<height;i++)
		{
			A[i]=i;
		}
		for(i=0;i<height;i++)
		{
			for(j=0;j<width;j++)
			{
				A1[i][j][0]=i;
				A1[i][j][1]=j;
			}
		}

		printf("press 1 to rotate the image to right\npress 2 to rotate the image to left\npress 3 to rotate the image by 180\npress 4 if wants to rotate the particulat portion of the image\npress 5 to blur the colored image\npress 6 for converting the rgb to gray scale\nenter your choice\n");
		scanf("%d",&ch);
		switch(ch)
		{
			case 1:/*rotating the complete image to the right*/
			{
				for(i=0;i<height;i++)
				{
					pthread_create(&t[i],NULL,rotate_right_color,(void*)&A[i]);
					pthread_join(t[i],NULL);
				}
				for(i=0;i<height;i++)
				{
					for(j=0;j<width;j++)
					{
						putc(out_buffer_color[i][j][0],fout);
						putc(out_buffer_color[i][j][1],fout);
						putc(out_buffer_color[i][j][2],fout);
					}
				}
				break;
			}
			case 2:/*rotating the complete image to the left*/
			{
				for(i=0;i<height;i++)
				{
					pthread_create(&t[i],NULL,rotate_left_color,(void*)&A[i]);
					pthread_join(t[i],NULL);
				}
				for(i=0;i<height;i++)
				{
					for(j=0;j<width;j++)
					{
						putc(out_buffer_color[i][j][0],fout);
						putc(out_buffer_color[i][j][1],fout);
						putc(out_buffer_color[i][j][2],fout);
					}
				}
				
				break;
			}
			case 3:/*inverting the complete image*/
			{	
				for(i=0;i<height;i++)
				{
					pthread_create(&t[i],NULL,rotate_invert_color,(void*)&A[i]);
					pthread_join(t[i],NULL);
				}
				for(i=0;i<height;i++)
				{
				for(j=0;j<width;j++)
				{
					putc(out_buffer_color[i][j][0],fout);
					putc(out_buffer_color[i][j][1],fout);
					putc(out_buffer_color[i][j][2],fout);
				}
				}
	
				break;

			}
			case 4:   /*code for rotating some part of the image*/
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
					out_buffer_color[i][j][0]=buffer_color[i][j][0];
					out_buffer_color[i][j][1]=buffer_color[i][j][1];
					out_buffer_color[i][j][2]=buffer_color[i][j][2];
				}
				}
				int arr[1][4]={x1,y1,x2,y2};
				pthread_create(&t[0],NULL,part_rotation_color,(void*)&arr[0]);
				pthread_join(t[0],NULL);
				for(i=0;i<height;i++)
				{
				for(j=0;j<width;j++)
				{
					putc(out_buffer_color[i][j][0],fout);
					putc(out_buffer_color[i][j][1],fout);
					putc(out_buffer_color[i][j][2],fout);
				}
				}

				break;
			}
			case 5:/* code for blurring the image using thread function matrix_multiply_coloured_image*/
			{
				for(i=0;i<height;i++)
				{
				for(j=0;j<width;j++)
				{
					out_buffer_color[i][j][0]=buffer_color[i][j][0];
					out_buffer_color[i][j][1]=buffer_color[i][j][1];
					out_buffer_color[i][j][2]=buffer_color[i][j][2];
				}
				}
				for(i=0;i<height-2;i++)
				{
					for(j=0;j<width-2;j++)
					{
						pthread_create(&t1[i][j],NULL,matrix_multiply_coloured_image,(void*)&A1[i][j]);
						pthread_join(t1[i][j],NULL);
					}
				}
				int pqr1,pqr2;
				for(pqr1=0;pqr1<height;pqr1++)
				{
					for(pqr2=0;pqr2<width;pqr2++)
					{
						putc(out_buffer_color[pqr1][pqr2][0],fout);
						putc(out_buffer_color[pqr1][pqr2][1],fout);
						putc(out_buffer_color[pqr1][pqr2][2],fout);

					}
				}
				break;
			}
			case 6:/*converting the rgb image to grayscale image*/
			{
				for(i=0;i<height;i++)
				{
					for(j=0;j<width;j++)
					{
						pthread_create(&t1[i][j],NULL,rgb_to_gray,(void*)&A1[i][j]);
						pthread_join(t1[i][j],NULL);
					}
				}
				for(i=0;i<height;i++)
				{
					for(j=0;j<width;j++)
					{

						putc(out_buffer1[i][j][2],fout);
						putc(out_buffer1[i][j][1],fout);
						putc(out_buffer1[i][j][0],fout);
					}
				}
				fwrite(out_buffer1,sizeof(unsigned char),size,fout);
				break;
			}

		}
		break;
	
	}
    }
	return 0;
}
