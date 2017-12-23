#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <ncurses.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include <stdint.h>

#include "version.h"

#define YX2I(y,x) (y * g_col   + x)
#define I2YX(i)  i/g_col, i%g_col

/*
* red-pill matrix
*/
char *g_theematrix;
int g_row, g_col;


typedef enum { P_BIN, P_CHAR, P_FILE } pattern_t;

/*
* 
*/

static char generator(pattern_t p)
{
    char randomchar;
    switch (p) {
    case P_BIN:
        randomchar = "01"[random() % 2];
        break;
    case P_CHAR:
        randomchar =
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#$%^&*()"
            [random() % 71];
        break;
	case P_FILE:
		// TODO:
		break;
    default:
        assert(0);
    }
    return randomchar;
}

static void terminate(char *msg)
{
    endwin();
    printf("%s\n", msg ? msg : "Note: unplugged from the matrix");
    exit(0);
}

void draw_pixil(int y, int x)
{
    move(y, x);
    addch(g_theematrix[YX2I(y,x)]);
    refresh();
}


void usage(){
            printf("tmatrix [OPTION]\n");
			printf("\t -b binary (default is char)\n");
			printf("\t -v print version and exit\n");
}
// Yet another main
void main(int argc, char **argv)
{
    int opt;
    int x, y;
    int i;

	// usec
	uint32_t refresh_rate;

    // CLI arguments
    pattern_t pattern_gen_flag = P_CHAR;

    while ((opt = getopt(argc, argv, "vb")) != -1) {
        switch (opt) {
        case 'v':
            printf("%s\n", VERSION_STRING);
            exit(0);
        case 'b':
            pattern_gen_flag = P_BIN;
            break;
        default:
			printf("Error: unknown option\n");
			usage();
            exit(0);
        }
    }


    srandom(time(NULL));

    // init ncurses
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    if (has_colors() == FALSE) {
        terminate("Error: color is not detected");
    }
    start_color();

    // Color pairs (green chars on black background)
    init_pair(1, COLOR_GREEN, COLOR_BLACK);     // chars
    attron(COLOR_PAIR(1));

    do {
        getmaxyx(stdscr, g_row, g_col);
        g_theematrix = malloc(sizeof(char) * g_row * g_col);
		for(i =0 ; i < g_col * g_row; i++)
		{
			g_theematrix[i] = generator(pattern_gen_flag);
			draw_pixil(I2YX(i));
			usleep(refresh_rate);
		}
        free(g_theematrix);
    } while (1);

  cleanup:
    terminate(NULL);
}

