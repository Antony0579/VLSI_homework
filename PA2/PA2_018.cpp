#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
// #include <stdio.h>
// #include <stdlib.h>
#include <time.h> 

using namespace std;
using std::string;

class Function {
// 宣告 public 的成員
public:
    void clock_on();
    void clock_off();
    void readfile(string ,vector <string>);
    double START,END;
    vector <string> myString;
    
// 宣告 private 的成員
private:
    int a;
    int b;
};

//--------------------------------------------------------------------------------------------//
void Function::clock_on(){
    START = clock();
}

void Function::clock_off(){
    END = clock();
    cout << "Time_All:   " << clock()/CLOCKS_PER_SEC << "s" << endl;
	cout << "Time_Main:   " << (END - START) / CLOCKS_PER_SEC << "s" << endl;
}

void Function::readfile(string file_name,vector <string> myString){
    ifstream ifs(file_name, ios::in);
    if (!ifs.is_open()) {
        cout << "Failed to open file.\n";
    } 
    else {
        // 將檔案存成陣列
        int count = 0 ;
        string s;
        while (getline(ifs, s)) {

            istringstream ss(s);
            string token;
            //  分割空格
            while (getline(ss,token,' ')){
                myString.push_back(token);
                //cout << myString[count] << endl;
                count++;
            }
        }
    }
}

//--------------------------------------------------------------------------------------------//
struct Node{
    vector<Node *>front_Ptr;
    vector<Node *>next_Ptr;

    int front_count = 0;
    int next_count = -1;

    int mynode;
    int layer = -1;

    string mynode_f; 
    bool completed = 0;
       
};

// void look_front_Ptr(int i){
// // for (int i = 1; i <= node_num; i++){
//     int k = 0;
//     if( mylist[i].mynode_f == "i" ){
//         while( k <= mylist[i].front_count ){
//             cout << "None" <<","<< mylist[i].mynode << endl;
//         k++;}
//     k=0;}
//     else{
//         while( k <= mylist[i].front_count-1 ){
//         cout << mylist[i].front_Ptr[k]->mynode <<","<< mylist[i].mynode << endl;
//         k++;}
//     k=0;}   
// // }

void lookback(Node now_Node){
    if ( now_Node.front_Ptr[0~now_Node.front_Pt] == NULL){}
    else{}
}

//--------------------------------------------------------------------------------------------//
int main(){
    Function F;
    cout << "//------------------------------------------------//" << endl ;
    F.clock_on();
    //F.readfile();
//---------------------------------------------------//
      
    string file_name = "E://C_VScode//testcase1";
   //  string file_name = "D://VS_C++//pa2_0406//testcase1";
    string file_save = "testcase1.out";
    
    vector <string> myString;    

    // F.readfile(file_name,myString);
    ifstream ifs(file_name, ios::in);
    if (!ifs.is_open()) {
        cout << "Failed to open file.\n";
    } 
    else {
        // 將檔案存成陣列
        int count = 0 ;
        string s;
        while (getline(ifs, s)) {

            istringstream ss(s);
            string token;
            //  分割空格
            while (getline(ss,token,' ')){
                myString.push_back(token);
                //cout << myString[count] << endl;
                count++;
            }
        }
    }

    //  定義前三個參數
    int layer,node_num,line_num;
    layer = stoi(myString[0]);
    node_num = stoi(myString[1]);
    line_num = stoi(myString[2]);

    // 生成每個 node    //  避免記憶體錯誤增加
    Node mylist[line_num];
    // 生成陣列對應每個 line
    int linein[line_num];
    int lineout[line_num];

    //  取得 node1~node13 及他的 operator，輸入mylist
    int i = 0,j = 1;
    while(i < (node_num)*2 ){
        if(i % 2 == 0){  
            mylist[j].mynode = stoi(myString[i+3]);
        }
        else{
            mylist[j].mynode_f = myString[i+3][0];
            j++;
        }
        i++;
    }
    //cout << i << endl;

    i = (node_num)*2; j = 1;
    while( i >= (node_num)*2 && i < ((node_num+line_num)*2)){
        if(i % 2 == 0){  
            linein[j] = stoi(myString[i+3]);
            // cout << linein[j] <<",";
        }
        else{
            lineout[j] = stoi(myString[i+3]);
            // cout << lineout[j] << endl;
            j++;
        }
        i++;
    }
    //cout << i << endl;

    // //   印出 node1~node13
    // for (int i = 1; i <= 13; i++){
    //     cout << mylist[i].mynode << mylist[i].mynode_f << endl;
    // }

    // // 印出 line1~line17
    // for (int i = 1; i <= 17; i++){
    //     cout << linein[i] << lineout[i] << endl;
    // }

    // 連接mylist上下Node
    Node *Pointer;
    for (int i = 1; i <= line_num; i++){
        int pin = linein[i];
        int pout = lineout[i];

        Pointer = &mylist[pin];//  Pointer = Node[第i行--前面的Node]
        mylist[pout].front_Ptr.push_back(Pointer);
        //  儲存Pointer在(Node[第i行--後面的Node]的前一個Node指標)
        mylist[pout].front_count += 1;

        Pointer = &mylist[pout];//  Pointer = Node[第i行--後面的Node]
        mylist[pin].next_Ptr.push_back(Pointer);
        //  儲存Pointer在(Node[第i行--前面的Node]的後一個Node指標)
        mylist[pin].next_count += 1;
    }

    // //  確認指向Node正確
    // for (int i = 1; i <= node_num; i++){
    //     int k = 0;
    //     if( mylist[i].mynode_f == "i" ){
    //         cout << "None" <<","<< mylist[i].mynode << endl;
    //     }
    //     else{
    //         while( k <= mylist[i].front_count-1 ){
    //         cout << mylist[i].front_Ptr[k]->mynode <<","<< mylist[i].mynode << ","<< mylist[i].front_count << endl;
    //         k++;}
    //     k=0;}   
    // }

    //  放入初始(輸入)的Node
    for (int i = 1; i <= node_num; i++){
        if( mylist[i].mynode_f == "i" ){   // strcmp：比較字元相同回傳0
            mylist[i].front_Ptr.push_back(NULL);
            mylist[i].completed = 1;
        }
        mylist[i].layer +=1;
    }
    //  cout << mylist[1].front_Ptr[0] <<","<< mylist[1].mynode << endl;

    // Node *Pointer_N;
    // //  放入第一層的Node
    // for (int i = 1; i <= node_num; i++){

    //     int k = 0;
    //     int t = mylist[i].front_Ptr[k]->completed;

    //     while( k <= mylist[i].front_count-1 ){
    //         if(t == 1){
    //             mylist[i].layer +=1;
    //         }k++; 
    //     }k=0;

    // }

    for (int i = 1; i <= node_num; i++){
        cout << mylist[i].mynode <<","<<mylist[i].layer << endl;
    }

//---------------------------------------------------//

    F.clock_off();
    cout << "//------------------------------------------------//" << endl ;
    system("pause");
    return 0;
}