/**
2401 Final Project
Moataz Dabbour
100863820
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_STR            64
#define FEAR_RATE           1
#define MAX_FEAR          100
#define MAX_HUNTERS         4
#define USLEEP_TIME      5000
#define BOREDOM_MAX        99

/** Evidence Type */
typedef enum { EMF, TEMPERATURE, FINGERPRINTS, SOUND } EvidenceClassType;

/** Ghost Type */
typedef enum { POLTERGEIST, BANSHEE, BULLIES, PHANTOM } GhostClassType;

/** EVID OBJ (NULL POSSIBLE)
    EvidenceClassType eType;
    float value;
    int ghostly;
*/
typedef struct _evid evidence;

/** ROOM OBJ
    char name [MAX_STR];
    ghost* hasGhost;
    RoomList adjacent;
    EvidList evidHere;
    hunter* huntersHere [MAX_HUNTERS];
*/
typedef struct _room room;

/** HUNTER OBJ
    char name [MAX_STR];
    room* currIn;
    EvidenceClassType canCollect;
    EvidList colEvid;
    int fear;
    int boredom;
*/
typedef struct _hunt hunter;

/** GHOST OBJ
    int locked;
    struct _build* building;
    GhostClassType gType;
    struct _room* currRoom;
    int boredom;
    int abil [3];
*/
typedef struct _ghost ghost;

/** ROOM NODE
    struct _rNode * next;
    room* room;
*/
typedef struct _rNode RoomNode;

/** ROOM L-LIST
    RoomNode* head;
    int count;
*/
typedef struct _rlist RoomList;

/** EVID NODE
    struct _eNode* next;
    evidence* evidence;
*/
typedef struct _eNode EvidNode;

/** EVID L-LIST
    EvidNode* head;
    int count;
*/
typedef struct _elist EvidList;

/** BUILDING OBJ
    hunter hunters [MAX_HUNTERS];
    RoomList rooms;
*/
typedef struct _build BuildingType;


struct _elist { 
    sem_t mutex;
    EvidNode* head;
    int count;
};

struct _rlist { 
    RoomNode* head;
    int count;
};

struct _ghost {
    int locked;
    struct _build* building;
    GhostClassType gType;
    struct _room* currRoom;
    int boredom;
    int abil [3];
};

struct _evid { 
    EvidenceClassType eType;
    float value;
    int ghostly;
};

struct _room { 
    sem_t roommutex;
    struct _ghost* hasGhost;
    char name [MAX_STR];
    RoomList adjacent;
    EvidList evidHere;
    hunter* huntersHere [MAX_HUNTERS];
};

struct _hunt {
    struct _build* building;
    int id;
    char name [MAX_STR];
    room* currIn;
    EvidenceClassType canCollect;
    EvidList colEvid;
    int fear;
    int boredom;
    int prog;
    int conc;
};

struct _build {
    hunter hunters [MAX_HUNTERS];
    RoomList rooms;
};

struct _rNode { 
  struct _rNode* next;
  room* room;
};

struct _eNode { 
    struct _eNode* next;
    evidence* evidence;
    float* av;
};

// INIT //

void initBuilding(BuildingType*);
void initRoom(room*, char*);
void initHunter(BuildingType*, hunter*, room*, int);
void initGhost(ghost*, BuildingType* );

void initRoomList(RoomList*);
void initEvidList(EvidList*);

// CLEAN // 

void cleanEvidList(EvidList* );
void cleanRoomList(RoomList* );
void sanitize(BuildingType* );

// OPS //

void appendRoomNode( RoomList*, RoomNode* );
void appendEvidNode( EvidList*, EvidNode* );
void connectRooms(room* , room* );

// Ghost // 

void placeGhost(ghost* , RoomList* , int);
void createEvid(room* , ghost* );
void takeActionGhost(ghost* ); 
int hasHunters(room* );
void* ghostThread(void* );

// Hunter // 

void createMundEvid(hunter* );
void moveHunter(hunter* );
int collectEvid(hunter* );
void communicate(hunter* , hunter* );
void takeActionHunter(hunter* );
int hasOthers(room* , hunter* , int[] ); 
int checkMyEvid(hunter* );
void* hunterThread(void* );
void leaveNow(hunter* ,int );
void solve(hunter* );

// Print // 

void printRoomList(RoomList* );
void printEvidList(EvidList* );
void printHunter(hunter* );
void printGhost(ghost* );
void printEvid(evidence* );
void printResults(BuildingType* ); 

// PROVIDED //

int randInt(int, int);          // Generates a pseudorandom integer between the parameters
float randFloat(float, float);  // Generates a pseudorandom float between the parameters
void populateRooms(BuildingType*, ghost*);   // Populates the building with sample data for rooms

// FORLOOP NODES
/**
    for (NodeType *currNode = list->head;
     currNode != NULL; 
     currNode = currNode->next) { ... }
*/
