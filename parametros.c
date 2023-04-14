#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <locale.h>

void get_params(int argc, char *argv[], int *o,int *f, int *c, double *e, int *r, int *s, int *p ){
    int opt;
    setlocale(LC_ALL, "");
    while((opt=getopt(argc,argv,"o:f:c:e:r:s:p:"))!=-1){
        switch(opt){
            case 'o':
                *o=atoi(optarg);
                break;
            case 'f':
                *f=atoi(optarg);
                break;
            case 'c':
                *c=atoi(optarg);
                break;
            case 'e':
                *e=atoi(optarg);
                break;
            case 'r':
                *r=atoi(optarg);
                break;
            case 's':
                *s=atoi(optarg);
                break;
            case 'p':
                *p=atoi(optarg);
                break;
            default:
               fprintf(stderr, "Usage: %s -o <operation> [-x <value>] [-y <value>] [-p <value>] [-q <value>] [-r <value>]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

}