/**
2401 Final Project
Moataz Dabbour
100863820
*/

#include "defs.h"

/**
Function: appendRoomNode
Purpose:  Appends a RoomNode to a RoomList
In:       RoomNode * 
Out:      RoomList * 
*/
void appendRoomNode(RoomList* list, RoomNode* node) {

    RoomNode* current = node;
    node->next = list->head;
    list->head = current;
    list->count ++;
}

/**
Function: appendEvidNode
Purpose:  Appends a EvidNode to a EvidList
In:       EvidNode * 
Out:      EvidList * 
*/
void appendEvidNode(EvidList* list, EvidNode* node) { 

    EvidNode* current = node;
    node->next = list->head;
    list->head = current;
    list->count ++;
}

/**
Function: connectRooms 
Purpose:  Adds r1 to r2's adjacent list and vice versa
In/Out:   2x room*
*/
void connectRooms(room* r1, room* r2) {

    RoomNode * r1n = calloc(1,sizeof(RoomNode));  /** MALLOC: */
    RoomNode * r2n = calloc(1,sizeof(RoomNode));  /** MALLOC: */

    r1n->room = r1;
    r2n->room = r2;

    appendRoomNode(&r2->adjacent,r1n);
    appendRoomNode(&r1->adjacent,r2n);
}

// GHOST OPS

/**
Function: createEvid
Purpose:  Randomly chooses an evidence and value (within spec'd range) and adds its to the rooms EvidList
In:       ghost* 
Out:      room* 
*/
void createEvid(room* room, ghost* ghost){ 
    // ghost->boredom --;
    
    float value;
    int evidChoice = randInt(0,3);
    EvidenceClassType evidType = ghost->abil[evidChoice];

    evidence * evid = calloc(1, sizeof(evidence));    /** MALLOC: */
    
    switch (evidType) 
    {
    case 0:
        value = randFloat(4.7,5.0);
        if (value > 4.9) {evid->ghostly = 1;}
        break;
    case 1:
        value = randFloat(-10,1.0);
        if (value < 0) {evid->ghostly = 1;}
        break;
    case 2:
        value = 1.00;
        evid->ghostly = 1;
        break;
    case 3:
        value = randFloat(65.0,75.0);
        if (value > 70.0) {evid->ghostly = 1;}
        break;
    default:
        break;
    }

    evid->value = value;
    evid->eType = evidType;
    

    EvidNode * evinode = calloc(1,sizeof(EvidNode));  /** MALLOC: */

    evinode->evidence = evid;
    evinode->av = &value;

    appendEvidNode(&room->evidHere, evinode);
    
    // VERBOSE
    printEvid(evid);
    printf(" DROPPED in the %s\n", room->name);
}

/**
Function: palceGhost
Purpose:  places the ghost somewhere in the RoomList, updates rooms pointers if init == 0;
In: RoomList, init
Out: ghost
*/
void placeGhost(ghost* ghost, RoomList* list, int init){ 


    // Not the first insert (clear room's hasGhost)
    if (init == 0) { ghost->currRoom->hasGhost = NULL; }

    
    int upperBound = list->count;
    int choice = randInt(0,upperBound);
    int counter = 0;
    RoomNode* curr;

    for (curr = (*list).head; counter < choice; curr = curr->next)
    {
        // VERBOSE
        // printf("GHOST considering %s\n", curr->room->name);
        counter ++;
    }


    int status;
    sem_getvalue(&curr->room->roommutex, &status);


 

    // update new room's hasGhost
    curr->room->hasGhost = ghost;

    // update ghost's Current
    ghost->currRoom = curr->room;

    printf("GHOST Moved to %s!\n", curr->room->name);

    if (hasHunters(curr->room) > 0) {

        // VERBOSE
        printf("GHOST has eyes on Hunters!\n");

        ghost->boredom = BOREDOM_MAX;

    }

}

