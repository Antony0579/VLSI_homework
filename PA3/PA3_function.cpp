#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
// #include <stdio.h>
// #include <stdlib.h>
#include <time.h> 
#include <algorithm>
#include "PA3_head.h"

using namespace std;
using std::string;

void Function::save_Result(string file_save, CELL_list myCell[], NET_list myNet[],GainBucket gainbucket_A[],GainBucket gainbucket_B[]){

   int count_cut = 0;
    for (int i = 1; i < net_num; i++){
        vector<int> cell_in_A;
        vector<int> cell_in_B;

        for (int j = 0; j < myNet[i].cell.size(); j++){
            int k = myNet[i].cell[j];
            if(myCell[k].AorB == "A"){
                cell_in_A.push_back(k);
                // cout << "A" << k << endl;
            }
            else if (myCell[k].AorB == "B"){
                cell_in_B.push_back(k);
                // cout << "B" << k << endl;
            }
        }

        if ( cell_in_A.size() != 0 && cell_in_B.size() != 0){
            count_cut++ ;
        }
    }
    
    int count_GB_A = 0;
    int count_GB_B = 0;
    savefile(file_save,"cut_size ");savefile(file_save,to_string(count_cut)+"\n");
    savefile(file_save,"A \n");
    for (int i = gain_to_num(max_pin_in_cell); i >= 0 ; i--){
        for (int j = 0; j < gainbucket_A[i].cell.size(); j++){
            string str_cell_save = "c" + to_string(gainbucket_A[i].cell[j]);
            savefile(file_save,str_cell_save +"\n");
            count_GB_A++;
        }
    }
    savefile(file_save,"B \n");
    for (int i = gain_to_num(max_pin_in_cell); i >= 0 ; i--){
        for (int j = 0; j < gainbucket_B[i].cell.size(); j++){
            string str_cell_save = "c" + to_string(gainbucket_B[i].cell[j]);
            savefile(file_save,str_cell_save +"\n");
            count_GB_B++;
        }
    }

    cout << "cut_size:  " << count_cut << endl;
    cout << "count:  A:" << count_GB_A << endl;
    cout << "count:  B:" << count_GB_B << endl;
}

void Function::do_FM_Partition(CELL_list myCell[], NET_list myNet[],GainBucket gainbucket_A[],GainBucket gainbucket_B[]){
        
    // print_cell_list_and_net_list(myCell, myNet);
    // print_gainbucket(myCell, myNet, gainbucket_A, gainbucket_B);
    int path_num = 0;
    while ( Ad_gain_move_before > Ad_gain_move_after ){   //持續排列的條件
    // for (int i = 0; i < 3; i++){
        path_num++;
        cout << "path_num " << path_num << endl;
       
        int count_locked;
        int r_size = cell_num/5-1;
        // int r_size = 5;

        Max_GainA = max_pin_in_cell;
        Max_GainB = max_pin_in_cell;
        cell_move = 0;

        Ad_gain_move_after = 0;
        Ad_gain_move_before = 0;

        for (int j = 1; j < cell_num; j++){
            int g = myCell[j].GB_Ptr->gain;
            Ad_gain_move_before += g;
        }

        count_locked = 0;
        while ( count_locked < cell_num ){
            cout << ".";

            get_Max_GainA(myCell,gainbucket_A);
            get_Max_GainB(myCell,gainbucket_B);
            count_gain_bucket_cells(gainbucket_A, gainbucket_B);
            
            int cell_move_before = cell_move;                    
            if( count_gb_B - count_gb_A >= r_size-1 ){      
                Total_move_B_to_A(Max_GainB, myCell, myNet, gainbucket_A, gainbucket_B);        
            }
            else if (count_gb_A - count_gb_B >= r_size-1 ){
                Total_move_A_to_B(Max_GainA, myCell, myNet, gainbucket_A, gainbucket_B);          
            }
            else if (Max_GainB > Max_GainA){
                Total_move_B_to_A(Max_GainB, myCell, myNet, gainbucket_A, gainbucket_B);
            }
            else if (Max_GainA > Max_GainB ){
                Total_move_A_to_B(Max_GainA, myCell, myNet, gainbucket_A, gainbucket_B);          
            }
            else{
                if ("A" == tie_breaking(cell_move_before, myCell, myNet, gainbucket_A, gainbucket_B) ){
                    Total_move_A_to_B(Max_GainA, myCell, myNet, gainbucket_A, gainbucket_B);
                }
                else if ("B" == tie_breaking(cell_move_before, myCell, myNet, gainbucket_A, gainbucket_B) ){
                    Total_move_B_to_A(Max_GainB, myCell, myNet, gainbucket_A, gainbucket_B);
                }
                else{
                    cout << "tie_breaking error" << endl;
                }
                
            }

            // 檢查是否鎖定 // (不太可能)
            for (int j = 1; j < cell_num; j++){
                if (myCell[j].locked == 1){
                    count_locked++;
                }
            }

            // 檢查是否移動失敗
            if (cell_move_before == cell_move){
                // cout << "move_end" << endl;
                count_locked = cell_num;
            }
        }

        for (int j = 1; j < cell_num; j++){
            myCell[j].locked = 0;   // unlock_all_cell

            int g = myCell[j].GB_Ptr->gain;
            Ad_gain_move_after += g;
        }
        cout << endl;
        cout << Ad_gain_move_after << ", " << Ad_gain_move_before  << endl;
        cout << endl;
    }
}

