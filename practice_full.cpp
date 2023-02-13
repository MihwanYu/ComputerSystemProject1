#include <iostream>
#include <string>
#include <vector>//가변 크기 배열을 만들기 위해 사용
#define LEN  30   // 30이나 50이나 똑같음
#define RLEN 9   // 범위를 넘어간 값을 표현 가능하지만, 소수점 9자리 이상부터는 정확도가 떨어짐
//https://computer-science-student.tistory.com/83 벡터 사용법
//벡터 value추가 2가지방법)       insert(인덱스, 값)   push_back(값)

using namespace std;
vector<int> intarr(string str);
//string 관련 사용 method~~ at: 문자열의 특정 인덱스 문자 반환
//substr(f, l): index f 부터 index (l-1)까지의 문자열 반환(l이 없으면 f부터 끝까지)
//erase(k): index k부터 끝까지 문자열에서 제거
//insert(n, m, 'c'): index n부터 m개의 'c'를 채움

class StringFloat{
private:
    string n_obj;//original full string number `n` except sign : need at `operator-`, to call `operator>`.
    string sign;//determination "+" or "-"
    string decimal;//string for decimal part
    string frac;//string for fraction part
    int dotIdx;//dot region - to distingush decimal part and fraction part

public:
    //if string number `n` is 123456.7890123, decimalArr = {3456, 12}, fracArr = {7890, 123}
    vector<int> decimalArr;//Four digit integer arrangement. Initialized by method `intarr`. {1234, 5678, 9...}
    vector<int> fracArr;//Four digit frac arrangement. Initialized by method `setFloatarr`. {1234, 5678, 9....}
    StringFloat(string n){

        //determine sign, and then erase it from n
        if(n.at(0)=='-'){
            sign="-"; n = n.substr(1);
        }else if(n.at(0)=='+'){
            sign="+"; n = n.substr(1);
        }else sign="+";
        
        n_obj = n;//original string `n` except sign in saved
        //dot index is determined: if it doesn't exists, regard it's exists in the last
        dotIdx = (n.find('.')==-1)? n.length() : n.find('.');
        //based on the decimal point, the front area is determined as the integer part
        decimal = n.substr(0, dotIdx);

        //based on the decimal point, the back area is determined as the fraction part
        //if `n` is natural number so that there isn't back area, regard `n` is `n.0` thus 0 is fraction part
        frac = (dotIdx==n.length())? "0": n.substr(dotIdx+1);
        //from decimal part decimalArr is initialized by calling method `intarr`
        decimalArr = intarr(decimal);
    }
    
    //operator overloading methods: +, -, *, /, >
    string operator+(StringFloat another);
    string operator-(StringFloat another);
    string operator*(StringFloat another);
    string operator/(StringFloat another);
    bool operator>(StringFloat another);

    //method - returns four digit frac arrangement from string for fraction part
    void setFloatarr(int n);
    string trim(string origin);

    //private 타입인 부호, 소수점, 정수부, 분수부 가져오는 함수
    string getSign() { return sign; }
    int getDot(){return dotIdx;}
    string getDecimal(){return decimal;}
    string getFrac(){ return frac;}
    void changeSign() { //덧셈->뺄셈 호출시, 뺄셈->덧셈 호출시 사용
        if (sign=="-") sign="+"; else sign="-";
    }
};

vector<int> intarr(string str){//str은 decimal이나 frac(문자열)을 받아와서 5자리씩 끊긴 숫자 리턴함
    vector<int> vec;//12121212 들어가면 vec size=2, 121 / 21212
    while (str.length()>4){//str 길이 5보다 작거나 같아질때까지 반복
        vec.push_back(stoi( str.substr(str.length()-4)  )   );//뒤에서 5번째 위치부터 끝까지-> 마지막 5개 문자열 추출해서 vec 벡터 앞쪽에 삽입
        str = str.substr(0,str.length()-4);//배열을 뒤 5개 제거해 저장
    }
    vec.push_back( stoi(str.substr(0)));//5보다 작거나 같으면 바로 넣어줌
    return vec;
}

