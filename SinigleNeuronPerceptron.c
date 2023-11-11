# include <stdio.h>
# include <math.h>

# define EPOCH 20*4
# define nb_in 2

void gracfet0();
void gracfet1();
void action0();
void action1();
void SetXT();
void StoreCurrentValue();
void WriteResult();
float ActivationFunction(float);
float Stimulate(float*);
void Learning(float*, float);
float Testing(float*, float);


//For grafcet
int x0 = 1, x1 = 0, x2 = 0,x10 = 1, x11 = 0, x12 = 0, x13 = 0;
int start = 1;

float E;
float x[nb_in];
float weight[nb_in] = {0, 0.4};
float weightPre[nb_in] = {0, 0};
float y;
int t = 0, tPre = 0;
float bias = 0.3, biasPre;
float eta = 0.1, etaPre = 0;
int times = 0;
FILE *fp = NULL ;
FILE *Efile = NULL ;

int main(){
    fp = fopen( "./test.csv", "w" ) ;
    // 寫入欄位名稱
    fprintf( fp, "w1,w2,θ,x1,x2,η,y,t,δW1,δW2,δθ\n" ) ;
    Efile = fopen("./e.csv", "w");
    while(times <= EPOCH){
        gracfet0();
    }
    return 0;
}

void gracfet0(){
    if (x0 == 1 && start == 1){
        x0 = 0;
        x1 = 1;
    }
    else if(x1 == 1 && x13 == 1){
        x1 = 0;
        x13 = 0;
        x10 = 1;
        x2 = 1;
    }
    else if(x2 == 1){
        x2 = 0;
        x0 = 1;
    }
    action0();
}

void action0(){
    printf("x0=%d, x1=%d, x2=%d\n", x0, x1, x2);
    if(x1 == 1){
        gracfet1();
    }
    else if(x2 == 1){
        WriteResult();
    }
}

void gracfet1(){
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
    printf("x10=%d, x11=%d, x12=%d, x13=%d\n", x10, x11, x12, x13);
    if(x11 == 1){
        SetXT();
        StoreCurrentValue();
    }
    else if(x12 == 1){
        Learning(x, t);
    }
    else if(x13 == 1){
        E = Testing(x, t);
    }
}


void SetXT(){
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
void WriteResult(){
    // w1,w2,θ,x1,x2,η,y,t,δW1,δW2,δθ
    fprintf( fp, "%f,", weightPre[0] ) ;
    fprintf( fp, "%f,", weightPre[1] ) ;
    fprintf( fp, "%f,", biasPre ) ;
    fprintf( fp, "%f,", x[0] ) ;  
    fprintf( fp, "%f,", x[1] ) ; 
    fprintf( fp, "%f,", eta ) ; 
    fprintf( fp, "%f,", y ) ;  
    fprintf( fp, "%f,", t ) ;  
    // fprintf( fp, "%.1f,", eta * ( t - y ) ) ; 
    fprintf( fp, "%f,", weight[0]-weightPre[0]);
    fprintf( fp, "%f,", weight[1]-weightPre[1]);
    fprintf( fp, "%f\n", bias - biasPre);
    // fprintf( fp, "%f,", ( eta * ( t - y ) * weight[0] ) ) ; 
    // fprintf( fp, "%f,", ( eta * ( t - y ) * weight[1] ) ) ; 
    // fprintf( fp, "%f\n", ( eta * ( t - y ) ) ) ; 
    if (times % 4 == 0)
        fprintf( Efile, "%f\n", E);
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
	float y=Stimulate(x);
	return  sqrt((y-target)*(y-target));
}
