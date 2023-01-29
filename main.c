/**
2401 Final Project
Moataz Dabbour
100863820
*/

#include "defs.h"

int main(int argc, char *argv[])
{
    // Initialize a random seed for the random number generators
    srand(time(NULL));

    BuildingType building;
    ghost casper;
    pthread_t t0, t1, t2, t3, tg;

    printf("Welcome to the ghost-hunt\n");
    printf("Enter the names (20 char/name max) of the our hunters:\n");
   
    // INPUT //
    for (int i = 0; i < MAX_HUNTERS; i++)
    {  
        printf("Name >> ");
        scanf("%20s", building.hunters[i].name);
        printf("%s - Added\n", building.hunters[i].name);
    }
    
    populateRooms(&building, &casper);   

    printRoomList(&building.rooms);
    sleep(2);
    
    pthread_create(&t0, NULL, hunterThread, &building.hunters[0]);
    pthread_create(&t1, NULL, hunterThread, &building.hunters[1]);
    pthread_create(&tg, NULL, ghostThread, &casper);
    pthread_create(&t2, NULL, hunterThread, &building.hunters[2]);
    pthread_create(&t3, NULL, hunterThread, &building.hunters[3]);

    pthread_join(tg, NULL);
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
 

    printResults(&building);
    
    printf("\nGhost was: \n");
    printGhost(&casper);

    // Mem cleaning
    sanitize(&building);

    return 0;
}























