#include <iostream>
#include <cstdlib>
#include <ctime>
#include <math.h>

/*
인공지능 #2 과제
2015920005 김대현
개발 환경
운영체제 : Windows 10
빌드 도구 : Visual Studio 2019 (v142)
개발 언어 : C++14
*/

using namespace std;

float c = 0.05;						//running mate		
int loop = 0;						//무한루프 확인용

class Perceptron {
private:
	int pow_two;						//2^num									
	int num;							//Input 개수											
	float* net;							//한 싸이클의 net을 저장하는 배열
	float* w;							//가중치 저장 배열
	float threshold;

public:
	void set_AND_Perceptron();			//AND-gate 학습
	void set_OR_Perceptron();			//OR-gate 학습
	void set_XOR_Perceptron();			//XOR-gate 학습
	void Reset_weight(int bit, int t, int o);	//가중치 재조정
	void Calculate_net();						//net 재조정
	void Initialize();

	Perceptron() {
		cout << fixed;
		cout.precision(2);						//float 프린트를 소수점 2자리까지
		cout << "Input 차원이 몇입니까?(10 이내의 자연수로 입력) : ";
		cin >> num;
		while (!(num > 0 && num <= 10)) {		//예외처리
			cout << "다르게 입력하셨습니다. 다시 입력하세요 : ";
			cin >> num;
		}
		pow_two = pow(2, num);
		w = new float[num];
		net = new float[pow(2, num)];
		srand((unsigned int)time(NULL));
		rand();									//초기 rand설정하여 안정적인 rand 설정
		Initialize();							//가중치 및 threshold를 랜덤한 값으로 초기화
		Calculate_net();						//net 계산
	}
};

void Perceptron::Initialize() {
	for (int i = 0; i < num; i++)
		w[i] = rand() / (float)RAND_MAX * 4.0f - 2.0f;		//가중치 -2 ~ 2로 랜덤설정
	threshold = rand() / (float)RAND_MAX * 2.0f;			//threshold 0 ~ 2로 랜덤설정
	cout << "현재 ";
	for (int n = 0; n < num; n++) {            //Input 차원이 n인 1-layer perceptron일 경우의 코드
		cout << "w" << n + 1 << " = " << w[n] << ", ";
	}
	cout << "threshold = " << threshold << " 입니다." << endl;
	printf("%.2f=%.2fx+%.2fy\n", threshold, w[0], w[1]);
	cout << "--------------------------------------------------------------------" << endl;
}

void Perceptron::Calculate_net() {				//각각의 net을 계산하는 함수
	for (int i = 0; i < pow_two; i++) {
		net[i] = 0;
		for (int j = 0; j < num; j++) {
			net[i] = net[i] + w[j] * (float)((i & (pow_two >> (j + 1))) >> (num - j - 1));	//순차적 x 값을 기준으로 계산
		}
	}
}

void Perceptron::Reset_weight(int bit, int t, int o) {	//가중치 재설정
	int temp_bit;										//각각의 가중치에 대한 x의 값
	cout << "w값을 재설정합니다." << endl;
	for (int i = 0; i < num; i++) {
		temp_bit = (bit & (pow_two >> (i + 1)));		//비트 연산을 통하여 x의 값들을 분류시킨다.
		temp_bit >>= (num - i - 1);
		cout << "w" << i << " = " << w[i] << " -> ";
		w[i] = w[i] + c * (t - o) * temp_bit;			//가중치 재설정
		cout << "w" << i << " = " << w[i] << endl;
	}
	printf("%.2f=%.2fx+%.2fy\n", threshold, w[0], w[1]);
	Calculate_net();						//가중치 재설정에 따른 net 재설정
}

