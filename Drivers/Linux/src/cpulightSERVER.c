#include <stdio.h>
#include <time.h>
#include <getopt.h>
#include <ctype.h> 
#include "config.h"
#include "systemReader.h"

char serialLocation[32] = "/dev/ttyACM0"; //Serial device
int temps[2] = {70,100}; //Temperatures to turn temp warning lights off
int fanThreshold = 1000; //Deviation in RPM for when to sound fan alarm
int safeShutdown = 1; // Send safe safe shutdown signal when process killed
int delayTime = 1; // Delay in ms for the lights
char versionString[32] = "V1.0.0B";

int interpretCommandLineArgs(int argc, char argv[]);

int main(int argc, char argv[]){
    FILE *serialpoint = fopen(serialLocation,"rw");
    printf("\033[32m[ PASS  ]\033[37m: Serial connected!\n"); //this will be mor e complex in future
    int argMode = interpretCommandLineArgs(argc, argv);
    if(argMode == 1){
        confUi(serialLocation,temps, fanThreshold,safeShutdown);
    } else if (argMode == 0) { //Normal mode
        char thermThrowaway;//Here because C doesnt have default options
        readConf(serialLocation,temps,fanThreshold,safeShutdown,thermThrowaway);
        printf("\033[32m[ PASS  ]\033[37m: Config file read\n");
        fputc('b',serialpoint);
        printf("\033[32m[ PASS  ]\033[37m: daemon started!\n");
        while (1){
            sleep(delayTime);
            //Update CPU light
            int cpu = readCpuInfo(255);
            fprintf(serialpoint,"c%d",cpu);
            //Send possible warning messages
            //Fan from /proc/acpi
            
            //Temperature warnings

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
            printf("Version: %s\n",versionString);
            puts("© 2024 bax2004 et als. Licensed under GNU GPL v3");
            return 2;
            break;
        }

    }
    return 0;
}