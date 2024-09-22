#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/graph.h"
#include "../include/Astar.h"
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define BOOSTS_AT_START 5


int main()
{
  int row;
  int width, height;
  int gasLevel;
  int boosts = BOOSTS_AT_START;
  int round = 0;
  Vector2D myPosition, secondPosition, thirdPosition;
  Vector2D myPreviousPosition;
  Vector2D speed = {0, 0};
  Vector2D acceleration;
  char action[100];
  char line_buffer[MAX_LINE_LENGTH];

  clock_t start_t, end_t;

  boosts = boosts;                     /* Prevent warning "unused variable" */

  int flag = 0;
  GraphNode*  start= NULL;
  GraphNode* end = NULL;
  DestinationList destinationlist;
  GraphList* path = NULL;
  int vitesse;

  if (round == 0) {
    boosts = BOOSTS_AT_START;
  }
  
  
  fgets(line_buffer, MAX_LINE_LENGTH, stdin);      /* Read gas level at Start */
  sscanf(line_buffer, "%d %d %d", &width, &height, &gasLevel);


  fprintf(stderr, "=== >Map< ===\n");
  fprintf(stderr, "Size %d x %d\n", width, height);
  fprintf(stderr, "Gas at start %d \n\n", gasLevel);
  int maxgas = gasLevel;
  TerrainMap circuit = initializeTerrainMap(width, height);

for (row = 0; row < height; ++row) {
    fgets(line_buffer, MAX_LINE_LENGTH, stdin);
    strncpy(circuit.grid[row], line_buffer, width);
    }
    displayTerrainMap(circuit);

fflush(stderr);
fprintf(stderr,"\n=== Race start ===\n");

  myPreviousPosition.x = -1;
  myPreviousPosition.y = -1;

  while (!feof(stdin)) {
    start_t = clock();
    round++;
    flag = 0;

    fprintf(stderr,"=== ROUND %d\n", round);
    fflush(stderr);
    fgets(line_buffer, MAX_LINE_LENGTH, stdin);   /* Read positions of pilots */
    sscanf(line_buffer, "%d %d %d %d %d %d", &myPosition.x, &myPosition.y, &secondPosition.x, &secondPosition.y, &thirdPosition.x, &thirdPosition.y);
    fprintf(stderr,"My position: %d %d\n", myPosition.x, myPosition.y);
    if (myPosition.x == myPreviousPosition.x && myPosition.y == myPreviousPosition.y) {
      speed.x = 0;
      speed.y = 0;
    }
    myPreviousPosition.x=myPosition.x;
    myPreviousPosition.y=myPosition.y;
    if (round == 1) {
      Vector2D speed = {0, 0};
      start = createGraphNode(myPosition, speed, maxgas, 0, NULL);
      destinationlist = generateDestinationList(circuit, start); 
      
    }
    vitesse=25;


    if (round > 1) {
      start->coordinates.x=myPosition.x;
      start->coordinates.y=myPosition.y;
      start->fuel=gasLevel;
      start->turbo=boosts;
    }

    if(circuit.grid[myPosition.y][myPosition.x]=='~' && Norme(speed)>1){
      speed.x=0;
      speed.y=0;
    }

    end=selectOptimalDestination(destinationlist,secondPosition,thirdPosition);

    path=run_a_star(start,end,circuit,secondPosition,thirdPosition,gasLevel,speed,vitesse,flag,round,path);

    path=find_path(destinationlist,start,circuit,myPosition,secondPosition,thirdPosition,gasLevel,speed,flag,round,path,end);


    
  

    if (path==NULL){
      fprintf(stderr,"No path found\n");
      fflush(stderr);

      
    }
    else{
      //fprintf(stderr,"Path found\n");
      //DisplayPath(path);
      fflush(stderr);
    

      GraphNode* firstNode=path->head->next;
      Vector2D firstNodePos=firstNode->coordinates;
      Vector2D startNodePos=start->coordinates;

      if ((firstNodePos.x==secondPosition.x && firstNodePos.y==secondPosition.y) || (firstNodePos.x==thirdPosition.x && firstNodePos.y==thirdPosition.y) || collisionDetection(startNodePos,firstNodePos,circuit,secondPosition,thirdPosition)==0){
        flag=1;
        path=run_a_star(start,end,circuit,secondPosition,thirdPosition,gasLevel,speed,vitesse,flag,round,path);
      }
      acceleration=getAcceleration(path,myPosition,speed,circuit);
      fprintf(stderr,"Acceleration : %d %d\n",acceleration.x,acceleration.y);
      if (abs(acceleration.x) > 1 || abs(acceleration.y) > 1) {
			  boosts--;
		}
    }
    fflush(stderr);

    if (path==NULL) {
      acceleration.x=0;
      acceleration.y=0;
      speed.x=0;
      speed.y=0;
    }
    /* Gas consumption cannot be accurate here. */
    gasLevel += gasConsumption(acceleration, speed, circuit.grid[myPosition.y][myPosition.x] == '~');
    
    speed.x += acceleration.x;
    speed.y += acceleration.y;
    /* Write the acceleration request to the race manager (stdout). */
    sprintf(action, "%d %d", acceleration.x, acceleration.y);
    fprintf(stdout, "%s", action);
    fflush(stdout);                           /* CAUTION : This is necessary  */
    fprintf(stderr, "    Action: %s Speed: %f  Gas remaining: %d\n", action, Norme(speed),gasLevel);
    fflush(stderr);
    end_t = clock();

    fprintf(stderr, "Round %d: %f seconds\n", round, (double)(end_t - start_t) / CLOCKS_PER_SEC);
    fflush(stderr);
    if (0 && round > 4) { /* (DISABLED) Force a segfault for testing purpose */
      int * p = NULL;
      fprintf(stderr, "Good Bye!\n");
      fflush(stderr);
      *p = 0;
    }
  
}
freeTerrainMap(circuit);
freeGraphNode(start);
freeDestinationList(destinationlist);
freeGraphList(path);
freeGraphNode(end);



  return EXIT_SUCCESS;
}