void Perceptron::set_AND_Perceptron() {		//AND-gate 학습
	int incorrect = 0;
	cout << "AND gate" << endl;
	cout << fixed;
	cout.precision(2);
	for (int i = 0; i < pow_two - 1; i++) {		//0이 나오도록 학습
		if (net[i] <= threshold) {
			cout << "net" << i << " <= threshold, output 0 ...... 일치" << endl;
		}
		else {
			cout << "net" << i << " > threshold, output 1 .... 불일치" << endl;
			Reset_weight(i, 0, 1);
			incorrect++;
		}
	}
	if (net[pow_two - 1] <= threshold) {		//마지막은 1이 나와야 함
		cout << "net" << pow_two - 1 << " <= threshold, output 0 .... 불일치" << endl;
		Reset_weight(pow_two - 1, 1, 0);
		incorrect++;
	}
	else {
		cout << "net" << pow_two - 1 << " > threshold, output 1 ...... 일치" << endl;
	}
	if (incorrect == 0) {
		cout << "모든 결과값이 AND gate와 일치합니다." << endl;
		cout << "--------------------------------------------------------------------" << endl;
		cout << "--------------------------------------------------------------------" << endl;
		return;
	}
	else {
		cout << "output이 " << incorrect << "회 틀렸습니다. 반복 학습하여 진행합니다." << endl;
		cout << "--------------------------------------------------------------------" << endl;
		loop++;
		if (loop == 100) {
			cout << "100번의 진행에도 학습을 완료하지 못했습니다." << endl;
			loop = 0;
			return;
		}
		set_AND_Perceptron();				//재귀적 반복
	}
}
void Perceptron::set_OR_Perceptron() {		//OR-gate 학습
	int incorrect = 0;
	cout << "OR gate" << endl;
	cout << fixed;
	cout.precision(2);
	if (net[0] <= threshold) {				//처음에는 0이 나와야 함
		cout << "net0 <= threshold, output 0 ...... 일치" << endl;
	}
	else {
		cout << "net0 > threshold, output 1 .... 불일치" << endl;
		Reset_weight(0, 0, 1);
		incorrect++;
	}
	for (int i = 1; i < pow_two; i++) {		//그 뒤에는 1이 나오도록 학습
		if (net[i] <= threshold) {
			cout << "net" << i << " <= threshold, output 0 .... 불일치" << endl;
			Reset_weight(i, 1, 0);
			incorrect++;
		}
		else {
			cout << "net" << i << " > threshold, output 1 ...... 일치" << endl;
		}
	}
	if (incorrect == 0) {
		cout << "모든 결과값이 OR gate와 일치합니다." << endl;
		cout << "--------------------------------------------------------------------" << endl;
		cout << "--------------------------------------------------------------------" << endl;
		return;
	}
	else {
		cout << "output이 " << incorrect << "회 틀렸습니다. 반복 학습하여 진행합니다." << endl;
		cout << "--------------------------------------------------------------------" << endl;
		loop++;
		if (loop == 100) {
			cout << "100번의 진행에도 학습을 완료하지 못했습니다." << endl;
			loop = 0;
			return;
		}
		set_OR_Perceptron();			//재귀적으로 반복
	}
}

void Perceptron::set_XOR_Perceptron() {	//XOR-gate 학습
	int incorrect = 0;
	cout << "XOR gate" << endl;
	cout << fixed;
	cout.precision(2);
	for (int i = 0; i < pow_two; i++) {
		int totalbits = 0;				//x를 xor연산 하기위하여, x의 1의 갯수를 저장하는 변수
		unsigned int bits = i;
		for (int j = 0; j < num; j++) {	//totalbits 계산
			totalbits += bits & 1;
			bits >>= 1;
		}
		if (totalbits % 2 == 0) {		//x의 1의 갯수가 짝수일 경우, output 0
			if (net[i] <= threshold)
				cout << "net" << i << " <= threshold, output 0 ...... 일치" << endl;
			else {
				cout << "net" << i << " > threshold, output 1 .... 불일치" << endl;
				Reset_weight(i, 0, 1);
				incorrect++;
			}
		}
		else {							//x의 1의 갯수가 홀수일 경우, output 1
			if (net[i] <= threshold) {
				cout << "net" << i << " <= threshold, output 0 .... 불일치" << endl;
				Reset_weight(i, 1, 0);
				incorrect++;
			}
			else
				cout << "net" << i << " > threshold, output 1 ...... 일치" << endl;
		}
	}
	if (incorrect == 0) {
		cout << "모든 결과값이 XOR gate와 일치합니다." << endl;
		cout << "--------------------------------------------------------------------" << endl;
		cout << "--------------------------------------------------------------------" << endl;
		return;
	}
	else {
		cout << "output이 " << incorrect << "회 틀렸습니다. 반복 학습하여 진행합니다." << endl;
		cout << "--------------------------------------------------------------------" << endl;
		loop++;
		if (loop == 100) {
			cout << "100번의 진행에도 학습을 완료하지 못했습니다." << endl;
			loop = 0;
			return;
		}
		set_XOR_Perceptron();		//재귀적으로 반복
	}
}

int main() {
	Perceptron p;

	p.set_AND_Perceptron();
	p.Initialize();
	p.set_OR_Perceptron();
	p.Initialize();
	p.set_XOR_Perceptron();

	return 0;
}