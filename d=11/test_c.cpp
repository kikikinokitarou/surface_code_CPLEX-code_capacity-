#include<iostream>
#include<vector>
#include<random>
#include<string>
#include<fstream>
#include<time.h>

using namespace std;
#define code_dist 11
#define num_qubit (code_dist*code_dist + (code_dist-1)*(code_dist-1))
#define num_stabilizer ((code_dist*code_dist + (code_dist-1)*(code_dist-1)-1)/2)
#define max_body 4
#define num_samp 10000

FILE *fp1;
FILE *fp2; 
FILE *fp4; 

vector<vector<int>> qubit(2,vector<int>(num_qubit));
vector<vector<int>> error_conf(3,vector<int>(num_qubit));
vector<vector<int>> syndrome(2,vector<int>(num_stabilizer));
vector<double> phys_error_prob;
vector<double> logi_error_prob;
vector<double> Excution_time;
vector<vector<vector<int>>> stabilizer=
{
    {
        {0,11,121,121},
        {1,12,121,122},
        {2,13,122,123},
        {3,14,123,124},
        {4,15,124,125},
        {5,16,125,126},
        {6,17,126,127},
        {7,18,127,128},
        {8,19,128,129},
        {9,20,129,130},
        {10,21,130,130},
        {11,22,131,131},
        {12,23,131,132},
        {13,24,132,133},
        {14,25,133,134},
        {15,26,134,135},
        {16,27,135,136},
        {17,28,136,137},
        {18,29,137,138},
        {19,30,138,139},
        {20,31,139,140},
        {21,32,140,140},
        {22,33,141,141},
        {23,34,141,142},
        {24,35,142,143},
        {25,36,143,144},
        {26,37,144,145},
        {27,38,145,146},
        {28,39,146,147},
        {29,40,147,148},
        {30,41,148,149},
        {31,42,149,150},
        {32,43,150,150},
        {33,44,151,151},
        {34,45,151,152},
        {35,46,152,153},
        {36,47,153,154},
        {37,48,154,155},
        {38,49,155,156},
        {39,50,156,157},
        {40,51,157,158},
        {41,52,158,159},
        {42,53,159,160},
        {43,54,160,160},
        {44,55,161,161},
        {45,56,161,162},
        {46,57,162,163},
        {47,58,163,164},
        {48,59,164,165},
        {49,60,165,166},
        {50,61,166,167},
        {51,62,167,168},
        {52,63,168,169},
        {53,64,169,170},
        {54,65,170,170},
        {55,66,171,171},
        {56,67,171,172},
        {57,68,172,173},
        {58,69,173,174},
        {59,70,174,175},
        {60,71,175,176},
        {61,72,176,177},
        {62,73,177,178},
        {63,74,178,179},
        {64,75,179,180},
        {65,76,180,180},
        {66,77,181,181},
        {67,78,181,182},
        {68,79,182,183},
        {69,80,183,184},
        {70,81,184,185},
        {71,82,185,186},
        {72,83,186,187},
        {73,84,187,188},
        {74,85,188,189},
        {75,86,189,190},
        {76,87,190,190},
        {77,88,191,191},
        {78,89,191,192},
        {79,90,192,193},
        {80,91,193,194},
        {81,92,194,195},
        {82,93,195,196},
        {83,94,196,197},
        {84,95,197,198},
        {85,96,198,199},
        {86,97,199,200},
        {87,98,200,200},
        {88,99,201,201},
        {89,100,201,202},
        {90,101,202,203},
        {91,102,203,204},
        {92,103,204,205},
        {93,104,205,206},
        {94,105,206,207},
        {95,106,207,208},
        {96,107,208,209},
        {97,108,209,210},
        {98,109,210,210},
        {99,110,211,211},
        {100,111,211,212},
        {101,112,212,213},
        {102,113,213,214},
        {103,114,214,215},
        {104,115,215,216},
        {105,116,216,217},
        {106,117,217,218},
        {107,118,218,219},
        {108,119,219,220},
        {109,120,220,220}
    },
    {
        {0,1,121,121},
        {1,2,122,122},
        {2,3,123,123},
        {3,4,124,124},
        {4,5,125,125},
        {5,6,126,126},
        {6,7,127,127},
        {7,8,128,128},
        {8,9,129,129},
        {9,10,130,130},
        {11,12,121,131},
        {12,13,122,132},
        {13,14,123,133},
        {14,15,124,134},
        {15,16,125,135},
        {16,17,126,136},
        {17,18,127,137},
        {18,19,128,138},
        {19,20,129,139},
        {20,21,130,140},
        {22,23,131,141},
        {23,24,132,142},
        {24,25,133,143},
        {25,26,134,144},
        {26,27,135,145},
        {27,28,136,146},
        {28,29,137,147},
        {29,30,138,148},
        {30,31,139,149},
        {31,32,140,150},
        {33,34,141,151},
        {34,35,142,152},
        {35,36,143,153},
        {36,37,144,154},
        {37,38,145,155},
        {38,39,146,156},
        {39,40,147,157},
        {40,41,148,158},
        {41,42,149,159},
        {42,43,150,160},
        {44,45,151,161},
        {45,46,152,162},
        {46,47,153,163},
        {47,48,154,164},
        {48,49,155,165},
        {49,50,156,166},
        {50,51,157,167},
        {51,52,158,168},
        {52,53,159,169},
        {53,54,160,170},
        {55,56,161,171},
        {56,57,162,172},
        {57,58,163,173},
        {58,59,164,174},
        {59,60,165,175},
        {60,61,166,176},
        {61,62,167,177},
        {62,63,168,178},
        {63,64,169,179},
        {64,65,170,180},
        {66,67,171,181},
        {67,68,172,182},
        {68,69,173,183},
        {69,70,174,184},
        {70,71,175,185},
        {71,72,176,186},
        {72,73,177,187},
        {73,74,178,188},
        {74,75,179,189},
        {75,76,180,190},
        {77,78,181,191},
        {78,79,182,192},
        {79,80,183,193},
        {80,81,184,194},
        {81,82,185,195},
        {82,83,186,196},
        {83,84,187,197},
        {84,85,188,198},
        {85,86,189,199},
        {86,87,190,200},
        {88,89,191,201},
        {89,90,192,202},
        {90,91,193,203},
        {91,92,194,204},
        {92,93,195,205},
        {93,94,196,206},
        {94,95,197,207},
        {95,96,198,208},
        {96,97,199,209},
        {97,98,200,210},
        {99,100,201,211},
        {100,101,202,212},
        {101,102,203,213},
        {102,103,204,214},
        {103,104,205,215},
        {104,105,206,216},
        {105,106,207,217},
        {106,107,208,218},
        {107,108,209,219},
        {108,109,210,220},
        {110,111,211,211},
        {111,112,212,212},
        {112,113,213,213},
        {113,114,214,214},
        {114,115,215,215},
        {115,116,216,216},
        {116,117,217,217},
        {117,118,218,218},
        {118,119,219,219},
        {119,120,220,220}
    }
};










