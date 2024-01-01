#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

char serialLocation[32] = "/dev/ttyACM0"; //Serial device
int temps[2] = {70,100}; //Temperatures to turn temp warning lights off
int fanThreshold = 1000; //Deviation in RPM for when to sound fan alarm
bool safeShutdown = true; // Send safe safe shutdown signal when process killed

int main(){
    FILE *serialpoint;
    FILE *config;

    serialpoint = fopen(serialLocation,"rw");
    // TODO: Conf file reader
    while (1){

    }
}