/**
2401 Final Project
Moataz Dabbour
100863820
*/

#include "defs.h"

/**
Function: initRoom
Purpose:  init room, init room's semaphore, assigns name, and init room's adjacent and evidence lists,
          nulls hunters here, nulls hasGhost
In:       room, name
Out:      room
*/
void initRoom(room* room, char* name) {

     sem_init(&room->roommutex, 0, 1);
     strcpy(room->name, name);
     room->hasGhost = NULL;
     initRoomList(&(room->adjacent));
     initEvidList(&(room->evidHere));
     for (int i = 0; i < MAX_HUNTERS; i++)
     {
        room->huntersHere[i] = NULL;
     }
     
}

/**
Function: initHunter
Purpose:  init hunter, init hunter's evid list, assigns random and preset values to hunter attributes
In:       b, start, id, hunter
Out:      hunter
*/
void initHunter(BuildingType* b, hunter* hunter, room* start, int id) {
    hunter->building = b;
    hunter->conc = 0;
    hunter->prog = 0;
    hunter->id = id;
    hunter->currIn = start;
    initEvidList(&hunter->colEvid);
    hunter->fear = 0;
    hunter->boredom = BOREDOM_MAX;
    hunter->canCollect = randInt(0,4);
    start->huntersHere[id] = hunter;
  
}

/**
Function:  initGhost
Purpose:   init ghost, assigns random and preset values to ghost's attributes
In:        building, ghost
Out:       ghost
*/
void initGhost(ghost* ghost, BuildingType* building) {
    ghost->building = building;
    ghost->locked = 0;
    ghost->gType = randInt(0,4);
    ghost->boredom = BOREDOM_MAX;
    switch (ghost->gType)
    {
    case 0:
        // (*ghost->abil) = {0,1,2};
        ghost->abil[0] = 0;
        ghost->abil[1] = 1;
        ghost->abil[2] = 2;
        break;
    case 1:
        // ghost->abil = {0,1,3};
        ghost->abil[0] = 0;
        ghost->abil[1] = 1;
        ghost->abil[2] = 3;
        break;
    case 2:
        // ghost->abil = {0,2,3};
        ghost->abil[0] = 0;
        ghost->abil[1] = 2;
        ghost->abil[2] = 3;
        break;
    case 3:
        // ghost->abil = {1,2,3};
        ghost->abil[0] = 1;
        ghost->abil[1] = 2;
        ghost->abil[2] = 3;
        break;
    
    default:
        break;
    }
}

// LISTS //

/**
Function: initRoomList
Purpose:  init RoomList, nulls head and sets count to 0;
In/Out:   list
*/
void initRoomList(RoomList* list) { 

    list->count = 0;
    list->head = NULL;
    
}

/**
Function: initEvidList
Purpose:  init EvidList, nulls head and sets count to 0, init EvidList's semaphore
In/Out:   list
*/
void initEvidList(EvidList* list) { 
    
    sem_init(&list->mutex, 0, 1);
    list->count = 0;
    list->head = NULL;

}

/*
  Function:  randInt
  Purpose:   returns a pseudo randomly generated number, 
             in the range min to (max - 1), inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [min, max-1) 
*/
int randInt(int min, int max)
{
    // printf("min : %d - max %d\n",min,max);
    return rand() % (max - min) + min;
}

/*
  Function:  randFloat
  Purpose:   returns a pseudo randomly generated number, 
             in the range min to max, inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [0, max-1) 
*/
float randFloat(float a, float b) {

    // Get a percentage between rand() and the maximum
    float random = ((float) rand()) / (float) RAND_MAX;
    // Scale it to the range we want, and shift it
    return random * (b - a) + a;
}

/**
Function: printRoomList
Purpose:  prints rooms and the locations of hunters.
In:       list
*/
void printRoomList(RoomList* list) { 


    // printf(": %d\n", list->count);
    printf("G  H H H H | ROOMS\n");
    printf("__________________\n");



    for (RoomNode* curr = (*list).head;
            curr != NULL;
            curr = curr->next)
    {

        if (curr->room->hasGhost != NULL){printf("G ");} else {printf("  ");}

        for (int i = 0; i < MAX_HUNTERS ; i++)
        {
            if (curr->room->huntersHere[i] != NULL) {printf(" *");} else { printf("  ");}
        }
        
        printf(" - %s\n", curr->room->name);
    }

}

/**
Function: printEvidList
Purpose:  prints detailed EvidList
In:       list
*/
void printEvidList(EvidList* list) { 

    printf("Length: %d\n", list->count);
    for (EvidNode* curr = (*list).head;
            curr != NULL;
            curr = curr->next)
    {
        if (curr->evidence != NULL){
        printEvid(curr->evidence);
        printf("\n");
        }
    }

}

