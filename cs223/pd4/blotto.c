#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define _GNU_SOURCE
// #define __STRICT_ANSI__
#include "entry.h"
#include "smap.h"
#include "smap_test_functions.h"

typedef struct battles{
	double score;
	double wins;
	double numGames;
	int *values;
	char *key;
}battles;

int comparatorScore(const void *p1, const void *p2){
	battles newp1 = *(const battles *)p1;
	battles newp2 = *(const battles *)p2;
	if (newp1.score > newp2.score){
		return -1;
	}
	else if (newp1.score == newp2.score){
		if (strcmp(newp1.key, newp2.key) > 0){
			return 1;
		}

		else{
			return -1;
		}
		// return 0;
	}
	else if (newp1.score < newp2.score){
		return 1;

	}
}
int comparatorWins(const void *p1, const void *p2){
	battles newp1 = *(const battles *)p1;
	battles newp2 = *(const battles *)p2;
	if (newp1.wins > newp2.wins){	
		return -1;
	}
	else if (newp1.wins == newp2.wins){
		if (strcmp(newp1.key, newp2.key) > 0){
			return 1;
		}

		else{
			return -1;
		}
	}
	else if (newp1.wins < newp2.wins){
		return 1;
	}

// 
}

int main(int argc, char const *argv[])
{

	smap *smap = smap_create(smap_default_hash);
// 	const char *hello = "hello";
// //smap_put(smap, hello, )

// 	// int y = 1;

// 	smap_put(smap, hello, y);
	
	FILE *fp = NULL;
	int numBattlefields = argc - 3;
	int values[numBattlefields];
	char id[31];
	char vsId[31];
	char *key;
	char *buffer = NULL;
	size_t size = 0;
	battles *p1Vals;
	battles *p2Vals;
	double p1Score;
	double p2Score;
	double p1Wins;
	double p2Wins;
	int type;
	char *token;

	if(argc < 4){
		perror ("Insufficient Inputs\n");
		exit(0);
	}
	while( getline(&buffer, &size, stdin) != -1){
		if (size > 0 && buffer[0] == '\n'){
			exit(0);
		}
		key = strtok(buffer,",");
		if(strlen(key) > 31 || key == NULL){
			exit(0);
		}

		battles *info = malloc(sizeof(battles));
		if(info == NULL){
			free(info);
			exit(1);
		}
		info->values = malloc(sizeof(int)*numBattlefields);
		if(info->values == NULL){
			free(info->values);
			exit(1);
		}
		info->wins = 0.0;
		info->score = 0.0;
		info->numGames = 0.0;
		info->key = strdup(key);


		for (int i =0; i<numBattlefields; i++){
			token = strtok(NULL, ",");
			if (token == NULL){
				perror ("Incorrect number of numBattlefields!\n");
				exit(4);
			}

			info->values[i] = atoi(token);


		}
		smap_put(smap, key, info);		
	}

	free(buffer);


	// UNUSED FOR NOW
	fp = fopen(argv[1], "r"); /* open the file for reading */
	if (!fp) {
		perror ("File open error!\n");
		return 1;
	}

	//printf("index %d", index);
	const char *win = argv[2];


	if(strcmp(win,"win") == 0) {
		type = 0;
	}

	else if (strcmp(win,"score") == 0){
		type = 1;
	}

	else{
		exit(1);
	}	
	for (int i = 3; i< argc; i++){
		values[i-3] = atoi(argv[i]);
	}
	while (fscanf(fp, "%s %s", id, vsId) == 2){
		p1Score = 0.0;
		p2Score = 0.0;
		p1Wins = 0.0;
		p2Wins = 0.0;
		p1Vals = (battles *)smap_get(smap, id);
		p2Vals = (battles *)smap_get(smap, vsId);
		if (p1Vals ==0 || p2Vals ==0){
			perror ("Invalid Matchup\n");
			exit(2);
		}

		for (int i =0; i<numBattlefields; i++){
			if(p1Vals->values[i] > p2Vals->values[i]){
				// p1Score += values[i];
				p1Score += values[i];
				// p1Wins++;
				// printf("%f. ", p1Wins);
				// printf("p1wins   p1: %i, p2: %i, value %i\n", p1Vals->values[i], p2Vals->values[i], values[i]);
			}
			else if (p1Vals->values[i] < p2Vals->values[i]){
				p2Score += values[i];
				// p2Wins++;
				// printf("p2wins   p1: %i, p2: %i, value %i\n", p1Vals->values[i], p2Vals->values[i], values[i]);
			}
			else{
				p1Score += (double)values[i]/2;
				p2Score += (double)values[i]/2;
				// p1Wins += 0.5;
				// p2Wins += 0.5;

				// printf("tie      p1: %i, p2: %i, value %i\n", p1Vals->values[i], p2Vals->values[i], values[i]);
			}
			
		}

		if (p1Score > p2Score){
			p1Vals->wins++;
		}
		else if (p1Score < p2Score){
			p2Vals->wins++;
		}

		else if (p1Score == p2Score){
			p1Vals->wins += 0.5;
			p2Vals->wins += 0.5;
		}



		p1Vals->numGames++;
		p2Vals->numGames++;

		p1Vals->score += p1Score;
		p2Vals->score += p2Score;

		// p1Vals->wins += p1Vals->wins;
		// p2Vals->wins += p2Vals->wins;

		// p1Vals->score = p1Vals->score/p1Vals->numGames;
		// p2Vals->score = p2Vals->score/p2Vals->numGames;

		// p1Vals->wins = p1Vals->wins/p1Vals->numGames;
		// p2Vals->wins = p2Vals->wins/p2Vals->numGames;


		// printf("%s: %f, %s: %f \n", id, p1Vals->score/p1Vals->numGames, vsId, p2Vals->score/p2Vals->numGames);
		// printf("%s: %f, %s: %f \n", id, p1Vals->wins/p1Vals->numGames, vsId, p2Vals->wins/p2Vals->numGames);
			
		// printf("%s, %s\n", id, vsId);
		// printf("%d, %d\n", player[index].id, player[index].vsId);
		// player[index]
		// index++;
	}

	const char **keys = smap_keys(smap);
	int mapSize = smap_size(smap);
	battles *arr = malloc(sizeof(battles) * mapSize);

	for (int i = 0; i< mapSize; i++){
		arr[i] = *(battles *)smap_get(smap, keys[i]);
	}
	if (type == 0){
		qsort(arr, mapSize, sizeof(arr[0]), comparatorWins); 
	
		for (int i = 0; i< mapSize; i++){
			printf("%7.3f %s\n", arr[i].wins/arr[i].numGames, arr[i].key);
			free(((battles *)smap_get(smap, arr[i].key))->values);
			free(smap_get(smap, arr[i].key));
			free(arr[i].key);
			// printf("%s %f\n", keys[i], ((battles *)smap_get(smap, keys[i]))->wins);
	
		}
	}
	else if(type == 1){
	qsort(arr, mapSize, sizeof(arr[0]), comparatorScore); 
	
		for (int i = 0; i< mapSize; i++){
			printf("%7.3f %s\n", arr[i].score/arr[i].numGames, arr[i].key);
			free(((battles *)smap_get(smap, arr[i].key))->values);
			free(smap_get(smap, arr[i].key));
			free(arr[i].key);
			// printf("%s %f\n", keys[i], ((battles *)smap_get(smap, keys[i]))->score);
	
		}
	}
	free(keys);
	free(arr);

	// for (int i = 0; i< mapSize; i++){
	// 		(arr[i].key);
	// 		// printf("%s %f\n", keys[i], ((battles *)smap_get(smap, keys[i]))->score);
	
	// 	}


	// free(temp);


	// printf("\n");

	// char *p1 = "P1";
	// char *p2 = "P2";
	// char *p3 = "P3";


	// int n = smap_default_hash(s);
	// printf("worked %i\n", n);

	//  battles *ex;

	//  ex = (battles *)smap_get(smap, p1);
	//  printf("\n %s  ", p1);
	// 	for (int i =0; i<numBattlefields; i++){
	// 		printf(" %i ", ex->values[i]);
			
	// 	}
	// ex = (battles *)smap_get(smap, p2);
	//  printf("\n %s  ", p2);
	// 	for (int i =0; i<numBattlefields; i++){
	// 		printf(" %i ", ex->values[i]);
			
	// 	}

	// ex = (battles *)smap_get(smap, p3);
	//  printf("\n %s  ", p3);
	// 	for (int i =0; i<numBattlefields; i++){
	// 		printf(" %i ", ex->values[i]);
			
	// 	}


	smap_destroy(smap);
	fclose(fp);

	// printf("\nexample %i\n", ex[0]);
}




