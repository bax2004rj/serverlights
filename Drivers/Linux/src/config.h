//Read and provide UI for config files
#include <stdio.h>
#include <stdlib.h>

int readConf(char serialLocation[32],int temps[2], int fanThreshold,int safeShutdown,char thermalUnits){
    printf("\033[35m[ NOTE  ]\033[37m: Reading conf file...\n");
    FILE *config = fopen("/etc/serverlights/config.conf","rw");   
    return 0; // For now. No installs have these files and it hasnt been implemented
}

void confUi(char serialLocation[32],int temps[2], int fanThreshold,int safeShutdown){
    printf("Serverlights Config UI\n");
    printf("\033[35m[ NOTE  ]\033[37m: If you would like to edit this file directly, it is availiable in /etc/serverlights/config.conf\n");
    char thermalUnits = 'c';
    int confExists = readConf(serialLocation,temps,fanThreshold,safeShutdown,thermalUnits);
    if(confExists == -1){
        printf("\033[31m[ FATAL ]\033[37m: Conf file does not exist. Make sure that /etc/serverlights/config.conf exists and that you installed this daemon correctly.\n");
    } else {
        puts("Pre-settings: Unit selection");
        printf("Please choose thermal units you would like to use (default: %c)\n",thermalUnits);
        puts("Options: c = Celsius, f = Fahrenheit, k = Kelvin, r = Rankine, <Enter/Return> = Default Option");
        char thermalInput;
        scanf("%c",&thermalInput);
    }
}