/**
Function: printHunter
Purpose:  prints detailed hunter inforamtion.
In:       hunter
*/
void printHunter(hunter* hunter) { 
    char* evidTypes [] = { "EMF", "TEMPERATURE", "FINGERPRINTS", "SOUND" };
    char* canCol;
    char* startingIn = (*hunter->currIn).name;

    if (hunter->canCollect >=0 && hunter->canCollect <= 3) {canCol = evidTypes[hunter->canCollect];} else { canCol = "Nothing???";}

    printf("Hunter - %d: %s - In: %s - Fear: %d - Boredom: %d\nCan Collect: %s\nCollected:\n",
        hunter->id,
        hunter->name,
        startingIn,
        hunter->fear,
        hunter->boredom,
        canCol);
    printEvidList(&hunter->colEvid);

}

/**
Function: printEvid
Purpose:  prints detailed evidence information
In:       evidence
*/
void printEvid(evidence* evidence) { 

    char* evidTypes [] = { "EMF", "TEMPERATURE", "FINGERPRINTS", "SOUND" };
    char* type;
    // char* myst = "      ";

    if (evidence->eType >= 0 && evidence->eType <=3 ) {type = evidTypes[evidence->eType];} else {type = "UNKNOWN";}
    if (evidence->ghostly == 1) {printf("Spooky ");}
    printf("Evidence: %s - Value: %f", type, evidence->value);

}

/**
Function: printGhost
Purpose:  prints ghostType and location
In:       ghost
*/
void printGhost(ghost* ghost) { 
    char* ghostTypes [] = { "POLTERGEIST", "BANSHEE", "BULLIES", "PHANTOM" };
    char* type;
    if (ghost->gType >= 0 && ghost->gType <=3 ) {type = ghostTypes[ghost->gType];} else { type = "Unknown???";}

    printf("%s - In: %s", type, ghost->currRoom->name);
}

/**
Function: printResults
Purpose:  checks status of hunters and prints results based on that. calls solve if hunter 
          has figured it out.
In:       build
*/
void printResults(BuildingType* build) { 
    printf("\n_____________________ SIM RESULT _____________________\n");
    char* b = "Gotten Bored!";
    char* s = "Ran Away!";
    char* d = "Uncovered the entity!";
    int fearAll = 0;
    for (int i = 0; i < MAX_HUNTERS; i++)
    {
        if (build->hunters[i].fear >= MAX_FEAR) {
            fearAll ++;
            printf("%s has %s with a fear level: %d\n",build->hunters[i].name, s, build->hunters[i].fear );
        
        }

        if (build->hunters[i].boredom == 0) {

            printf("%s has %s with a fear level: %d\n",build->hunters[i].name, b, build->hunters[i].fear);
        
        }

        if (build->hunters[i].prog >= 3) {

            printf("%s has %s with a fear level: %d\nTheir conclusion was: ",build->hunters[i].name, d, build->hunters[i].fear);
            solve(&build->hunters[i]);

        }

    }

    if (fearAll == MAX_HUNTERS ) {printf("GHOST HAS WON!");}
}

// CLEANING // 

/**
Function: cleanEvidList
Purpose:  deallocate memory from EvidList (Evidence & Nodes)
In//Out:  tbc
*/
void cleanEvidList(EvidList* tbc) { 
    for (EvidNode* currNode= tbc->head; currNode != NULL ; currNode = currNode->next)
    {
        if (currNode->evidence != NULL) { 
            free(currNode->evidence);
            currNode->evidence = NULL;

        }
    }

    EvidNode * curr = tbc->head;
    while(curr != NULL) {

        EvidNode* temp = curr->next;
        free(curr);
        curr = temp;
    }
}

/**
Function: cleanRoomList
Purpose:  deallocate memory from RoomList (Nodes only)
In:       |
Out:      |
Return:   |
*/
void cleanRoomList(RoomList* tbc) { 
    
    RoomNode * curr = tbc->head;
    while(curr != NULL) {

        RoomNode* temp = curr->next;
        free(curr);
        curr = temp;
    }
    
}

/**
Function: sanitize
Purpose:  deallocate all memory called in program, calls cleanRoomList & cleanRoomList on building
          also deallocates memory allocated by populateRooms()
In/Out:   tbc
*/
void sanitize(BuildingType* tbc) { 

    // Clean Hunters Collected Evidence
    for (int i = 0; i < MAX_HUNTERS ; i++)
    {
        cleanEvidList(&tbc->hunters[i].colEvid);
    }

    for (RoomNode* curr = tbc->rooms.head; curr != NULL; curr = curr->next) 
    { 
        cleanEvidList(&curr->room->evidHere);
        cleanRoomList(&curr->room->adjacent);
        free(curr->room);
    }

    cleanRoomList(&tbc->rooms);
      
}