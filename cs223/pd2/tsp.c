#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include "location.h"

// #define MAXLINE 80
// #define MAXRECORDS 10

// TSP: missing filename when there are no command-line arguments;
// TSP: could not open FILENAME when there was an error opening the file whose name is given as the first command-line argument (replace FILENAME with the name of the file);
// TSP: too few cities when the number of cities given on the first line of the file is not at least two;
// TSP: invalid method METHOD when an argument after the first does not immediately follow -insert and is not one of -nearest, -insert, or -optimal (replace METHOD with the offending argument);
// TSP: invalid criterion CRITERION when -insert is followed by an argument that is neither nearest nor farthest (replace CRITERION with the offending argument); and
// TSP: missing criterion when -insert is the last argument.

//input is a file containing cities and their locations

//1) on the first line, the number of cities in a format readable by atoi;
//2) on the second line, a list of three-character labels separated by single spaces
//3) one line per city containing the latitude and longitude of the city in a format readable by atof and separated by a single space.

//https://users.cs.cf.ac.uk/C.L.Mumford/howard/FarthestInsertion.html
//insert farthest


//Length of a city plus one character 
const int N = 4;
// char seen[][];

typedef struct info{
  char names[4];
  location locs;
} Info;

typedef struct distances{
  char from[4];
  char to[4];
  double dist;
  int index;
} Distances;

typedef struct sortable{
  char names[4];
} Sortable;