string StringFloat::operator+(StringFloat another){
    vector<int> frac_ans, deci_ans;//frac_ans, deci_ans: 각각 정수부, 분수부 dump끼리 합한 dump 저장 벡터
    string sumFrac ="", sumDeci = "", temp;//sumFrac, sumDeci: 각각 frac_ans, deci_ans에 저장된 벡터를 문자열로 전환한 값 저장할 문자열
    int i, carry=0;//carry: 올림 수 저장 변수

    for(i=0; i<fracArr.size(); i++){//분수 파트 벡터 생성
        frac_ans.push_back(fracArr[i]+another.fracArr[i]);
    }
    for(i=0; i<another.decimalArr.size(); i++){//정수 파트 벡터 생성
        deci_ans.push_back(decimalArr[i]+another.decimalArr[i]); }
    for(;i<decimalArr.size(); i++){
        deci_ans.push_back(decimalArr[i]);  
    }
    
    /*frac_ans[i] 또는 deci_ans[i] 값이 10000보다 커지면 1을 올리고, 그 나머지(10000으로 나눴을때 나머지)를 다시 frac_ans[i]에 저장한다
    저장된 10000미만의 수를 "_ _ _ _" 네 자리 숫자 문자열 형식으로 바꿔서 각각 sumFrac, sumDeci에 저장한다
    소수 부분에서 마지막에 올라온 올림 수가 있으면 그대로 정수부분 문자열에 올려준다
    (그래서 carry 변수를 정수, 소수가 같이 공유해서 씀)
    */
    
    //분수부분 벡터 -> 문자열로 바꿔줌(내림 고려, 0000형식)
    for(i=0; i<frac_ans.size(); i++){
        temp="0000";//temp: 숫자 90 들어오면 0090으로 바꿔서 문자열에 더해줌(숫자 1921->문자 1921, 숫자 1-> 문자 0001)
        frac_ans[i]+=carry;

        if (frac_ans[i]>9999) {carry=1; frac_ans[i] %= 10000; } else carry=0;

        temp = temp.replace(4-to_string(frac_ans[i]).length(), to_string(frac_ans[i]).length(), to_string(frac_ans[i]));
        sumFrac = temp + sumFrac;
    }

    //정수부분 벡터 -> 문자열로 바꿔줌
    for(i=0; i<deci_ans.size(); i++){
        temp="0000";
        deci_ans[i]+=carry;

        if (deci_ans[i]>9999) { carry=1; deci_ans[i] %= 10000;} else carry=0;

        temp = temp.replace(4-to_string(deci_ans[i]).length(), to_string(deci_ans[i]).length(), to_string(deci_ans[i]));
        sumDeci = temp + sumDeci;
    }

    string result = sumDeci + "." + sumFrac;
    // cout<<"덧셈 속 result: "<<result<<endl;
    result = trim(result); //앞,뒤에 붙어있는 0 지워줌 ex) 0098.7620 --> 98.762
    if(getSign()=="-" && result!="0.0") return getSign()+result;//음수 중 -0.0 제외한 모든 수에는 -를 붙여서 리턴
    else return result;//양수는 부호 + 생략하고 리턴
}