/**
Function: takeActionGhost
Purpose:  Decision function on action to take by Ghost, takes into consideration whether hunters are in the room
In/Out: ghost   
*/
void takeActionGhost(ghost* ghost) { 
    
    room * room = ghost->currRoom;
    int upperBound = 2;


    // Checking if ghost is alone or not 
    if (hasHunters(ghost->currRoom) == 0) {
        upperBound = 3;
        ghost->boredom --;
        }

    int action = randInt(0, upperBound);

    switch (action)
    {
    case 0:
        sem_wait(&room->evidHere.mutex);
        createEvid(room, ghost);
        sem_post(&room->evidHere.mutex);
        ghost->locked ++;
        break;
    case 1:
        ghost->locked ++;
        break;
    case 2:
        if (ghost->locked > 30) { placeGhost(ghost, &ghost->building->rooms, 0); }
        placeGhost(ghost, &room->adjacent, 0);
        break;
    
    default:
        break;
    }
}

/**
Function: hasHunters
Purpose:  Checks if a room has any hunters
In:       room
Return:   int count of the number of hunters  
*/
int hasHunters(room* room) { 

    int ret = 0;
    for (int i = 0; i < MAX_HUNTERS; i++)
    {
        if(room->huntersHere[i] != NULL) { ret++ ;}
    }
    return ret;

}

/**
Function: ghostThread
Purpose:  While loop for threading purposes
In/Out:   void* g typecasted into (ghost*)
Return:   void* g 
*/
void* ghostThread(void* g) { 

    ghost* c = (ghost*) g;
    while (1)
    {
        if(c->boredom <= 0) {
            printf("_____________________ GHOST LEFT _____________________\n");
            break;
            }
        takeActionGhost(c);
        usleep(USLEEP_TIME);
    }
}
    
// HUNTER OPS

/**
Function: createMundEvid
Purpose:  creates and appends mundane (non ghostly) evidence to the hunter's collected EvidList if the hunter is unable to find any. 
In/Out:   hunter
*/
void createMundEvid(hunter* hunter){
    float value;
    EvidenceClassType evidType = hunter->canCollect;
    evidence * evid = calloc(1, sizeof(evidence));    /** MALLOC: */

    switch (evidType) 
    {
    case 0:
        value = randFloat(0, 4.9);
        break;
    case 1:
        value = randFloat(0, 27.0);
        break;
    case 2:
        value = 0.0;
        break;
    case 3:
        value = randFloat(40.0, 70.0);
        break;
    default:
        break;
    }

    evid->eType = evidType;
    evid->value = value;
    evid->ghostly = 0;

    EvidNode * evinode = calloc(1,sizeof(EvidNode));  /** MALLOC: */

    evinode->evidence = evid;
    evinode->av = NULL;

    appendEvidNode(&hunter->colEvid,  evinode);

    // VERBOSE 
    printEvid(evid);
    printf(" FOUND in the %s by %s\n", hunter->currIn->name, hunter->name);

}

/**
Function: moveHunter
Purpose:  moves the hunter to an adjacent room by random selection, checks for precense of ghost.
In/Out:   hunter
*/
void moveHunter(hunter* hunter) { 

    room * start = hunter->currIn;
    RoomList adj = start->adjacent;
    int choice = randInt(0,adj.count);
    int counter = 0;
    RoomNode* curr;

    for (curr = adj.head; counter < choice; curr = curr->next)
    {
        // printf("Hunter %s considering %s\n", hunter->name, curr->room->name);
        counter ++;
    }

    // update the 'new' rooms hunters here with index ID and value hunter* 
    curr->room->huntersHere[hunter->id] = hunter;

    // clear old one
    start->huntersHere[hunter->id] = NULL;
    hunter->currIn = curr->room;

    printf("%s: Moving to %s\n", hunter->name, curr->room->name);

    hunter->boredom --;



    if (curr->room->hasGhost != NULL) {printf("%s: Feels weird here!\n", hunter->name); hunter->fear++;}


}

