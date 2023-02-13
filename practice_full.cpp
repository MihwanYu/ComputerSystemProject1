#include <iostream>
#include <string>
#include <vector>//���� ũ�� �迭�� ����� ���� ���
#define LEN  30   // 30�̳� 50�̳� �Ȱ���
#define RLEN 9   // ������ �Ѿ ���� ǥ�� ����������, �Ҽ��� 9�ڸ� �̻���ʹ� ��Ȯ���� ������
//https://computer-science-student.tistory.com/83 ���� ����
//���� value�߰� 2�������)       insert(�ε���, ��)   push_back(��)

using namespace std;
vector<int> intarr(string str);
//string ���� ��� method~~ at: ���ڿ��� Ư�� �ε��� ���� ��ȯ
//substr(f, l): index f ���� index (l-1)������ ���ڿ� ��ȯ(l�� ������ f���� ������)
//erase(k): index k���� ������ ���ڿ����� ����
//insert(n, m, 'c'): index n���� m���� 'c'�� ä��

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

    //private Ÿ���� ��ȣ, �Ҽ���, ������, �м��� �������� �Լ�
    string getSign() { return sign; }
    int getDot(){return dotIdx;}
    string getDecimal(){return decimal;}
    string getFrac(){ return frac;}
    void changeSign() { //����->���� ȣ���, ����->���� ȣ��� ���
        if (sign=="-") sign="+"; else sign="-";
    }
};

vector<int> intarr(string str){//str�� decimal�̳� frac(���ڿ�)�� �޾ƿͼ� 5�ڸ��� ���� ���� ������
    vector<int> vec;//12121212 ���� vec size=2, 121 / 21212
    while (str.length()>4){//str ���� 5���� �۰ų� ������������ �ݺ�
        vec.push_back(stoi( str.substr(str.length()-4)  )   );//�ڿ��� 5��° ��ġ���� ������-> ������ 5�� ���ڿ� �����ؼ� vec ���� ���ʿ� ����
        str = str.substr(0,str.length()-4);//�迭�� �� 5�� ������ ����
    }
    vec.push_back( stoi(str.substr(0)));//5���� �۰ų� ������ �ٷ� �־���
    return vec;
}

string StringFloat::operator+(StringFloat another){
    vector<int> frac_ans, deci_ans;//frac_ans, deci_ans: ���� ������, �м��� dump���� ���� dump ���� ����
    string sumFrac ="", sumDeci = "", temp;//sumFrac, sumDeci: ���� frac_ans, deci_ans�� ����� ���͸� ���ڿ��� ��ȯ�� �� ������ ���ڿ�
    int i, carry=0;//carry: �ø� �� ���� ����

    for(i=0; i<fracArr.size(); i++){//�м� ��Ʈ ���� ����
        frac_ans.push_back(fracArr[i]+another.fracArr[i]);
    }
    for(i=0; i<another.decimalArr.size(); i++){//���� ��Ʈ ���� ����
        deci_ans.push_back(decimalArr[i]+another.decimalArr[i]); }
    for(;i<decimalArr.size(); i++){
        deci_ans.push_back(decimalArr[i]);  
    }
    
    /*frac_ans[i] �Ǵ� deci_ans[i] ���� 10000���� Ŀ���� 1�� �ø���, �� ������(10000���� �������� ������)�� �ٽ� frac_ans[i]�� �����Ѵ�
    ����� 10000�̸��� ���� "_ _ _ _" �� �ڸ� ���� ���ڿ� �������� �ٲ㼭 ���� sumFrac, sumDeci�� �����Ѵ�
    �Ҽ� �κп��� �������� �ö�� �ø� ���� ������ �״�� �����κ� ���ڿ��� �÷��ش�
    (�׷��� carry ������ ����, �Ҽ��� ���� �����ؼ� ��)
    */
    
    //�м��κ� ���� -> ���ڿ��� �ٲ���(���� ���, 0000����)
    for(i=0; i<frac_ans.size(); i++){
        temp="0000";//temp: ���� 90 ������ 0090���� �ٲ㼭 ���ڿ��� ������(���� 1921->���� 1921, ���� 1-> ���� 0001)
        frac_ans[i]+=carry;

        if (frac_ans[i]>9999) {carry=1; frac_ans[i] %= 10000; } else carry=0;

        temp = temp.replace(4-to_string(frac_ans[i]).length(), to_string(frac_ans[i]).length(), to_string(frac_ans[i]));
        sumFrac = temp + sumFrac;
    }

    //�����κ� ���� -> ���ڿ��� �ٲ���
    for(i=0; i<deci_ans.size(); i++){
        temp="0000";
        deci_ans[i]+=carry;

        if (deci_ans[i]>9999) { carry=1; deci_ans[i] %= 10000;} else carry=0;

        temp = temp.replace(4-to_string(deci_ans[i]).length(), to_string(deci_ans[i]).length(), to_string(deci_ans[i]));
        sumDeci = temp + sumDeci;
    }

    string result = sumDeci + "." + sumFrac;
    // cout<<"���� �� result: "<<result<<endl;
    result = trim(result); //��,�ڿ� �پ��ִ� 0 ������ ex) 0098.7620 --> 98.762
    if(getSign()=="-" && result!="0.0") return getSign()+result;//���� �� -0.0 ������ ��� ������ -�� �ٿ��� ����
    else return result;//����� ��ȣ + �����ϰ� ����
}