void Function::get_Max_GainA(CELL_list myCell[],GainBucket gainbucket_A[]){
    for (int i = gain_to_num(max_pin_in_cell); i >= 0 ; i--){
        for (int j = 0; j < gainbucket_A[i].cell.size(); j++){
            int k = gainbucket_A[i].cell[j];
            if (myCell[k].locked == 0){
                Max_GainA = myCell[k].GB_Ptr->gain;
                // cout << k << "Max_GainA  " << Max_GainA << endl;
                return;
            }
        }
    }
}
void Function::get_Max_GainB(CELL_list myCell[],GainBucket gainbucket_B[]){
    for (int i = gain_to_num(max_pin_in_cell); i >= 0 ; i--){
        for (int j = 0; j < gainbucket_B[i].cell.size(); j++){
            int k = gainbucket_B[i].cell[j];
            if (myCell[k].locked == 0){
                Max_GainB = myCell[k].GB_Ptr->gain;
                // cout << k << "Max_GainB  " << Max_GainB << endl;
                return;
            }
        }
    }
    // cout << "get_Max_Gain end " << endl;
}

void Function::Total_move_A_to_B(int Max_GainA, CELL_list myCell[], NET_list myNet[],GainBucket gainbucket_A[],GainBucket gainbucket_B[]){
    vector<int> cal_gain_A = gainbucket_A[gain_to_num(Max_GainA)].cell;
 
    for (int j = 0; j < cal_gain_A.size(); j++){

        cell_move = cal_gain_A[j];  //  選擇移動的gain中排序第j個的cell
        if (myCell[cell_move].locked == 0){     // 如果cell未鎖定

            int gain_before_move = myCell[cell_move].GB_Ptr->gain;  
            move_A_to_B(cell_move,Max_GainA, myCell, myNet, gainbucket_A, gainbucket_B);                
            recal_gain_A_to_B(cell_move, myCell, myNet, gainbucket_A, gainbucket_B);
            int gain_after_move = myCell[cell_move].GB_Ptr->gain;

            //  如果移動效果不好->移回去
            if (gain_after_move - gain_before_move > 0){
                move_B_to_A(cell_move,gain_after_move, myCell, myNet, gainbucket_A, gainbucket_B);               //  更改 cell_move
                recal_gain_B_to_A(cell_move, myCell, myNet, gainbucket_A, gainbucket_B);
            }

            myCell[cell_move].locked = 1;
            break;
        }
    }
    
}

