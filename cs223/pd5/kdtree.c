
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include <stdio.h>
#include "kdtree.h"

#include "location.h"
#define RIGHT (1)
#define LEFT (-1)
#define YDEPTH (1)
#define XDEPTH (0)



struct _kdtree{
	location *loc;
	int depth;    /* height of this node */
	size_t size;
	bool contains;
	kdtree *rightChild, *leftChild;
};


bool isKey(const kdtree *t, const location *p);
void nearest(kdtree *t, const location *p, location *neighbor, double *d, location *min, location *max);
void merge(int n1, const location a1[], int n2, const location a2[], location out[], int x);
void mergeSort(int n, const location a[], location out[], int x);
void add(kdtree *t, location *XCut, location *YCut, location toInsert, int n);
void sortHelper(location *X_Left, location *X_Right, location *Y_Left, location *Y_Right, location *Exs, location *Whys, location comp, int n, int sortBy, int *Indices);
kdtree *newNode(kdtree *t, const location *p);
bool compX(location p1, location p2);
bool compY(location p1, location p2);
bool Equal(location p1, location p2);

bool GreatX(location p1, location p2);
bool GreatY(location p1, location p2);


/**
 * A set of geographic locations given by latitude and longitude
 * implemented using a k-d tree, where k = 2.  For all functions,
 * points are considered different based on the values of the
 * coordinates that are passed in, even if the coordinates represent
 * the same physical point, so, for example, longitude -180 is
 * considered different than longitude 180.
 */


/**
 * Creates a balanced k-d tree containing copies of the points in the
 * given array of locations.  If the array is NULL and n is 0 then the
 * returned tree is empty.  If the tree could not be created then the
 * returned value is NULL.
 *
 * @param pts an array of valid locations, non-NULL if n != 0
 * @param n the number of points in that array, or 0 if pts is NULL
 * @return a pointer to the newly created collection of points, or NULL
 * if it could not be created
 */


kdtree *kdtree_create(const location *pts, int n){
	kdtree *tree = malloc(sizeof(kdtree));

	if (tree == NULL){
		return NULL;
	}
	else {
		tree->loc = NULL;
		tree->contains = false;
		tree->depth = 0;
		tree->leftChild = NULL;
		// tree->leftChild->contains = false;
		tree->rightChild = NULL;
		// tree->rightChild->contains = false;
		// tree->rightChild->depth = tree->depth +1;
		// tree->leftChild->depth = tree->depth +1;		

		// tree->rightChild->rightChild = NULL;
		// tree->rightChild->leftChild = NULL;
		// tree->leftChild->rightChild = NULL;
		// tree->leftChild->leftChild = NULL;
		// tree->rightChild->loc = NULL;
		// tree->leftChild->loc = NULL;
	}
	
	if (n > 0){



	int mid;

	location *Exs = malloc(sizeof(location) *n);
	location *Whys= malloc(sizeof(location) *n);
	mergeSort(n, pts, Exs, 0);
	mergeSort(n, pts, Whys, 1);


		// for (int j = 0; j < n; j++){
		// 	printf("x %f y %f\n", Exs[j].x, Exs[j].y);
		// }
		// printf("Whys\n \n");
		// printf("==================\n");
		// for (int j = 0; j < n; j++){
		// 	printf("x %f y %f\n", Whys[j].x, Whys[j].y);
		// }

		(n%2 == 1) ? (mid = n/2) : (mid = n/2 -1);
		add(tree, Exs, Whys, Exs[mid], n);


	free(Exs);
	free(Whys);

}

	// else if (n == 1){

	// 		kdtree *temp;

	// 		temp = newNode(tree, &pts[0]);

	// 		*tree = *temp;
	// 		// free(temp->loc);
	// 		free(temp);

	// }
	return tree;

	

}

