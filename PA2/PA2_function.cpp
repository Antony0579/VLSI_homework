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

void Function::clock_on(){
    START = clock();
}

void Function::clock_off(){
    END = clock();
    cout << "Time_All:   " << clock()/CLOCKS_PER_SEC << "s" << endl;
	cout << "Time_Main:   " << (END - START) / CLOCKS_PER_SEC << "s" << endl;
}

vector<string> Function::readfile(string file_name){
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
    return myString;
}

void Function::clearfile(string file_save) {
    ofstream ofs;
    std::vector<std::string> str;
    str.push_back("");
    
    ofs.open(file_save);
    if (!ofs.is_open()) {
        cout << "Failed to open file.\n";
    } else {
        for (auto &s : str) {
            ofs << s;
        }
        ofs.close();
    }
}
void Function::savefile(string file_save,string swd) {
    ofstream ofs;
    std::vector<std::string> str;
    str.push_back(swd);
    
    ofs.open(file_save,ofstream::out | ofstream::app);
    if (!ofs.is_open()) {
        cout << "Failed to open file.\n";
    } else {
        for (auto &s : str) {
            ofs << s;
        }
        ofs.close();
    }
}
// Node mylist,vector <string> myString
void Function::data_in(Node mylist[], int linein[], int lineout[], int node_num, int line_num){
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
    // for (int i = 1; i <= node_num; i++){
    //     cout << mylist[i].mynode <<","<< mylist[i].mynode_f << endl;
    // }

    // // 印出 line1~line17
    // for (int i = 1; i <= line_num; i++){
    //     cout << linein[i] <<","<< lineout[i] << endl;
    // }
}

void Function::Connect_Node(Node *Pointer, Node mylist[], int linein[], int lineout[], int node_num, int line_num){
    // 連接mylist上下Node
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
    //         cout << F.lookback(mylist[i],k) <<","<< mylist[i].mynode << ","<< mylist[i].front_count << endl;
    //         k++;}
    //     k=0;}   
    // }
}

//  取得前面的Node,第k個
int Function::lookback(Node now_Node,int k){
    int j =now_Node.front_Ptr[k]->mynode;
    return j;
}
int Function::looknext(Node now_Node,int k){
    int j =now_Node.next_Ptr[k]->mynode;
    return j;
}

void Function::do_ASAP(Node mylist[], int node_num, int layer_num){
    //ASAP----------------------------ASAP
    Function F;
    //  放入初始(輸入)的Node
    for (int i = 1; i <= node_num; i++){
        if( mylist[i].mynode_f == "i" ){   
            mylist[i].front_Ptr.push_back(NULL);
            mylist[i].layer_ASAP = 0;
        }
        else if( mylist[i].mynode_f == "o" ){
            mylist[i].layer_ASAP = layer_num+1;
        }
        else{
            mylist[i].layer_ASAP +=1;
        }
    }
    // //-->
    // for (int i = 1; i <= node_num; i++){
    // cout << mylist[i].layer_ASAP <<","<< mylist[i].mynode << endl;
    // }
    // cout <<"-------------------"<<endl;

    // 比較我前面的Node,誰最靠後(layer最大)
    for (int i = 1; i <= node_num; i++){
        if( mylist[i].mynode_f == "i" ){}
        else if( mylist[i].mynode_f == "o" ){}

        else if( mylist[i].mynode_f == "+" ){
            int max = 0;

            for (int k = 0; k <= mylist[i].front_count-1; k++){
                int t = F.lookback(mylist[i],k); // t = mylist[i]前面的Node,第k號(k+1個)
                //  比大小
                if ( mylist[t].layer_ASAP >= max){
                    max = mylist[t].layer_ASAP;
                }
                // cout << mylist[i].mynode <<","<< mylist[i].front_Ptr[k]->mynode << endl;
            }
            mylist[i].layer_ASAP = max + 1; //  下一層的層數+1
        } 

        else if( mylist[i].mynode_f == "*" ){
            int max = 0;
            for (int k = 0; k <= mylist[i].front_count-1; k++){
                int t = F.lookback(mylist[i],k); // t = mylist[i]前面的Node,第k個
                //  比大小
                if ( mylist[t].layer_ASAP >= max){
                    max = mylist[t].layer_ASAP;
                }
            }
            mylist[i].layer_ASAP = max + 3;
        }     
    }

    // 將乘法器的Node向前移動
    for (int i = 1; i <= node_num; i++){
        if( mylist[i].mynode_f == "*" ){
            mylist[i].layer_ASAP = mylist[i].layer_ASAP -2;
        }
    }
}

