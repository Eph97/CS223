#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include "location.h"
#include "lugraph.h"
#define _GNU_SOURCE


 // used to read in values
typedef struct info{
  char names[4];
  location locs;
} Info;
// used to store all city and distance combinations
typedef struct distances{
  int to;
  int from;
  double dist;
} Distances;

void merge(int n1, const Distances a1[], int n2, const Distances a2[], Distances out[]);
void mergeSort(int n, const Distances a[], Distances out[]);
void greedy(Distances *sorted, int num, Info *cities);


// char seen[100][4];
double finalDist;



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
    fclose(fp);
    fprintf(stderr, "TSP: too few cities\n");
    return 1;
  }


  int arguments[argc];
  for (int i = 0; i < argc; i++){
    arguments[i] = 0;
  }

  // checks which functions to call
  for (int i = 2; i < argc; i++){
    if (strcmp(argv[i], "-greedy") == 0){
      arguments[i-2] = 5;
    }

    else if (strcmp(argv[i], "-nearest") == 0){
      arguments[i-2] = 1;
    }

    else if(strcmp(argv[i], "-insert") == 0){
      if (i == argc - 1){
        fclose(fp);
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
        fclose(fp);
        fprintf (stderr, "TSP: invalid criterion %s\n", argv[i +1]);
        return 1;
      }
    }

    else if (strcmp(argv[i], "-optimal") == 0){
      arguments[i-2] = 4;
    }
    else{
      fclose(fp);
      fprintf (stderr, "TSP: invalid method %s\n", argv[i]);
      return 1;
    }

  }
  Distances *dist = malloc(sizeof(Distances)*(num*(num-1))/2);
  Info *cities = malloc(sizeof(Info)*num);
  for (int i = 0; i< num; i++){


    if (fscanf(fp, "%3s", cities[i].names) != 1){
      // printf("%s ", cities[i].names);
          free(dist);
          free(cities);
          fclose(fp);
          fprintf (stderr, "TSP: Invalid cities\n");
          return 1;

    }
  }

  for (int i = 0; i< num; i++){
    if (fscanf(fp, "%lf %lf", &cities[i].locs.lat, &cities[i].locs.lon) != 2){
      free(dist);
      free(cities);
      fclose(fp);
      fprintf (stderr, "TSP: invalid number of cities given\n");
      return 1;
    }
  }
    fclose(fp);  /* close the file prior to exiting the routine */

  int count = 0;
  for (int i = 0; i< num; i++){
    for (int j = i +1; j< num; j++){

      dist[count].from = i;
      dist[count].to = j;
      dist[count].dist = location_distance(&cities[i].locs, &cities[j].locs);
       
       count++;
   }
 }


 Distances *sorted = malloc(sizeof(Distances)*count);

 mergeSort(count, dist, sorted);
 //see which functions to call
 for (int i = 0; i<argc-2;i++){
    // printf("%d",arguments[i]);
  // clear(num*num, &seen[0]);
    // printf("test %d\n",arguments[i-2]);
  if (arguments[i] == 5){
      // printf("reached");
    printf("%-16s:", "-greedy");
    greedy(sorted, num, cities);

    printf("\n");
  }

  else if (arguments[i] == 1){
      // printf("reached");
    printf("%-16s:", "-nearest");
    // ListNearest(&dist[0], num, &seen[0]);

    printf("\n");
  }
  else if (arguments[i] ==2){
    printf("%-16s:", "-insert nearest");
    // insert(&dist[0], num, 1, &seen[0]);
    printf("\n");
  }
  else if (arguments[i] ==3){
    printf("%-16s:", "-insert farthest");
    // insert(&dist[0], num, 2, &seen[0]);

    printf("\n");
  }
  else if (arguments[i] ==4){
    printf("%-16s:", "-optimal");
    // optimal(&dist[0], num, &seen[0]);
    printf("\n");

  }
}

  free(dist);
  free(cities);
  free(sorted);

}
// The partial tour is represented by a graph, which initially has no edges in it. 
// Adding a segment is adding an edge to the graph. 
// Checking if two cities are already connected is checking if there is already a path between 
// them in the graph. Checking how many pairs each city has 
// already been included in is checking the degree of the corresponding vertices. 
// Finally, to construct the final tour, you can find a path between the two vertices with degree 1.
void greedy(Distances *sorted, int num, Info *cities){
  double dist = 0;
  lugraph *g = lugraph_create(num);
  for (int i = 0; i < (num*(num -1)/2); i++){

    if (lugraph_connected(g, sorted[i].from, sorted[i].to) == false
      && (lugraph_degree(g, sorted[i].from) <= 1) && (lugraph_degree(g, sorted[i].to) <= 1)){
      // printf("===== %s %s %f ===== \n", cities[sorted[i].from].names, cities[sorted[i].to].names, sorted[i].dist);
      lugraph_add_edge(g, sorted[i].from, sorted[i].to);
      dist += sorted[i].dist;
    }
  }
  int to = -1;
  int from = -1;

  for (int i = 0; i < num; i++){
    if ((lugraph_degree(g, i) == 1) && from == -1){
      from = i;
    }
    if((lugraph_degree(g, i) == 1) && from != -1){
      to = i;
    }
  }
  dist += location_distance(&cities[from].locs, &cities[to].locs);
  lug_search *s = lugraph_bfs(g, from);
  int d;
  int *path = lug_search_path(s, to, &d);

  int start = 0;

  for (int i = 0; i < num; i++){
    if(path[i] == 0){
      start = i;
      break;
    }
  }

  printf("%10.2lf", dist);
  // printf(" %d\n", path[(start) % num +1]);
  // printf(" %s\n", cities[path[(start) % num +1]].names);
  if(path[(1 + start) % num ] < path[(num  - 1 + start) % num ]){

    for (int i = 0; i <=num; i++){
      printf(" %s",cities[path[(i + start) % num]].names);
    }
  }
  else{
    for (int i = num; i >=0; i--){
      printf(" %s", cities[path[(i + start) % num]].names);
    }
  }
  lugraph_destroy(g);
  free(path);
  lug_search_destroy(s);
}

/* merge sorted arrays a1 and a2, putting result in out */
void merge(int n1, const Distances a1[], int n2, const Distances a2[], Distances out[])
{
  int i1;
  int i2;
  int iout;

  i1 = i2 = iout = 0;

  while(i1 < n1 || i2 < n2) {

          // (a1[i1].x < a2[i2].x)
          if(i2 >= n2 || ((i1 < n1) && a1[i1].dist < a2[i2].dist)) {
                  /* a1[i1] exists and is smaller */
                  out[iout++] = a1[i1++];
          }  else {
                  /* a1[i1] doesn't exist, or is bigger than a2[i2] */
                  out[iout++] = a2[i2++];
          }
  }

}

// /* sort a, putting result in out */
// /* we call this mergeSort to avoid conflict with mergesort in libc */
void mergeSort(int n, const Distances a[], Distances out[])
{
  Distances *a1;
  Distances *a2;

  if(n < 2) {
    /* 0 or 1 elements is already sorted */
    memcpy(out, a, sizeof(Distances) * n);
  }
  else {
    /* sort into temp arrays */
    a1 = malloc(sizeof(Distances) * (n/2));
    a2 = malloc(sizeof(Distances) * (n - n/2));

    mergeSort(n/2, a, a1);
    mergeSort(n - n/2, a + n/2, a2);

    /* merge results */

    merge(n/2, a1, n - n/2, a2, out);

    /* free the temp arrays */
    free(a1);
    free(a2);
  }
}

