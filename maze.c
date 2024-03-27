#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int crossedEdge;

typedef struct {
  int rows;
  int cols;
  unsigned char *cells;
} Map;

int guide(){
    printf("--test - Controls if the file that contains the maze has valid definition.\n");
    printf("--rpath - Finds the exit of the maze by following right hand rule.\n");
    printf("--lpath - Finds the exit of the maze by following left hand rule.\n");
    return 0;
}

int tester(Map *v1){
    for (int i = 0; i < v1->rows * v1->cols * 2; i++){
        //controls allowed symbols
        if (v1->cells[i] != 0 && v1->cells[i] != 1 && v1->cells[i] != 2 && v1->cells[i] != 3 && v1->cells[i] != 4 && v1->cells[i] != 5 && v1->cells[i] != 6 && v1->cells[i] != 7){
            fprintf(stderr, "Invalid\n");
            printf("Invalid\n");
            return -1;
        }
        //controls if the side borders are from both sides
        if ((v1->cells[i] == 2 || v1->cells[i] == 3 || v1->cells[i] == 7) && (v1->cells[i + 1] != 1 && v1->cells[i + 1] != 3 && v1->cells[i + 1] != 7 && v1->cells[i + 1] != 5)){
            for (int n = 1; n < v1->rows; n++){
                if ((i + 1) % (v1->cols) == 0){
                    continue;
                }
                fprintf(stderr, "Invalid\n");
                printf("Invalid\n");
                return -1;
                
            }
        } 
        // controls if the bottom and top borders are from both sides
        if ((i % 2 == 1) && (v1->cells[i] == 4 || v1->cells[i] == 5 || v1->cells[i] == 6 || v1->cells[i] == 7) && (v1->cells[i+v1->cols] != 4 && v1->cells[i+v1->cols] != 5 && v1->cells[i+v1->cols] != 6 && v1->cells[i+v1->cols] != 7)){
            if (i < v1->rows * v1->cols - v1->cols){
                fprintf(stderr, "Invalid\n");
                printf("Invalid\n");
                return -1;
            }
        }
    } 
    printf("Valid");
    return 0;
}

// Initialize map
int map_init(Map *v1, const char *file){
    FILE *fp = fopen(file, "rb");
    fscanf(fp, "%d %d", &v1->rows, &v1->cols);

    v1->cells = (unsigned char *)malloc(v1->rows * v1->cols * sizeof(unsigned char));
    if (v1->cells == NULL){
        return 0;
    }
    fclose(fp);
    return 1;
}

// Load map
int map_load(Map *v1,const char *file){
    FILE *fp = fopen(file, "rb");

    if (fp == NULL){
        return 0;
    }
    fscanf(fp,"%d %d", &v1->rows, &v1->cols);
    for (int i = 0; i < (v1->rows * v1->cols * 2); i++){
        fscanf(fp, "%hhd", &v1->cells[i]);
    }
    fclose(fp);
    return 1;
}

// Free memory from malloc
void free_map(Map *v1){
    free(v1->cells);
    v1->rows = 0;
    v1->cols = 0;
    v1->cells = NULL;
}

//Check if there is border
bool isborder(Map *map, int r, int c, int border){
    int index = (r-1) * map->cols + c - 1;

    switch(border){
        case 1: //left
            return map->cells[index] == 1 || map->cells[index] == 3 || map->cells[index] == 5;
        case 2: //right
            return map->cells[index] == 2 || map->cells[index] == 3 || map->cells[index] == 6;
        case 3: //down/up
            return map->cells[index] == 4 || map->cells[index] == 5 || map->cells[index] == 6;
        default:
            return false;
    }
}