/**
Function:  collectEvid
Purpose:   function cycles thru EvidList in the room and copies it to the hunter's if the hunter is capabile of reading it.
In/Out:    (inexplicit room*), hunter
Return:    int count of found evidence
*/
int collectEvid(hunter* hunter) {
    int found = 0;
    room* room = hunter->currIn;
    EvidList list = room->evidHere;
    int c = 0;
    
    for (EvidNode* currNode = list.head; currNode != NULL;  currNode = currNode->next) {

        if (currNode->av != NULL) {
                                                                // Possible redundancy 
            if (currNode->evidence->eType == hunter->canCollect & currNode->evidence->ghostly == 1) { 

                printEvid(currNode->evidence);

                currNode->av = NULL;

                evidence * evid = calloc(1, sizeof(evidence));    /** MALLOC: */

                evid->eType = currNode->evidence->eType;
                evid->ghostly = 1;
                evid->value = currNode->evidence->value;

                EvidNode * evinode = calloc(1,sizeof(EvidNode));  /** MALLOC: */

                evinode->evidence = evid;
                evinode->av = &evid->value;

                appendEvidNode(&hunter->colEvid, evinode);

                printf(" UNCOVERED by %s \n", hunter->name);

                found ++;
            }
        }
        // c++;
    }
    if (found > 0) {hunter->boredom = BOREDOM_MAX;}
    return found;

}

/**
Function: communicate
Purpose:  communicate ghostly evidence from one hunter to another. calls checkMyEvid on the recieving hunter.
In: from
Out: to   
*/
void communicate(hunter* from, hunter* to) {

    EvidList fromList = from->colEvid;
    
    // VERBOSE
    // printf("%s has collected %d evid\n", from->name, evid.count);
    // printEvidList(&evid);

    for (EvidNode *currNode = fromList.head;
        currNode != NULL;
        currNode = currNode->next) { 
        if (currNode->av != NULL) {

            // Possible redundancy 
            if(currNode->evidence->ghostly == 1) {

                evidence * evid = calloc(1, sizeof(evidence));    /** MALLOC: */

                evid->eType = currNode->evidence->eType;
                evid->ghostly = 1;
                evid->value = currNode->evidence->value;

                EvidNode * evinode = calloc(1,sizeof(EvidNode));  /** MALLOC: */

                evinode->evidence = evid;
                evinode->av = &evid->value;

                appendEvidNode(&to->colEvid, evinode);

                // VERBOSE 
                // printEvid(evid);
                // printf(" Passed from %s to %s\n", from->name, to->name);

                currNode->av = NULL;
            }
        } 
     }

  checkMyEvid(to);

}

/**
Function: takeActionHunter
Purpose:  Decision function on action to be taken by Hunters. takes into consideration the presence of other hunters.
In/Out:   h 
*/
void takeActionHunter(hunter* h) {

    room* room = h->currIn;
    int numofchoices, f, commchoice, countothers;
    int others [MAX_HUNTERS];

    countothers = hasOthers(room, h, others);

    // Give the option to communicate if others are present.
    if (countothers > 0) { 
        numofchoices = 3;
    } else { 
        numofchoices = 2;
        } 

    int action = randInt(0, numofchoices);
    
    switch (action)
    {
    case 0:
        moveHunter(h);
        break;
    case 1:
        // lock room's sem
        sem_wait(&room->roommutex);
        sem_wait(&h->colEvid.mutex);
        f = collectEvid(h);
        sem_post(&room->roommutex);
        sem_post(&h->colEvid.mutex);


        // Lock collection's sem
        sem_wait(&h->colEvid.mutex);
        sem_wait(&room->roommutex);
        if (f == 0) { createMundEvid(h); }
        sem_post(&h->colEvid.mutex);
        sem_post(&room->roommutex);


        break;
    case 2:
        commchoice = randInt(0, countothers);
        hunter* to = room->huntersHere[others[commchoice]];
        // VERBOSE
        printf("%s: Pssst** Hey %s let me share my evidence!\n",h->name, room->huntersHere[others[commchoice]]->name);

        // Lock both Lists before transfer.
        sem_wait(&room->roommutex);
        sem_wait(&to->colEvid.mutex);
        sem_wait(&h->colEvid.mutex);
        communicate(h, to);
        sem_post(&to->colEvid.mutex);
        sem_post(&h->colEvid.mutex);
        sem_post(&room->roommutex);

        break;
    default:
        break;
    }
}