void Function::do_ALAP(Node mylist[], int node_num, int layer_num){
//ALAP----------------------------ALAP
    Function F;
    //  從底層開始看起
    //  放入初始(輸出)的Node
    for (int i = 1; i <= node_num; i++){  
        if( mylist[i].mynode_f == "o" ){  
            mylist[i].next_Ptr.push_back(NULL);
            mylist[i].layer_ALAP = layer_num+1;
        }
        else if( mylist[i].mynode_f == "i" ){
            mylist[i].layer_ALAP = 0;
        }
        else{
            mylist[i].layer_ALAP = layer_num;
        }
    }

    // //-->
    // for (int i = 1; i <= node_num; i++){
    // cout << mylist[i].layer_ALAP <<","<< mylist[i].mynode << endl;
    // }
    // cout <<"-------------------"<<endl;

    // 比較我前面的Node,誰最靠後(layer最大)
    for (int i = node_num; i >= 1; i--){
        if( mylist[i].mynode_f == "i" ){}
        else if( mylist[i].mynode_f == "o" ){}
        else if( mylist[i].mynode_f == "+" ){
            int min = layer_num+1;
            for (int k = mylist[i].next_count-1; k >= 0; k--){
                int t = F.looknext(mylist[i],k); // t = mylist[i]後面的Node,第k號
                //  比大小
                if ( mylist[t].layer_ALAP <= min){
                    min = mylist[t].layer_ALAP;
                }
            }
            mylist[i].layer_ALAP = min-1;
        } 
        else if( mylist[i].mynode_f == "*" ){
            int min = layer_num+1;
            for (int k = mylist[i].next_count-1; k >= 0; k--){
                int t = F.looknext(mylist[i],k); // t = mylist[i]後面的Node,第k個
                //  比大小
                if ( mylist[t].layer_ALAP <= min){
                    min = mylist[t].layer_ALAP;
                }
            }
            mylist[i].layer_ALAP = min - 3;
        } 
    }
}

void Function::Count_operator(Node mylist[], int node_num, int layer_num, vector<int> add_num, vector<int> multi_num, int *add_max, int *multi_max){
    
    // add_max = 0;
    // multi_max = 0;
    for (int i = 0; i < layer_num; i++){
        add_num.push_back(0);
        multi_num.push_back(0);    
    }

    
    for (int i = 0; i < layer_num; i++){
        for (int j = 1; j <= node_num; j++){            
            if (i+1 == mylist[j].layer_force_ASAP){
                if( mylist[j].mynode_f == "+" ){
                    add_num[i]++;  //cout << add_num[i] <<endl;
                }
                else if( mylist[j].mynode_f == "*"){
                    multi_num[i]++; //cout << multi_num[i] <<endl;
                    multi_num[i+1]++;
                    multi_num[i+2]++;
                }
            }
        }
        //  比大小
        if ( add_num[i] >= *add_max){
            *add_max = add_num[i];
            // cout << *add_max <<endl;
        }
        if ( multi_num[i] >= *multi_max){
            *multi_max = multi_num[i];
        }
    }
}

