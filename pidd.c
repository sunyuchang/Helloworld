#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <string.h>
#include <dirent.h>

#define READ_BUF_SIZE 50

extern long* find_pid_by_name( char* pidName) 
{

   DIR *dir;
   struct dirent *next;
   long* pidList=NULL;
   int i=0;
   dir = opendir("/proc");
   if (!dir)
   {
	  fprintf(stderr, "Cannot open /proc\n");						    exit(1);
    }

				 

				    while ((next = readdir(dir)) != NULL)
					        {

							        FILE *status;

								        char filename[READ_BUF_SIZE];

									        char buffer[READ_BUF_SIZE];

										        char name[READ_BUF_SIZE];

											 

											        /* Must skip ".." since that is outside /proc */

											        if (strcmp(next->d_name, "..") == 0)

													            continue;

												 

												        /* If it isn't a number, we don't want it */

												        if (!isdigit(*next->d_name))

														            continue;

													 

													        sprintf(filename, "/proc/%s/status", next->d_name);

														        if (! (status = fopen(filename, "r")) ) 

															{

																            continue;

																	            }

															 

															/* Read first line in /proc/?pid?/status */

															        if (fgets(buffer, READ_BUF_SIZE-1, status) == NULL) 

																{

																	            fclose(status);

																		     

																		                continue;

																				        }

																 

																        fclose(status);

																	 

																	        /* Buffer should contain a string like "Name:   binary_name" */

																	sscanf(buffer, "%*s %s", name);

																	        if (strcmp(name, pidName) == 0) 

																		{

																			            pidList=realloc( pidList, sizeof(long) * (i+2));

																				                pidList[i++]=strtol(next->d_name, NULL, 0);

																						        }

																		    }

				     

				        if (pidList) 

						    {

							            pidList[i]=0;

								        } 

					    else 

						        {

								        pidList=realloc( pidList, sizeof(long));

									        pidList[0]=-1;

										    }

					     

					        return pidList;

}

 

 

 

int main(int argc, char **argv)

{

	    int opt, n = 0;

	        int single_flag = 0;

		    int fail = 1;

		     

		        /* do normal option parsing */

		        while ((opt = getopt(argc, argv, "s")) > 0) 

				    {

					            switch (opt) 

						    {

							                case 's':

										                single_flag = 1;

												                break;

														 

														            default:

														printf("usage:\n");

														       printf("\t%s processname: list all pids by processname\n", argv[0]);

														       printf("\t%s -s processname: list the first pid by processname\n", argv[0]);

														               }

						        }

			 

			    /* Looks like everything is set to go. */

			    while(optind < argc) 

				        {

						        long *pidList;

							        long *pl;

								 

								        pidList = find_pid_by_name(argv[optind]);

									        for(pl = pidList; *pl > 0; pl++) 

										{

											            printf("%s%ld", (n++ ? " " : ""), *pl);

												                fail = 0;

														 

														            if (single_flag)

																                    break;

															            }

										        free(pidList);

											        optind++;

												    }

			     

			        printf("\n");

				    return(fail);

}