string StringFloat::operator-(StringFloat another){
    //this�� another���� ���� ũ�ٰ� ����, ���� �� ū ���� ��ȣ �˾Ƽ� ���ϰ���
    string finsign;
    vector<int> frac_ans, deci_ans; //���� ��� ���� ����
    int i;

    if (StringFloat(n_obj)>another){//��ȣ�����ϱ�: this�� another���� ���� ū ���
        finsign = getSign();
        cout<<"n1�� ���� �� ũ��"<<endl;
    } 
    else {//this�� another���� ���� ���� ���
        if (getSign()=="+") finsign = "-";
        else finsign="+";
    }

    if(finsign=="+"){
    //���� ū��-�������� ������...������-ū�ִ� �Ųٷ� �ǵ��� �ٲ��ֱ�...
        for(i=0; i<fracArr.size(); i++){//�м� ��Ʈ ���� ����
            frac_ans.push_back(fracArr[i]-another.fracArr[i]);
        }
        for(i=0; i<another.decimalArr.size(); i++){//���� ��Ʈ ���� ����
            deci_ans.push_back(decimalArr[i]-another.decimalArr[i]);   }
        for(;i<decimalArr.size(); i++){
            deci_ans.push_back(decimalArr[i]);   }
    }else{
    //���� ������-ū�ִ� �Ųٷ� �ǵ��� �ٲ��ֱ�...
        for(i=0; i<fracArr.size(); i++){//�м� ��Ʈ ���� ����
            frac_ans.push_back(another.fracArr[i]-fracArr[i]);
        }
        for(i=0; i<decimalArr.size(); i++){//���� ��Ʈ ���� ����
            deci_ans.push_back(another.decimalArr[i]-decimalArr[i]);  }
        for(;i<another.decimalArr.size(); i++){
            deci_ans.push_back(another.decimalArr[i]);   }
    }
    // cout<<"<<<<<<<<<<<<�߰�����>>>>>>>>>>>"<<endl;
    // for(i=0; i<deci_ans.size(); i++){
    //     cout<<frac_ans[i]<<", ";
    // }
    // cout<<endl;
    string subFrac ="", subDeci = "", temp="0000";
    int carry=0;//���� �� ���� ����(���������� �ø� ���� ������)
    for(i=0; i<frac_ans.size(); i++){//�м��κ� ���� -> ���ڿ��� �ٲ���(���� ���, 0000����)
        temp="0000";
        frac_ans[i]-=carry;
        if (frac_ans[i]<0) {frac_ans[i] += 10000; carry=1;} else carry=0;
        temp = temp.replace(4-to_string(frac_ans[i]).length(), to_string(frac_ans[i]).length(), to_string(frac_ans[i]));
        subFrac = temp + subFrac;
    }
    for(i=0; i<deci_ans.size(); i++){//�����κ� ���� -> ���ڿ��� �ٲ���
        temp="0000";
        deci_ans[i]-=carry;
        if (deci_ans[i]<0) {deci_ans[i] += 10000; carry=1;} else carry=0;
        temp = temp.replace(4-to_string(deci_ans[i]).length(), to_string(deci_ans[i]).length(), to_string(deci_ans[i]));
        subDeci = temp + subDeci;
    }
    string result = subDeci + "." + subFrac;
    result = trim(result);//�翷 0 ������
    if(finsign=="-" && result!="0.0") return finsign+result;//���� �� -0.0 ������ ��� ������ -�� �ٿ��� ����
    else return result;//����� ��ȣ + �����ϰ� ����
}