string StringFloat::operator-(StringFloat another){
    //this가 another보다 절댓값 크다고 가정, 뺄셈 후 큰 수의 부호 알아서 붙일거임
    string finsign;
    vector<int> frac_ans, deci_ans; //뺄셈 결과 저장 벡터
    int i;

    if (StringFloat(n_obj)>another){//부호결정하기: this가 another보다 절댓값 큰 경우
        finsign = getSign();
        cout<<"n1이 절댓값 더 크다"<<endl;
    } 
    else {//this가 another보다 절댓값 작은 경우
        if (getSign()=="+") finsign = "-";
        else finsign="+";
    }

    if(finsign=="+"){
    //절댓값 큰애-작은애의 로직임...작은애-큰애는 거꾸로 되도록 바꿔주기...
        for(i=0; i<fracArr.size(); i++){//분수 파트 벡터 생성
            frac_ans.push_back(fracArr[i]-another.fracArr[i]);
        }
        for(i=0; i<another.decimalArr.size(); i++){//정수 파트 벡터 생성
            deci_ans.push_back(decimalArr[i]-another.decimalArr[i]);   }
        for(;i<decimalArr.size(); i++){
            deci_ans.push_back(decimalArr[i]);   }
    }else{
    //절댓값 작은애-큰애는 거꾸로 되도록 바꿔주기...
        for(i=0; i<fracArr.size(); i++){//분수 파트 벡터 생성
            frac_ans.push_back(another.fracArr[i]-fracArr[i]);
        }
        for(i=0; i<decimalArr.size(); i++){//정수 파트 벡터 생성
            deci_ans.push_back(another.decimalArr[i]-decimalArr[i]);  }
        for(;i<another.decimalArr.size(); i++){
            deci_ans.push_back(another.decimalArr[i]);   }
    }
    // cout<<"<<<<<<<<<<<<중간점검>>>>>>>>>>>"<<endl;
    // for(i=0; i<deci_ans.size(); i++){
    //     cout<<frac_ans[i]<<", ";
    // }
    // cout<<endl;
    string subFrac ="", subDeci = "", temp="0000";
    int carry=0;//내림 수 저장 변수(덧셈에서는 올림 수를 저장함)
    for(i=0; i<frac_ans.size(); i++){//분수부분 벡터 -> 문자열로 바꿔줌(내림 고려, 0000형식)
        temp="0000";
        frac_ans[i]-=carry;
        if (frac_ans[i]<0) {frac_ans[i] += 10000; carry=1;} else carry=0;
        temp = temp.replace(4-to_string(frac_ans[i]).length(), to_string(frac_ans[i]).length(), to_string(frac_ans[i]));
        subFrac = temp + subFrac;
    }
    for(i=0; i<deci_ans.size(); i++){//정수부분 벡터 -> 문자열로 바꿔줌
        temp="0000";
        deci_ans[i]-=carry;
        if (deci_ans[i]<0) {deci_ans[i] += 10000; carry=1;} else carry=0;
        temp = temp.replace(4-to_string(deci_ans[i]).length(), to_string(deci_ans[i]).length(), to_string(deci_ans[i]));
        subDeci = temp + subDeci;
    }
    string result = subDeci + "." + subFrac;
    result = trim(result);//양옆 0 지워줌
    if(finsign=="-" && result!="0.0") return finsign+result;//음수 중 -0.0 제외한 모든 수에는 -를 붙여서 리턴
    else return result;//양수는 부호 + 생략하고 리턴
}

string StringFloat::operator*(StringFloat another){
    int i, j, k;
    string finsign;
    vector<int> finResultArr;
    
    int lshift = frac.length()+another.getFrac().length();//소수점이 오른쪽 끝에서 몇번째인지 합 구함
    vector<int> resultArr1 = intarr(getDecimal()+getFrac());
    vector<int> resultArr2 = intarr(another.getDecimal()+another.getFrac()); 
    
    if(getSign()==another.getSign()){
        finsign = "+";
    }
    else{
        finsign = "-";
    }
    vector<int> resultArr3(resultArr1.size()+resultArr2.size()-1);
    //{7891,3456,12}, {6543,987}이 있을 때, 3+2-1=4, {0,0,0,0}으로 초기화된 벡터 생성

    for(i = 0; i<resultArr1.size(); i++){
        for(j = 0; j<resultArr2.size(); j++){
            k = (i+j);
            resultArr3[k] += resultArr1[i] * resultArr2[j];
            //k인덱스에 값을 더해줌
            // cout<<"인덱스 k: "<<k<<", 더해지는 곱셈 결과: "<<resultArr1[i] * resultArr2[j]<<endl;
        }
    }

    int carry=0;
    // cout<<"\n<<<<<<<<<<<<<<<<<<<<<<<중간점검>>>>>>>>>>>>>>>>>>>>>>>>"<<endl;
    // cout<<"순서대로 더해지는 값: ";
    string temp, finalVal="", result;
        for(i=0; i<resultArr3.size(); i++){//정수부분 벡터 -> 문자열로 바꿔줌
        temp="0000";//0009
        resultArr3[i]+=carry;
        if (resultArr3[i]>9999) {carry=resultArr3[i]/10000; resultArr3[i] %= 10000; } else carry=0;
        temp = temp.replace(4-to_string(resultArr3[i]).length(), to_string(resultArr3[i]).length(), to_string(resultArr3[i]));
        finalVal = temp + finalVal;
        // cout<<temp<<", ";
    }
    // cout<<carry<<endl;
    finalVal = to_string(carry) + finalVal;
    //final에다가 소수점 넣기
    finalVal = finalVal.insert(finalVal.length()-lshift, ".");
    result = trim(finalVal);
    if(finsign=="-" && result!="0.0") return finsign+result;//음수 중 -0.0 제외한 모든 수에는 -를 붙여서 리턴
    else return result;//양수는 부호 + 생략하고 리턴
}
    