// char seen[100][4];
double finalDist;
int reorder(Distances *data, int num, char seen[][N]);
void clear(int num, char seen[][N]);
int nearest(Distances *data, int num, int city, char seen[][N]);
int find(Distances *data, int num, char *city);
void ListNearest(Distances *data, int num, char seen[][N]);
int isElement(char ele[], int num, char seen[][N]);
int set(Distances *data, int city, int num, char seen[][N]);
// void ListFurthest(Distances *data, int num, char seen[][4]);
int farthest(Distances *data, int num, int city, char seen[][N]);
// void insert(Distances *data, int num, int (*NF)(Distances *, int, int));
void insert(Distances *data, int num, int type, char seen[][N]);
// void sort(char sort[], char pair[], int num);
int findLoc(char city[], int num);
// int CheckDistance(int num, char inserting[]);
void sort(char sort[], int num, int insertpos, char seen[][N]);
double CheckDistance(Distances *data, int numseen, int num, char seen[][N]);
double finDist(Distances *data, char city1[], char city2[], int num);
void unsort(char sort[], int num, int remove, int insertpos, char seen[][N]);
// int direction(Distances *data, int num, int first);
void optimal(Distances *data, int num, char seen[][N]);
int main(int argc, char *argv[]){
  FILE *fp = NULL;
      /* Check for cmd arguments */
  if (argc < 2) {
    fprintf (stderr, "TSP: missing filename\n");
    return 1;
  }
      /* Open file */
      fp = fopen(argv[1], "r"); /* open the file for reading */
  if (!fp) {
    fprintf (stderr, "TSP: could not open %s\n", argv[1]);
    return 1;
  }

      /* initialize hella stuff */
  char line[80];
  int num = atoi(fgets(line, 80, fp));
  if (num < 2) {
    fprintf(stderr, "TSP: too few cities\n");
    return 1;
  }


  int arguments[argc];
  for (int i = 0; i < argc; i++){
    arguments[i] = 0;
  }
  // char seen[num + 100][4];
  // memset(seen, '\0', (num+1)*sizeof(char));
  // clear(100, &seen[0]);
  char seen[num*num][N];


      // char test[30];
  for (int i = 2; i < argc; i++){
    // printf("reached");

    if (strcmp(argv[i], "-nearest") == 0){
      // printf("hit %d\n",i);
      arguments[i-2] = 1;
      // printf("0");
    }

    else if(strcmp(argv[i], "-insert") == 0){
      if (i == argc - 1){
        fprintf(stderr, "TSP: missing criterion\n");
        return 1;
      }
      else if (strcmp(argv[i + 1], "nearest") == 0){
        arguments[i-2] = 2;
        i++;
      }
      else if (strcmp(argv[i + 1], "farthest") == 0){
        arguments[i-2] = 3;
        i++;
      }
      else {
        fprintf (stderr, "TSP: invalid criterion %s\n", argv[i +1]);
        return 1;
      }
    }

    else if (strcmp(argv[i], "-optimal") == 0){
      arguments[i-2] = 4;
    }
    else{
      fprintf (stderr, "TSP: invalid method %s\n", argv[i]);
      return 1;
    }

  }
  Distances dist[num*num];
  Info cities[num];

  for (int i = 0; i< num; i++){
    fscanf(fp, "%s", cities[i].names);
    // for (int j = 0; j< 4; j++){
    //   seen[i][j] = '\0';
    // } 
  }

  for (int i = 0; i< num; i++){
    fscanf(fp, "%lf %lf", &cities[i].locs.lat, &cities[i].locs.lon);
  }
    fclose(fp);  /* close the file prior to exiting the routine */

  //GenerateDistances(&cities);
  int count = 0;
  for (int i = 0; i< num; i++){
    for (int j = 0; j< num; j++){
    // printf("count %d ", count);
      if(i != j){
       strcpy(dist[count].from, cities[i].names);
       strcpy(dist[count].to, cities[j].names);
       dist[count].dist = location_distance(&cities[i].locs, &cities[j].locs);
       
       // printf("from: %s to: %s dist: %f \n", dist[count].from, dist[count].to, dist[count].dist);
       // printf("%s, %s, %f \n", dist[count].from, dist[count].to, dist[count].dist);
       count++;
     }
   }
 }
 for (int i = 0; i<argc-2;i++){
    // printf("%d",arguments[i]);
  clear(num*num, &seen[0]);
    // printf("test %d\n",arguments[i-2]);
  if (arguments[i] == 1){
      // printf("reached");
    printf("%-16s:", "-nearest");
    ListNearest(&dist[0], num, &seen[0]);
    printf("\n");
  }
  else if (arguments[i] ==2){
    printf("%-16s:", "-insert nearest");
    insert(&dist[0], num, 1, &seen[0]);
    printf("\n");
  }
  else if (arguments[i] ==3){
    printf("%-16s:", "-insert farthest");
    insert(&dist[0], num, 2, &seen[0]);

    printf("\n");
  }
  else if (arguments[i] ==4){
    printf("%-16s:", "-optimal");
    optimal(&dist[0], num, &seen[0]);
    printf("\n");



  }

}
// for (int i = 0; i<argc+1;i++){
//     printf("test %d",arguments[i]);
//     arguments[i] = -1;  }
  // ListNearest(&dist[0], num);
  // finalDist= insert(&dist[0], num, farthest);
  // ListFurthest(&dist[0], num);
  // printf("%.2lf ", finalDist);
  // for (int i = 0; i <=  num; i++){
  //   printf("%s ", seen[i]);
  // }
  // printf("num = %d" , num);
  // insert(&dist[0], num, 1);

  // printf("\n");
  // for (int  l = 0; l < num* (num - 1); l++){
    // printf("from: %s to: %s dist: %f location: %d\n", dist[l].from, dist[l].to, dist[l].dist, l);
  // }
}
void ListNearest(Distances *data, int num, char seen[][N]){
  double dist = 0;
  int next = nearest(&data[0], num, 0, &seen[0]);
  dist = data[next].dist;
  strcpy(seen[0], data[next].from);
 // printf("%d", counter);
  int temp;
  for (int i = 1; i < num; i++){
    temp = find(&data[0], num, data[next].to);
    next = nearest(&data[0], num, temp, &seen[0]); 
    dist += data[next].dist;
    strcpy(seen[i], data[next].from);
  }
  strcpy(seen[num], data[next].to);
  // -nearest\t:\t
  printf("%10.2lf", dist);
  for (int i = 0; i <=  num; i++){
    printf(" %s", seen[i]);
  }
}

// void ListFurthest(Distances *data, int num, char seen[][4]){
//   int next = farthest(&data[0], num, 0, &seen[0]);
//   strcpy(seen[0], data[next].from);
//   int temp;
//   for (int i = 1; i < num; i++){
//     temp = find(&data[0], num, data[next].to);
//     next = farthest(&data[0], num, temp, &seen[0]); 
//     strcpy(seen[i], data[next].from);
//   }
// }

