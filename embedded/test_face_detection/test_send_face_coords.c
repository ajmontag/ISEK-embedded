#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
	double x, y;
	double angle;

	for(x = -5; x <=5 ; x++)
	{	
		for(y = -5; y <=5; y++)
		{
			angle = atan(y/x)*180/3.14159;
			if( x < 0 && y > 0 ) // Will receive 0 to -90 but need 90 to 180
				angle = angle + 180;
			else if( x > 0 && y < 0) // Will receive 0 to -90 but need 270 to 360
				angle = angle + 360;
			else if( x < 0 && y < 0) // Will receive 0 to 90 but need 180 to 270
				angle = angle + 180;

			printf("X: %lf   Y: %lf\nAngle: %lf\n\n", x, y, angle);
		}
	}
}