// Create Helper
void add(kdtree *t, location *XCut, location *YCut, location comp, int n)
{
	kdtree *temp;
	int Indices[4];
	int leftMid, rightMid, X_LeftInd, X_RightInd, Y_LeftInd, Y_RightInd;

	location *X_Left = malloc(sizeof(location)*n);
	location *X_Right = malloc(sizeof(location)*n);
	location *Y_Left = malloc(sizeof(location)*n);
	location *Y_Right = malloc(sizeof(location)*n);

	sortHelper(X_Left, X_Right, Y_Left, Y_Right, XCut, YCut, comp, n, t->depth %2, &Indices[0]);
			X_LeftInd = Indices[0];
			X_RightInd = Indices[1];
			Y_LeftInd = Indices[2];
			Y_RightInd = Indices[3];

	if (n == 1){

		assert(t);

			temp = newNode(t, &comp);
			*t = *temp;
			free(temp);

				free(X_Left);
			free(X_Right);
			free(Y_Left);
			free(Y_Right);

			return;

		
	}

	if(t== NULL || t->contains != true) 
	{

		if(t!= NULL){

				temp = newNode(t, &comp);

			*t = *temp;
			// free(temp->loc);
			free(temp);
		}
	}
		assert(Y_LeftInd == X_LeftInd);
		assert(Y_RightInd == X_RightInd);


	// from Exs
	if (t->depth % 2 == 0){

		(X_LeftInd%2 == 1) ? (leftMid = X_LeftInd/2) : (leftMid = X_LeftInd/2 -1);
		



		if (X_RightInd > 0){
			(X_RightInd%2 == 1) ? (rightMid = X_RightInd/2) : (rightMid = X_RightInd/2 -1);
			assert(rightMid >= 0);

			add(t->rightChild, X_Right, Y_Right, Y_Right[rightMid], X_RightInd);

		}

		if (X_LeftInd > 0){

			(X_LeftInd%2 == 1) ? (leftMid = X_LeftInd/2) : (leftMid = X_LeftInd/2 -1);
			assert(leftMid >= 0);
			add(t->leftChild, X_Left, Y_Left, Y_Left[leftMid], X_LeftInd);
		}


	}
	else if (t->depth % 2 == 1){

		if (Y_RightInd > 0){

			(Y_RightInd%2 == 1) ? (rightMid = Y_RightInd/2) : (rightMid = Y_RightInd/2 -1);
			add(t->rightChild, X_Right, Y_Right, X_Right[rightMid], X_RightInd);


		}

		if (Y_LeftInd > 0){

			(Y_LeftInd%2 == 1) ? (leftMid = Y_LeftInd/2) : (leftMid = Y_LeftInd/2 -1);
			add(t->leftChild, X_Left, Y_Left, X_Left[leftMid], X_LeftInd);

		}

	}
		free(X_Left);
		free(X_Right);
		free(Y_Left);
		free(Y_Right);
	return;

}

void sortHelper(location *X_Left, location *X_Right, location *Y_Left, location *Y_Right, location *Exs, location *Whys, location comp, int n,  int sortBy, int *Indices){
	int j;

	int X_LeftInd = 0;
	int X_RightInd = 0;
	int Y_LeftInd = 0;
	int Y_RightInd = 0;

	if (sortBy == XDEPTH){
			for(j = 0; j < n; j++){
				// && (Equal(Exs[j], comp) == false)
				// Exs[j].x < comp.x
				// compX
				if(compX(Exs[j], comp) == true){

					X_Left[X_LeftInd] = Exs[j];
					X_LeftInd++;
				}

				else if  (GreatX(Exs[j], comp) == true){
					X_Right[X_RightInd] = Exs[j];
					X_RightInd++;
				}
			}



			for(j = 0; j < n; j++){
				// Whys[j].x < comp.xm
				if(compX(Whys[j], comp) == true ){
					Y_Left[Y_LeftInd] = Whys[j];
					// Y_Left[Y_LeftInd].y = Whys[j].y;
					Y_LeftInd++;
				}
				else if (GreatX(Whys[j] , comp) == true){
					Y_Right[Y_RightInd] = Whys[j];
					Y_RightInd++;
				}
			}

		}


		else if (sortBy == YDEPTH){
			for(j = 0; j < n; j++){
				if(compY(Exs[j], comp) == true ){

					X_Left[X_LeftInd] = Exs[j];
					X_LeftInd++;
				}

				else if (GreatY(Exs[j], comp) == true){
					X_Right[X_RightInd] = Exs[j];
					X_RightInd++;
				}
			}

			for(j = 0; j < n; j++){
				if(compY(Whys[j], comp) == true ){
					Y_Left[Y_LeftInd] = Whys[j];
					Y_LeftInd++;
				}
				else if (GreatY(Whys[j] , comp) == true){
					Y_Right[Y_RightInd] = Whys[j];
					Y_RightInd++;
				}
			}

		}
	


			Indices[0] = X_LeftInd;
			Indices[1] = X_RightInd;
			Indices[2] = Y_LeftInd;
			Indices[3] = Y_RightInd;


}

/**
 * Adds a copy of the given point to the given k-d tree.  There is no
 * effect if the point is already in the tree.  The tree need not be
 * balanced after the add.  The return value is true if the point was
 * added successfully and false otherwise (if the point was already in the
 * tree or if there was a memory allocation error when trying to add).
 *
 * @param t a pointer to a valid k-d tree, non-NULL
 * @param p a pointer to a valid location, non-NULL
 * @return true if and only if the point was successfully added
 */

