#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "track.h"
#include "trackpoint.h"
#include "location.h"




void map_destroy(int **map, int rows, int cols){
	// *map = malloc((*rows) * sizeof(int *));


	for (int i = 0 ; i < (rows); i++){
		free(map[i]);

	}
	free(map);

	// free(map);
	// free(rows);
	// free(cols);
}


int main(int argc, char const *argv[])
{


	if (argc != 5){
		fprintf(stderr, "Heatmap: inadequate number of inputs \n");
		exit(0);
	}

	double width = atof(argv[1]);
	double height = atof(argv[2]);

	// UNUSED FOR NOW
	const char *characters = argv[3];
	int ranges = atoi(argv[4]);

	double lat;
	double lon;
	long time;
	int index;

	track *t = track_create();

	// while (scanf("%lf %lf %ld", &lat, &lon, &time) != EOF){
	
	trackpoint *trk;
	char *buffer = NULL;
	size_t size = 0;
	// int ch;

	// while ((ch = getchar()) != EOF){

	// 	if(ch == '\n'){
	// 		track_start_segment(t);
	// 	}
	// 	else{
	// 		 ungetc(ch,stdin);
	// 	}



	// while (fgets(line, sizeof(line), stdin) != NULL){

	while( getline(&buffer, &size, stdin) != -1){
		if (size > 0 && buffer[0] == '\n'){
			track_start_segment(t);
		}


		else if( sscanf(buffer, "%lf %lf %ld", &lat, &lon, &time) == 3){
			if(!(lat >= -90.0 && lat <= 90.0 && lon >= -180.0 && lon < 180.0)){
				fprintf(stderr, "improper input\n");
				exit(3);
			}

			trk = trackpoint_create(lat,lon,time);
			if (!track_add_point(t,trk)){
				fprintf(stderr, "couldn't add trackpoint\n");
				trackpoint_destroy(trk);
				exit(2);
			}
			else {
				trackpoint_destroy(trk);

				// free(trk);
			}



			// track_add_point(trk,trackpoint_create(lat,lon,time));

			// else{
			// printf("lat %lf ", trackpoint_location(trackpoint_create(lat,lon,time)).lat);
			// printf("lon %lf ", trackpoint_location(trackpoint_create(lat,lon,time)).lon);
			// printf("time %ld ", trackpoint_time(trackpoint_create(lat,lon,time)));
			// printf("\n");

			// }
		}

		else{
			fprintf(stderr, "read error\n");
			exit(1);
		}

	}
	free(buffer);






	







	// for (int i = 0; i < track_count_segments(t); i++){
	// 	printf("Segment #%d has %d points.\n", track_count_segments(t) - i - 1, track_count_points(t, track_count_segments(t) - i - 1));
	// }

	// int ***map = malloc(sizeof(int **));

	// int *rows = malloc(sizeof(int));
	// int *cols = malloc(sizeof(int));

	int **map;
	int rows;
	int cols;
	int len = strlen(characters);

	track_heatmap(t, width, height, &map, &rows, &cols);
	// printf("rows %d cols %d", rows, cols);
	// printf("range %d \n", ranges);
	// printf("1 %c, 2, %c3, %c 4%c", characters[0], characters[1], characters[2], characters[3] );

	for (int i = 0; i< rows; i++){
		for (int j =0; j< cols; j++){
			// printf("%d ", map[i][j]);
			index = (int)floor(map[i][j]/ranges) ;
			// printf("index %d", index);
			if (index < len  ){

				printf("%c", characters[index ]);
			}
			else{
				printf("%c", characters[len -1]);
			}

		}
		printf("\n");
	}

	map_destroy(map, rows, cols); 
	// free(t->t);

	track_destroy(t);




}