//Check where your starting border is
int start_border(Map *map, int r, int c, int leftright){
    int index = (r-1) * map->cols + c - 1;
    if (leftright == 2){
        if (r % 2 == 1 && c == 1 && (map->cells[index] == 2 || map->cells[index] == 4 || map->cells[index] == 6)){
            crossedEdge = 1;
            return 2;
        }
        else if (r % 2 == 0 && c == 1 && (map->cells[index] == 2 || map->cells[index] == 4 || map->cells[index] == 6)){
            crossedEdge = 1;
            return 3; 
        }
        else if (r % 2 == 1 && c == map->cols && (map->cells[index] == 1 || map->cells[index] == 4 || map->cells[index] == 5)){
            crossedEdge = 2;
            return 3;
        }
        else if (r % 2 == 0 && c == map->cols && (map->cells[index] == 1 || map->cells[index] == 4 || map->cells[index] == 5)){
            crossedEdge = 2;
            return 1;
        }
        else if (r == 1 && (map->cells[index] == 1 || map->cells[index] == 2 || map->cells[index] == 3)){
            crossedEdge = 3;
            return 1;
        }
        else if (r == map->rows && (map->cells[index] == 1 || map->cells[index] == 2 || map->cells[index] == 3)){
            crossedEdge = 3;
            return 2;
        }
    }
    if (leftright == 1){
        if (r % 2 == 1 && c == 1 && (map->cells[index] == 2 || map->cells[index] == 4 || map->cells[index] == 6)){
            crossedEdge = 1;
            return 3;
        }
        else if (r % 2 == 0 && c == 1 && (map->cells[index] == 2 || map->cells[index] == 4 || map->cells[index] == 6)){
            crossedEdge = 1;
            return 2;
        }
        else if (r % 2 == 1 && c == map->cols && (map->cells[index] == 1 || map->cells[index] == 4 || map->cells[index] == 5)){
            crossedEdge = 2;
            return 1;
        }
        else if (r % 2 == 0 && c == map->cols && (map->cells[index] == 1 || map->cells[index] == 4 || map->cells[index] == 5)){
            crossedEdge = 2;
            return 3;
        }
        else if (r == 1 && (map->cells[index] == 1 || map->cells[index] == 2 || map->cells[index] == 3)){
            crossedEdge = 3;
            return 2;
        }
        else if (r == map->rows && (map->cells[index] == 1 || map->cells[index] == 2 || map->cells[index] == 3)){
            crossedEdge = 3;
            return 1;
        }
    }
    return 0;
}