vector<vector<int>> logical={{0,1,2,3,4,5,6,7,8,9,10},{0,11,22,33,44,55,66,77,88,99,110}};
double prob_error;
double prob_logi;


//乱数発生
random_device rnd;
//mt19937 mt(0);  //乱数のシードを固定
mt19937 mt(rnd());
uniform_real_distribution<double> ran_mt(0,1);

//量子状態の初期化
void initialize(void){
    for(int i=0;i<num_qubit;++i){
        for(int j=0;j<2;++j){
            qubit[j][i]=0;
        }
    }
}


//エラー発生
void data_error(double x, double z,double y){
    int i;
    double prob,prob_x,prob_y,prob_z;
    prob_x=x;
    prob_z=z+prob_x;
    prob_y=y+prob_z;

    for (i=0;i<num_qubit;++i){
        prob=ran_mt(mt);
        if(prob<=prob_x){
            qubit[0][i]^=1;
        }
        else if(prob<=prob_z){
            qubit[1][i]^=1;
        }
        else if(prob<=prob_y){
            qubit[0][i]^=1;
            qubit[1][i]^=1;
        }
    }
}
//各量子ビットにどんな演算子が作用しているのか確認
void show_data(){
    int i; 
    printf("data qubit:\n");
    for(i=0;i<num_qubit;i++){
        cout<<i<<" x"<<qubit[0][i]<<" z"<<qubit[1][i]<<endl;
    }
}

