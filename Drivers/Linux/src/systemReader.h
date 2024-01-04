#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int readCpuInfo(int scale){ //Reads data from /proc/stat. Scale indicates how much to multiply by when all data is found (ex. 100 for percentages, 255 for PWM values)
    //Update CPU light
    FILE *cpuUsage = fopen("/proc/stat","r");
    // Read /proc/stat
    // skip first item "cpu", sum first line
    char throwawayData[5];
    fgets(throwawayData,5,cpuUsage);
    // get all numbers in first line
    int values[10];
    fscanf(cpuUsage,"%d %d %d %d %d %d %d %d %d %d",&values[0],&values[1],&values[2],&values[3],&values[4],&values[5],&values[6],&values[7],&values[8],&values[9]);// Longest scanf ever!
    // add all numbers
    int timeSum = 0;
    for(int i = 0; i<10; i++){
        timeSum += values[i];
    }
    // Divide idle (4th item) by total
    float idlePercent = values[3]/timeSum;
    // Subtract from 1 
    float nonIdlePercent = 1-idlePercent;
    //Multiply by scale
    nonIdlePercent *= scale;
    printf("total: %d values: %d\n",timeSum,values[3]);
    return (int) nonIdlePercent;
}