#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "location.h"
#include "track.h"
#include "trackpoint.h"

#define INITIAL_CAPACITY 10


typedef struct segment{
    double distance;
    int length;
    int segement_capacity;
    trackpoint **trk;

}segment;

struct track
{
    int segCount;
    int totalTrackpoints;
    segment *t;
};
void swap(trackpoint **first, int j) ;  
void bubbleSort(trackpoint **arr, int n) ;

track *track_create(){
    track *trk = malloc(sizeof(track));
    if (trk == NULL){
	return NULL;
    } else {
	trk->t = malloc(sizeof(segment));
	// trk->t[0].trk = malloc(sizeof(trackpoint *));
	trk->t[0].length = 0;
	// trk->t[0].trk[0] = malloc(sizeof(trackpoint));

	trk->totalTrackpoints = 0;
	trk->segCount = 1;
	trk->t[0].distance = 0.0;
	trk->t[0].segement_capacity = 0;

	return trk;
    }



}

void track_destroy(track *trk){
    int n = track_count_segments(trk) ;
    int points;
    int count = 0;
    int flag = 0;
    for (int i = 0; i<n; i++){

	// printf("track_count_points test %d",  track_count_points(trk, i));
	points = track_count_points(trk, i);

	for (int j = 0; j< points; j++){
		if( count < trk->totalTrackpoints){
	   		trackpoint_destroy(trk->t[i].trk[j]);
	    	count++;
		}
		else{
			flag =1;
			break;
		}
	    // free(trk->t[i].trk[j]);
	    // track_count_points(trk, i)
	}

	// if(points >0){
		if(flag == 1){
    		break;
   		}

		if(points >0){

	    free(trk->t[i].trk);
		}

	// }

	// free(trk->t[i].length);


    }
    free(trk->t);
    free(trk);
}


int track_count_segments(const track *trk){
    if (trk != NULL){
	return trk->segCount ;
    }
    else{
	return 0;
    }

}

int track_count_points(const track *trk, int i){
    int segCount = track_count_segments(trk) -1;
    if (i>segCount){
	return 0;
    }
    else{
	return trk->t[i].length ;
    }
}
trackpoint *track_get_point(const track *trk, int i, int j){
    int segCount = track_count_segments(trk) -1;
    int pointCount = track_count_points(trk, i) ;
    if (i> segCount || j> pointCount ){
	return NULL;
    }
    else{
	return trackpoint_copy(trk->t[i].trk[j]);
    }
}
double *track_get_lengths(const track *trk){
    int segs = track_count_segments(trk);
    double *total = malloc(segs*sizeof(double));
    // double *total;
    for (int i = 0; i< segs; i++){
	total[i] = trk->t[i].distance;
    }
    return total;
}
bool track_add_point(track *trk, const trackpoint *pt){

    int seg = track_count_segments(trk) -1;
    // printf("seg, %d", seg);
    int segInd = track_count_points(trk, seg);


    if (segInd == 0){

	if (seg == 0 ){
	    // printf("seg=0");
	    trk->t[seg].trk = malloc(sizeof(trackpoint *) * INITIAL_CAPACITY);
	    if (trk->t[seg].trk == NULL){
		return false;
	    }
	    trk->t[seg].trk[segInd] = trackpoint_copy(pt);
	    // free(pt);
	    // trk->segCount++;
	    trk->totalTrackpoints++;
	    trk->t[seg].length++;
	    trk->t[seg].segement_capacity = INITIAL_CAPACITY;
	    // free(pt);
	    return true;
	}
	// int PrevSegInd = trk->t[seg-1].length;
	else if (seg != 0 &&  (trackpoint_time(pt) > trackpoint_time(trk->t[seg-1].trk[track_count_points(trk, seg-1) -1]))){
	    // printf("seg!=0");
	    trk->t[seg].trk = malloc(sizeof(trackpoint *) * INITIAL_CAPACITY);
	    if (trk->t[seg].trk == NULL){
		return false;
	    }
	    trk->t[seg].trk[segInd] = trackpoint_copy(pt);
	    trk->totalTrackpoints++;
	    trk->t[seg].length++;
	    trk->t[seg].segement_capacity = INITIAL_CAPACITY;
	    // free(pt);
	    return true;
	}

	else {
	    // free(pt);
	    return false;
	}
    }
    // edit this thing. How to know at the end of a segment
    else if (segInd > 0){
	// printf("bad");
	// compares time of inputing trackpoint with last inputed trackpoint
	if (trackpoint_time(pt) > trackpoint_time(trk->t[seg].trk[segInd -1])){

	    if (trk->t[seg].segement_capacity <= trk->t[seg].length){
		trk->t[seg].trk = realloc(trk->t[seg].trk, 2 * trk->t[seg].segement_capacity*sizeof(trackpoint *));
		trk->t[seg].segement_capacity *=2;
	    }


	    trk->t[seg].trk[segInd] = trackpoint_copy(pt);

	    trk->t[seg].length++;
	    trk->totalTrackpoints++;
	    location l1 = trackpoint_location(pt);
	    location l2 = trackpoint_location(trk->t[seg].trk[segInd-1]);
	    trk->t[seg].distance += location_distance(&l1, &l2);
	    // trackpoint_destroy(pt);
	    // free(l2);
	    // free(pt); 
	    return true;

	}
    }
    //if all else fails
    return false;


    // 	int PrevSegInd = trk->t[seg-1].length;

    // // }
    // 		printf("%lu\n", trackpoint_time(trk->t[seg].trk[segInd]));


    }

