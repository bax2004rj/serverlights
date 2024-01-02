#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <ctype.h> 
#include "config.h"

char serialLocation[32] = "/dev/ttyACM0"; //Serial device
int temps[2] = {70,100}; //Temperatures to turn temp warning lights off
int fanThreshold = 1000; //Deviation in RPM for when to sound fan alarm
bool safeShutdown = true; // Send safe safe shutdown signal when process killed
char versionString[32] = "V1.0.0B" 

int main(int argc, char argv[]){
    FILE *serialpoint;
    FILE *config;
    serialpoint = fopen(serialLocation,"rw");
    int argMode = interpretCommandLineArgs(int argc, char argv[])
    if(argMode == 1){
        confUi(char serialLocation[32],int temps[2], int fanThreshold,bool safeShutdown);
    } else if (argMode == 0) {
        readConf(serialLocation,temps,fanThreshold,safeShutdown);
        while (1){
            
        }    
    }
}

int interpretCommandLineArgs(int argc, char argv[]){//Read inputs from command line
    int option;
    while((option = getopt(argc,argv,"sv"))!=-1){
        switch (option)
        {
        case 'c':
        case 's': //Enter options mode
            return 1;
            break;
        case 'v': // Display version info
            puts("serverlights");
            printf("Version: %s\n",version);
            puts("© 2024 bax2004 et als. Licensed under GNU GPL v3");
            return 2;
            break;
        }

    }
}