void Function::Saved_ASAP(string file_save, Node mylist[], int node_num, int layer_num, int add_max, int multi_max){
    
    //ASAP----------------------------ASAP_Saved
    Function F;
    cout << add_max << endl;
    F.savefile(file_save,to_string(add_max)); F.savefile(file_save,"\n");
    cout << multi_max << endl;
    F.savefile(file_save,to_string(multi_max));   F.savefile(file_save,"\n");

    int copy[node_num];
    for (int i = 1; i <= node_num; i++){
        copy[i] = 1;   
    }

    int frontsp = 0;
    for (int i = 1; i <= layer_num; i++){
        for (int j = 1; j <= node_num; j++){
            if (i == mylist[j].layer_ASAP){

                frontsp ++;
                if (frontsp == 1){
                    cout << mylist[j].mynode;
                    string save_node =  to_string(mylist[j].mynode);
                    F.savefile(file_save,save_node);
                }
                else{
                    cout << " " << mylist[j].mynode;
                    string save_node =  to_string(mylist[j].mynode);
                    F.savefile(file_save," " + save_node);
                }

                if( (mylist[j].mynode_f == "*") && (copy[j] < 3)){
                    mylist[j].layer_ASAP ++;
                    copy[j]++;
                }
            }
        }
        //F.erase_file(" ")
        if (i != layer_num){
            frontsp = 0;
            cout <<  endl;
            F.savefile(file_save,"\n");
        }
    }
}   
void Function::Saved_ALAP(string file_save, Node mylist[], int node_num, int layer_num, int add_max, int multi_max){

    //ALAP----------------------------ALAP_Saved
    Function F;
    cout << add_max << endl;
    F.savefile(file_save,to_string(add_max)); F.savefile(file_save,"\n");
    cout << multi_max << endl;
    F.savefile(file_save,to_string(multi_max));   F.savefile(file_save,"\n");

    int copy[node_num];
    for (int i = 1; i <= node_num; i++){
        copy[i] = 1;   
    }

    int frontsp = 0;
    for (int i = 1; i <= layer_num; i++){
        for (int j = 1; j <= node_num; j++){
            if (i == mylist[j].layer_ALAP){

                frontsp ++;
                if (frontsp == 1){
                    cout << mylist[j].mynode;
                    string save_node =  to_string(mylist[j].mynode);
                    F.savefile(file_save,save_node);
                }
                else{
                    cout << " " << mylist[j].mynode;
                    string save_node =  to_string(mylist[j].mynode);
                    F.savefile(file_save," " + save_node);
                }

                if( (mylist[j].mynode_f == "*") && (copy[j] < 3)){
                    mylist[j].layer_ALAP ++;
                    copy[j]++;
                }
            }
        }
        //F.erase_file(" ")
        if (i != layer_num){
            frontsp = 0;
            cout <<  endl;
            F.savefile(file_save,"\n");
        }
    }
}

//  重新計算每個node的probability
void Function::recal_probability(Node mylist[], int node_num){
    
    for (int i = 1; i <= node_num; i++){
        mylist[i].probability = 0;
    }
    for (int i = 1; i <= node_num; i++){
        float  mu_mobility = mylist[i].layer_force_ALAP - mylist[i].layer_force_ASAP;
        mylist[i].probability = 1/( 1 + mu_mobility);
        // cout << mylist[i].mynode <<","<< mylist[i].probability << endl;
        if (mylist[i].probability == 1){
            mylist[i].completed = 1;
        }
    }
}

//  重新計算各層distribution
void Function::recal_distribution(Node mylist[], Qdis q_distribution[], int layer_num, int node_num){
    
    //  歸零q_distribution
    for (int i = 1; i <= layer_num; i++){
        q_distribution[i].add = 0;
        q_distribution[i].multi = 0;
    }

    for (int j = 1; j <= node_num; j++){        
        for (int i = 1; i <= layer_num; i++){
            if (i == mylist[j].layer_force_ASAP && mylist[j].mynode_f == "+"){
                q_distribution[i].add = q_distribution[i].add + mylist[j].probability;
            }
            if (i == mylist[j].layer_force_ASAP && mylist[j].mynode_f == "*"){
                q_distribution[i].multi = q_distribution[i].multi + mylist[j].probability;
            }
        }
        if (mylist[j].completed != 1){
            if (mylist[j].mynode_f == "+"){
                for (int k = mylist[j].layer_force_ASAP+1; k <= mylist[j].layer_force_ALAP; k++){
                    q_distribution[k].add = q_distribution[k].add + mylist[j].probability;
                }
            }
            else if (mylist[j].mynode_f == "*"){
                for (int k = mylist[j].layer_force_ASAP+1; k <= mylist[j].layer_force_ALAP; k++){
                    q_distribution[k].multi = q_distribution[k].multi + mylist[j].probability;
                }
            }     
        }
    }

//--->check probability and q_distribution-----------//+
    // for (int i = 1; i <= node_num; i++){
    //     cout << mylist[i].mynode <<","<< mylist[i].probability << endl;
    // }
    // cout << "-------" << endl; 
    // for (int i = 1; i <= layer_num; i++){
    //     cout << i <<","<< q_distribution[i].add << endl;
    // }
//--->check probability and q_distribution-----------//-
}