void Function::Total_move_B_to_A(int Max_GainB, CELL_list myCell[], NET_list myNet[],GainBucket gainbucket_A[],GainBucket gainbucket_B[]){
    vector<int> cal_gain_B = gainbucket_B[gain_to_num(Max_GainB)].cell;
    
    for (int j = 0; j < cal_gain_B.size(); j++){

        cell_move = cal_gain_B[j];  //  選擇移動的gain中排序第j個的cell //  更改 cell_move
        if (myCell[cell_move].locked == 0){

            int gain_before_move = myCell[cell_move].GB_Ptr->gain;
            move_B_to_A(cell_move,Max_GainB, myCell, myNet, gainbucket_A, gainbucket_B);               
            recal_gain_B_to_A(cell_move, myCell, myNet, gainbucket_A, gainbucket_B);
            int gain_after_move = myCell[cell_move].GB_Ptr->gain;

            //  如果移動效果不好->移回去
            if (gain_after_move - gain_before_move > 0){
                move_A_to_B(cell_move,gain_after_move, myCell, myNet, gainbucket_A, gainbucket_B);                
                recal_gain_A_to_B(cell_move, myCell, myNet, gainbucket_A, gainbucket_B);
            }

            myCell[cell_move].locked = 1;
            break;
        }
    }
    
}

void Function::move_A_to_B(int cell_move,int gain_moved, CELL_list myCell[], NET_list myNet[],GainBucket gainbucket_A[],GainBucket gainbucket_B[]){
    
    myCell[cell_move].AorB = "B";
    myCell[cell_move].GB_Ptr = &gainbucket_B[gain_to_num(gain_moved)];

    GainBucket* gbA_Ptr = &gainbucket_A[gain_to_num(gain_moved)]; 
    vector<int>::iterator iter = remove(   gbA_Ptr->cell.begin(),  gbA_Ptr->cell.end(),  cell_move);
    gbA_Ptr->cell.erase(  iter, gbA_Ptr->cell.end());
    //  貼上新gain的cell  
    gainbucket_B[gain_to_num(gain_moved)].cell.push_back(cell_move);
    
}

void Function::move_B_to_A(int cell_move,int gain_moved, CELL_list myCell[], NET_list myNet[],GainBucket gainbucket_A[],GainBucket gainbucket_B[]){
    
    myCell[cell_move].AorB = "A";
    myCell[cell_move].GB_Ptr = &gainbucket_A[gain_to_num(gain_moved)];
    
    GainBucket* gbB_Ptr = &gainbucket_B[gain_to_num(gain_moved)]; 
    vector<int>::iterator iter = remove(   gbB_Ptr->cell.begin(),  gbB_Ptr->cell.end(),  cell_move);
    gbB_Ptr->cell.erase(  iter, gbB_Ptr->cell.end());
    //  貼上新gain的cell  
    gainbucket_A[gain_to_num(gain_moved)].cell.push_back(cell_move);

}

