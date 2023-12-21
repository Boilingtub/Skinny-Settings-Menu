#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define NULL ((void *)0)

typedef enum Window_Place{
    Top_Left,
    Top_Middle,
    Top_Right,
    Middle_Left,
    Middle_Middle,
    Middle_Right,
    Bottom_Left,
    Bottom_Middle,
    Bottom_Right
} Window_Place;

typedef struct Vec2 {
    int x;
    int y;
} Vec2;

//Returns Pointer To Int[2] 
Vec2 Window_Place_to_Position(Window_Place window_placement , Vec2 Max , Vec2 BoxSize) {
    Vec2 Pos;
    switch(window_placement) {
        case Top_Left:
            break;
        case Top_Middle:
            break;
        case Top_Right:
            break;
        case Middle_Left:
            break;
        case Middle_Middle:
            Pos.y = (Max.y/2) - (BoxSize.y/2);
            Pos.x = (Max.x/2) - (BoxSize.x/2);     
            break;
        case Middle_Right:
            break;
        case Bottom_Left:
            break;
        case Bottom_Middle:
            break;
        case Bottom_Right:
            break;
    }
    return Pos;
}

char* edit_box(char* descriptor) {
    echo();

}

int selection_box(char* descriptor , Vec2 box_size , Window_Place window_placement , const char* choices[], int choice_count) { 
    if(choice_count == 0) {
        printw("No choices provided to selection_box , choice_count = %d" , choice_count);
        getch();
    }

    Vec2 max_box_size;
    getmaxyx(stdscr , max_box_size.y , max_box_size.x);


    Vec2 Pos = Window_Place_to_Position(window_placement , max_box_size , box_size);    
    WINDOW* menuwin = newwin(box_size.y , box_size.x, Pos.y , Pos.x);
    box(menuwin,0,0);
    refresh();
    wrefresh(menuwin);
    keypad(menuwin,TRUE);

    int input_key;
    int highlight = 0;

    wattron(menuwin , A_BOLD);
    wattron(menuwin , A_UNDERLINE);
    mvwprintw(menuwin , 1 , 3 , descriptor);
    wattroff(menuwin , A_UNDERLINE);
    wattroff(menuwin , A_BOLD);

    while(1) {
        for(int i = 0; i < choice_count ; i++) {
            if(i == highlight) {
                wattron(menuwin, A_REVERSE);
            }
            mvwprintw(menuwin, i+3 , 4 , choices[i]);
            wattroff(menuwin , A_REVERSE); 
        }
        curs_set(0);
        input_key = wgetch(menuwin);

        switch(input_key) {
            case KEY_UP:
                highlight--;
                if(highlight == -1){
                    highlight = 0;
                }
                break;
            case KEY_DOWN:
                highlight++;
                if(highlight == choice_count) {
                    highlight = choice_count-1;
                }
                break;
            default:
                break;
        }
        if(input_key == 10) {
            break;
        }
    }
    curs_set(1); 
    return highlight;
}

char* execute_shell_command(char shell_cmd[]) {
    FILE *pipe;
    int buffer_size = sizeof(char)*1000;
    pipe = popen(shell_cmd , "r");
    
    if (NULL == pipe) {
        perror("pipe");
        exit(1);
    }

    

    char* buffer = malloc(buffer_size);
    if (buffer) {
        fread(buffer, 1 , buffer_size , pipe);
    }

    printw("length = %d", buffer_size);
    getch(); 
    //buffer[(length*sizeof(char))-1] = '\0';
    
    pclose(pipe);
    return buffer; 
}

/////////////////////////////////////////////////


int configure_wifi() {
    clear();
    
    char* cmd_output = execute_shell_command("iw dev | grep \"Interface\"");
    printw("Device_Name = %s \n\n",cmd_output);
    free(cmd_output);
    cmd_output = execute_shell_command("iw dev wlo1 scan | grep \"SSID\"");
    printw("Wifi_Networks = %s\n\n",cmd_output);

    getch();
    free(cmd_output);
}
int configure_brightness() {
    clear();
    printw("brightness");
    getch();
}
int configure_sound() {
    clear();
    printw("sound");
    getch();
}

int settings_menu() {
    const char* choices[] = {
        "Wifi",
        "Brightness",
        "Sound",
        "Quit"
    };
    int choice_count = sizeof(choices) / sizeof(char*);
    Vec2 box_size = { .y = choice_count + 4 , .x = 18};
    while(1) {
        clear();
        int awnser = selection_box("Settings",  box_size, Middle_Middle , choices , choice_count);
    
        switch(awnser) {
            case 0:
                configure_wifi();
            break;
            case 1:
                configure_brightness();
            break;
            case 2:
                configure_sound();
            break;
            case 3:
                return 0;
            break;
            default:
            break;
    }    
    }
}

int main(int argc , char *argv[]) {
    initscr(); 
    noecho();
   
    settings_menu();

    endwin();
    return 0;
}
