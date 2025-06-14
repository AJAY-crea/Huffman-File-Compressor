#include "huffman_decoder.cpp"
using namespace std;

int main(int argc,char * argv[]){
    if(argc!=3) {
        cerr << "Failed to detect files" << '\n';
        exit(1);
    }
    Huffman h1(argv[1], argv[2]);
    h1.compress();
    
    return 0;
}