void Function::recal_gain_A_to_B(int cell_move, CELL_list myCell[], NET_list myNet[],GainBucket gainbucket_A[],GainBucket gainbucket_B[]){

    for (int i = 1; i < net_num; i++){         
        vector<int> cell_in_A;
        vector<int> cell_in_B;
        vector<int> d_gain_A(cell_num,0);
        vector<int> d_gain_B(cell_num,0);

        vector<int>::iterator it = find(myNet[i].cell.begin(), myNet[i].cell.end(), cell_move);
        if (it != myNet[i].cell.end()){   // 有找到
            // cout << i <<endl;
            for (int j = 0; j < myNet[i].cell.size(); j++){
                int k = myNet[i].cell[j];
                if(myCell[k].AorB == "A"){
                    cell_in_A.push_back(k);
                    // cout << "A" << k << endl;
                }
                else if (myCell[k].AorB == "B"){
                    cell_in_B.push_back(k);
                    // cout << "B" << k << endl;
                }
            }
        
            if (cell_in_B.size() == 1){         // (移動後)整個net只有一個cell在B (T0)
                for (int k = 0; k < myNet[i].cell.size(); k++){
                    int t = myNet[i].cell[k];
                    d_gain_A[t]++;
                    if ( t == cell_move){
                        d_gain_B[t]+=2;
                    }
                }
            }
            else if(cell_in_B.size() == 2){     // (移動後)整個net有2個cell在B (T1)
                for (int k = 0; k < myNet[i].cell.size(); k++){
                    int t = myNet[i].cell[k];
                    d_gain_B[t]--;
                    if ( t == cell_move){
                       d_gain_B[t]++;
                    }
                }
            }        
            if (cell_in_A.size() == 0){         // (移動後)整個net沒有cell在A (F1) 
                for (int k = 0; k < myNet[i].cell.size(); k++){
                    int t = myNet[i].cell[k];
                    d_gain_B[t]--;
                    if ( t == cell_move){
                        d_gain_B[t]--;
                    }
                }
            }
            else if(cell_in_A.size() == 1){     // (移動後)整個net只有一個cell在A (F2)
                int t = cell_in_A[0];                    
                d_gain_A[t]++;
            }

            for (int t = 1; t < cell_num ; t++){ 
                if( myCell[t].AorB == "A"){ 
                    move_gain_in_A(t, d_gain_A, myCell, gainbucket_A);
                }
                else if(myCell[t].AorB == "B"){ 
                    move_gain_in_B(t, d_gain_B, myCell, gainbucket_B);
                }
            }      
        }
    }
}
// cell_move
void Function::recal_gain_B_to_A(int cell_move, CELL_list myCell[], NET_list myNet[],GainBucket gainbucket_A[],GainBucket gainbucket_B[]){ 

    for (int i = 1; i < net_num; i++){         
        vector<int> cell_in_A;
        vector<int> cell_in_B;
        vector<int> d_gain_A(cell_num,0);
        vector<int> d_gain_B(cell_num,0);
        
        vector<int>::iterator it = find(myNet[i].cell.begin(), myNet[i].cell.end(), cell_move);
        if (it != myNet[i].cell.end()){   // 有找到->myNet[i]為包含移動的cell的Net
            // cout << i <<endl;
            for (int j = 0; j < myNet[i].cell.size(); j++){
                int k = myNet[i].cell[j];   // 分AB存入暫存器cell_in_A、cell_in_B
                if(myCell[k].AorB == "A"){
                    cell_in_A.push_back(k);
                    // cout << "A" << k << endl;
                }
                else if (myCell[k].AorB == "B"){
                    cell_in_B.push_back(k);
                    // cout << "B" << k << endl;
                }
            }

            //  根據老師提供的各種情況，分別計算gainA、gainB的變化值        
            if (cell_in_A.size() == 1){     // (移動後)整個net只有一個cell在A (T0)
                for (int k = 0; k < myNet[i].cell.size(); k++){
                    int t = myNet[i].cell[k];
                    d_gain_B[t]++;
                    if ( t == cell_move){
                        d_gain_A[t]+=2;
                    }
                }
            }
            else if(cell_in_A.size() == 2){   // (移動後)整個net有2個cell在A (T1)
                for (int k = 0; k < myNet[i].cell.size(); k++){
                    int t = myNet[i].cell[k];
                    d_gain_A[t]--;
                    if ( t == cell_move){
                        d_gain_A[t]++;
                    }
                }
            }        
            if (cell_in_B.size() == 0){ //  (移動後)整個net沒有cell在B (F1)  
                for (int k = 0; k < myNet[i].cell.size(); k++){
                    int t = myNet[i].cell[k];
                    d_gain_A[t]--;
                    if ( t == cell_move){
                        d_gain_A[t]--;
                    }
                }
            }
            else if(cell_in_B.size() == 1){   //  (移動後)整個net只有一個cell在B (F2)
                int t = cell_in_B[0];                    
                d_gain_B[t]++;
            }

            for (int t = 1; t < cell_num ; t++){ 
                if( myCell[t].AorB == "A"){ 
                    move_gain_in_A(t, d_gain_A, myCell, gainbucket_A);
                }
                else if(myCell[t].AorB == "B"){ 
                    move_gain_in_B(t, d_gain_B, myCell, gainbucket_B);
                }
            }         
        }
    }
}

