#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "mpi.h"

bool isPrime(int x)
{
if(x==1)
        return false;
    int i;
    if(x==2)
        return true;
    else
    {

        for(i=2;i<=x/2;i++)
        {
            if(x%i==0)
            {
                return false;
            }
        }
        return true;
    }

}
int main(int argc, char * argv[])
{
    int Globalsum=0;
    int my_rank;		/* rank of process	*/
    int p;			/* number of process	*/
    MPI_Status status;	/* return status for 	*/
    /* recieve		*/

    /* Start up MPI */
    MPI_Init( &argc, &argv );

    /* Find out process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /* Find out number of process */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if( my_rank == 0)///Master
    {
        int x=1,y=16;///Lower&upper Bound
        int portion_size=(y-x)/(p-1);
	
        int xsend=x;
        int i;
        for( i=1; i<p; i++)
        {
	
          MPI_Send( &portion_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

            MPI_Send( &xsend, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            xsend+=portion_size;
        }
        for(i=1; i<p; i++)
        {
            int sum=0;
            MPI_Recv(&sum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status );
            
            Globalsum+=sum;
        }
        printf("Total number of prime numbers is: %d /n",Globalsum);
    }
    else///slave
    {
        int portion_size;
         MPI_Recv(&portion_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status );
         int xsend;
         MPI_Recv(&xsend, 1,MPI_INT, 0, 0, MPI_COMM_WORLD, &status );
         int i=xsend,cnt=0;
	 bool prime;
         for(;i<xsend+portion_size;i++)
         {
           prime  =false;
            prime=isPrime(i);
            if(prime==true)
{
cnt++;


}
                

         }
	    printf(" %s,%d,%s,%d /n","Total number of prime numbers in P" , my_rank ,  "is: " ,cnt);
            MPI_Send( &cnt, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            cnt=0;
    }

    /* shutdown MPI */
    MPI_Finalize();
    return 0;
}
