#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int previousTimeSum = 0;
int previousIdle = 0;
int timeSum = 0;
int idleTime = 0;

int readCpuInfo(int scale){ //Reads data from /proc/stat. Scale indicates how much to multiply by when all data is found (ex. 100 for percentages, 255 for PWM values).
    puts("reading cpu");
    previousTimeSum = timeSum; //Original attempt at code
    previousIdle = idleTime;
    timeSum = 0;
    idleTime = 0;
    // Open /proc/stat
    FILE *cpuUsage = fopen("/proc/stat","r");
    // Read /proc/stat
    // skip first item "cpu", sum first line
    char throwawayData[6];
    fgets(throwawayData,6,cpuUsage);
    // get all numbers in first line
    int values[10];
    for(int i = 0;i<10;i++){// fscanf didnt exactly work so were taking it on our own hands
        char data[32]="\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
        char tmp = '0';
        puts("reading data");
        for(int j = 0;isalnum(tmp);j++){
            data[j] = fgetc(cpuUsage);
            tmp = data[j];
            printf("char %d = %c",j,data[j]);
        }
        values[i] = atoi(data);
        printf("data at %d = %d ",i,values[i]);
        //fgetc(cpuUsage); //Skip the space
    }
    printf("\n");
    //fscanf(cpuUsage,"%d %d %d %d %d %d %d %d %d %d",&values[0],&values[1],&values[2],&values[3],&values[4],&values[5],&values[6],&values[7],&values[8],&values[9]);// Longest scanf ever!
    // add all numbers
    timeSum = 0;
    for(int i = 0; i<10; i++){
        timeSum += values[i];
    }
    idleTime = values[4];
    // Divide idle (4th item) by total
    float idlePercent = (idleTime-previousIdle)/(timeSum-previousTimeSum);
    // Subtract from 1 
    float nonIdlePercent = 1-idlePercent;
    //Multiply by scale
    nonIdlePercent *= scale;
    printf("total: %d idle time: %d\n",timeSum,values[3]);
    //Close before reporting
    fclose(cpuUsage);
    return (int) nonIdlePercent;
}