int nearest(Distances *data, int num, int city, char seen[][N]){
  int location;
  double minimum;
  location = set(&data[0], city, num, &seen[0]);
      // printf("yell %d\n", location);
  minimum = data[location].dist;
      // printf("min %lf\n", minimum);
  for (int i = city; i < city + num -1; i++){
    if (data[i].dist < minimum)
    {
      if (isElement(data[i].to,num, &seen[0]) != 0){
       minimum = data[i].dist;
       location = i;
           // printf("yell %d\n", city);
     }
   }
 }
 // strcpy(seen[count+1], data[location].to);
   // printf("from: %s to: %s dist: %f location: %d\n", data[location].from, data[location].to, minimum, location);
 return(location);
}
int set(Distances *data, int city, int num, char seen[][N]){
  for (int i = 0; i < num ; i++){
    if (isElement(data[city + i].to, num, &seen[0]) != 0){
      return city + i;
    }
  }
  return city;
}

int farthest(Distances *data, int num, int city, char seen[][N]){
  int location;
  double maximum;
  location = set(&data[0], city, num, &seen[0]);
  maximum = data[location].dist;
  for (int i = city; i < city + num -1; i++){
    if (data[i].dist > maximum)
    {
      if (isElement(data[i].to,num, &seen[0]) != 0){
       maximum = data[i].dist;
       location = i;
     }
   }
 }
 // strcpy(seen[counter], data[location].from);
 // strcpy(seen[counter+1], data[location].to);
   // printf("from: %s to: %s dist: %f location: %d\n", data[location].from, data[location].to, maximum, location);
 return(location);
}
//searches entire list for from city location
int find(Distances *data, int num, char city[]){
  for (int i =0; i<num; i++){
    if (strcmp(data[i*(num-1)].from, city) == 0){
      return i*(num-1);
    }
  }
  return 0;
    // wtfffff how to reconcile.
}



int isElement(char ele[], int num, char seen[][N]){
  for (int i = 0; i <=num; i++){
    if (strcmp(seen[i], ele) == 0){
      return 0;
    }
  }
  return 1;
}

void insert(Distances *data, int num, int type, char seen[][N]){
  int first;
  // char example[21];
  Distances candidates[num];
  Distances checkDist[num];
  char needToSort[4];
  double bestDist;
  int bestCand;
  int bestLoc = 1;
  int location = 0;
  if (type == 1){
    location = nearest(&data[0], num*(num-1), 0, &seen[0]);
    // printf("location: %d", location);
  }
  else if (type == 2){
    location = farthest(&data[0], num*(num-1), 0, &seen[0]);
  }
  strcpy(seen[0], data[location].from);
  strcpy(seen[1], data[location].to);
  strcpy(seen[2], data[location].from);
  for (int j = 3; j<=num; j++){
    // for (int i = 0; i<j ; i++){
    //   // printf("cands %d\n", find(&data[0], num, seen[i]));
    // }
    for (int i = 0; i<j ; i++){
      if (type == 1){
        candidates[i].index = nearest(&data[0], num, find(&data[0], num, seen[i]), &seen[0]);
      }
      else if (type == 2){
        candidates[i].index = farthest(&data[0], num, find(&data[0], num, seen[i]), &seen[0]);
       
      }

    }
    double shortestDist = data[candidates[0].index].dist;
    for (int i=0; i<j; i++){
      if (type == 1){
        if (data[candidates[i].index].dist <= shortestDist){
          bestCand = candidates[i].index;
          shortestDist = data[candidates[i].index].dist;
        }
      }
      else if (type == 2){
        if (data[candidates[i].index].dist >= shortestDist){
          // printf("cands %s dist: %lf\n", data[candidates[i].index].to, data[candidates[i].index].dist);
          bestCand = candidates[i].index;
          shortestDist = data[candidates[i].index].dist;
        }

      }
    }
    // printf("needToSort %s", data[bestCand].to);
    strcpy(needToSort, data[bestCand].to); 

    for (int i = 1; i<j ; i++){
      sort(needToSort, num, i, &seen[0]);
  //     for (int i = 0; i <=  5; i++){
  //   printf("%s ", seen[i]);
  // }
  // printf("is element %d \n", isElement("BDL",num));
      checkDist[i].dist =  CheckDistance(&data[0], j, num, &seen[0]);

      strcpy(needToSort, data[bestCand].to); 
      unsort(needToSort, num, i, j, &seen[0]);
    }

    bestDist = checkDist[1].dist;
    for (int i = 1; i<j; i++){
    // printf(" TotalDistcheck: %.2lf\n", checkDist[i].dist);
      if (bestDist >= checkDist[i].dist){
      // printf("reached ");
        bestDist = checkDist[i].dist;
        bestLoc = i;

      }

    }
    sort(needToSort, num, bestLoc, &seen[0]);
    bestLoc = 0;

    // printf("TotalDist %lf ", CheckDistance(&data[0], j, num));

    for (int i = 0; i<j ; i++){

      candidates[i].index = 0;
      checkDist[i].dist = 0;
    }
  }
  // if (type == 1){
  //   strcpy(example, "nearest\t");
  // } 
  // else if (type == 2){
  //   strcpy(example, "farthest");
  // }
  // -insert %s:
  printf("%10.2lf", CheckDistance(&data[0], num+1, num, &seen[0]));
  first = reorder(&data[0], num, &seen[0]);
  // printf("%d\n", first);
  // direct = direction(&data[0], num, first);
  // printf("direct %d", direct);
  // find(, int num, char *city)
  // printf("sec %s pent %s",seen[(1 + first) % num], seen[(num - 1 + first) % num]);
  // find(&data[0], num, seen[(1 + first) % num]) < find(&data[0], num, seen[(num + first) % num])
  // here
  if (find(&data[0], num, seen[(1 + first) % num]) < find(&data[0], num, seen[(num -1 + first) % num])){
    for (int i = 0; i <=  num; i++){
      printf(" %s", seen[(i + first) % num]);
    }
  }
  else{
    for (int i = num; i >= 0; i--){
      printf(" %s", seen[(i + first) % num]);
    }
  }
  // for (int i = 0; i <=  num; i++){
  //     printf(" %s", seen[i]);
  //   }
  
  
}

