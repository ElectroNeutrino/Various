#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//#include <termios.h>
#include <unistd.h>


#define MAX 2000000


/*int getch( ) {
  struct termios oldt,
                 newt;
  int            ch;
  tcgetattr( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
  return ch;
}*/

double x,xx,step;

double Jo;

long i,j,k;


FILE *pFile[5];
char filename[5][25];



int main()
{

    printf("\n Bessel_functions.cpp, ver 1.0 \n");
    printf("\n by Tom Irvine ");
    printf("\n Email:  tomirvine@aol.com \n\n");

    printf("\n This program calculates the Bessel function Jo(x). \n");

	printf( "\n\n Enter the output filename for Jo(x) \n");

	scanf("%s",filename[1]);
	pFile[1]=fopen(filename[1], "w");


   step=0.01;


   double a,b;


   for(i=0; i<2200; i++)
   {

	   x= step * i;

	   xx = x/2;

	   Jo = 1.;

	   for(j=1; j<30; j++)
	   {

			b = 1;


			for(k=1; k<=j; k++)
			{

				b*=k;
            }


			a = pow(xx,(2*j))/pow(b,2.);

			a*=pow( -1., j );

			Jo += a;

//			printf("x=%12.4g a=%12.4g b=%12.4g Jo=%12.4g \n",x,a,b,Jo);

	   }

	   fprintf(pFile[1],"%12.7e %12.6e\n",x,Jo );

   }
   fclose(pFile[1]);



	printf("\n\n Calculation complete. \n\n Press any key to exit.\n\n");
//	getch();
}
