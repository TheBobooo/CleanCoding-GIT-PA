#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
    int data;
    struct Node *next;
} NODE;

typedef struct g {
    int v;
    int *vis;
    NODE **alst;
} GPH;

typedef struct s {
    int t;
    int scap;
    int *arr;
} STK;

NODE *create_node(int v) {
    NODE *nn = malloc(sizeof(NODE));
    nn->data = v;
    nn->next = NULL;
    return nn;
}

void add_edge(GPH *g, int src, int dest) {
    NODE *nn = create_node(dest);
    nn->next = g->alst[src];
    g->alst[src] = nn;

    nn = create_node(src);
    nn->next = g->alst[dest];
    g->alst[dest] = nn;
}

GPH *create_g(int v) {
    GPH *g = malloc(sizeof(GPH));
    g->v = v;
    g->alst = malloc(v * sizeof(NODE *));
    g->vis = malloc(sizeof(int) * v);

    for (int i = 0; i < v; i++) {
        g->alst[i] = NULL;
        g->vis[i] = 0;
    }
    return g;
}

STK *create_s(int scap) {
    STK *s = malloc(sizeof(STK));
    s->arr = malloc(scap * sizeof(int));
    s->t = -1;
    s->scap = scap;
    return s;
}

void push(int pshd, STK *s) {
    if (s->t + 1 < s->scap) {
        s->t++;
        s->arr[s->t] = pshd;
    }
}

void DFS(GPH *g, STK *s, int v_nr) {
    NODE *aux = g->alst[v_nr];
    g->vis[v_nr] = 1;
    printf("%d ", v_nr);
    push(v_nr, s);
    while (aux != NULL) {
        int con_ver = aux->data;
        if (g->vis[con_ver] == 0)
            DFS(g, s, con_ver);
        aux = aux->next;
    }
}

void insert_edges(GPH *g, int edg_nr) {
    int src, dest;
    printf("Adauga %d muchii (nodurile sunt indexate de la 0):\n", edg_nr);
    for (int i = 0; i < edg_nr; i++) {
        scanf("%d%d", &src, &dest);
        add_edge(g, src, dest);
    }
}

void wipe(GPH *g) {
    for (int i = 0; i < g->v; i++) {
        g->vis[i] = 0;
    }
}

void canbe(GPH *g, int nrv, STK *s1, STK *s2) {
    for (int i = 0; i < nrv; i++) {
        wipe(g);
        s1->t = -1;
        DFS(g, s1, i);

        for (int j = 0; j < nrv; j++) {
            if (i == j) continue;
            wipe(g);
            s2->t = -1;
            DFS(g, s2, j);

            int found = 0;
            for (int a = 0; a <= s1->t && !found; a++) {
                for (int b = 0; b <= s2->t && !found; b++) {
                    if (s1->arr[a] == s2->arr[b]) {
                        found = 1;
                        printf("\n%d si %d au trasee care se intersecteaza in nodul %d\n", i, j, s1->arr[a]);
                    }
                }
            }

            if (!found)
                printf("\n%d si %d NU au trasee comune.\n", i, j);
        }
    }
}

int main() {
    int nrv;
    int edg_nr;

    printf("Cate noduri are graful? ");
    scanf("%d", &nrv);

    printf("Cate muchii are graful? ");
    scanf("%d", &edg_nr);

    GPH *g = create_g(nrv);
    STK *s1 = create_s(2 * nrv);
    STK *s2 = create_s(2 * nrv);

    insert_edges(g, edg_nr);
    printf("\nVerificam intersectii de trasee DFS:\n");
    canbe(g, nrv, s1, s2);

    // cleanup (optional)
    free(s1->arr); free(s1);
    free(s2->arr); free(s2);
    for (int i = 0; i < nrv; i++) {
        NODE *curr = g->alst[i];
        while (curr) {
            NODE *temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
    free(g->alst);
    free(g->vis);
    free(g);

    return 0;
}