//Find path from the maze by right hand rule
int rpath(Map *v1, int r, int c){
    printf("%d,%d\n", r, c);
    start_border(v1, r, c, 2);
    while((c > 0 && r > 0) && (c <= v1->cols && r <= v1->rows)){
        if (crossedEdge == 1){ //from the left
            if ((c+r) % 2 == 1){
                if (isborder(v1, r, c, 3) == false){
                    r += 1;
                    if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                        printf("%d,%d\n", r, c);
                    }
                    crossedEdge = 3;
                }
                else if(isborder(v1, r, c, 3) == true){
                    if (isborder(v1, r, c, 2) == false){
                        c += 1;
                        if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                            printf("%d,%d\n", r, c);
                        }
                        crossedEdge = 1;
                    }
                    else if(isborder(v1, r, c, 2) == true){
                        c -= 1;
                        if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                            printf("%d,%d\n", r, c);
                        }
                        crossedEdge = 2;
                    }
                }
            }
            else if ((c+r) % 2 == 0){
                if (isborder(v1, r, c, 2) == false){
                    c += 1;
                    if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                        printf("%d,%d\n", r, c);
                    }
                    crossedEdge = 1;
                }
                else if(isborder(v1, r, c, 2) == true){
                    if (isborder(v1, r, c, 3) == false){
                        r -= 1;
                        if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                            printf("%d,%d\n", r, c);
                        }
                        crossedEdge = 4;
                    }
                    else if (isborder(v1, r, c, 3) == true){
                        c -= 1;
                        if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                            printf("%d,%d\n", r, c);
                        }
                        crossedEdge = 2;
                    }
                }
            }
        }
        if (crossedEdge == 2){ //from the right
            if ((c+r) % 2 == 1){
                if (isborder(v1, r, c, 1) == false){
                    c -= 1;
                    if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                        printf("%d,%d\n", r, c);
                    }
                    crossedEdge = 2;
                }
                else if(isborder(v1, r, c, 1) == true){
                    if (isborder(v1, r, c, 3) == false){
                        r += 1;
                        if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                            printf("%d,%d\n", r, c);
                        }
                        crossedEdge = 3;
                    }
                    else if(isborder(v1, r, c, 3) == true){
                        c += 1;
                        if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                            printf("%d,%d\n", r, c);
                        }
                        crossedEdge = 1;
                    }
                }
            }
            else if ((c+r) % 2 == 0){
                if (isborder(v1, r, c, 3) == false){
                    r -= 1;
                    if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                        printf("%d,%d\n", r, c);
                    }
                    crossedEdge = 4;
                }
                else if(isborder(v1, r, c, 3) == true){
                    if (isborder(v1, r, c, 1) == false){
                        c -= 1;
                        if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                            printf("%d,%d\n", r, c);
                        }
                        crossedEdge = 2;
                    }
                    else if (isborder(v1, r, c, 1) == true){
                        c += 1;
                        if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                            printf("%d,%d\n", r, c);
                        }
                        crossedEdge = 1;
                    }
                }
            }
        }
        if (crossedEdge == 3){ //from the top
            if (isborder(v1, r, c, 1) == false){
                c -= 1;
                if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                    printf("%d,%d\n", r, c);
                }
                crossedEdge = 2;
            }
            else if (isborder(v1, r, c, 1) == true){
                if (isborder(v1, r, c, 2) == false){
                    c += 1;
                    if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                        printf("%d,%d\n", r, c);
                    }
                    crossedEdge = 1;
                }
                else if(isborder(v1, r, c, 2) == true){
                    r -= 1;
                    if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                        printf("%d,%d\n", r, c);
                    }
                    crossedEdge = 4;
                }
            }
        }
        if (crossedEdge == 4){ //from the bottom
            if (isborder(v1, r, c, 2) == false){
                c += 1;
                if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                    printf("%d,%d\n", r, c);
                }
                crossedEdge = 1;
            }
            else if (isborder(v1, r, c, 2) == true){
                if (isborder(v1, r, c, 1) == false){
                    c -= 1;
                    if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                        printf("%d,%d\n", r, c);
                    }
                    crossedEdge = 2;
                }
                else if(isborder(v1, r, c, 1) == true){
                    r += 1;
                    if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                        printf("%d,%d\n", r, c);
                    }
                    crossedEdge = 3;
                }
            }
            
        }
    }
    
    return 0;
}