void Function::move_gain_in_A(int cell_t, vector<int> d_gain_A, CELL_list myCell[], GainBucket gainbucket_A[]){
    int t = cell_t;
    int gain_before = myCell[t].GB_Ptr->gain;
    myCell[t].GB_Ptr = &gainbucket_A[gain_to_num(gain_before + d_gain_A[t])]; //  更改cell的gain

    //  剪下原gain的cell
    GainBucket* gbA_Ptr = &gainbucket_A[gain_to_num(gain_before)];                   
    vector<int>::iterator iter = remove(   gbA_Ptr->cell.begin(),  gbA_Ptr->cell.end(),  t);
    gbA_Ptr->cell.erase(    iter, gbA_Ptr->cell.end());
    //  貼上新gain的cell
    gainbucket_A[gain_to_num(gain_before + d_gain_A[t])].cell.push_back(t);
}

void Function::move_gain_in_B(int cell_t, vector<int> d_gain_B, CELL_list myCell[], GainBucket gainbucket_B[]){
    int t = cell_t;
    int gain_before = myCell[t].GB_Ptr->gain;
    myCell[t].GB_Ptr = &gainbucket_B[gain_to_num(gain_before + d_gain_B[t])]; //  更改cell的gain
    
    //  剪下原gain的cell
    GainBucket* gbB_Ptr = &gainbucket_B[gain_to_num(gain_before)];                   
    vector<int>::iterator iter = remove(   gbB_Ptr->cell.begin(),  gbB_Ptr->cell.end(),  t);
    gbB_Ptr->cell.erase(    iter, gbB_Ptr->cell.end());
    //  貼上新gain的cell
    gainbucket_B[gain_to_num(gain_before + d_gain_B[t])].cell.push_back(t);
}

void Function::count_gain_bucket_cells(GainBucket gainbucket_A[],GainBucket gainbucket_B[]){
    int counter_A = 0;
    int counter_B = 0;
    for (int i = gain_to_num(max_pin_in_cell); i >= 0 ; i--){
        for (int j = 0; j < gainbucket_A[i].cell.size(); j++){
            counter_A++;
        }
        for (int j = 0; j < gainbucket_B[i].cell.size(); j++){
            counter_B++;
        }
    }
    count_gb_A = counter_A;
    count_gb_B = counter_B;
}

string Function::tie_breaking(int cell_move_before, CELL_list myCell[], NET_list myNet[],GainBucket gainbucket_A[],GainBucket gainbucket_B[]){
    
    vector<int> counter_unlocked_gb_A(max_pin_in_cell*2+1, 0);
    vector<int> counter_unlocked_gb_B(max_pin_in_cell*2+1, 0);

    for (int i = gain_to_num(max_pin_in_cell); i >= 0 ; i--){
        for (int j = 0; j < gainbucket_A[i].cell.size(); j++){
            int k = gainbucket_A[i].cell[j];
            if (myCell[k].locked == 0){
               counter_unlocked_gb_A[i]++; 
            }
        }
        for (int j = 0; j < gainbucket_B[i].cell.size(); j++){
            int k = gainbucket_B[i].cell[j];
            if (myCell[k].locked == 0){
               counter_unlocked_gb_B[i]++; 
            }
        }
    }

    for (int i = gain_to_num(max_pin_in_cell); i >= 0 ; i--){
        if (counter_unlocked_gb_A[i] > counter_unlocked_gb_B[i]){
            return "A";
        }
        else if (counter_unlocked_gb_A[i] < counter_unlocked_gb_B[i]){
            return "B";
        }
    }

    if(cell_move_before == 0){
        return "B";
    }
    else if (myCell[cell_move_before].AorB == "A"){
        return "A";
    }
    else if (myCell[cell_move_before].AorB == "B"){
        return "B";
    }

    return 0;
}