bool kdtree_add(kdtree *t, const location *p){
	kdtree *temp;

	// assert(t);
	// assert(t->contains == false || t->contains == true);
	// assert(t->leftChild);
	// assert(t->rightChild);
	// assert(t->depth);
	if(t== NULL || t->contains != true) {
		/* not already there, put it in */
		if(t!= NULL){

			temp = newNode(t, p);
			*t = *temp;
			free(temp);
		}

		return true;
		// free(t);
	} 

	else if(kdtree_contains(t, p) == true) {
		/* already there, do nothing */
		return false;
	} 

	else {
		/* do this recursively so we can fix data on the way back out */
		// if (isKey(t, p) == true){
		//    return true;

		// }
		if (t->depth % 2 == 0){
			if(compX(*t->loc, *p) == true){
				kdtree_add(t->rightChild, p);
			}
			else{
				kdtree_add(t->leftChild, p);
			}
		}
		else if (t->depth % 2 == 1){
			if(compY(*t->loc, *p) == true){
				kdtree_add(t->rightChild, p);
			}
			else{
				kdtree_add(t->leftChild, p);
			}
		}
	}
	assert(t->loc);
	return true;
}

/**
 * Determines if the given tree contains a point with the same coordinates
 * as the given point.
 *  
 * @param t a pointer to a valid k-d tree, non-NULL
 * @param p a pointer to a valid location, non-NULL
 * @return true if and only of the tree contains the location
 */
bool kdtree_contains(const kdtree *t, const location *p){
	// assert(t);
	// assert(t->contains);
	// assert(p);
	while(t != NULL && t->contains == true && !(isKey(t, p))) 
	{
		// assert(p->x);
		if (t->depth % 2 == 0){
			if(compX(*t->loc, *p) == true){
				t = t->rightChild;
			}
			else{
				t = t->leftChild;

			}

		}
		else if (t->depth % 2 == 1){
			if(compY(*t->loc, *p)){
				t = t->rightChild;
			}
			else{
				t = t->leftChild;
			}
		}
	}

	return isKey(t, p);
}

/**
 * Copies the nearest neighbor to the given point among points in the
 * given tree to the given location.  There is no change to neighbor and
 * distance is set to infinity if the tree is empty.  If p is equal to
 * a point in the tree then p is copied into neighbor and distance is
 * set to zero.
 *
 * @param t a pointer to a valid k-d tree, non-NULL
 * @param p a pointer to a valid location, non-NULL
 * @param d a pointer to a double, non-NULL
 * @param neighbor a pointer to a valid location, non-NULL
 */
void kdtree_nearest_neighbor(kdtree *t, const location *p, location *neighbor, double *d){
	*d = INFINITY;
	if(t == NULL || t->contains == false){
		return;
	}
	else{
		location min = (location){(double)-INFINITY, (double)-INFINITY};
		location max = (location){(double)INFINITY, (double)INFINITY};
		nearest(t, p, neighbor, d, &min, &max);
	}
}

void nearest(kdtree *t, const location *p, location *neighbor, double *d, location *min, location *max){
	if (t->contains == false || d == 0){
		// if(*d == NULL){
		// 	*d = INFINITY;
		// }
		return;
	}
	if (location_distance_to_rectangle(p, min, max) >= *d){
		return;
	}
	if (location_distance(t->loc, p) <= *d){
		*neighbor = *t->loc;
		*d = location_distance(t->loc, p);


	}
	if (t->depth % 2 == 0){
		if (compX(*t->loc, *p) == true){
			min->x = t->loc->x;
			nearest(t->rightChild, p, neighbor, d, min, max);
			nearest(t->leftChild, p, neighbor, d, min, max);
		}
		else{
			max->x = t->loc->x;
			nearest(t->leftChild, p, neighbor, d, min, max);
			nearest(t->rightChild, p, neighbor, d, min, max);
		}
	}
	else if (t->depth % 2 == 1){
		if( compY(*t->loc, *p) == true){
			min->y = t->loc->y;
			nearest(t->rightChild, p, neighbor, d, min, max);
			nearest(t->leftChild, p, neighbor, d, min, max);
		}
		else{
			max->y = t->loc->y;
			nearest(t->leftChild, p, neighbor, d, min, max);
			nearest(t->rightChild, p, neighbor, d, min, max);
		}
	}
}


/**
 * Passes the points in the given tree to the given function
 * in an arbitrary order.  The last argument to this function is also passed
 * to the given function along with each point.
 *
 * @param t a pointer to a valid k-d tree, non-NULL
 * @param f a pointer to a function that takes a location and an extra
 * argument, and does not modify t, non-NULL
 * @param arg a pointer
 */
void kdtree_for_each(const kdtree* r, void (*f)(const location *, void *), void *arg){
	// assert(t);
	// assert(t->contains);
	// assert(p);
	if(r->contains != false) 
	{
		f(r->loc, arg);
		kdtree_for_each(r->rightChild, f, arg);
		kdtree_for_each(r->leftChild, f, arg);    
	}
}