void sort(char sort[], int num, int insertpos, char seen[][N]){
  for(int i = num; i >= insertpos; i--){
    if (strcmp(seen[i],sort ) != 0){
      strcpy(seen[i+1], seen[i]);
    }
    if (insertpos == i){
      strcpy(seen[i], sort);
      break;
    }
  }
}
void unsort(char sort[], int num, int remove, int insertpos, char seen[][N]){
  for(int i = 1; i <= num; i++){
    if (remove < i){
      strcpy(seen[i-1], seen[i]);   

    }
  }
  strcpy(seen[insertpos], sort);

}

double CheckDistance(Distances *data, int numseen, int num, char seen[][N]){
  double TotalDist = 0;
  for (int i = 0; i<numseen; i++){
    TotalDist += finDist(&data[0], seen[i], seen[i+1], num);
  }
  return TotalDist;
}

double finDist(Distances *data, char city1[], char city2[], int num){
  for (int i = 0; i<num*(num-1); i++){
    if ((strcmp(city1, data[i].from) == 0) && (strcmp(city2, data[i].to) == 0)){
      return data[i].dist;
    }
  }
  return 0;
}

void clear(int num, char seen[][N]){
  for (int i = 0; i<num; i++){
    for (int j = 0; j< 4; j++){
      seen[i][j] = '\0';
    } 
  }
}

void optimal(Distances *data, int num, char seen[][N]){
  for (int i = 0 ; i < num; i++){
    strcpy(seen[i], data[i*(num -1)].from);
  }
  strcpy(seen[num], data[0].from);
  // -optimal\t:
  printf("%10.2lf", CheckDistance(&data[0], num+1, num, &seen[0]));
  for (int i = 0; i <=  num; i++){
    printf(" %s", seen[i]);
  }

}
// shuffles around so you start and end from the desired city
int reorder(Distances *data, int num, char seen[][N]){
  // int sortby;
  for (int i = 0; i< num; i++){
    // printf("seen %s compare %s", seen[i], data[0].from);
    if (strcmp(seen[i], data[0].from) == 0){
    // printf("seen %s compare %s", seen[i], data[0].from);

      return i;
    }
  }
  // for (int i = 0; i< num; i++){
  //   printf()
  // }
  return 0;
}



// int direction(Distances *data, int num, int first){
//   char sec[4];
//   char pent[4];
//   strcpy(sec, seen[(1 + first) % num]);
//   strcpy(pent, seen[(num + first) % num]);
//   printf("sec %s pent %s",sec, pent);
//   // for (int i = 0; i <=  num; i++){
//     if (strcmp(sec, seen[(i + first) % num]) ==0){
//       printf("reached");
//       return 0;

//     }
//     if (strcmp(pent, seen[(i + first) % num]) == 0){
//       return 1;
//     }
//   }
//   printf("reached");
//   return 0;
// }
// find