/**
Function: hasOthers
Purpose:  determine the number and indexes→(Whom) of other hunter in the room.
In:       room, hunter
Out:      arr
Return:   int count of the number of other hunters in the same room.
*/
int hasOthers(room* room, hunter* hunter, int arr[MAX_HUNTERS]) { 

    int id = hunter->id;
    int ret = 0;
    int retindx = 0;
    for (int i = 0; i < MAX_HUNTERS ; i++)
    {
        if (i == id) { continue; }
        if (room->huntersHere[i] != NULL) {
            arr[retindx] = i;
            retindx ++;
            ret ++;
         }
    }
    return ret;
} 

/**
Function: checkMyEvid
Purpose:  Hunter checks the number of different unique evidence types he has aquired after a communication.,
            also updates hunter's prog bar.
In/Out:   hunter
Return:   number of different types of evidence's found
*/
int checkMyEvid(hunter* hunter) { 

    // VERBOSE
    printf("%s: OK! me consult my collection! * ", hunter->name);
    EvidList list = hunter->colEvid;

    int t0 = 0;
    int t1 = 0;
    int t2 = 0;
    int t3 = 0;
    for (EvidNode *currNode = list.head;
     currNode != NULL; 
     currNode = currNode->next) {

        if (currNode != NULL & currNode->av != NULL) {
            switch (currNode->evidence->eType)
            {
            case 0:
            t0 = 1;
                break;
            case 1:
            t1 = 1;
                break;
            case 2:
            t2 = 1;
                break;
            case 3:
            t3 = 1;
                break;
            default:
                break;
            }
        }
      }
    int t = t0 + t1 + t2 + t3;

    // VERBOSE 
    printf("I found %d type(s)!\n", t);

    hunter->prog = t; 
    if (t >= 3) {

        // VERBOSE
        printf("%s: Aha! GOTCHA!\n", hunter->name);

        if (t0 == 0) { hunter->conc = 3;}
        if (t1 == 0) { hunter->conc = 2;}
        if (t2 == 0) { hunter->conc = 1;}
        if (t3 == 0) { hunter->conc = 0;}

        } 

}

/**
Function: leaveNow
Purpose:  clear's hunter from room by NULL'ing his pointer, prints status message.
In:       hutner, reason
Out:      hunter (inexplicit room*)
*/
void leaveNow(hunter* hunter, int reason) { 
    
    hunter->currIn->huntersHere[hunter->id] = NULL;
    switch (reason)
    {
    case 0:
            // VERBOSE
            printf("_____________________ %s SPOOKED _____________________\n", hunter->name);
            printRoomList(&hunter->building->rooms);
        break;
    case 1:
            // VERBOSE
            printf("_____________________ %s BORED _____________________\n", hunter->name);
            printRoomList(&hunter->building->rooms);
        break;
    case 2:
            // VERBOSE
            printf("_____________________ %s CONCLUDED _____________________\n", hunter->name);
            printRoomList(&hunter->building->rooms);
        break;
    
    default:
        break;
    }

}

/**
Function: hunterThread
Purpose:  While loop for threading purposes
In/Out:   void* hptr typecasted into (hunter*)
Return:   void* hptr 
*/
void* hunterThread(void* hptr) { 
    hunter* h = (hunter*) hptr;
    while (1)
    {
        if(h->currIn->hasGhost != NULL) {h->fear++;}
        if(h->fear > MAX_FEAR) {
            leaveNow(h,0);
            break;}
        if(h->boredom == 0) {
            leaveNow(h,1);
            break;}
        if(h->prog >= 3) {
            leaveNow(h,2);
            break;}

        takeActionHunter(h);
        usleep(USLEEP_TIME);
    }

}

/**
Function: solve 
Purpose:  enables the hunter to solve which ghost it concluded on
In:       hunter* h  
*/
void solve(hunter* h) { 
    char* ghostTypes [] = { "POLTERGEIST", "BANSHEE", "BULLIES", "PHANTOM" };
    printf("%s \n", ghostTypes[h->conc]);
    
}


