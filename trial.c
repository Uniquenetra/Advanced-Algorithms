#include <stdio.h>
#include <stdlib.h>

typedef struct node* node;

struct node
{
	int v; /* The value to store */
	int rank; /* Number of childen */
	node child; /* First child if it has any */
	node brother; /* In heap list points to larger rank.
								Inside a tree points to smaller rank. */
	node father; /* Points up */
};

node A;

int ptr2loc(node v, node A) {
	int r;
	r = -1;
  if(NULL != v)
		r = ((size_t) v - (size_t) A) / sizeof(struct node);
  return (int)r;
}

/* Function Def: Show Node */
void showNode(node v) {
	if(NULL == v)
    printf("NULL\n");
	else {
		printf("node: %d ", ptr2loc(v, A));
		printf("v: %d ", v->v);
		printf("rank: %d ", v->rank);
		printf("child: %d ", ptr2loc(v->child, A)); 
		printf("brother: %d ", ptr2loc(v->brother, A)); 
		printf("father: %d ", ptr2loc(v->father, A)); 
		printf("\n");
	} 
}


/* Function Def: Show List */
void showList(int in_index) {
	node curr;
	curr = &A[in_index];
	do {
		showNode(curr);
		curr = curr->brother;
	} 
	while (curr != NULL);
}

/* Function Def: set*/
/*V (node, v) The Set function that changes the v field of the current node. 
Note that this function can only be executed when the node is a heap by itself
, i.e., its child, brother and father fields are all NULL.*/

void set(int new_val, int in_index) {
	node curr;
	curr = &A[in_index];

	if (curr->child == NULL && curr->brother == NULL && curr->father == NULL){
		A[in_index].v = new_val;
	}
	/*showNode(curr);*/
}


void swap(node node_1, node node_2) {
	int temp;
	temp = node_1->v;
	node_1->v = node_2->v;
	node_2->v = temp;
}

void re_order(node new_node){
	if (new_node->father != NULL){
		if (new_node->v >= new_node->father->v) {
			NULL;
		}
		else{
			swap(new_node , new_node->father);
			re_order(new_node->father);
		}
	}
}


/*R (node, v) The DecreaseKey function is used to decrease the v field of the current node. 
This node may be part of a larger binomial tree and therefore this operation might ne`ed to move the value upwards on the tree.*/

int decKey(int in_index, int new_val){
	if (new_val < A[in_index].v){
		set(new_val, in_index);
		re_order(&A[in_index]);
	}
	return in_index;
}

/*M (heap) The Min function returns the minimum value that exists in the given binomial heap.*/
int min(int in_index) {
	node curr;
	curr = &A[in_index];

	int min_val;
	min_val = curr->v;

	do {
		curr = curr->brother;
		
		if (curr->v < min_val){
			min_val = curr->v;
		}	
	}
	while (curr != NULL);
	return min_val;
}

/*A (heap) The ArgMin function returns an identification of the node that contains the minimum value that exists in the given binomial heap.*/
int argMin(int in_index) {
	node curr;
	curr = &A[in_index];

	int min_val;
	min_val = curr->v;

	int pos;
	pos = in_index;

	do {
		curr = curr->brother;

		if (curr->v < min_val){
			min_val = curr->v;
			pos = ptr2loc(curr,A);
		}	
	}
	while (curr != NULL);
	return pos;
}

void link(node n, node f){
	printf("link %d as child of %d\n", ptr2loc(n, A), ptr2loc(f, A));
	n->brother=f->child;
	n->father=f;
	f->rank++;
	f->child=n;
}

static node clink(node *L1, node *L2, node *L0){ /*Compara entre 2 raizes e decide baseada nos ranks se copia uma árvore para a lista final, ou se as liga e nesse caso compara antes os valores V (A com menor valor V fica como pai)*/
	if((*L1)->rank<(*L2)->rank)
		L0=L1;
	if((*L1)->rank==(*L2)->rank){/*min -> father; max ->child;*/
		if((*L1)->v<=(*L2)->v){
			link(*L1,*L2);
			L0=L1;
		}
		else{
			link(*L2,*L1);/*Caso B1 Fica como raiz a que tiver o filho mais pequeno*/
			L0=L2;
		}
	}
	if((*L1)->rank>(*L2)->rank)
		L0=L2;
	return *L0;
}

/*U*/
int unite(int x,int p){
	node *z;
	node a=&A[x];
	node pp=&A[p];
	while(A[x].brother!=NULL || A[p].brother!=NULL){
		*z=clink(&a,&pp,z);
	}
	return ptr2loc(*z,A);
}





/* Main */
int main()
{
	int n;
	int i;
	int c;

	printf("ascii value of letter P is :%d\n", 'P');

	printf("Enter the no of nodes:\n");
	scanf("%d", &n);


	A = (node)calloc(n, sizeof(struct node));

	A[0].v = 7;

	c = getchar();
	do {
		int p;
		int x;

		switch(c){
			case 'P': /*Show List*/
				scanf("%d", &p);
				showList(p);
				break;

			case 'S': /*Show Node*/
				scanf("%d", &p);
				showNode(&A[p]);
				break;

			case 'V': /*Set cell*/
				scanf("%d", &p);
				scanf("%d", &x);
				set(x, p);
				break;

			case 'R': /*Decrease Key*/
				scanf("%d", &p);
				scanf("%d", &x);
				printf("%d\n",decKey(p, x));
				break;

			case 'M': /*Min key*/
				scanf("%d", &p);
				printf("%d\n",min(p));
				break;

			case 'A': /*Position of Min*/
				scanf("%d", &p);
				printf("%d\n",argMin(p));
				break;
		}
		c = getchar();
	} while (c != EOF && c !='X');

	printf("Final configuration:\n");
	for(i = 0; i<n; i++) {
		showNode(&A[i]);
	}

	free(A);
	return 0;
}