void Function::initial_force(Node mylist[], int layer_num, int node_num){
    for (int i = 1; i <= node_num; i++){
        if (mylist[i].probability != 1){
            for (int k = 0; k <= layer_num; k++){
                mylist[i].self_force.push_back(0);
                mylist[i].predecessor_force.push_back(0);
                mylist[i].total_force.push_back(0);
            }
        }
        else{
            mylist[i].total_force.push_back(0);
            mylist[i].total_force.push_back(0);
        }
    }
}

//  重新計算各點self_force
void Function::recal_self_force(Node mylist[], Qdis q_distribution[], int layer_num, int node_num){
    
    //  歸零self_force
    for (int i = 1; i <= node_num; i++){
        if (mylist[i].completed != 1){
            for (int k = 0; k <= layer_num; k++){
                mylist[i].self_force[k] = 0;
            }
        }
    }

    for (int i = 1; i <= node_num; i++){
        if (mylist[i].completed != 1){
            float q_All_add = 0;
            float q_All_multi = 0;
            for (int k = mylist[i].layer_force_ASAP; k <= mylist[i].layer_force_ALAP; k++){ // mylist[5].layer_force_ALAP =4
                q_All_add = q_All_add + q_distribution[k].add; 
                q_All_multi = q_All_multi + q_distribution[k].multi; 
            }
            for (int k = mylist[i].layer_force_ASAP; k <= mylist[i].layer_force_ALAP; k++){
                if (mylist[i].mynode_f == "+"){
                    mylist[i].self_force[k] = q_distribution[k].add - mylist[i].probability * q_All_add;
                }
                if (mylist[i].mynode_f == "*"){
                    mylist[i].self_force[k] = q_distribution[k].multi - mylist[i].probability * q_All_multi;
                }
            }  
        }
    }
}
//--->check self_force-----------//+
    // for (int i = 1; i <= node_num; i++){
    //     if (mylist[i].completed != 1){
    //         for (int k = 1; k <= layer_num; k++){
    //             cout << i <<","<< mylist[i].layer_ALAP <<","<< k <<","<< mylist[i].self_force[k] << endl;
    //         }
    //     }
    //     else{
    //        cout << i <<","<< mylist[i].layer_ALAP <<","<< 1 <<","<< mylist[i].self_force[1] << endl; 
    //     }
    // }
//--->check self_force-----------//-

void Function::recal_total_force(Node mylist[], int layer_num, int node_num){
    for (int i = 1; i <= node_num; i++){
        if (mylist[i].completed != 1){
            for (int k = 0; k <= layer_num; k++){
                mylist[i].total_force[k] = mylist[i].self_force[k] + mylist[i].predecessor_force[k];
            }
        }
    }
}

void Function::Schedule(int i, Node mylist[]){
    for (int j = mylist[i].layer_force_ASAP; j <= mylist[i].layer_force_ALAP; j++){
        if (mylist[i].total_force[j] < mylist[i].total_force_min){
            mylist[i].total_force_min = mylist[i].total_force[j];
            mylist[i].total_force_min_layer = j;
            // mylist[i].completed = 1;
            // cout << i <<","<< mylist[i].total_force_min <<endl;
        }
    }
    mylist[i].completed = 1;
    mylist[i].layer_force_ASAP = mylist[i].total_force_min_layer;
}

void Function::Saved_TFS(string file_save, Node mylist[], int node_num, int layer_num, int add_max, int multi_max){
    
    //ASAP----------------------------ASAP_Saved
    Function F;
    cout << add_max << endl;
    F.savefile(file_save,to_string(add_max)); F.savefile(file_save,"\n");
    cout << multi_max << endl;
    F.savefile(file_save,to_string(multi_max));   F.savefile(file_save,"\n");

    int copy[node_num];
    for (int i = 1; i <= node_num; i++){
        copy[i] = 1;   
    }

    int frontsp = 0;
    for (int i = 1; i <= layer_num; i++){
        for (int j = 1; j <= node_num; j++){
            if (i == mylist[j].layer_force_ASAP){

                frontsp ++;
                if (frontsp == 1){
                    cout << mylist[j].mynode;
                    string save_node =  to_string(mylist[j].mynode);
                    F.savefile(file_save,save_node);
                }
                else{
                    cout << " " << mylist[j].mynode;
                    string save_node =  to_string(mylist[j].mynode);
                    F.savefile(file_save," " + save_node);
                }

                if( (mylist[j].mynode_f == "*") && (copy[j] < 3)){
                    mylist[j].layer_force_ASAP ++;
                    copy[j]++;
                }
            }
        }
        //F.erase_file(" ")
        if (i != layer_num){
            frontsp = 0;
            cout <<  endl;
            F.savefile(file_save,"\n");
        }
    }
}