//シンドローム測定
void synd_meas(){
    int i,j,k;
    for(int i=0;i<num_stabilizer;++i){
        for (int k=0;k<2;k++){
            syndrome[k][i]=0;
            for(int j=0;j<max_body;++j){
                if(j==0){
                    syndrome[k][i]^=qubit[k][stabilizer[k][i][j]];
                }
                else if(stabilizer[k][i][j]!=stabilizer[k][i][j-1]){
                    syndrome[k][i]^=qubit[k][stabilizer[k][i][j]];
                }
            }
        }
    }
}

//各スタビライザーのシンドロームの値を確認
void show_synd(void){
    cout<<"syndrome:"<<endl;
    for(int i=0;i<num_stabilizer;i++){
        cout<<i<<" x"<<syndrome[0][i]<<" z"<<syndrome[1][i]<<endl;
    }

}

//シンドロームの値からcplexに投げる式を作成
void get_lp_format(){
    int i,j,k,hoge;
    ofstream writing_file;
    string filename="Int_prog.lp";
    writing_file.open(filename,ios::out);
    string writing_text="enter example\n\nminimize";
    writing_file<<writing_text<<endl;
    for(int i=0;i<num_qubit;++i){
        if(i!=num_qubit-1){
            string writing_text=" x";
            writing_file<<writing_text<<i<<" +";
        }
        else{
            string writing_text=" x";
            writing_file<<writing_text<<i<<" +"<<endl;
        }
    }
    for(int i=0;i<num_qubit;++i){
        if(i!=num_qubit-1){
            string writing_text=" z";
            writing_file<<writing_text<<i<<" +";
        }
        else{
            string writing_text=" z";
            writing_file<<writing_text<<i<<" +"<<endl;
        }
    }
    for(int i=0;i<num_qubit;++i){
        if(i!=num_qubit-1){
            string writing_text=" y";
            writing_file<<writing_text<<i<<" +";
        }
        else{
            string writing_text=" y";
            writing_file<<writing_text<<i<<endl;
        }
    }

    writing_text="subject to ";
    writing_file<<endl<<writing_text<<endl;
    for(int i=0;i<num_stabilizer;++i){
        hoge=0;
        for(k=0;k<max_body;k++){    
            if(k==0){
                writing_file<<"x"<<stabilizer[0][i][k]<<" + y"<<stabilizer[0][i][k];
            }
            else if(stabilizer[0][i][k] != stabilizer[0][i][k-1]){
                writing_file<<" + x"<<stabilizer[0][i][k]<<" + y"<<stabilizer[0][i][k];
            }       
        }
        writing_file<<" -2 w"<<i;
        writing_file<<" = "<<syndrome[0][i]<<endl;
    }
    for(int i=0;i<num_stabilizer;++i){
        hoge=0;
        for(k=0;k<max_body;k++){
            if(k==0){
                writing_file<<"z"<<stabilizer[1][i][k]<<" + y"<<stabilizer[1][i][k];
            }
            else if(stabilizer[1][i][k] != stabilizer[1][i][k-1]){
                writing_file<<" + z"<<stabilizer[1][i][k]<<" + y"<<stabilizer[1][i][k];
            }       
        }
    writing_file<<" -2 w"<<i+num_stabilizer;
    writing_file<<" = "<<syndrome[1][i]<<endl;
    }

    writing_file<<endl<<endl<<"bound"<<endl<<endl<<"general"<<endl;
    for(int i=0;i<2*num_stabilizer;++i){
        writing_file<<"w"<<i<<endl;
    }
    writing_file<<endl<<endl<<"binary"<<endl<<endl;
    for(int i=0;i<num_qubit;i++){
        writing_file<<"x"<<i<<endl;
    }
    for(int i=0;i<num_qubit;i++){
        writing_file<<"y"<<i<<endl;
    }
    for(int i=0;i<num_qubit;i++){
        writing_file<<"z"<<i<<endl;
    }
    writing_file<<endl<<endl<<"end";

    writing_file.close();


}

