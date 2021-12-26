#define DEBUG
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

bool error_has_rised = false;


struct Pair {
    long x;
    long y;
} typedef Pair;


struct Cell {
    Pair coords;
    bool has_bomb;
} typedef Cell;


struct Field {
    Cell** ptr_to_cells_arr;
    long height;
    long width;
    long bombs_quantity;
} typedef Field;

Field* define_field(long _heigth, long _width, long _bomb_quantity) {
    if (_heigth < 0 || _width < 0 || _bomb_quantity < 0 || 
        (_heigth * _width < _bomb_quantity)) {
        error_has_rised = true;
        return;
    }
    Cell** cells_array = (Cell**)malloc(_heigth * sizeof(Cell*));
    for (unsigned i = 0; i < _heigth; ++i) {
        cells_array[i] = (Cell*)malloc(_width * sizeof(Cell));
    }
    if (cells_array == NULL) {
        error_has_rised = true;
        return;
    }
    for (unsigned i = 0; i < _width; ++i) {
        for (unsigned j = 0; j < _heigth; ++j) {
            cells_array[i][j].coords.x = i;
            cells_array[i][j].coords.y = j;
            cells_array[i][j].has_bomb = false;
        }
    }
    Field* field = (Field*) malloc(sizeof(Field));
    field->ptr_to_cells_arr = cells_array;
    field->bombs_quantity = _bomb_quantity;
    field->height = _heigth; 
    field->width = _width;
    Pair* bomb_positions = (Pair*)malloc(field->bombs_quantity * sizeof(Pair));
    for (unsigned i = 0; i < field->bombs_quantity; ++i) {
        bomb_positions->x = 0;
        bomb_positions->y = 0;
    }
    for (unsigned i = 0; i < field->bombs_quantity; ++i) {
        bomb_positions[i].x = rand()%_width;
        bomb_positions[i].y = rand()%_heigth;
        field->ptr_to_cells_arr[bomb_positions[i].x][bomb_positions[i].y].has_bomb = true;
    }
    free(bomb_positions);

    return field;
}

void print_field (Field* _field) {
    for (int i = 0; i < _field->width; ++i) {
        for (int j = 0; j < _field->height; ++j) {
            printf("{%i | %i | {%s}}\t", (_field->ptr_to_cells_arr)[i][j].coords.x, 
                                         (_field->ptr_to_cells_arr)[i][j].coords.y, 
                                         _field->ptr_to_cells_arr[i][j].has_bomb ? "true" : "false");
        }
        printf("\n");
    }
}

long bomb_count(Field* _field, long pos_x, long pos_y) {
    long count = 0;
    if (pos_x >= 0 && pos_y >= 0) {
        if (pos_y == 0) {
            if (pos_x == 0) {
                count = _field->ptr_to_cells_arr[pos_x + 1][pos_y    ].has_bomb ? count + 1 : count;
                count = _field->ptr_to_cells_arr[pos_x    ][pos_y + 1].has_bomb ? count + 1 : count;
                count = _field->ptr_to_cells_arr[pos_x + 1][pos_y + 1].has_bomb ? count + 1 : count;
            }
            else if (pos_x == _field->width - 1) {
                count = _field->ptr_to_cells_arr[pos_x    ][pos_y + 1].has_bomb ? count + 1 : count;
                count = _field->ptr_to_cells_arr[pos_x - 1][pos_y    ].has_bomb ? count + 1 : count;
                count = _field->ptr_to_cells_arr[pos_x - 1][pos_y + 1].has_bomb ? count + 1 : count;
            }
            else {
                count = _field->ptr_to_cells_arr[pos_x - 1][pos_y    ].has_bomb ? count + 1 : count;
                count = _field->ptr_to_cells_arr[pos_x - 1][pos_y + 1].has_bomb ? count + 1 : count;
                count = _field->ptr_to_cells_arr[pos_x    ][pos_y + 1].has_bomb ? count + 1 : count;
                count = _field->ptr_to_cells_arr[pos_x + 1][pos_y    ].has_bomb ? count + 1 : count;
                count = _field->ptr_to_cells_arr[pos_x + 1][pos_y + 1].has_bomb ? count + 1 : count;
            }
        }
    }
    return count;
}

void play(long _heigth, long _width, long _bombs_quantity) {
    Field* _field = define_field(_heigth, _width, _bombs_quantity);
    if (error_has_rised) {
        printf("Something went wrong\n");
        return;
    }
    print_field(_field); 
    
    free(_field);
}

int main() {
    srand(time(NULL));
    #ifdef DEBUG
    long _heigth = 5, _width = 5, _bombs_quantity = 0;
    Field* _field = define_field(_heigth, _width, _bombs_quantity);
    _field->ptr_to_cells_arr[0][0].has_bomb =
    _field->ptr_to_cells_arr[0][1].has_bomb =
    _field->ptr_to_cells_arr[1][1].has_bomb = 
    _field->ptr_to_cells_arr[2][1].has_bomb = 
    _field->ptr_to_cells_arr[3][1].has_bomb = 
    _field->ptr_to_cells_arr[4][1].has_bomb = 
    _field->ptr_to_cells_arr[4][0].has_bomb = true;
    print_field(_field);

    printf("%li",bomb_count(_field, 4, 0));
    free(_field);
    #else
    long _heigth = 0, _width = 0, _bombs_quantity = 0;
    scanf("%li %i %li", &_heigth, &_width, &_bombs_quantity);
    play(_heigth, _width, _bombs_quantity);
    #endif
    
    return error_has_rised ? -1 : 0;
}
