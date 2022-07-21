#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

using namespace std;
using std::string;
using std::endl;

//---------------------------------------------------------------------------------------------//

int maxn = 6;  //定義抓取數字最大位數
string cfile_name;
string file_save;

//--------------------------------------------//從字串中尋找指定字串
int find_word(string c_file, string tg_word){ 
    string str = c_file;
    str.find(tg_word);
    size_t found = str.find(tg_word);
    
    return found;
}

//--------------------------------------------//擷取分支行
string read_bfline(int line){
    ifstream text; 
    text.open(cfile_name, ios::in);

    vector<string> strVec;
    while (!text.eof()){
        string inbuf; 
        getline(text, inbuf, '\n'); 
        strVec.push_back(inbuf);
    }    
    return strVec[line - 1];
}

//--------------------------------------------//擷取gat前的數字
string numc(int zz,string file){    
    string zz1 = file.substr(zz-maxn,maxn);
    int zz2 = stoi(zz1);
    string zz3 = to_string(zz2);
    return zz3;
}

//--------------------------------------------//擷取數字(移除空格)
string numd(string zz1){
    int zz2 = stoi(zz1);
    string zz3 = to_string(zz2);
    return zz3;
}

//--------------------------------------------//尋找分支來源
string main2read(string conin){
    std::ifstream ifs(cfile_name, std::ios::in);
    string s;
    string s01, s02,s03;

    if (!ifs.is_open()) {
        cout << "Failed to open file.\n";
    } 
    else {
        // for(int i = 0; i < 36; i++){       
        while (getline(ifs, s)) {
            //getline(ifs, s);
            if (s.length()>27){
                s01 = s.substr(0,5);
                s02 = s.substr(15,4);
                s03 = s.substr(19,9);
                if (conin == s01 && s02 == "from"){
                    //cout <<  s01 << s02 << s03 <<endl; 
                    string conout = s03;
                    break;
                }                       
            } 
        }   
        ifs.close();
    }
    ifs.close();
    return s03;
}

//--------------------------------------------//清空輸出檔案
int clearV() {
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
    return 0;
}

//--------------------------------------------//將字串寫進輸出檔案
int saveV(string swd) {
    ofstream ofs;
    std::vector<std::string> str;
    str.push_back(swd+"\n");
    
    ofs.open(file_save,ofstream::out | ofstream::app);
    if (!ofs.is_open()) {
        cout << "Failed to open file.\n";
    } else {
        for (auto &s : str) {
            ofs << s;
        }
        ofs.close();
    }
    return 0;
}

//--------------------------------------------//找到分支來源並整理為輸出形式
string find_wireout(string impt,string acol3_1,string awire45) {
    string acol3_0 = acol3_1;
    acol3_1.erase(begin(acol3_1));
    acol3_1 = main2read(acol3_1);  
     
    int acol3_2 = find_word(acol3_1,"gat");
    
    if (acol3_2 != -1){
        //
        string acol3_3 = acol3_1;
        acol3_3.erase(acol3_3.end()-1);
        acol3_3.erase(acol3_3.end()-1);
        acol3_3.erase(acol3_3.end()-1);
        acol3_1 = acol3_3;
    }
    else{
        acol3_1 = acol3_0;
    } 

    acol3_1 = numd(acol3_1); 
    string erin = "gat"+ acol3_1;
    int inputnumfw = find_word(impt,erin);
    int mat1w = find_word(awire45+",",acol3_1+","); // 必須併入

    if (mat1w != -1 && inputnumfw == -1){
        acol3_1 = "_out" + acol3_1;
    }
    return acol3_1;
}

//--------------------------------------------//outpot gat格式
string re_out(int out00,string sa){
    string outputnum4 = numc(out00,sa);
    string outputnum5 = "gat_out" + outputnum4 + ", ";

    return outputnum5;
}

//--------------------------------------------//outpot wire格式
string re_log(int wi00,string sa,string logicf){
    string nand_o1 = numc(wi00,sa);
    string nand_o2 = logicf + nand_o1 + " (gat_out" + nand_o1 + ", ";
    
    return nand_o2; 
}      

//--------------------------------------------//移除最後2個字元
string erase_last(string iptn, string ttt){
    iptn.erase(iptn.end()-1);
    iptn.erase(iptn.end()-1);
    string iptn0 = ttt + iptn +";";

    return iptn0;
}