void get_lp_format_c(){
    int i,j,k,hoge;
    char filepath[256];

    sprintf(filepath,"Int_prog.lp");
    if((fp1= fopen(filepath,"w"))==NULL){printf("error!!!!!!!!!!!!! \n");
    exit(1);}   

    fprintf(fp1,"enter example\n\nminimize");
    for(i=0;i<num_qubit;i++){
        if(i!=num_qubit-1){
            fprintf(fp1," x%d +",i);
        }
        else{
            fprintf(fp1," x%d +\n",i);  
        }
    }

    for(i=0;i<num_qubit;i++){
        if(i!=num_qubit-1){
            fprintf(fp1," z%d +",i);
        }
        else{
            fprintf(fp1," z%d +",i);    
        }
    }


    for(i=0;i<num_qubit;i++){
        if(i!=num_qubit-1){
            fprintf(fp1," y%d +",i);
        }
        else{
            fprintf(fp1," y%d\n",i);    
        }
    }



    fprintf(fp1,"\nsubject to \n");


    for(i=0;i<num_stabilizer;i++){
        
        //fprintf(fp1,"0 ");
        hoge=0;
        for(k=0;k<max_body;k++){
                if(k==0){   
                    fprintf(fp1,"x%d + y%d ",stabilizer[0][i][k],stabilizer[0][i][k]);
                }
                else if(stabilizer[0][i][k] != stabilizer[0][i][k-1]){
                    fprintf(fp1,"+ x%d + y%d ",stabilizer[0][i][k],stabilizer[0][i][k]);    
                }
            }
            fprintf(fp1,"- 2 w%d ",i);
        
        // printf("%d\n",syndrome[i]);
        fprintf(fp1,"= %d \n",syndrome[0][i]);
    }

    for(i=0;i<num_stabilizer;i++){
        
        //fprintf(fp1,"0 ");
        hoge=0;
        for(k=0;k<max_body;k++){
                if(k==0){   
                    fprintf(fp1,"z%d + y%d ",stabilizer[1][i][k],stabilizer[1][i][k]);
                }
                else if(stabilizer[1][i][k] != stabilizer[1][i][k-1]){
                    fprintf(fp1,"+ z%d + y%d ",stabilizer[1][i][k],stabilizer[1][i][k]);    
                }
            }
            fprintf(fp1,"- 2 w%d ",i+num_stabilizer);
        
        // printf("%d\n",syndrome[i]);
        fprintf(fp1,"= %d \n",syndrome[1][i]);
    }

    


    fprintf(fp1,"\n\nbound \n\ngeneral\n");
    for(i=0;i<2*num_stabilizer;i++){
        fprintf(fp1,"w%d\n",i);
    }
    fprintf(fp1,"\n\nbinary\n\n");
    for(i=0;i<num_qubit;i++){
        fprintf(fp1,"x%d\n",i);
    }
    for(i=0;i<num_qubit;i++){
        fprintf(fp1,"y%d\n",i);
    }
    for(i=0;i<num_qubit;i++){
        fprintf(fp1,"z%d\n",i);
    }
    fprintf(fp1,"\n\nend");
    fclose(fp1);
}

//cplexに解かせる
void solve_lp(){
    char bash_command[256];
    sprintf(bash_command,"python test.py\n");
    system(bash_command);
}

