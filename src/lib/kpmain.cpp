#include <iostream>
#include <keypad.h>

using namespace std;

int main(int argc, char **argv) {
	GtkWidget *window;
	KEYPAD kp;

	gtk_init(&argc, &argv);
	
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	
	g_signal_connect (G_OBJECT (window), "destroy",
		G_CALLBACK (gtk_main_quit), NULL);

	g_signal_connect (G_OBJECT (window), "delete_event",
		G_CALLBACK (gtk_main_quit), NULL);
	
	gtk_widget_show_all(window);
	
	kp.init();
	std::cout << "Keypad entry: " << kp.run() << std::endl;
	
	gtk_main();
	return 0;
}