//--------------------------------------------//處理分支
string lo_con(string impt, string rd0,int conleng, string COL00,string wire45b){

    if (rd0.length() >= conleng ){
        string col3_1 = rd0.substr(conleng-maxn,maxn);
        string bcol3_1 = find_wireout(impt,col3_1, wire45b); 
        COL00 = ", gat" + bcol3_1;
    }else{ COL00 ="";}

    return COL00;
}

//--------------------------------------------//處理分支1234
string logic_wire(string impt,int zcol1,string zwire_c1,string zmat5,string zs){
                        
    string rdl1 = read_bfline(zcol1); // cout << rdl1.length() << endl;
    string aCOL1,aCOL2,aCOL3,aCOL4,aCOL5,aCOL6;

    aCOL1 = lo_con(impt,rdl1 ,6 ,aCOL1 ,zwire_c1); //+cout << aCOL1 << endl;
    aCOL2 = lo_con(impt,rdl1 ,12 ,aCOL2 ,zwire_c1);
    aCOL3 = lo_con(impt,rdl1 ,18 ,aCOL3 ,zwire_c1);
    aCOL4 = lo_con(impt,rdl1 ,24 ,aCOL4 ,zwire_c1);
    aCOL5 = lo_con(impt,rdl1 ,30 ,aCOL5 ,zwire_c1);
    aCOL6 = lo_con(impt,rdl1 ,36 ,aCOL6 ,zwire_c1);

    zmat5 =  aCOL1 + aCOL2 + aCOL3 + aCOL4 + aCOL5 + aCOL6 + ");"; //+cout << zmat5 << endl;
    zmat5.erase(begin(zmat5));
    zmat5.erase(begin(zmat5));

    return zmat5;
    }


//---------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------//

