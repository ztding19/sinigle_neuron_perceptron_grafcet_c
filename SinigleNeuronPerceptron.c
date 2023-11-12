# include <stdio.h>
# include <math.h>
# include <windows.h>

# define EPOCH 4*4
# define nb_in 2

void grafcet0();
void grafcet1();
void grafcet2();
void action0();
void action1();
void action2();
void SetXT();
void StoreCurrentValue();
void WriteLearningDetail();
float ActivationFunction(float);
float Stimulate(float*);
void Learning(float*, float);
float Testing(float*, float);
void PrintState();
void AddE();
void WriteAvgE();


//For grafcet
int x0 = 1, x1 = 0, x2 = 0,x10 = 1, x11 = 0, x12 = 0, x13 = 0, x20 = 1, x21 = 0, x22 = 0, x23 = 0;
int start = 1;

float E = 0, ECur = 0;
float x[nb_in];
float weight[nb_in] = {0, 0.4};
float weightPre[nb_in] = {0, 0};
float y;
int t = 0, tPre = 0;
float bias = 0.3, biasPre;
float eta = 0.6, etaPre = 0;
int times = 0;
FILE *detailF = NULL ;
FILE *Efile = NULL ;

int main(){
    SetConsoleOutputCP(65001);
    detailF = fopen( "./LearningDetail.csv", "w" ) ;
    // 寫入欄位名稱
    fprintf( detailF, "w1,w2,θ,x1,x2,y,t,δW1,δW2,δθ\n" ) ;
    Efile = fopen("./E.csv", "w");
    while(times <= EPOCH){
        grafcet0();
    }
    system("pause");
    fclose(detailF);
    fclose(Efile);
    return 0;
}

void grafcet0(){
    PrintState();
    if (x0 == 1){
        x0 = 0;
        x1 = 1;
    }
    else if(x1 == 1 && x13 == 1){
        x1 = 0;
        x13 = 0;
        x10 = 1;
        x2 = 1;
    }
    else if(x2 == 1 && x22 == 1){
        x2 = 0;
        x22 = 0;
        x20 = 1;
        x0 = 1;
    }
    else if(x2 == 1 && x23 == 1){
        x2 = 0;
        x23 = 0;
        x20 = 1;
        x0 = 1;
    }
    action0();
}

void action0(){
    if(x1 == 1){
        grafcet1();
    }
    else if(x2 == 1){
        grafcet2();
    }
}

void grafcet1(){
    if (x10 == 1){
        x10 = 0;
        x11 = 1;
    }
    else if(x11 == 1){
        x11 = 0;
        x12 = 1;
    }
    else if(x12 == 1){
        x12 = 0;
        x13 = 1;
    }
    else if(x13 == 1){
        x13 = 0;
        x10 = 1;
    }
    action1();
}

void action1(){
    if(x11 == 1){
        SetXT();
        StoreCurrentValue();
    }
    else if(x12 == 1){
        Learning(x, t);
    }
    else if(x13 == 1){
        ECur = Testing(x, t);
    }
}

void grafcet2(){
    if (x20 == 1){
        x20 = 0;
        x21 = 1;
    }
    else if(x21 == 1 && times % 4 != 0){
        x21 = 0;
        x22 = 1;
    }
    else if(x21 == 1 && times % 4 == 0){
        x21 = 0;
        x23 = 1;
    }
    else if(x22 == 1){
        x22 = 0;
        x20 = 1;
    }
    else if(x23 == 1){
        x23 = 0;
        x20 = 1;
    }
    action2();
}

void action2(){
    if(x21 == 1){
        WriteLearningDetail();
    }
    else if(x22 == 1){
        AddE();
    }
    else if(x23 == 1){
        WriteAvgE();
    }
}


void SetXT(){
    printf("設定訓練資料\n\n");
    if (times % 4 == 0){
        x[0] = 0, x[1] = 0;
        t = 0;
    }
    else if (times % 4 == 1){
        x[0] = 0, x[1] = 1;
        t = 0;
    }
    else if (times % 4 == 2){
        x[0] = 1, x[1] = 0;
        t = 0;
    }
    else if (times % 4 == 3){
        x[0] = 1, x[1] = 1;
        t = 1;
    }
}

void StoreCurrentValue(){
    weightPre[0] = weight[0];
    weightPre[1] = weight[1];
    biasPre = bias;
}

// 寫入結果檔案
void WriteLearningDetail(){
    printf("記錄學習過程數值\n\n");
    // w1,w2,θ,x1,x2,y,t,δW1,δW2,δθ
    fprintf( detailF, "%f,", weightPre[0] ) ;
    fprintf( detailF, "%f,", weightPre[1] ) ;
    fprintf( detailF, "%f,", biasPre ) ;
    fprintf( detailF, "%f,", x[0] ) ;  
    fprintf( detailF, "%f,", x[1] ) ; 
    fprintf( detailF, "%f,", y ) ;  
    fprintf( detailF, "%d,", t ) ;  
    // fprintf( fp, "%.1f,", eta * ( t - y ) ) ; 
    fprintf( detailF, "%f,", weight[0]-weightPre[0]);
    fprintf( detailF, "%f,", weight[1]-weightPre[1]);
    fprintf( detailF, "%f\n", bias - biasPre);
    // fprintf( fp, "%f,", ( eta * ( t - y ) * weight[0] ) ) ; 
    // fprintf( fp, "%f,", ( eta * ( t - y ) * weight[1] ) ) ; 
    // fprintf( fp, "%f\n", ( eta * ( t - y ) ) ) ; 
    
}

void AddE(){
    printf("加本次誤差值\n\n");
    E += ECur;
}

void WriteAvgE(){
    printf("紀錄誤差總和平均\n\n");
    fprintf( Efile, "%f\n", E/4);
    E = 0;
}

void PrintState(){
    printf("x0=%d x1=%d x2=%d x10=%d x11=%d x12=%d x13=%d x20=%d x21=%d x22=%d x23=%d\n\n", x0, x1, x2, x10, x11, x12, x13, x20, x21, x22, x23);
}

//感知機轉移函數
float ActivationFunction(float net)
{
	return 1.0/(1.0+exp(-net));
}

//感知機推論輸出
float Stimulate(float *x)
{
	float net=0;
	for(int i=0;i<nb_in;i++)net+=x[i]*weight[i];
	net+=bias;
	return ActivationFunction(net);
}



// 感知機學習演算法
void Learning(float *x, float target)
{
    printf("感知機學習\n\n");
    times++;
	float net=0;
	for(int i=0;i<nb_in;i++)net+=x[i]*weight[i];
	net+=bias;
	y=ActivationFunction(net);
	for(int i=0;i<nb_in;i++) 
        weight[i] = weight[i] + eta*(target-y)* x[i];
	bias = bias + eta*(target-y);
}

float Testing(float *x, float target)
{
    printf("感知機分類性能測試\n\n");
	float y_s=Stimulate(x);
	return  sqrt((y_s-target)*(y_s-target));
}
