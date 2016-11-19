#include <tmgimage.h>
#include <iostream>
#include <fstream>

void usage() {
    std::cout << "applydiff imageone <diff file>" << std::endl;
}

int main(int argc, char **argv) {
    std::vector<TmgImageDiff> v;
    std::ifstream is;
    TmgImage one, two;
    TmgImageDiff diff;
    GtkWidget *window;
    GtkWidget *vbox;
    
    gtk_init(&argc, &argv);
    
    if (argc != 3) {
        usage();
    }
    
    is.open(argv[2]);
    if (!is) {
        std::cout << "Could not open " << argv[3] << std::endl;
        return -1;
    }
    
    one.loadFPImage(argv[1]);
    two.loadFPImage(argv[1]);
    
    while (is.read((char *)&diff, sizeof(TmgImageDiff))) {
        v.push_back(diff);
    }
    is.close();
    
    two.applyDifference(&v);
    
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	
    g_signal_connect(G_OBJECT (window), "destroy",
                      G_CALLBACK (gtk_main_quit), NULL);

    g_signal_connect(G_OBJECT (window), "delete_event",
                      G_CALLBACK (gtk_main_quit), NULL);
    
    
    vbox = gtk_hbox_new(FALSE, 0);
    gtk_container_add((GtkContainer *)window, vbox);
    
    gtk_box_pack_start((GtkBox *)vbox, one.widget(), FALSE, FALSE, 0);
    gtk_box_pack_start((GtkBox *)vbox, two.widget(), FALSE, FALSE, 0);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