int main(int argc, char *argv[]) {

//------------------------//計時器
    double START,END; 
    START = clock();
//------------------------//輸入指定檔案
    cfile_name = argv[1];
    file_save = argv[2];

    cfile_name.erase(cfile_name.end()-1);
    cfile_name.erase(cfile_name.end()-1);
    cfile_name.erase(cfile_name.end()-1);
    cfile_name.erase(cfile_name.end()-1);
    string F_name = cfile_name;

    cfile_name = argv[1];

//------------------------//讀取指定檔案
    int col = 1;
    int col1,col2,col3,col4,col5,col6,col7,col8;
    vector<string> names;
    ifstream ifs(cfile_name, ios::in);
    if (!ifs.is_open()) {
        cout << "Failed to open file.\n";
    } 
    else {
//------------------------------------------------//定義
        string s,s01,ST01;
        string modulenum, wireA, wireA0, wire4;
        string wire_c;
        string inputnumA, inputnumA0;
        string outputnumA, outputnumA0, outputnum4;
        string nand_o1,  nand_o2, nand_w3, nand_o3;
        string col3_1, col3_2, col3_3, col3_4;
        string mat5;
//------------------------------------------------//逐行讀取檔案
        while (getline(ifs, s)) {        
            
            int inputnum = find_word(s,"gat inpt"); 
            if (inputnum == 11){
                
                string inputnum4 = numc(inputnum,s); 
                string inputnum5 = "gat" + inputnum4 + ", ";
                inputnumA += inputnum5;
            }
//------------------------------------------------//定義wire跟out的尋找條件
            int Wire,Outpn;
            string logicin;
            string impt = inputnumA;
            
            int wire01 = find_word(s,"gat and");
            int wire02 = find_word(s,"gat nand");
            int wire03 = find_word(s,"gat or");
            int wire04 = find_word(s,"gat nor");
            int wire05 = find_word(s,"gat not");
            int wire06 = find_word(s,"gat buff");
            int wire07 = find_word(s,"gat xor");
            int wire08 = find_word(s,"gat xnor");

            int outputnum01 = find_word(s,"gat and     0");
            int outputnum02 = find_word(s,"gat nand    0");
            int outputnum03 = find_word(s,"gat or      0");
            int outputnum04 = find_word(s,"gat nor     0");
            int outputnum05 = find_word(s,"gat not     0");
            int outputnum06 = find_word(s,"gat buff    0"); 
            int outputnum07 = find_word(s,"gat xor     0");
            int outputnum08 = find_word(s,"gat xnor    0");           
        
//------------------------------------------------//wire跟out統一成陣列，之後放進for loop
            int ar_wire[8] = {wire01,wire02,wire03,wire04,wire05,wire06,wire07,wire08};
            int ar_out[8] = {outputnum01,outputnum02,outputnum03,outputnum04,outputnum05,outputnum06,outputnum07,outputnum08};
            string ar_logic[8] = {"and gat","nand gat","or gat","nor gat","not gat","buf gat","xor gat","xnor gat"};        
            
//------------------------------------------------//符合條件者進行處理分支
            if (wire01 == 11){
                col1 = col +1;                
                string wire441 = numc(wire01,s);  
                wire_c += wire441 +",";  
                mat5 = logic_wire(impt, col1,wire_c,mat5,s);         
            }            
            if (wire02 == 11){
                col2 = col +1;                 
                string wire442 = numc(wire02,s);  
                wire_c += wire442 +",";                           
                mat5 = logic_wire(impt,col2,wire_c,mat5,s);         
            }            
            if (wire03 == 11){
                col3 = col +1; 
                string wire443 = numc(wire03,s); 
                wire_c += wire443 +","; 
                mat5 = logic_wire(impt,col3,wire_c,mat5,s);
            }
            if (wire04 == 11){
                col4 = col +1; 
                string wire444 = numc(wire04,s); 
                wire_c += wire444 +","; 
                mat5 = logic_wire(impt,col4,wire_c,mat5,s);
            }
            if (wire05 == 11){
                col5 = col +1; 
                string wire445 = numc(wire05,s); 
                wire_c += wire445 +",";
                mat5 = logic_wire(impt,col5,wire_c,mat5,s); 
            }
            if (wire06 == 11){
                col6 = col +1; 
                string wire446 = numc(wire06,s); 
                wire_c += wire446 +","; 
                mat5 = logic_wire(impt,col6,wire_c,mat5,s);
            }
            if (wire07 == 11){
                col7 = col +1; 
                string wire447 = numc(wire07,s); 
                wire_c += wire447 +","; 
                mat5 = logic_wire(impt,col7,wire_c,mat5,s);
            }
            if (wire08 == 11){
                col8 = col +1; 
                string wire448 = numc(wire08,s); 
                wire_c += wire448 +","; 
                mat5 = logic_wire(impt,col8,wire_c,mat5,s);
            }

            col += 1; 

//------------------------------------------------//outpot gat/wire 輸出        
            for (int np = 0; np<8; np++) {

                Wire = ar_wire[np];
                Outpn = ar_out[np];
                logicin = ar_logic[np];
                if (Wire == 11 ){

                    if (Outpn == 11 ){  
                        outputnumA += re_out(Outpn,s);
                        nand_o3 += re_log(Outpn,s,logicin) + mat5 +"\n";
                    }
                    else{
                        wireA += re_out(Wire,s);
                        nand_w3 += re_log(Wire,s,logicin) + mat5 + "\n";  
                    }
                    
                }                     
            }    
        }
        
//------------------------------------------------//文字格式修正     
        inputnumA0 = erase_last(inputnumA,"input ");
        outputnumA0 = erase_last(outputnumA,"output ");
        wireA0 = erase_last(wireA,"wire ");

        modulenum = inputnumA + outputnumA;
        modulenum.erase(modulenum.end()-1);
        modulenum.erase(modulenum.end()-1);
        modulenum = "module " + F_name +"(" + modulenum + ");";
        
        nand_w3.erase(nand_w3.end()-1);
        nand_o3.erase(nand_o3.end()-1);

//------------------------------------------------//寫入輸出檔        
        // cout << "`timescale 1ns/1ps" << endl;
        // cout << modulenum << endl;
        // cout << inputnumA0 << endl;
        // cout << outputnumA0 << endl;
        // cout << wireA0 << endl;
        // cout << endl;         
        // cout << nand_w3 << endl;        
        // cout << nand_o3 << endl;
        // cout << "endmodule" << endl;
        //cout << ppp << endl;
        //cout <<  main2read("5") << endl;
        // cout <<  ST01  <<endl;

        clearV();
        saveV("`timescale 1ns/1ps");
        saveV(modulenum);
        saveV(inputnumA0);
        saveV(outputnumA0);
        saveV(wireA0);
        saveV("");        
        saveV(nand_w3);        
        saveV(nand_o3);
        saveV("endmodule");

//---------------------------------------------------------------------------------------------//
        ifs.close();
    }
    ifs.close();

    cout << "--------DONE--------" << endl << endl;
    /*---要計算的程式效率區域---*/
    END = clock(); 
    cout << "Time_All:   " << clock()/CLOCKS_PER_SEC << "s" << endl;
	cout << "Time_Main:   " << (END - START) / CLOCKS_PER_SEC << "s" << endl;   

    cout << "\n";
    system("pause");
    return 0;
}
