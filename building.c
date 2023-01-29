/**
2401 Final Project
Moataz Dabbour
100863820
*/

#include "defs.h"

/*
    Note: While this function is provided, you will likely want
    to modify it to suit your solution. This has been provided
    to give you a starting point to lay out the room connectivity,
    but it is entirely possible that you will want to use different
    data structures to represent the rooms and their connectivity,
    or different function names.

    Not following this function's structure exactly will not result
    in a deduction of points.

    The map that this is recreating is visible in the assignment
    specification, and also available by the original creator here:
    https://steamcommunity.com/sharedfiles/filedetails/?id=2251267947
    
*/


/**
Function: populateRooms
Purpose:  Setup the workspace with hunters, introduces hunters, counts down, and init's them, 
          init's rooms and room's adjacent arrays, 
In/Out: ghost* and building* 
*/
void populateRooms(BuildingType* building, ghost* ghost) {

    // VAR ARRIVES 
    room* van = calloc(1, sizeof(room));
    initRoom(van, "Van");
    
    // INTRO OUR TEAM
    printf("INTRODUCING OUR HUNTERS!\n");
    for (int i = 0; i < MAX_HUNTERS ; i++)
    {
        initHunter(building, building->hunters + i, van, i);
        printHunter(building->hunters + i);
        printf("\n");
    }
    
    // COUNTDOWN
    printf("Starting in:\n");
    for (int i = 3; i > 0; i--)
    {
        printf("%d\n", i );
        sleep(1);
    }
    

    /** BUILDING MATERIALIZES MALLOC: */
    
    room* hallway = calloc(1, sizeof(room));
    initRoom(hallway, "Hallway");
    room* master_bedroom = calloc(1, sizeof(room));
    initRoom(master_bedroom, "Master Bedroom");
    room* boys_bedroom = calloc(1, sizeof(room));
    initRoom(boys_bedroom, "Boy's Bedroom");
    room* bathroom = calloc(1, sizeof(room));
    initRoom(bathroom, "Bathroom");
    room* basement = calloc(1, sizeof(room));
    initRoom(basement, "Basement");
    room* basement_hallway = calloc(1, sizeof(room));
    initRoom(basement_hallway, "Basement Hallway");
    room* right_storage_room = calloc(1, sizeof(room));
    initRoom(right_storage_room, "Right Storage Room");
    room* left_storage_room = calloc(1, sizeof(room));
    initRoom(left_storage_room, "Left Storage Room");
    room* kitchen = calloc(1, sizeof(room));
    initRoom(kitchen, "Kitchen");
    room* living_room = calloc(1, sizeof(room));
    initRoom(living_room, "Living Room");
    room* garage = calloc(1, sizeof(room));
    initRoom(garage, "Garage");
    room* utility_room = calloc(1, sizeof(room));
    initRoom(utility_room, "Utility Room");

    // Now create a linked list of rooms using RoomNodeType in the Building
    RoomNode* van_node = calloc(1, sizeof(RoomNode));
    van_node->room = van;
    RoomNode* hallway_node = calloc(1, sizeof(RoomNode));
    hallway_node->room = hallway;
    RoomNode* master_bedroom_node = calloc(1, sizeof(RoomNode));
    master_bedroom_node->room = master_bedroom;
    RoomNode* boys_bedroom_node = calloc(1, sizeof(RoomNode));
    boys_bedroom_node->room = boys_bedroom;
    RoomNode* bathroom_node = calloc(1, sizeof(RoomNode));
    bathroom_node->room = bathroom;
    RoomNode* basement_node = calloc(1, sizeof(RoomNode));
    basement_node->room = basement;
    RoomNode* basement_hallway_node = calloc(1, sizeof(RoomNode));
    basement_hallway_node->room = basement_hallway;
    RoomNode* right_storage_room_node = calloc(1, sizeof(RoomNode));
    right_storage_room_node->room = right_storage_room;
    RoomNode* left_storage_room_node = calloc(1, sizeof(RoomNode));
    left_storage_room_node->room = left_storage_room;
    RoomNode* kitchen_node = calloc(1, sizeof(RoomNode));
    kitchen_node->room = kitchen;
    RoomNode* living_room_node = calloc(1, sizeof(RoomNode));
    living_room_node->room = living_room;
    RoomNode* garage_node = calloc(1, sizeof(RoomNode));
    garage_node->room = garage;
    RoomNode* utility_room_node = calloc(1, sizeof(RoomNode));
    utility_room_node->room = utility_room;

    // Building->rooms might be a linked list structre, or maybe just a node.
    initRoomList(&building->rooms);

    appendRoomNode(&building->rooms, hallway_node);
    appendRoomNode(&building->rooms, master_bedroom_node);
    appendRoomNode(&building->rooms, boys_bedroom_node);
    appendRoomNode(&building->rooms, bathroom_node);
    appendRoomNode(&building->rooms, basement_node);
    appendRoomNode(&building->rooms, basement_hallway_node);
    appendRoomNode(&building->rooms, right_storage_room_node);
    appendRoomNode(&building->rooms, left_storage_room_node);
    appendRoomNode(&building->rooms, kitchen_node);
    appendRoomNode(&building->rooms, living_room_node);
    appendRoomNode(&building->rooms, garage_node);
    appendRoomNode(&building->rooms, utility_room_node);


    // GHOST ENTERS
    initGhost(ghost, building);
    placeGhost(ghost, &building->rooms, 1);
    
    appendRoomNode(&building->rooms, van_node);


    // Now connect the rooms. It is possible you do not need a separate
    // function for this, but it is provided to give you a starting point.
    connectRooms(van, hallway);
    connectRooms(hallway, master_bedroom);
    connectRooms(hallway, boys_bedroom);
    connectRooms(hallway, bathroom);
    connectRooms(hallway, kitchen);
    connectRooms(hallway, basement);
    connectRooms(basement, basement_hallway);
    connectRooms(basement_hallway, right_storage_room);
    connectRooms(basement_hallway, left_storage_room);
    connectRooms(kitchen, living_room);
    connectRooms(kitchen, garage);
    connectRooms(garage, utility_room);
}