string StringFloat::operator/(StringFloat another){
    if(getSign()=="+" && another.getDecimal()=="0" && another.getFrac()=="0") return "inf";
    else if(getSign()=="-" && another.getDecimal()=="0" && another.getFrac()=="0") return "-inf";

    string finsign;
    int E1 = getDot()-1, E2 = another.getDot()-1;
    int E = E1-E2;
    
    if(getSign()==another.getSign())
        finsign = "+";
    else
        finsign = "-";

    string num1 = getDecimal()+getFrac();//소수점 제거
    string num2 = another.getDecimal()+another.getFrac();
    //34.3434 같은 num에 대해서 소수점 없이 총 8자리로 만듦(0채우거나 뒤에 자르기)
    // cout<<"num1: "<<num1<<", num2: "<<num2<<endl;

    if (num1.length()>LEN) //LEN보다 자릿수 크면 LEN자리까지만 남기고 뒤는 자르기
        num1.erase(LEN);
    else                    //LEN보다 자릿수 작으면 LEN까지 0채우기
        num1.insert(num1.end(), LEN - num1.length(), '0');
    
    if (num2.length()>LEN)
        num2.erase(LEN);
    else
        num2.insert(num2.end(), LEN - num2.length(), '0');

    // cout<<"num1: "<<num1<<", num2: "<<num2<<endl;

    float num1_f = stof(num1);
    float num2_f = stof(num2);

    // cout<<"num1_f: "<<num1_f<<", num2_f: "<<num2_f;
    int M[RLEN];
    int quotient;
        for (int j = 0; j < RLEN; j++) {
    
        quotient = -1;//여기 quotient 새로 생성하는게 맞나?
        for (int i = 0; i < 10; i++) {
            if (num2_f * i <= num1_f) {
                quotient += 1;
            }
        }
        M[j] = quotient;
        num1_f = 10*(num1_f - (num2_f * quotient));
    }

    string result;
    for (int i = 0; i < RLEN; i++) {
        result.append(to_string(M[i]));
    }
    result.insert(1, ".");
    // cout<<finsign << result << "e(" << E << ")";
    result = result+" e(" + to_string(E)+")";
    if(finsign=="-" && result!="0.00000000 e(0)") return finsign+result;//음수 중 -0.0 제외한 모든 수에는 -를 붙여서 리턴
    return result;//양수는 부호 + 생략하고 리턴
}

bool StringFloat::operator>(StringFloat another){
    //자릿수확인, another이 크면 |나|<|쟤|
    if(getDot()<another.getDot()) return false;

    //자릿수는 동일한데 가장 큰 정수 dump 절댓값이 |나| < |쟤|
    if(getDot()==another.getDot() && decimalArr.back()<another.decimalArr.back()) return false;

    //정수는 0일 때 소수점 아래 부분 절댓값 계산: 소수점 아래 0이 아닌 값이 저장된 가장 빠른 자릿수 찾기
    if(getDecimal()=="0" && another.getDecimal()=="0"){
        int idx = 0;
        while(getFrac()[idx]=='0' && another.getFrac()[idx]=='0') idx++;
        //소수점 아래 절댓값 비교해보니 |나| < |쟤| 인 경우. ex) 0.01, 0.02
        if((getFrac()[idx]-'0') < (another.getFrac()[idx])-'0') return false;
    }
    return true;
}

void StringFloat::setFloatarr(int n){//2개 문자열의 분수 부분 길이 맞춰주면서(n으로) 4자리수를 요소로 갖는 벡터 초기화
    vector<int> vec;
    string temp_s = frac;
    int count = 0;
    while(temp_s.length()>=4){
        vec.insert(vec.begin(), stoi( temp_s.substr(0, 4)  )  );
        temp_s = temp_s.substr(4);
        count++;
    }
    while(count<n){
        string zeros="";
        if (temp_s.length()>0){
            for(int i=temp_s.length(); i<4; i++){ temp_s += "0";}
            vec.insert(vec.begin(), stoi(temp_s));
            temp_s="";
        }else{ vec.insert(vec.begin(), 0 );  }
        count++;
    }
    fracArr = vec;
}