//Find path from the maze by left hand rule
int lpath(Map *v1, int r, int c){
    printf("%d,%d\n", r, c);
    start_border(v1, r, c, 1);
    while((c > 0 && r > 0) && (c <= v1->cols && r <= v1->rows)){
        if (crossedEdge == 1){ //from the left
            if ((c+r) % 2 == 1){
                if (isborder(v1, r, c, 2) == false){
                    c += 1;
                    if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                        printf("%d,%d\n", r, c);
                    }
                    crossedEdge = 1;
                }
                else if(isborder(v1, r, c, 2) == true){
                    if (isborder(v1, r, c, 3) == false){
                        r += 1;
                        if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                            printf("%d,%d\n", r, c);
                        }
                        crossedEdge = 4;
                    }
                    else if(isborder(v1, r, c, 3) == true){
                        c -= 1;
                        if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                            printf("%d,%d\n", r, c);
                        }
                        crossedEdge = 2;
                    }
                }
            }
            else if ((c+r) % 2 == 0){
                if (isborder(v1, r, c, 3) == false){
                    r -= 1;
                    if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                        printf("%d,%d\n", r, c);
                    }
                    crossedEdge = 3;
                }
                else if(isborder(v1, r, c, 3) == true){
                    if (isborder(v1, r, c, 2) == false){
                        c += 1;
                        if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                            printf("%d,%d\n", r, c);
                        }
                        crossedEdge = 1;
                    }
                    else if (isborder(v1, r, c, 2) == true){
                        c -= 1;
                        if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                            printf("%d,%d\n", r, c);
                        }
                        crossedEdge = 2;
                    }
                }
            }
        }
        if (crossedEdge == 2){ //From the right
            if ((c+r) % 2 == 1){
                if (isborder(v1, r, c, 3) == false){
                    r += 1;
                    if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                        printf("%d,%d\n", r, c);
                    }
                    crossedEdge = 4;
                }
                else if(isborder(v1, r, c, 3) == true){
                    if (isborder(v1, r, c, 1) == false){
                        c -= 1;
                        if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                            printf("%d,%d\n", r, c);
                        }
                        crossedEdge = 2;
                    }
                    else if(isborder(v1, r, c, 1) == true){
                        c += 1;
                        if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                            printf("%d,%d\n", r, c);
                        }
                        crossedEdge = 1;
                    }
                }
            }
            else if ((c+r) % 2 == 0){
                if (isborder(v1, r, c, 1) == false){
                    c -= 1;
                    if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                        printf("%d,%d\n", r, c);
                    }
                    crossedEdge = 2;
                }
                else if(isborder(v1, r, c, 1) == true){
                    if (isborder(v1, r, c, 3) == false){
                        r -= 1;
                        if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                            printf("%d,%d\n", r, c);
                        }
                        crossedEdge = 3;
                    }
                    else if (isborder(v1, r, c, 3) == true){
                        c += 1;
                        if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                            printf("%d,%d\n", r, c);
                        }
                        crossedEdge = 1;
                    }
                }
            }
        }
        if (crossedEdge == 3){ //From the top
            if (isborder(v1, r, c, 1) == false){
                c -= 1;
                if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                    printf("%d,%d\n", r, c);
                }
                crossedEdge = 2;
            }
            else if (isborder(v1, r, c, 1) == true){
                if (isborder(v1, r, c, 2) == false){
                    c += 1;
                    if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                        printf("%d,%d\n", r, c);
                    }
                    crossedEdge = 1;
                }
                else if(isborder(v1, r, c, 2) == true){
                    r -= 1;
                    if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                        printf("%d,%d\n", r, c);
                    }
                    crossedEdge = 4;
                }
            }
        }
        if (crossedEdge == 4){ //from the bottom
            if (isborder(v1, r, c, 2) == false){
                c += 1;
                if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                    printf("%d,%d\n", r, c);
                }
                crossedEdge = 1;
            }
            else if (isborder(v1, r, c, 2) == true){
                if (isborder(v1, r, c, 1) == false){
                    c -= 1;
                    if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                        printf("%d,%d\n", r, c);
                    }
                    crossedEdge = 2;
                }
                else if(isborder(v1, r, c, 1) == true){
                    r -= 1;
                    if (r != 0 && c != 0 && r <= v1->rows && c <= v1->cols){
                        printf("%d,%d\n", r, c);
                    }
                    crossedEdge = 3;
                }
            }
            
        }
    }
    return 0;
}

int main(int argc,char **argv){
    Map v1;

    map_init(&v1, argv[argc - 1]);
    map_load(&v1, argv[argc - 1]);

    if (argc > 1){
        if (strcmp(argv[1], "--help") == 0){
            guide();
        }
        if (strcmp(argv[1], "--test") == 0){
            tester(&v1);
        }
        if (strcmp(argv[1], "--rpath") == 0){
            rpath(&v1, atoi(argv[2]), atoi(argv[3]));
        }
        if (strcmp(argv[1], "--lpath") == 0){
            lpath(&v1, atoi(argv[2]), atoi(argv[3]));
        }
    }
    free_map(&v1);
    return 0;
}