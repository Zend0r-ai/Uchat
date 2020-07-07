#include <stdio.h>
#include <gtk/gtk.h>

#include "start.h"
#include "login.h"

int main(int argc,char *argv[]){

    gtk_init(&argc, &argv);
    init_start_window();
    gtk_widget_show_all(StartWindow);
    gtk_main();
    return 0;
}

