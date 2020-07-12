/**--Name: 	Siyuan Yan
  	 ID: 	28498704
	 Email: syan0009@student.monash.edu
**/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Main program
int main()
 {  
    struct color{
        unsigned char r;
        unsigned char g;
        unsigned char b;
     }col;
    int rank,size,counter,row_remain,row_per_procs,i,j;
	double t1,t2,t3;
    struct color* rowBuff;
	struct color* remainBuff;
    struct color* buff;
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	/* screen ( integer) coordinate */
	int iX,iY;
	const int iXmax = 8000; // default
	const int iYmax = 8000; // default
	row_remain=iYmax%size;
	/* world ( double) coordinate = parameter plane*/
	double Cx, Cy;
	const double CxMin = -2.5;
	const double CxMax = 1.5;
	const double CyMin = -2.0;
	const double CyMax = 2.0;
	/* */
	double PixelWidth = (CxMax - CxMin)/iXmax;
	double PixelHeight = (CyMax - CyMin)/iYmax;
	/* color component ( R or G or B) is coded from 0 to 255 */
	/* it is 24 bit color RGB file */
	const int MaxColorComponentValue = 255; 
	FILE * fp;
	char *filename = "Mandelbrot.ppm";
	char *comment = "# ";	/* comment should start with # */
	// RGB color array
	// static unsigned char color[3];
    // unsigned char buff[12];

	/* Z = Zx + Zy*i;	Z0 = 0 */
	double Zx, Zy;
	double Zx2, Zy2; /* Zx2 = Zx*Zx;  Zy2 = Zy*Zy  */
	/*  */
	int Iteration;
	const int IterationMax = 2000; // default

	/* bail-out value , radius of circle ;  */
	const double EscapeRadius = 400;
	double ER2 = EscapeRadius * EscapeRadius;
	/*create new file,give it a name and open it in binary mode  */
    if (rank==0){
	
        fp = fopen(filename, "wb"); /* b -  binary mode */
		

        buff = (struct color*)malloc((iXmax*iYmax)*sizeof(struct color));
		remainBuff=(struct color*)malloc((row_remain*iXmax)*sizeof(struct color));


        /*write ASCII header to the file (PPM file format)*/
	    fprintf(fp,"P6\n %s\n %d\n %d\n %d\n", comment, iXmax, iYmax, MaxColorComponentValue);
	    printf("File: %s successfully opened for writing.\n", filename);
	    printf("Computing Mandelbrot Set. Please wait...\n");
       
    }
	
	// Get current clock time.
	// start = clock();

	/* compute and write image data bytes to the file */
    row_per_procs=iYmax/size;
	
    rowBuff=(struct color*)malloc((row_per_procs*iXmax)*sizeof(struct color));// each process initialize a 2000*8000 array.   
    counter=0; 
	t1 = MPI_Wtime();
	for(iY = rank; iY<iYmax-row_remain; iY+=size)
	{
		Cy = CyMin + (iY * PixelHeight);
		if (fabs(Cy) < (PixelHeight / 2))
		{
			Cy = 0.0; /* Main antenna */
		}

		for(iX = 0; iX < iXmax; iX++)
		{
			Cx = CxMin + (iX * PixelWidth);
			/* initial value of orbit = critical point Z= 0 */
			Zx = 0.0;
			Zy = 0.0;
			Zx2 = Zx * Zx;
			Zy2 = Zy * Zy;

			/* */
			for(Iteration = 0; Iteration < IterationMax && ((Zx2 + Zy2) < ER2); Iteration++)
			{
				Zy = (2 * Zx * Zy) + Cy;
				Zx = Zx2 - Zy2 + Cx;
				Zx2 = Zx * Zx;
				Zy2 = Zy * Zy;
			};

			/* compute  pixel color (24 bit = 3 bytes) */
			if (Iteration == IterationMax)
			{
				// Point within the set. Mark it as black
                col.r=0;
                col.g=0;
                col.b=0;
			}
			else 
			{
				// Point outside the set. Mark it as white
				double c = 3*log((double)Iteration)/log((double)(IterationMax) - 1.0);
				if (c < 1)
				{
                    col.r=0;
                    col.g=0;
					col.b = 255*c;
				}
				else if (c < 2)
				{
					col.r = 0;
					col.g = 255*(c-1);
					col.b = 255;
				}
				else
				{
					col.r = 255*(c-2);
					col.g = 255;
					col.b = 255;
                    
				}
			}
            rowBuff[counter++]=col;
		}
    }
		t2 = MPI_Wtime();
		printf("Rank %d costs %f\n",rank,t2-t1);

        int blocklens[3];
        MPI_Datatype old_types[3];
        MPI_Datatype my_struct;
        MPI_Aint indices[3];
        blocklens[0]=1;
        blocklens[1]=1;
        blocklens[2]=1;
        old_types[0]=MPI_CHAR;
        old_types[1]=MPI_CHAR;
        old_types[2]=MPI_CHAR;
        MPI_Get_address(&col.r,&indices[0]);
        MPI_Get_address(&col.g,&indices[1]);
        MPI_Get_address(&col.b,&indices[2]);
        indices[2]=indices[2]-indices[0];
        indices[1]=indices[1]-indices[0];
        indices[0]=0;
        MPI_Type_create_struct( 3, blocklens, indices, old_types, &my_struct );
        MPI_Type_commit( &my_struct );
        MPI_Gather(rowBuff,(row_per_procs*iXmax),my_struct,buff,(row_per_procs*iXmax),my_struct,0,MPI_COMM_WORLD);
	if (rank==0){
		counter=0; 
		for(iY = iYmax-row_remain; iY<iYmax; iY++)
		{
			Cy = CyMin + (iY * PixelHeight);
			if (fabs(Cy) < (PixelHeight / 2))
			{
				Cy = 0.0; /* Main antenna */
			}

			for(iX = 0; iX < iXmax; iX++)
			{
				Cx = CxMin + (iX * PixelWidth);
				/* initial value of orbit = critical point Z= 0 */
				Zx = 0.0;
				Zy = 0.0;
				Zx2 = Zx * Zx;
				Zy2 = Zy * Zy;

				/* */
				for(Iteration = 0; Iteration < IterationMax && ((Zx2 + Zy2) < ER2); Iteration++)
				{
					Zy = (2 * Zx * Zy) + Cy;
					Zx = Zx2 - Zy2 + Cx;
					Zx2 = Zx * Zx;
					Zy2 = Zy * Zy;
				};

				/* compute  pixel color (24 bit = 3 bytes) */
				if (Iteration == IterationMax)
				{
					// Point within the set. Mark it as black
					col.r=0;
					col.g=0;
					col.b=0;
				}
				else 
				{
					// Point outside the set. Mark it as white
					double c = 3*log((double)Iteration)/log((double)(IterationMax) - 1.0);
					if (c < 1)
					{
						col.r=0;
						col.g=0;
						col.b = 255*c;
					}
					else if (c < 2)
					{
						col.r = 0;
						col.g = 255*(c-1);
						col.b = 255;
					}
					else
					{
						col.r = 255*(c-2);
						col.g = 255;
						col.b = 255;
						
					}
				}
				remainBuff[counter++]=col;
			}
		}
		
		t3=MPI_Wtime();
		printf("The computation and communication cost %f\n",t3-t1);
		for (j=0;j<row_per_procs;j++){
			for(i=0;i<size;i++){
				fwrite(buff+(i*iXmax*row_per_procs)+(j*iXmax),sizeof(struct color),iXmax,fp);

			}
		}
		fwrite(remainBuff,sizeof(struct color),row_remain*iXmax,fp);
        fclose(fp);
        printf("Completed Computing Mandelbrot Set.\n");
        printf("File: %s successfully closed.\n", filename);
        free(buff);
		free(remainBuff);
	}
    free(rowBuff);
	MPI_Type_free( &my_struct );
    MPI_Finalize();
    return 0;
 }