void Function::recal_predecessor_force(Node mylist[], Qdis q_distribution[], int layer_num, int node_num){
    Function F;
    for (int i = 1; i <= node_num; i++){ // i node, j layer, p/, k front_count, b lookback
        if (mylist[i].completed != 1){

            int iAS = mylist[i].layer_force_ASAP;
            int iLS = mylist[i].layer_force_ALAP;
            int mu = iLS - iAS;
            float PSF_d[iLS];
            for (int p = iAS; p <= iLS ; p++){
                PSF_d[p] = 0;
            }
                 
            for (int j = iAS; j <= iLS; j++){
                mylist[i].completed = 1;
                //  看前面
                for (int k = 0; k <= mylist[i].front_count-1; k++){
                    int b = F.lookback(mylist[i],k); // t = mylist[i]前面的Node,第k號(k+1個)

                    if (mylist[b].completed != 1){

                        if (mylist[b].mynode_f == "+"){
                            PSF_d[j] += ( q_distribution[j].add - mylist[b].self_force[j]);
                        }
                        else if (mylist[b].mynode_f == "*"){
                            PSF_d[j] += ( q_distribution[j].multi - mylist[b].self_force[j]);
                        }

                        // cout << i << "," <<  j << "," << b << "," << PSF_d[j] << endl;
                        mylist[b].layer_force_ASAP++;
                        if (mylist[b].layer_force_ASAP == mylist[b].layer_force_ALAP){
                            mylist[b].completed = 1;
                        }
                        else{   
                            F.recal_probability(mylist,node_num);                        
                            F.recal_distribution(mylist, q_distribution, layer_num, node_num);  
                            F.recal_self_force(mylist, q_distribution, layer_num, node_num);
                        }
                    }                    
                }
                
                //  看後面
                for (int k = 0; k <= mylist[i].next_count-1; k++){
                    int n = F.looknext(mylist[i],k); // t = mylist[i]前面的Node,第k號(k+1個)
                    if (mylist[n].completed != 1){

                        if (mylist[n].mynode_f == "+"){
                            PSF_d[j] += ( q_distribution[j].add - mylist[n].self_force[j]);
                        }
                        else if (mylist[n].mynode_f == "*"){
                            PSF_d[j] += ( q_distribution[j].multi - mylist[n].self_force[j]);
                        }

                        // cout << i << "," <<  j << "," << n << "," << q_distribution[j].add - mylist[n].self_force[j] << endl;
                        mylist[n].layer_force_ASAP++;
                        if (mylist[n].layer_force_ASAP == mylist[n].layer_force_ALAP){
                            mylist[n].completed = 1;
                        }
                        else{
                            F.recal_probability(mylist,node_num);
                            F.recal_distribution(mylist, q_distribution, layer_num, node_num);  
                            F.recal_self_force(mylist, q_distribution, layer_num, node_num);
                        }
                    }               
                }
                mylist[i].completed = 0;
                mylist[i].predecessor_force[j] = PSF_d[j] - PSF_d[iAS];
                F.recal_probability(mylist,node_num);
                F.recal_distribution(mylist, q_distribution, layer_num, node_num);  
                F.recal_self_force(mylist, q_distribution, layer_num, node_num);
            }
        }
    }
}
//--->check predecessor_force-----------//+
    // for (int i = 1; i <= node_num; i++){
    //     int iAS = mylist[i].layer_force_ASAP;
    //     int iLS = mylist[i].layer_force_ALAP;
    //     for (int j = iAS; j <= iLS; j++){
    //         if (mylist[i].predecessor_force[j] != 0){
    //             cout << i << "," << j << "," << mylist[i].predecessor_force[j] << endl;
    //         }
    //     }
    // }
//--->check predecessor_force-----------//-