void track_start_segment(track *trk){
    if (track_count_points(trk, track_count_segments(trk) -1) == 0){
	return;
    }
    trk->segCount++;
    // (*trk)segCount++;
    int seg = trk->segCount;

    // printf("track_start_segment %d", seg);
    // printf("track_start_segment func %d", track_count_segments(trk));

    // trk->t = realloc(trk->t, seg*sizeof(segment));
 //    segment *temp = malloc(seg*sizeof(segment));

 //    if (temp == NULL){
	// return;
 //    }

 //    // copy all the previous samples from the old array to the new one
 //    memcpy(temp, trk->t, (seg-1) *sizeof(segment));

 //    // get rid of the old, too-small array
 //    free(trk->t);

 //    // remember the new one

 //    trk->t = temp;
    trk->t = realloc(trk->t, seg*sizeof(segment));
    // (*trk).segCount++;
    trk->t[seg-1].length = 0;
    trk->t[seg-1].distance = 0.0;

    // printf("track_start_point %d ", track_count_points(trk, seg));


    // int seg = trk->segCount;
    // trk->t[seg] = malloc(sizeof(segment));


}


void track_merge_segments(track *trk, int start, int end){
    int segCount = track_count_segments(trk);
    int length = 0;
    int total = 0;
    int numTrackpoints = 0; 
    // int pointCount = track_count_points(trk, i) -1 ;
    // printf("start %d end %d ", start, end);
    // printf("endlength %d", track_count_points(trk, end));
    // printf("segCount %d", segCount);
    if (start < 0 || end < start || end > segCount){
	return;

    }

    else{

	for (int i = start; i < end; i++){
	    total += track_count_points(trk, i);
	}
	trk->t[start].trk = realloc(trk->t[start].trk , 2*total*sizeof(trackpoint *));
	length += track_count_points(trk, start);
	// trk->t[start].length = total;
	for (int i = start +1; i < end; i++){


	    for (int j = 0; j < track_count_points(trk, i) ;  j++){
		// printf("num points %d", j + length - track_count_points(trk, i)  );
		// printf("total %d\n", length);
		// printf("time %ld ", trackpoint_time(trk->t[i].trk[j]));

		trk->t[start].trk[j + length ] = trk->t[i].trk[j];
		// trackpoint_destroy(trk->t[i].trk[j]);
		// printf("time %ld ", trackpoint_time(trk->t[start].trk[j + length - track_count_points(trk, i) ]));
	    }
	    free(trk->t[i].trk);
	    length += track_count_points(trk, i);


	    // trk->t[start].trk[ + 1] = trk->t[i].trk[0];

	}
	trk->t[start].length = total ;
	// if (end ==segCount)
	for (int i = end; i<segCount; i++){

	    // printf("%d  %d\n", end, segCount);
	    numTrackpoints = track_count_points(trk, i);
	    // printf("totalTrackpoints %d", track_count_points(trk, i));

	    // if (count == 0){
	    // 	return;
	    // }
	    trackpoint **newPtr = malloc( (numTrackpoints+1)*sizeof(trackpoint *));

	    if (newPtr == NULL){
		return;
	    }
	    // copy all the previous samples from the old array to the new one
	    memcpy(newPtr, trk->t[i].trk, numTrackpoints*sizeof(trackpoint *));

	    // get rid of the old, too-small array
	    free(trk->t[i].trk);
	    // free(trk->t[start + i - end +1].trk);

	    // remember the new one
	    // trk->t[start].trk = newPtr;
	    trk->t[start + i - end+1].trk = newPtr;
	    trk->t[start + i - end +1].length = numTrackpoints;


	}
	// if (end ==segCount){
	// 	trk->t[start + i - end+1].trk = newPtr;
	// 	trk->t[start + i - end +1].length = numTrackpoints;
	// }
	trk->segCount -= ((end -1)- start) ;
	// printf("segCount %d", trk->segCount);
    }

    // printf("segCount %d\n", track_count_segments(trk));
    // printf("totalTrackpoints %d", trk->totalTrackpoints);
    // printf("pointCount %d\n", track_count_points(trk, start));

    // printf("Test: %d\n", sizeof(trackpoint*) * 10);	


}
void track_heatmap(const track *trk, double cell_width, double cell_height, int ***map, int *rows, int *cols){
    // int segCount = track_count_segments(trk) -1;
    double calcRow;
    double calcCol;
    int insertRow;
    int insertCol;
    double leftMost;
    double rightMost;
    double northernMost;
    double southernMost;
    int **heatmap;
    if (trk->totalTrackpoints == 0){
    	heatmap = malloc(sizeof(int *));
    	heatmap[0] = calloc(1, sizeof(int));
    	*rows = 1;
    	*cols = 1;
    	*map = heatmap;
    	return;
    }

    if (trk->totalTrackpoints == 1){
    	heatmap = malloc(sizeof(int *));
    	heatmap[0] = calloc(1, sizeof(int));
    	*rows = 1;
    	*cols = 1;
    	heatmap[0][0] = 1;
    	*map = heatmap;
    	return;
    }
    // printf("total trackpoints %d", trk->totalTrackpoints);

    // find northern most points
    // int n = track_count_segments(trk);

    // track *temp;
    track *temp = malloc(sizeof(track));
    memcpy(temp, trk, sizeof(track));
    track_merge_segments(temp, 0, track_count_segments(temp));
    bubbleSort(temp->t[0].trk, trk->totalTrackpoints );
    // printf("segs %d \n", track_count_segments(temp));
    // printf("points %d ", track_count_points(temp, track_count_segments(temp) -1));


    // for (int i = 0; i< trk->totalTrackpoints; i++){
    // 	printf("temp->t[0].trk[i]).lon, %lf  \n", trackpoint_location(temp->t[0].trk[i]).lon );

    // }
    // insertRow = (int)((northernMost - trackpoint_location(temp->t[0].trk[i]).lat )/ cell_height);
    // insertCol = (int)(leftMost - (trackpoint_location(temp->t[0].trk[i]).lon)/ cell_width );
    // for (int j = 0; j<track_count_points(trk, i); j++){
    // printf("temp->t[0].trk[i]).lon, %lf  ", trackpoint_location(temp->t[0].trk[i]).lon );
    // printf("lon %lf", trackpoint_location(temp->t[0].trk[0]).lon);


    // find shortest wedge
    double shortestWedge = (360.0 - fabs(trackpoint_location(temp->t[0].trk[0]).lon - trackpoint_location(temp->t[0].trk[1]).lon));
    int BestIndex = 0;
    northernMost = trackpoint_location(temp->t[0].trk[0]).lat;
    southernMost = trackpoint_location(temp->t[0].trk[0]).lat;
    // printf("shortestWedge %lf", shortestWedge);


    for (int i = 0; i< trk->totalTrackpoints; i++){

	// printf("totalTrackpoints: %d, i %d (i+1) mod trk->totalTrackpoints %d \n", trk->totalTrackpoints, i, (i+1)%trk->totalTrackpoints);
	if(trackpoint_location(temp->t[0].trk[i]).lat > northernMost){
	    northernMost = trackpoint_location(temp->t[0].trk[i]).lat;
	}
	if(trackpoint_location(temp->t[0].trk[i]).lat < southernMost){
	    southernMost = trackpoint_location(temp->t[0].trk[i]).lat;

	}
	// printf("temp->t[0].trk[i]).lon, %lf \n", trackpoint_location(temp->t[0].trk[i]).lon );
	// printf("check %lf\n", trackpoint_location(temp->t[0].trk[(i+1)%trk->totalTrackpoints]).lon);

	// printf("Check Wedge %lf", (360.0 - fabs(trackpoint_location(temp->t[0].trk[i]).lon - trackpoint_location(temp->t[0].trk[(i+1)%trk->totalTrackpoints]).lon)));
	if (shortestWedge > (360.0 - fabs(trackpoint_location(temp->t[0].trk[i]).lon - trackpoint_location(temp->t[0].trk[(i+1)%trk->totalTrackpoints]).lon))){
	    shortestWedge = (360.0 - fabs(trackpoint_location(temp->t[0].trk[i]).lon - trackpoint_location(temp->t[0].trk[(i+1)%trk->totalTrackpoints]).lon));
	    BestIndex = i;
	    // printf("bestInd!!!!! %d \n", i);
	}
    }

    if (trackpoint_location(temp->t[0].trk[BestIndex]).lon <  trackpoint_location(temp->t[0].trk[(BestIndex+1)%trk->totalTrackpoints]).lon){
	leftMost = trackpoint_location(temp->t[0].trk[BestIndex]).lon;
	rightMost = trackpoint_location(temp->t[0].trk[(BestIndex+1)%trk->totalTrackpoints]).lon;
    }

    else{
	leftMost = trackpoint_location(temp->t[0].trk[(BestIndex+1)%trk->totalTrackpoints]).lon;
	rightMost = trackpoint_location(temp->t[0].trk[BestIndex]).lon;
	BestIndex = (BestIndex+1)%trk->totalTrackpoints;
    }
    // printf("BestIndex %d", BestIndex);
    // printf("leftMost lon %lf", leftMost);

    // printf("rightMost lon %lf", rightMost);




    // printf("\n");
    // printf("rightMost %lf\n", rightMost);
    // printf("leftMost %lf\n", leftMost);
    // printf("southernMost %lf\n", southernMost);
    // printf("northernMost %lf\n", northernMost);

    // printf("rows %d\n", (int)ceil((northernMost - southernMost)/cell_height));
    // printf("rows %d\n", (int)ceil((rightMost - leftMost)/cell_width));

    // check if positive;


    // last COMMENT!!!!
    *rows = (int)ceil((northernMost - southernMost)/cell_height);
    *cols = (int)ceil((rightMost - leftMost)/cell_width);
    // printf("rows %d\n", *rows);
    // printf("cols %d\n", *cols);
    // printf("rows_width %lf\n", cell_width);
    // printf("col_height %lf\n", cell_height);
    // for 




    // **map = malloc(sizeof(int *));
    if (map == NULL){
	return;
    }
    // printf("rows %d");

    // if ((*rows) == 0 || (*cols) == 0){
    // 	heatmap = malloc(sizeof(int *));
    // 	heatmap[0] = calloc(1, sizeof(int));
    // 	*rows = 1;
    // 	*cols = 1;
    // 	heatmap[0][0] = trk->totalTrackpoints;

    // 	*map = heatmap;
    // 	return;
    // }
    if((*rows) ==0){
    	*rows = 1;
    }
    if((*cols) ==0){
    	*cols = 1;
    }
    

    heatmap = malloc((*rows) *sizeof(int *));
    if (map == NULL){
	return;
    }

    for (int i = 0 ; i < (*rows); i++){
	heatmap[i] =  calloc((*cols), sizeof(int));
	// for (int j = 0; j<(*cols); j++){
	// 	map[i][j] = calloc(1, sizeof(int));
	// }

	if (heatmap[i] == NULL){
	    return;
	}

    }





    // (*map)[(*rows)-1][(*cols) -1] = 1;
    // printf(" map test %d \n", (*map)[0][0]);

    for (int i = 0; i< trk->totalTrackpoints; i++){
	// insertRow = (int)((northernMost - trackpoint_location(temp->t[0].trk[i]).lat )/ cell_height);
	// insertCol = (int)(leftMost - (trackpoint_location(temp->t[0].trk[i]).lon)/ cell_width );

	// printf("temp->t[0].trk[i]).lon, %lf  ", trackpoint_location(temp->t[0].trk[i]).lon );
	// printf("temp->t[0].trk[i]).lat, %lf ", trackpoint_location(temp->t[0].trk[i]).lat );

	if((*rows) == 1 ){
	    insertRow = 0;

	} else {
	    calcRow = ((northernMost - trackpoint_location(temp->t[0].trk[i]).lat) / cell_height);

	    if (calcRow >= *rows) {
		calcRow = *rows-1;
	    }

	    insertRow = (int)floor(calcRow);

	}

	if((*cols) ==1) {
	    insertCol = 0;
	} else {
	    calcCol = ((trackpoint_location(temp->t[0].trk[i]).lon - leftMost)/ cell_width );
	    if (calcCol >= *cols) {
		calcCol = *cols-1;
	    }

	    insertCol = (int)floor(calcCol);
	}
	assert(insertRow < *rows);
	assert(insertCol < *cols);
	// printf("insertRow %lf  insertCol %lf\n", calcRow, calcCol);
	heatmap[insertRow][insertCol]++;
	// printf("map count of  indici row %d col: %d \n", insertRow, insertCol);
	// printf("map count of %d indici row %d col: %d \n", (*map)[insertRow-1][insertCol-1], insertRow, insertCol);
    }


    // }

    free(temp);
    // printf("segments %d\n", track_count_segments(temp));
    // printf("points %d\n", track_count_points(temp, track_count_segments(temp) -1));

    *map = heatmap;
    // (*map)[0][0]++;
}

void swap(trackpoint **first, int j)  
{  
    trackpoint *temp = first[j];  
    // trackpoint_destroy(first[j]);
    first[j] = first[j+1];  
    // trackpoint_destroy(first[j+1]);;
    first[j+1] = temp;  
} 


// A function to implement bubble sort 
void bubbleSort(trackpoint **arr, int n) 
{ 
    int i, j; 
    for (i = 0; i < n-1; i++) {  
	// Last i elements are already in place    
	for (j = 0; j < n-i-1; j++) { 
	    if (trackpoint_location(arr[j]).lon > trackpoint_location(arr[j+1]).lon) 
		swap(arr, j);

	}

    }
}