string StringFloat::operator*(StringFloat another){
    int i, j, k;
    string finsign;
    vector<int> finResultArr;
    
    int lshift = frac.length()+another.getFrac().length();//�Ҽ����� ������ ������ ���°���� �� ����
    vector<int> resultArr1 = intarr(getDecimal()+getFrac());
    vector<int> resultArr2 = intarr(another.getDecimal()+another.getFrac()); 
    
    if(getSign()==another.getSign()){
        finsign = "+";
    }
    else{
        finsign = "-";
    }
    vector<int> resultArr3(resultArr1.size()+resultArr2.size()-1);
    //{7891,3456,12}, {6543,987}�� ���� ��, 3+2-1=4, {0,0,0,0}���� �ʱ�ȭ�� ���� ����

    for(i = 0; i<resultArr1.size(); i++){
        for(j = 0; j<resultArr2.size(); j++){
            k = (i+j);
            resultArr3[k] += resultArr1[i] * resultArr2[j];
            //k�ε����� ���� ������
            // cout<<"�ε��� k: "<<k<<", �������� ���� ���: "<<resultArr1[i] * resultArr2[j]<<endl;
        }
    }

    int carry=0;
    // cout<<"\n<<<<<<<<<<<<<<<<<<<<<<<�߰�����>>>>>>>>>>>>>>>>>>>>>>>>"<<endl;
    // cout<<"������� �������� ��: ";
    string temp, finalVal="", result;
        for(i=0; i<resultArr3.size(); i++){//�����κ� ���� -> ���ڿ��� �ٲ���
        temp="0000";//0009
        resultArr3[i]+=carry;
        if (resultArr3[i]>9999) {carry=resultArr3[i]/10000; resultArr3[i] %= 10000; } else carry=0;
        temp = temp.replace(4-to_string(resultArr3[i]).length(), to_string(resultArr3[i]).length(), to_string(resultArr3[i]));
        finalVal = temp + finalVal;
        // cout<<temp<<", ";
    }
    // cout<<carry<<endl;
    finalVal = to_string(carry) + finalVal;
    //final���ٰ� �Ҽ��� �ֱ�
    finalVal = finalVal.insert(finalVal.length()-lshift, ".");
    result = trim(finalVal);
    if(finsign=="-" && result!="0.0") return finsign+result;//���� �� -0.0 ������ ��� ������ -�� �ٿ��� ����
    else return result;//����� ��ȣ + �����ϰ� ����
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

    string num1 = getDecimal()+getFrac();//�Ҽ��� ����
    string num2 = another.getDecimal()+another.getFrac();
    //34.3434 ���� num�� ���ؼ� �Ҽ��� ���� �� 8�ڸ��� ����(0ä��ų� �ڿ� �ڸ���)
    // cout<<"num1: "<<num1<<", num2: "<<num2<<endl;

    if (num1.length()>LEN) //LEN���� �ڸ��� ũ�� LEN�ڸ������� ����� �ڴ� �ڸ���
        num1.erase(LEN);
    else                    //LEN���� �ڸ��� ������ LEN���� 0ä���
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
    
        quotient = -1;//���� quotient ���� �����ϴ°� �³�?
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
    if(finsign=="-" && result!="0.00000000 e(0)") return finsign+result;//���� �� -0.0 ������ ��� ������ -�� �ٿ��� ����
    return result;//����� ��ȣ + �����ϰ� ����
}

bool StringFloat::operator>(StringFloat another){
    //�ڸ���Ȯ��, another�� ũ�� |��|<|��|
    if(getDot()<another.getDot()) return false;

    //�ڸ����� �����ѵ� ���� ū ���� dump ������ |��| < |��|
    if(getDot()==another.getDot() && decimalArr.back()<another.decimalArr.back()) return false;

    //������ 0�� �� �Ҽ��� �Ʒ� �κ� ���� ���: �Ҽ��� �Ʒ� 0�� �ƴ� ���� ����� ���� ���� �ڸ��� ã��
    if(getDecimal()=="0" && another.getDecimal()=="0"){
        int idx = 0;
        while(getFrac()[idx]=='0' && another.getFrac()[idx]=='0') idx++;
        //�Ҽ��� �Ʒ� ���� ���غ��� |��| < |��| �� ���. ex) 0.01, 0.02
        if((getFrac()[idx]-'0') < (another.getFrac()[idx])-'0') return false;
    }
    return true;
}

void StringFloat::setFloatarr(int n){//2�� ���ڿ��� �м� �κ� ���� �����ָ鼭(n����) 4�ڸ����� ��ҷ� ���� ���� �ʱ�ȭ
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

string StringFloat::trim(string origin){//���� �� or �м� �� 0�� �������ִ� �Լ�
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
    // n1 = "1", n2 = "0";//�� ���ϸ� +. ���� ������ 9.999999���ȡڡڡڡڡڡ�
    // n1 = "0", n2 = "1"; 
    // n1 = "-12.34567", n2 = "999.666";
    // n1 = "25.1", n2 = "3.000001";
    // n1 = "112.9", n2="345.67891";
    // n1 = "12345.67891", n2 = "98.76543";
    // n1 = "121212121212121212121212.1212121212121212";
    // n2 = "98989898989898989898989898.9898989898989898989898";
    // n1 = "1000", n2 = "8";
    // n1 = "-1", n2 = "0"//�ڡڡڡڡڡ� ���� Ʋ��
    // n1 = "-123456789123456789123456789123456789123456789", n2 = "123456789123456789123456789123456789";
    // n1 = "-0.002", n2 = "0.00000000914";//�ڡڡڡڡڡ� ���� Ʋ��
    string n1, n2, answer, operate="";
    char con;
    int input=-1;

    while(true){
        cout<<"\nenter your two numbers: ";
        cin>> n1;
        cout<<"enter your another number: ";
        cin>> n2;  

        cout<<"n1: "<<n1<<", n2: "<<n2<<endl;
        StringFloat f1(n1);//fr, f2 ��ü ����
        StringFloat f2(n2);

        // �м��� ������ ��������-�����ڷ� �Ҽ��� ���� ���� �Ұ���: �ΰ� ������ �Ҽ��� ���̸� �� �� �־�� �Ҽ��� ���� ������־
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
            else if(f1.getSign()=="-"){//��ȣ�� �ٸ��� ���� �θ���
                f1.changeSign();//��ȣ �����ϰ� ������
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