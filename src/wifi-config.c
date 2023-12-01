#include <ncurses.h>
#include <stdlib.h>

char* selection_box(char* descriptor , char* choices[] , int choice_count) {
    int yMax , xMax;
    getmaxyx(stdscr , yMax , xMax);
    
    WINDOW* menuwin = newwin(choice_count + 2, 0 , 0 , 0);
    box(menuwin,0,0);
    refresh();
    wrefresh(menuwin);
    keypad(menuwin,TRUE);
   
    int input_key;
    int highlight = 0;

    while(1) {
        for(int i = 0; i < choice_count ; i++) {
            if(i == highlight) {
                wattron(menuwin, A_REVERSE);
            }
            mvwprintw(menuwin, i+1 , 1 , choices[i]);
            wattroff(menuwin , A_REVERSE); 
        }
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
    
    return choices[highlight];
}

int configure_wifi() {
    char* awnser;

    char* choices[] = {
        "ssid" ,
        "psk" ,
        "other" ,
        "There is no truth",
    };
    awnser = selection_box("This is A slection_box" , choices,4);
 
}

int main() {
    initscr(); 
    noecho();
   
    configure_wifi();

    endwin();
    return 0;
}