string StringFloat::trim(string origin){//정수 앞 or 분수 뒤 0들 제거해주는 함수
    // cout<<"before: "<<origin;
    while(origin.at(0)=='0'){
        origin = origin.substr(1);
    }
    while(origin.at(origin.length()-1)=='0'){
        origin = origin.substr(0, origin.length()-1);
    }
    if (origin.at(0)=='.') origin.insert(0,"0");
    if (origin.at(origin.length()-1)=='.') origin=origin+"0";
    // cout<<", after: "<<origin<<endl;
    return origin;
}

int main(){
    //----------------TEST CASE---------------------
    // n1 = "9999999999.99999999999999", n2 = "11111.111";
    // n1 = "100000000", n2 = "300";
    // n1 = "53.95", n2 = "195.488";
    // n1 = "123456.78901234", n2 = "9.00000000000009";
    // n1 = "1", n2 = "0";//얘 곱하면 +. 나옴 나누면 9.999999나옴★★★★★★
    // n1 = "0", n2 = "1"; 
    // n1 = "-12.34567", n2 = "999.666";
    // n1 = "25.1", n2 = "3.000001";
    // n1 = "112.9", n2="345.67891";
    // n1 = "12345.67891", n2 = "98.76543";
    // n1 = "121212121212121212121212.1212121212121212";
    // n2 = "98989898989898989898989898.9898989898989898989898";
    // n1 = "1000", n2 = "8";
    // n1 = "-1", n2 = "0"//★★★★★★ 덧셈 틀림
    // n1 = "-123456789123456789123456789123456789123456789", n2 = "123456789123456789123456789123456789";
    // n1 = "-0.002", n2 = "0.00000000914";//★★★★★★ 덧셈 틀림
    string n1, n2, answer, operate="";
    char con;
    int input=-1;

    while(true){
        cout<<"\nenter your two numbers: ";
        cin>> n1;
        cout<<"enter your another number: ";
        cin>> n2;  

        cout<<"n1: "<<n1<<", n2: "<<n2<<endl;
        StringFloat f1(n1);//fr, f2 객체 생성
        StringFloat f2(n2);

        // 분수부 세팅은 직접해줌-생성자로 소수부 벡터 생성 불가능: 두개 숫자의 소수점 길이를 알 수 있어야 소수부 벡터 만들수있어서
        if (f1.getFrac().length()>f2.getFrac().length()){
            f1.setFloatarr( f1.getFrac().length()/4+1  );
            f2.setFloatarr( f1.getFrac().length()/4+1  );
        }else{
            f1.setFloatarr( f2.getFrac().length()/4+1  );
            f2.setFloatarr( f2.getFrac().length()/4+1  );
        }
    
        cout<<"\nselect options 1-4: "<<endl;
        cout<<"1. Addition\n2. Subtraction\n3. Multiplication\n4. Division\n\ninput: ";
        cin>>input;
        if(input==1){//case 1 Addition
            if (f1.getSign()==f2.getSign()) answer=(f1+f2);
            else if(f1.getSign()=="-"){//부호가 다르면 뺄셈 부르기
                f1.changeSign();//부호 동일하게 맞춰줌
                answer = (f2-f1);
            }else{
                f2.changeSign();
                answer = (f1-f2);
            }
            operate = "+";
        }
        else if(input==2){//case 2 Subtraction
            if(f1.getSign()==f2.getSign()) answer = (f1-f2);
            else{
                f2.changeSign();
                answer = (f1+f2);
            }
            operate = "-";
        }
        else if(input==3){//case 3 Multiplication
            answer = (f1*f2);
            operate = "*";
        }
        else if(input==4){//case 4 Division
            answer = (f1/f2);
            operate = "/";
        }
        else continue;
        cout<<"\n-------ANSWER-------"<<endl;
        cout<<n1<<operate<<"("<<n2<<")= "<<answer<<endl;
        cout<<"wanna continue?(Y/n)";
        cin>>con;
        if(!(con=='Y' || con=='y')) break;
    }

    return 0;
}