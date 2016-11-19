#include <tmgimage.h>
#include <iostream>
#include <fstream>

void usage() {
    std::cout << "diffimages imageone imagetwo <output file>" << std::endl;
}

int main(int argc, char **argv) {
    std::vector<TmgImageDiff> v;
    std::ofstream os;
    TmgImage one, two;
    
    gtk_init(&argc, &argv);
    
    if (argc != 4) {
        usage();
    }
    
    os.open(argv[3]);
    if (!os) {
        std::cout << "Could not open " << argv[3] << std::endl;
        return -1;
    }
    
    one.loadFPImage(argv[1]);
    two.loadFPImage(argv[2]);
    
    one.getDifference(&two, &v);
    
    for (std::vector<TmgImageDiff>::const_iterator it = v.begin(); it != v.end(); ++it) {
        TmgImageDiff diff(*it);
        os.write((char *)&diff, sizeof(TmgImageDiff));
    }
    
    os.close();
}