void Function::count_cell_of_gain_more_than0(GainBucket gainbucket_A[],GainBucket gainbucket_B[]){
    int cell_m0 = 0;
    for (int i = gain_to_num(max_pin_in_cell); i > gain_to_num(0) ; i--){       // 不含0
        for (int j = 0; j < gainbucket_A[i].cell.size(); j++){
            // cout << gainbucket_A[i].cell[j] << "A ";
            cell_m0++;
        }
        for (int j = 0; j < gainbucket_B[i].cell.size(); j++){
            // cout << gainbucket_B[i].cell[j] << "B ";
            cell_m0++;
        }
    }
    cell_of_gain_more_than0 = cell_m0;
    // cout << cell_of_gain_more_than0 << endl;
}



void Function::initial_gain_bucket(CELL_list myCell[], NET_list myNet[],GainBucket gainbucket_A[],GainBucket gainbucket_B[]){
    for (int i = 0; i < max_pin_in_cell*2+1; i++){
        gainbucket_A[i].gain = -max_pin_in_cell+i;
        gainbucket_B[i].gain = -max_pin_in_cell+i;
    }
    
    // 初始狀態最大的net是哪個？
    for (int i = 1; i < net_num; i++){
        for (int j = 0; j < myNet[i].cell.size(); j++){
            // cout << j << endl;
            //  比大小
            if ( j >= max_cell_in_net){
                max_cell_in_net = j+1;
                net_with_max_cell = i;
            }
        }
    }
    
    // 初始狀態取最大的net，其中的cell在一邊
    for (int i = 0; i < max_cell_in_net; i++){
        int cell_in_A = myNet[net_with_max_cell].cell[i];
        myCell[cell_in_A].GB_Ptr = &gainbucket_A[gain_to_num(0)];// 調整初始gain為0
        myCell[cell_in_A].AorB = "A";
        gainbucket_A[gain_to_num(0)].cell.push_back(cell_in_A);
    }
    
    // 剩下的cell放另一邊
    for (int i = 1; i < cell_num; i++){
        int k = 0;
        vector<int> all_cellinA = myNet[net_with_max_cell].cell; 
        vector<int>::iterator it = find(all_cellinA.begin(), all_cellinA.end(), i);

        if (it == all_cellinA.end()){    //不符合，註：end()為超量元素
            int cell_in_B = i;
            myCell[cell_in_B].GB_Ptr = &gainbucket_B[gain_to_num(0)];// 調整初始gain為0
            myCell[cell_in_B].AorB = "B";
            gainbucket_B[gain_to_num(0)].cell.push_back(cell_in_B);
        }
    }

    // 如果cell數量差距過大，移動直到數量均衡    
    int count_gb0_inA = gainbucket_A[gain_to_num(0)].cell.size();
    int count_gb0_inB = gainbucket_B[gain_to_num(0)].cell.size();
    while( count_gb0_inB - count_gb0_inA >= 1){   // while 

        for (int i = 1; i < cell_num; i++){
            if (myCell[i].AorB == "B"){
                move_B_to_A(i, 0, myCell, myNet, gainbucket_A, gainbucket_B); 
                break; 
            }
        }
        count_gb0_inA = gainbucket_A[gain_to_num(0)].cell.size();
        count_gb0_inB = gainbucket_B[gain_to_num(0)].cell.size();       
    }

    // 計算原始gain
    vector<int> cal_gain0_A;    //  cal_gain0_A == gainbucket_A[gain_to_num(0)].cell
    for (int i = 0; i < gainbucket_A[gain_to_num(0)].cell.size(); i++){
        cal_gain0_A.push_back(gainbucket_A[gain_to_num(0)].cell[i]);
        // cout << gainbucket_A[gain_to_num(0)].cell[i] << endl;
    } 
    
    vector<int> recal_gain_A_to_B_A(cell_num,0) ;
    vector<int> recal_gain_A_to_B_B(cell_num,0) ;

    for (int i = 1; i < net_num; i++){
        vector<int> cell_in_A;
        vector<int> cell_in_B;
        int count_inA = 0;
        int count_inB = 0;

        for (int j = 0; j < myNet[i].cell.size(); j++){ //  遍歷Netlist[i] 裡的cell            
            vector<int>::iterator it = find(cal_gain0_A.begin(), cal_gain0_A.end(), myNet[i].cell[j]);
            if (it != cal_gain0_A.end()){   //  若這個cell在 GB_A ,gain = 0
                cell_in_A.push_back(myNet[i].cell[j]);  //  紀錄這個cell在 cell_in_A
                // cout << i << myNet[i].cell[j] << endl;
                count_inA++; 
            }
            else{
                cell_in_B.push_back(myNet[i].cell[j]);
                count_inB++;
            }
        }        

        if (count_inA == myNet[i].cell.size()){ 
            for (int k = 0; k < myNet[i].cell.size(); k++){
                int t = myNet[i].cell[k];
                recal_gain_A_to_B_A[t]--;
            }
        }
        else if(count_inB == myNet[i].cell.size()){ 
            for (int k = 0; k < myNet[i].cell.size(); k++){
                int t = myNet[i].cell[k];
                recal_gain_A_to_B_B[t]--;
            }
        }
        else{
            if (count_inA == 1){  //  
                // cout << cell_in_A[0] << endl;                             
                int k = cell_in_A[0];                    
                recal_gain_A_to_B_A[k]++;
            }
            if (count_inB == 1){  //  
                // cout << cell_in_B[0] << endl;                             
                int k = cell_in_B[0];                    
                recal_gain_A_to_B_B[k]++;
            }
        }
    }

    for (int t = cell_num; t >= 1; t--){
        if( myCell[t].AorB == "A"){ 
            myCell[t].GB_Ptr = &gainbucket_A[gain_to_num(0 + recal_gain_A_to_B_A[t])]; //  更改cell的gain

            GainBucket* gbA_Ptr = &gainbucket_A[gain_to_num(0)];            
            //  剪下原gain的cell
            vector<int>::iterator iter = remove(   gbA_Ptr->cell.begin(),  gbA_Ptr->cell.end(),  t);
            gainbucket_A[gain_to_num(0)].cell.erase(    iter, gbA_Ptr->cell.end());
            //  貼上新gain的cell
            gainbucket_A[gain_to_num(0 + recal_gain_A_to_B_A[t])].cell.push_back(t);     
        }
        else if( myCell[t].AorB == "B"){ 
            myCell[t].GB_Ptr = &gainbucket_B[gain_to_num(0 + recal_gain_A_to_B_B[t])];

            GainBucket* gbB_Ptr = &gainbucket_B[gain_to_num(0)];
            //  剪下原gain的cell
            vector<int>::iterator iter = remove(   gbB_Ptr->cell.begin(),  gbB_Ptr->cell.end(),  t);
            gainbucket_B[gain_to_num(0)].cell.erase(    iter, gbB_Ptr->cell.end());
            //  貼上新gain的cell  
            gainbucket_B[gain_to_num(0 + recal_gain_A_to_B_B[t])].cell.push_back(t);
        }
    }
}