//エラーの推定結果を取得
void get_lp_result(){
    int i;
    ifstream reading_file;
    string filename = "result.txt";
    reading_file.open(filename, ios::in);
    string reading_line_buffer;
    while(getline(reading_file, reading_line_buffer)){
        if(0<=i<num_qubit){
            int num = atoi(reading_line_buffer.c_str());
            error_conf[0][i]=num;
            i++;
            continue;

        }
        if(num_qubit<=i<2*num_qubit){
            int num = atoi(reading_line_buffer.c_str());
            error_conf[1][i]=num;
            i++;
            continue;

        }
        if(2*num_qubit<=i<3*num_qubit){
            int num = atoi(reading_line_buffer.c_str());
            error_conf[2][i]=num;
            i++;
            continue;

        }


    }
    reading_file.close();
}
void get_lp_result_c(){
    int i;
    char filepath[256];
    sprintf(filepath,"result.txt");

    
    fp4 = fopen(filepath,"r");
    if( fp4 == NULL ){printf( "%sファイルが開けません\n","result.txt");
    }

    for(i=0;i<num_qubit;i++){       
      fscanf(fp4,"%d\n",&error_conf[0][i]);
    }
    for(i=0;i<num_qubit;i++){       
      fscanf(fp4,"%d\n",&error_conf[1][i]);
    }
    for(i=0;i<num_qubit;i++){       
      fscanf(fp4,"%d\n",&error_conf[2][i]);
    }

    fclose(fp4);
  
}

void show_file_result(){
    int i;
    ifstream reading_file;
    string filename = "result.txt";
    reading_file.open(filename, ios::in);
    string reading_line_buffer;
    cout<<"resultファイルの中身は"<<endl;
    while(getline(reading_file, reading_line_buffer)){
        cout<<reading_line_buffer<<endl;
    }

}

//エラー訂正
void recovery(){
    int i,j;
    for(i=0;i<num_qubit;i++){
        for(j=0;j<2;j++){
            qubit[j][i]^=error_conf[j][i];
        }
        qubit[0][i]^=error_conf[2][i];
        qubit[1][i]^=error_conf[2][i];
    }
}

void show_result(){
    cout<<"result"<<endl;
    for(int i=0;i<num_qubit;++i){
        cout<<i<<" x"<<error_conf[0][i]<<" z"<<error_conf[1][i]<<" y"<<error_conf[2][i]<<endl;
    }
}



int main(){
    int i,j,l,k,a,b,c,n,m,ww;
    int flg_logi_errorx;
    int flg_logi_errorz;

    for(double prob_error=0.03;prob_error<=0.20001;prob_error=prob_error+0.01){
        prob_logi=0;
        double excution_time=0;
        for(int l=0;l<num_samp;l++){
            initialize();
            data_error(prob_error/3,prob_error/3,prob_error/3);
            //show_data();
            synd_meas();
            clock_t start=clock();
            get_lp_format();
            solve_lp();
            get_lp_result_c();
            clock_t end=clock();
            excution_time+=double(end-start);
            //show_result();
            recovery();
            synd_meas();
            for(i=0;i<num_stabilizer;i++){
                if(syndrome[0][i]!=0 || syndrome[1][i]!=0){
                printf("syndrom error!!!!!!");
                exit(0);
                }
            }
            flg_logi_errorx=0;
            for(i=0;i<code_dist;i++){
                flg_logi_errorx ^= qubit[0][logical[0][i]];
            }
            flg_logi_errorz=0;
            for(i=0;i<code_dist;i++){
                flg_logi_errorz ^= qubit[1][logical[1][i]];
            }
            if(flg_logi_errorx!=0 || flg_logi_errorz!=0){
                cout<<"論理エラー"<<endl;
                prob_logi+=1.0;
                //printf("%d\n",l);
                //if ( qq2>4){
                //printf("%d\n",l);
                    
                //}
    
            }



        cout<<prob_error<<"の"<<l<<"回目"<<endl;
        }
        prob_logi=prob_logi/((double) num_samp);
        logi_error_prob.push_back(prob_logi);
        Excution_time.push_back(excution_time);
    }
    for(int i=0;i<logi_error_prob.size();i++){
        if(i==logi_error_prob.size()-1){
            cout<<logi_error_prob[i]<<endl;
            break;
        }
        cout<<logi_error_prob[i]<<",";
    }
    for(int i=0;i<Excution_time.size();i++){
        if(i==Excution_time.size()-1){
            cout<<Excution_time[i]<<endl;
            break;
        }
        cout<<Excution_time[i]<<",";
    }
    return 0;


}