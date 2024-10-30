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

// int main(){
int main(int argc, char *argv[]) {
    string file_name = argv[1];
    string file_save = argv[2];    
    // string choose_case = "2";
    // string file_name = "D://VS_C++//CAD_PA3//pa3case//case" + choose_case;
    // string file_save = "D://VS_C++//CAD_PA3//pa3out//case" + choose_case + ".out";

    cout << "//-------------------------//" << endl ;
    Function F;
    F.clock_on();  
    F.myString = F.readfile(file_name);
//-----------------------------------------------------------------------------------------------------------//
    F.net_num = F.net_num+1;
    NET_list myNet[F.net_num]; 
    F.make_net_list(myNet);

    F.cell_num = F.cell_num+1;
    CELL_list myCell[F.cell_num];
    F.make_cell_list(myCell,myNet);

    F.count_max_pin(myCell); 
    GainBucket gainbucket_A[F.max_pin_in_cell*2+1];
    GainBucket gainbucket_B[F.max_pin_in_cell*2+1];

    F.initial_gain_bucket(myCell, myNet, gainbucket_A, gainbucket_B);    
    F.do_FM_Partition(myCell, myNet, gainbucket_A, gainbucket_B);
    F.print_gainbucket(myCell, myNet, gainbucket_A, gainbucket_B);
    
//-----------------------------------------------------------------------------------------------------------//
    F.clearfile(file_save);
    F.save_Result(file_save, myCell, myNet, gainbucket_A, gainbucket_B);
    cout << endl;
    F.clock_off();
    system("pause");
    return 0;
}