void Function::print_gainbucket(CELL_list myCell[], NET_list myNet[],GainBucket gainbucket_A[],GainBucket gainbucket_B[]){
// // print cell-gainbucket
//     cout << "gainbucket_A" << endl;
//     for (int i = gain_to_num(max_pin_in_cell); i >= 0 ; i--){
//         for (int j = 1; j < cell_num ; j++){
//             int g = myCell[j].GB_Ptr->gain;
//             if( num_to_gain(i) == g && myCell[j].AorB == "A"){
//                 cout <<  "gain:" << g << " " << "cell:" << j << " in " << myCell[j].AorB << endl;
//             }
//         }
//     }
//     cout << endl;
//     cout << "gainbucket_B" << endl;
//     for (int i = gain_to_num(max_pin_in_cell); i >= 0 ; i--){
//         for (int j = 1; j < cell_num ; j++){
//             int g = myCell[j].GB_Ptr->gain;
//             if( num_to_gain(i) == g && myCell[j].AorB == "B"){
//                 cout <<  "gain:" << g << " " << "cell:" << j << " in " << myCell[j].AorB << endl;
//             }
//         }
//     }
//     cout << endl;

// print gainbucket-cell 
    cout << "gainbucket_A" << endl;
    for (int i = gain_to_num(max_pin_in_cell); i >= 0 ; i--){
        cout << num_to_gain(i) << ":";
        for (int j = 0; j < gainbucket_A[i].cell.size(); j++){
            cout << gainbucket_A[i].cell[j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "gainbucket_B" << endl;
    for (int i = gain_to_num(max_pin_in_cell); i >= 0 ; i--){
        cout << num_to_gain(i) << ":";
        for (int j = 0; j < gainbucket_B[i].cell.size(); j++){
            cout << gainbucket_B[i].cell[j] << " ";
        }
        cout << endl;
    }
    cout << endl;

}

int Function::gain_to_num(int x){
    return x + max_pin_in_cell;
}
int Function::num_to_gain(int x){
    return x - max_pin_in_cell;
}

void Function::count_max_pin(CELL_list myCell[]){
    for (int i = 1; i < cell_num; i++){
        for (int j = 0; j < myCell[i].pin.size(); j++){
            // cout << j << endl;
            //  比大小
            if ( j >= max_pin_in_cell){
                max_pin_in_cell = j+1;
            }
        }
    }
    // cout << max_pin_in_cell << endl;
}

void Function::print_cell_list_and_net_list(CELL_list myCell[], NET_list myNet[]){
    // print cell_list
    cout << "cell_list" <<endl;
    for (int i = 1; i < cell_num; i++){
        cout << i <<": ";
        for (int j = 0; j < myCell[i].pin.size(); j++){
            cout << myCell[i].pin[j] << " ";
        }
        cout << endl;
    }cout << endl;

    // print net_list
    cout << "net_list" <<endl;
    for (int i = 1; i < net_num; i++){
        cout << i <<": ";
        for (int j = 0; j < myNet[i].cell.size(); j++){
            cout << myNet[i].cell[j] << " ";
        }
        cout << endl;
    }cout << endl;
}

void Function::make_cell_list(CELL_list myCell[], NET_list myNet[]){
    for (int i = 1; i < cell_num; i++){
        myCell[i].cell = i;
    }
    for (int i = 1; i < net_num; i++){
        for (int j = 0; j < myNet[i].cell.size(); j++){
            for (int k = 1; k < cell_num; k++){
                if (myNet[i].cell[j] == k){
                    myCell[k].pin.push_back(i);
                }
            }
        }
    }
}

void Function::make_net_list(NET_list myNet[]){
    cell_num = 0;
    int count_1 = 1;
    for (int i = 0; i < file_word; i++){ //myString[49] = 隨機值
        if ( myString[i] == "{"){
            i++;
            while( myString[i] != "}" ){
                myString[i] = myString[i].replace(myString[i].find("c"),1,""); // 移除字元c
                myNet[count_1].cell.push_back(stoi(myString[i]));

                //  比大小
                if ( stoi(myString[i]) >= cell_num){
                    cell_num = stoi(myString[i]);
                }

                i++;
            }
            count_1++;
        } 
    }
}

void Function::clock_on(){
    START = clock();
}

void Function::clock_off(){
    END = clock();
    cout << "Time_All:   " << clock()/CLOCKS_PER_SEC << "s" << endl;
	cout << "Time_Main:   " << (END - START) / CLOCKS_PER_SEC << "s" << endl;
}

vector<string> Function::readfile(string file_name){
    net_num = 0;
    file_word = 0;

    ifstream ifs(file_name, ios::in);
    if (!ifs.is_open()) {
        cout << "Failed to open file.\n";
    } 
    else {
        // 將檔案存成陣列
        // int file_word = 0;
        string s;
        while (getline(ifs, s)) {
            istringstream ss(s);
            string token;
            //  分割空格
            while (getline(ss,token,' ')){
                if (token != ""){   //  移除空白
                    myString.push_back(token);
                    // cout << myString[file_word] << endl;
                    
                    //  分行關鍵字
                    if ( myString[file_word] == "NET"){ 
                        net_num++;
                    }

                    file_word++;
                }
            }
        }
    }
    // cout << file_word << endl;
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
void Function::savefile(string file_save, string swd) {
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