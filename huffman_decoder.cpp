#include <bits/stdc++.h>
#include <sys/stat.h>
using namespace std;

class Huffman;

uint32_t cnt = 0;

// node class representing a node in huffman tree
class node {
    unsigned char data;
    uint32_t freq;
    string code;
    node* left;
    node* right;
    uint32_t time = 0;
public:
    node() : left(NULL), right(NULL) {}
    friend class Huffman;
};

class Huffman {
    node* root;
    class comp {
    public:
        bool operator()(node* a, node* b) {
            return (a->freq == b->freq ? a->time > b->time : a->freq > b->freq);
        }
    };
    string inpfname, outfname;
    priority_queue<node*, vector<node*>, comp> pq;
    vector<node*> arr;
    vector<uint16_t> inputdata;
    vector<unsigned char> saveContents;
public:
    Huffman(string a, string b) : inpfname(a), outfname(b) { root = NULL; }
    void compress() {
        readinpfile();
        Huffmantree();
        encode();
        writeinoutput();
        return;
    }
    //read input file ,counts freq  and store contents
    void readinpfile() {
        arr.resize(256, NULL);
        for(int32_t i = 0; i < 256; i++) {
            arr[i] = new node();
            arr[i]->freq = 0;
            arr[i]->data = i;
            arr[i]->time = cnt++;
        }
        //open input file in binary mode
        ifstream file(inpfname, ios::binary);
        if(!file.is_open()) {
            cerr << "Error while opening the file" << '\n';
            exit(1);
        }
        unsigned char ch;
        //read file byte by byte
        while(file.read(reinterpret_cast<char*>(&ch), 1)) {
            inputdata.emplace_back(int(ch)); //store byte as integer
            arr[ch]->freq++;
        }
        file.close();
        return;
    }
    // creating hufman tree 
    void Huffmantree() {
        for(auto& c : arr) {
            if(c->freq > 0) pq.push(c);
        }
        while(pq.size() > 1) {
            node* l = pq.top();
            pq.pop();
            node* r = pq.top();
            pq.pop();
            root = new node();
            root->freq = l->freq + r->freq;
            root->left = l, root->right = r;
            root->time = cnt++;
            pq.push(root);
        }
        return;
    }
    //generates huffman codes by traversing the tree
    void encode() {
        if(root == NULL) {
            cerr << "EMPTY file case" << '\n';
            exit(1);
        }
        dfs(root, "");
        return;
    }
    //for assigning codes
    void dfs(node* cur, string s) {
        if(cur->left == NULL && cur->right == NULL) {
            cur->code = s;
            return;
        }
        if(cur->left != NULL) dfs(cur->left, s + '0');
        if(cur->right != NULL) dfs(cur->right, s + '1');
        return;
    }
    // write compressed data to output file
    void writeinoutput() {
        //open output file in binary mode
        ofstream file(outfname, std::ios::binary);
        if(!file.is_open()) {
            cerr << "Error while opening the output file" << endl;
            exit(1);
        }
        string s = "";
        uint32_t noOFCharsUsed = 0;
        map<uint16_t, pair<uint16_t, uint32_t>> mp;
        // amking metadata
        for(uint16_t i = 0; i < 256; i++) {
            uint32_t temp = arr[i]->freq;
            if(temp == 0) continue;
            noOFCharsUsed += 1;
            int16_t j = 31;
            while(temp >> j == 0) j--;
            mp[i] = {(j + 8) / 8, temp};
        }
        s += char(noOFCharsUsed);
        //writing metadata into output file :character, bytes for freq ,freq
        for(map<uint16_t, pair<uint16_t, uint32_t>>::iterator it = mp.begin(); it != mp.end(); ++it) {
            uint16_t k = it->first;
            pair<uint16_t, uint32_t> v = it->second;
            s += char(k);
            s += char(v.first);
            if(v.first == 1) {
                s += char(v.second);
            } else if(v.first == 2) {
                uint32_t freq16 = static_cast<uint16_t>(v.second);
                s += static_cast<char>(freq16 & 0xFF);
                s += static_cast<char>((freq16 >> 8) & 0xFF);
            } else {
                //writing 4 byte freq in little endian order
                uint32_t freq = v.second;
                s += static_cast<char>(freq & 0xFF);
                s += static_cast<char>((freq >> 8) & 0xFF);
                s += static_cast<char>((freq >> 16) & 0xFF);
                s += static_cast<char>((freq >> 24) & 0xFF);
            }
            //flush string to file
            while(!s.empty()) {
                char t = s[0];
                s.erase(0, 1);
                file.put(t);
            }
        }
        string temp = "";
        //write encoded data
        for(auto& c : inputdata) {
            if(c < 0 || c >= 256) {
                cout << -1 << endl;
                return;
            }
            temp += arr[c]->code;
            while(temp.size() > 8) {
                string AppendToS = temp.substr(0, 8);
                file.put(convertTOChar(AppendToS));
                temp.erase(0, 8);
            }
        }
        uint32_t appi = 0;
        //for remaining bits
        if((!temp.empty()) && temp.size() % 8 != 0) {
            appi = 8 - (temp.size() % 8); //padding needed 
            temp += string(8 - (temp.size() % 8), '0'); //padd with zeros
        }
        //write final byte and padding count
        if(!temp.empty()) {
            file.put(convertTOChar(temp));
            file.put(char(appi));
        }
        file.close();
        return;
    }
    //converting binary string to characters 
    char convertTOChar(string str) {
        uint32_t ans = 0;
        for(int i = 7; i >= 0; i--) {
            ans = ans * 2;
            ans += (str[i] == '1');
        }
        return char(ans);
    }
    // restore contents by decoding ,creating huffman tree 
    void decompress() {
        int j = 0;
        restoreContents(j);
        createTree();
        decodingAndWrite(j);
        return;
    }
    void restoreContents(int& temp) {
        // intialize array for 256 chars
        arr.resize(256);
        for(int32_t i = 0; i < 256; i++) {
            arr[i] = new node();
            arr[i]->freq = 0;
            arr[i]->data = i;
            arr[i]->time = cnt++;
        }
        ifstream file(inpfname, ios::binary);
        if(!file.is_open()) {
            cerr << "Unable to open file" << '\n';
            exit(1);
        }
        unsigned char ch;
        // read no of distinct characters
        file.read(reinterpret_cast<char*>(&ch), 1);
        temp++;
        uint16_t noOFchars = int(ch);
        // read metadata for each character
        for(int i = 0; i < noOFchars; i++) {
            file.read(reinterpret_cast<char*>(&ch), 1);
            int32_t Char = int(ch);
            file.read(reinterpret_cast<char*>(&ch), 1);
            uint16_t j = int(ch);
            temp += 2;
            uint32_t fre;
            // read freq based on byte count
            if(j > 2) {
                char32_t wideChar;
                file.read(reinterpret_cast<char*>(&wideChar), sizeof(char32_t));
                temp += 4;
                fre = static_cast<uint32_t>(wideChar);
            } else if(j == 2) {
                char16_t wideChar;
                file.read(reinterpret_cast<char*>(&wideChar), sizeof(char16_t));
                temp += 2;
                fre = static_cast<uint16_t>(wideChar);
            } else {
                file.read(reinterpret_cast<char*>(&ch), sizeof(char));
                temp++;
                fre = static_cast<uint8_t>(ch);
            }
            arr[Char]->freq = fre;
        }
        // read remaining  compressed data
        unsigned char cha;
        while(file.read(reinterpret_cast<char*>(&cha), 1)) {
            saveContents.push_back(cha);
        }
        file.close();
        return;
    }
    void createTree() {
        for(int i = 0; i < 256; i++) {
            if(arr[i]->freq > 0) pq.push(arr[i]);
        }
        while(pq.size() > 1) {
            auto l = pq.top();
            pq.pop();
            auto r = pq.top();
            pq.pop();
            root = new node();
            root->freq = l->freq + r->freq;
            root->left = l, root->right = r;
            root->time = cnt++;
            pq.push(root);
        }
        return;
    }
    //decode compressed data and writes output file
    void decodingAndWrite(int pos) {
        ofstream file1(outfname, ios::binary | ios::out);
        if(!file1) {
            cerr << "Failed to open output file!" << endl;
            return;
        }
        int temp = 0, idx = 0;
        //read padding count from last byte
        string append = std::bitset<8>(int(saveContents.back())).to_string();
        while(idx < 8) {
            temp *= 2;
            temp += (append[idx++] == '1');
        }
        node* travel = root;
        for(int i = 0; i < saveContents.size() - 1; i++) {
            string path = std::bitset<8>(int(saveContents[i])).to_string();
            reverse(path.begin(), path.end());   // reverse for correct bit order
            //adjust last byte to exclude padding
            if(i == saveContents.size() - 2) path = path.substr(0, 8 - (temp));
            // traverse tree based on bits
            for(int j = 0; j < path.size(); j++) {
                travel = (path[j] == '0' ? travel->left : travel->right);
                if(travel->left == NULL && travel->right == NULL) {
                    file1.put(travel->data);
                    travel = root;
                }
            }
        }
        file1.close();
        return;
    }
};