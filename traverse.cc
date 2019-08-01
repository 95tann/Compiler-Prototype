#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Node {
    int data;
    int children;
    vector<Node> childvec;
};

vector<Node> getchild(int num){

    vector<Node> voc;
    int inum1;
    int inum2;

    for (int i=0; i < num; i++){
        cin >> inum1 >> inum2;
        Node newNode;
        newNode.data = inum1;
        newNode.children = inum2;

        if (inum2 == 0){
            voc.push_back(newNode);
        }
        else{
            newNode.childvec = getchild(inum2);
            voc.push_back(newNode);
        }
    }

    return voc;
}

void printer(vector<Node> voc){
    int len = voc.size();

    for(int i = 0; i < len; i++){
        if (voc[i].children == 0){
            cout << voc[i].data << " " << voc[i].children << endl;
        }
        else{
            printer(voc[i].childvec);
            cout << voc[i].data << " " << voc[i].children << endl;

        }
    }
}

int main(){

    int num1;
    int num2;
    cin >> num1 >> num2;

    Node mynode;
    mynode.data = num1;
    mynode.children = num2;
    mynode.childvec = getchild(num2);

    printer(mynode.childvec);

    return 0;
}