/**
 * Destroys the given k-d tree.  The tree is invalid after being destroyed.
 *
 * @param t a pointer to a valid k-d tree, non-NULL
 */
void kdtree_destroy(kdtree *t){


	if(t != NULL){
		kdtree_destroy(t->leftChild);
		// }
		// if(t->rightChild != NULL){
	kdtree_destroy(t->rightChild);
	if (t->loc != NULL){
		free(t->loc);
	}

	free(t);
	// t = NULL;
}
}



bool isKey(const kdtree *t, const location *p){

	if(t!= NULL && t->contains == true && (t->loc->x == p->x) && (t->loc->y == p->y)){
		return true;
	}
	else{
		return false;
	}
}

/* merge sorted arrays a1 and a2, putting result in out */
void merge(int n1, const location a1[], int n2, const location a2[], location out[], int x)
{
	int i1;
	int i2;
	int iout;

	i1 = i2 = iout = 0;
	if( x == 0){
		while(i1 < n1 || i2 < n2) {
			
			// (a1[i1].x < a2[i2].x)
			if(i2 >= n2 || ((i1 < n1) && (compX(a1[i1], a2[i2]) == true) && Equal(a1[i1], a2[i2]) == false)) {
				/* a1[i1] exists and is smaller */
				out[iout++] = a1[i1++];
			}  else {
				/* a1[i1] doesn't exist, or is bigger than a2[i2] */
				out[iout++] = a2[i2++];
			}
		}
	}

	else{
		while(i1 < n1 || i2 < n2) {
			if(i2 >= n2 || ((i1 < n1) && (compY(a1[i1], a2[i2]) == true) && Equal(a1[i1], a2[i2]) == false)) {
				/* a1[i1] exists and is smaller */
				out[iout++] = a1[i1++];
			}  else{
				/* a1[i1] doesn't exist, or is bigger than a2[i2] */
				out[iout++] = a2[i2++];
			}
		}

	}
}

// /* sort a, putting result in out */
// /* we call this mergeSort to avoid conflict with mergesort in libc */
void mergeSort(int n, const location a[], location out[], int x)
{
	location *a1;
	location *a2;

	if(n < 2) {
		/* 0 or 1 elements is already sorted */
		if(a){
			memcpy(out, a, sizeof(location) * n);
		}
	} 
	else {
		/* sort into temp arrays */
		a1 = malloc(sizeof(location) * (n/2));
		a2 = malloc(sizeof(location) * (n - n/2));

		mergeSort(n/2, a, a1, x);
		mergeSort(n - n/2, a + n/2, a2, x);

		/* merge results */

		merge(n/2, a1, n - n/2, a2, out, x);

		/* free the temp arrays */
		free(a1);
		free(a2);
	}
}


kdtree *newNode(kdtree *t, const location *p){
	kdtree *e;
	// free(t->leftChild);
	// free(t->rightChild);
	e = malloc(sizeof(kdtree));
	assert(e);
	e->loc = malloc(sizeof(location));
	if (p != NULL){
		memcpy(e->loc, p, sizeof(location));

		e->contains = true;
	}
	e->leftChild = malloc(sizeof(kdtree));
	e->leftChild->contains = false;
	e->rightChild = malloc(sizeof(kdtree));
	e->rightChild->contains = false;
	e->depth = t->depth;
	e->rightChild->depth = t->depth +1;
	e->leftChild->depth = t->depth +1;

	e->rightChild->rightChild = NULL;
	e->rightChild->leftChild = NULL;
	e->leftChild->rightChild = NULL;
	e->leftChild->leftChild = NULL;
	e->rightChild->loc = NULL;
	e->leftChild->loc = NULL;
	return e;
}


bool compX(location p1, location p2){
	if(p1.x < p2.x ){
		return true;
	}
	if (p1.x == p2.x){
		if (p1.y < p2.y){
			return true;
		}
	}

	return false;
}

bool compY(location p1, location p2){
	if(p1.y < p2.y){
		return true;
	}
	if (p1.y == p2.y){
		if (p1.x < p2.x){
			return true;
		}
	}

	return false;
}

bool Equal(location p1, location p2){
	if((p1.x == p2.x) && (p1.y == p2.y)){
		return true;
	}
	else{
		return false;
	}
}

bool GreatX(location p1, location p2){
	if(p1.x > p2.x ){
		return true;
	}
	if (p1.x == p2.x){
		if (p1.y > p2.y){
			return true;
		}
	}

	return false;
}

bool GreatY(location p1, location p2){
	if(p1.y > p2.y){
		return true;
	}
	if (p1.y == p2.y){
		if (p1.x > p2.x){
			return true;
		}
	}

	return false;
}


