#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <time.h> 
#include "PA2_head.h"

using namespace std;
using std::string;

int main(int argc, char *argv[]){
// int main(){
    Function F;
    cout << "//------------------------------------------------//" << endl ;
    F.clock_on();
//---------------------------------------------------//

    string file_name = argv[1];
    string file_save = argv[2];
    // string file_name = "D://VS_C++//testcase//testcase2";
    // string file_save = "D://VS_C++//testcase2.out";   
    vector <string> myString = F.readfile(file_name);

    //  定義前三個參數
    int layer_num,node_num,line_num;
    layer_num = stoi(myString[0]);
    node_num = stoi(myString[1]);
    line_num = stoi(myString[2]);
    
    // 生成每個 node    //  避免記憶體錯誤增加
    Node mylist[line_num];
    // 生成陣列對應每個 line
    int linein[line_num];
    int lineout[line_num];
    F.data_in( mylist, linein, lineout, node_num, line_num);

    Node *Pointer;
    F.Connect_Node( Pointer, mylist, linein, lineout, node_num, line_num);

    F.do_ASAP(mylist, node_num, layer_num);
    F.do_ALAP(mylist, node_num, layer_num);
    
    // 計算所需加法器、乘法器
    vector<int> add_num;    int add_max = 0;
    vector<int> multi_num;  int multi_max = 0;    

    // #需更改輸入變數時，傳地址不傳值。
    // 因為傳值後，函式複製值，只在函式中計算，#計算後不會更改main中的變數
    // 傳地址後，可更改指向的變數值，可更改main中的變數
    // F.Count_operator(mylist, node_num, layer_num, add_num, multi_num, &add_max, &multi_max);
    
//---------------------------------------------------//
    //  初始排序
    for (int i = 1; i <= node_num; i++){
        mylist[i].layer_force_ASAP = mylist[i].layer_ASAP;
        mylist[i].layer_force_ALAP = mylist[i].layer_ALAP;
    }
    F.Count_operator(mylist, node_num, layer_num, add_num, multi_num, &add_max, &multi_max);
    
    //  初始化 self_force / predecessor_force / total_force 
    F.initial_force(mylist, layer_num, node_num);
    Qdis q_distribution[layer_num];
 

    //  將node放置於最小force那層
    for (int i = 1; i <= node_num; i++){
        if (mylist[i].completed != 1){
            F.Schedule(i, mylist);            
            
            //  重新計算probability
            F.recal_probability(mylist,node_num);        
            //  重新計算distribution    
            F.recal_distribution(mylist, q_distribution, layer_num, node_num);    
            //  重新計算self_force
            F.recal_self_force(mylist, q_distribution, layer_num, node_num);
            //  重新計算successor_force
            //F.recal_predecessor_force(mylist, q_distribution, layer_num, node_num);
            //  重新計算total_force
            F.recal_total_force(mylist, layer_num, node_num);
        }
    }
    // F.Count_operator(mylist, node_num, layer_num, add_num, multi_num, &add_max, &multi_max);

//---------------------------------------------------//
    F.clearfile(file_save);
    F.Saved_ASAP(file_save, mylist, node_num, layer_num, add_max, multi_max); //會干擾其他輸出
    // F.savefile(file_save,"\n");       cout <<  endl;
    // F.savefile(file_save,"-------");  cout << "-------";
    // F.savefile(file_save,"\n");       cout <<  endl;
    // F.Saved_ALAP(file_save, mylist, node_num, layer_num, add_max, multi_max);
    // F.Saved_TFS(file_save, mylist, node_num, layer_num, add_max, multi_max);
//---------------------------------------------------//
    cout <<  endl;
    F.clock_off();
    cout << "//------------------------------------------------//" << endl ;
    system("pause");
    return 0;
}
