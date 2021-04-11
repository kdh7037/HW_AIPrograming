#include <iostream>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <fstream>
#include <string>

/*
인공지능 #2 과제
2015920005 김대현
개발 환경
운영체제 : Windows 10
빌드 도구 : Visual Studio 2019 (v142)
개발 언어 : C++14
*/

using namespace std;

float c = 0.05;                //learning rate      
int loop = 0;                  //무한루프 확인용

class Perceptron {
private:
	int layer;						//몇 층인가?
	int trainnum;					//test case의 개수
	int incorrect = 1;				//test case의 실패 개수
	int* node;						//각각의 layer의 node 개수, node[0]에는 input 개수가 들어가며, 그 뒤로는 layer의 개수, node[layer]는 1이 들어간다.

	float* train_set_y;				//목표값
	float** train_set_x;			//입력값
	float** x;                      //net을 저장하는 배열
	float*** w;						//가중치를 저장하는 배열
	float** deltax;					//x편미분값(변화량)을 저장하는 배열
	float*** deltaw;				//가중치 변화량을 저장하는 배열                                                                                               

public:
	void Reset_weight();					//가중치 재조정
	void Calculate_net();                   //net 재조정
	void Weight_Initialize();				//가중치 랜덤 초기화
	void Backpropagation(float t);			//오차 역전파
	void Training();						//반복 학습
	void File_Write();						//최종 가중치 파일 생성
	float Elu(float nett);					//활성화 함수
	float Delu(float elu);					//활성화 함수의 미분

	Perceptron() {
		cout << "몇 층 퍼셉트론을 활용합니까?(2 이상의 자연수로 입력) : ";
		cin >> layer;
		node = new int[layer + 1];
		cout << "Input 차원이 몇입니까?(자연수로 입력) : ";
		cin >> node[0];			
		cout << "tast case는 몇 개입니까?(자연수로 입력) : ";
		cin >> trainnum;


		for (int i = 1; i < layer; i++) {
			cout << i << "번 째 layer에는 몇 개의 유닛이 존재합니까?(1보다 큰 자연수로 입력) : ";
			cin >> node[i];
			if (node[i] <= 1) {
				cout << "잘못 입력하셨습니다. 다시 입력해주세요." << endl;
				i--;
				continue;
			}
		}
		node[layer] = 1;

		train_set_x = new float* [trainnum];			//input값 입력
		cout << "input값을 입력해주세요. : " << endl;
		for (int i = 0; i < trainnum; i++) {
			train_set_x[i] = new float[node[0]];
			for (int j = 0; j < node[0]; j++) {
				cin >> train_set_x[i][j];
			}
		}
		cout << endl;

		train_set_y = new float[trainnum];				//output값 입력
		cout << "output값을 입력해주세요. : " << endl;
		for (int i = 0; i < trainnum; i++) {
			cin >> train_set_y[i];
		}

		w = new float** [layer];				//w, deltaw는 3차원 배열로 구성, [layer의 개수][현재 layer의 node 순서][다음 layer의 node 순서]로 저장된다.
		deltaw = new float** [layer];
		x = new float* [layer + 1];				//x, deltax는 2차원 배열로 구성, [layer의 개수 + 1][현재 layer의 node 순서]로 저장된다.
		deltax = new float* [layer + 1];

		for (int i = 0; i < layer; i++) {
			w[i] = new float* [node[i] + 1];	//threshold를 위해 각각 한자리 더 추가
			deltaw[i] = new float* [node[i] + 1];	
			x[i] = new float[node[i] + 1];
			deltax[i] = new float[node[i] + 1];
			for (int j = 0; j < node[i] + 1; j++) {
				w[i][j] = new float[node[i + 1]];
				deltaw[i][j] = new float[node[i + 1]];
			}
		}
		x[layer] = new float[1];			//마지막 결과는 무조건 출력 1개입니다.
		deltax[layer] = new float[1];

		srand((unsigned int)time(NULL));
		rand();                           //초기 rand설정하여 안정적인 rand 설정
		Weight_Initialize();              //가중치를 랜덤한 값으로 초기화
		while (incorrect != 0) {		//incorrect가 0이 아닐시, train을 다시 실행한다.

			Training();
			
			//cout << incorrect << "번 틀렸습니다. 반복하여 진행합니다." << endl;
			//cout << "-------------------------------------------------" << endl;
			loop++;
		}


		cout << loop << "번 후에 학습이 종료되었습니다. " << endl;
		loop = 0;
	}
};



void Perceptron::Weight_Initialize() {			//가중치 랜덤 초기화
	for (int i = 0; i < layer; i++) {
		for (int j = 0; j < node[i] + 1; j++) {
			for (int k = 0; k < node[i + 1]; k++) {
				w[i][j][k] = rand() / (float)RAND_MAX * 2.0f - 1.0f;      //가중치 -0.01 ~ 0.01로 랜덤설정
			}
		}
	}

	//cout << "현재 " << endl;					//가중치 출력
	for (int i = 0; i < layer; i++) {
		for (int j = 0; j < node[i]; j++) {
			for (int k = 0; k < node[i + 1]; k++) {
				//cout << "w" << i << j << k << " = " << w[i][j][k] << " ";
			}
			//cout << endl;
		}
		//cout << endl;
	}

	for (int i = 0; i < layer; i++) {
		for (int k = 0; k < node[i + 1]; k++) {
			//cout << "threshold" << i << k << " = " << w[i][node[i]][k] << " ";
		}
		//cout << endl;
	}


		//cout << "입니다. ㅎㅎ" << endl;
	//cout << "--------------------------------------------------------------------" << endl;
}

void Perceptron::Calculate_net() {            //각각의 net을 계산하는 함수
	for (int i = 1; i < layer + 1; i++) {
		for (int j = 0; j < node[i]; j++) {
			x[i][j] = 0;
		}
	}
	for (int i = 1; i < layer + 1; i++) {	//net 계산
		for (int k = 0; k < node[i]; k++) {
			for (int j = 0; j < node[i - 1] + 1; j++) {
				x[i][k] = x[i][k] + w[i - 1][j][k] * x[i - 1][j];   //순차적 x 값을 기준으로
			}
			x[i][k] = Elu(x[i][k]);			//활성화 함수 처리
		}
	}
	for (int i = 0; i < layer + 1; i++) {
		for (int j = 0; j < node[i]; j++) {
			//cout << "x" << i << j << "의 값은 " << x[i][j] << endl;
		}
	}
}


void Perceptron::Backpropagation(float t) {
	for (int i = 0; i < layer; i++) {										//deltax 0으로 초기화
		for (int j = 0; j < node[i] + 1; j++) {
			deltax[i][j] = 0;
		}
	}
	deltax[layer][0] = -(t - x[layer][0]) * Delu(x[layer][0]);				//맨 마지막 델타
	for (int i = layer - 1; i > 0; i--) {
		for (int j = 0; j < node[i] + 1; j++) {
			for (int k = 0; k < node[i + 1]; k++) {
				deltax[i][j] = deltax[i][j] + deltax[i + 1][k] * w[i][j][k];	//델타값 백 프로퍼게이션
				deltaw[i][j][k] = deltax[i + 1][k] * x[i][j];					//가중치 변화량 백 프로퍼게이션
			}
			deltax[i][j] = Delu(x[i][j]) * deltax[i][j];						//활성화 함수 미분값 곱
		}
	}
	for (int j = 0; j < node[0] + 1; j++) {											//가중치 변화량 최종 계산
		for (int k = 0; k < node[1]; k++) {
			deltaw[0][j][k] = deltax[1][k] * x[0][j];
		}
	}
}

void Perceptron::Reset_weight() {												//가중치 재설정
	for (int i = 0; i < layer; i++) {
		for (int j = 0; j < node[i]; j++) {
			for (int k = 0; k < node[i + 1]; k++) {
				//cout << "w" << i << j << k << " = " << w[i][j][k] << " -> ";
				w[i][j][k] = w[i][j][k] - c * deltaw[i][j][k];
				//cout << w[i][j][k] << endl;
			}
		}
	}
	for (int i = 0; i < layer; i++) {
		for (int k = 0; k < node[i + 1]; k++) {
			//cout << "threshold" << i << k << " = " << w[i][node[i]][k] << " -> ";
			w[i][node[i]][k] = w[i][node[i]][k] - c * deltaw[i][node[i]][k];
			//cout << w[i][node[i]][k] << endl;
		}
	}
}

void Perceptron::Training() {													//학습 함수
	//cout << loop << "번 째 시도" << endl;

	incorrect = 0;																

	for (int i = 0; i < trainnum; i++) {										//x[0]에 입력값 대입
		for (int j = 0; j < node[0]; j++) {
			x[0][j] = train_set_x[i][j];
		}

		for (int j = 0; j < layer; j++) {										//x의 마지막 행에는 -1을 넣어 threshold값을 부여한다.
			x[j][node[j]] = -1;
		}

		Calculate_net();														//net 계산

		float error = pow(train_set_y[i] - x[layer][0], 2) * 0.5;				//에러 범위
		if (error <= 0.05) {													//에러 범위 내의 값이면 통과
			continue;
		}	
		incorrect++;															//범위 밖의 값이면 incorrect값 증가
		Backpropagation(train_set_y[i]);										//오차 역전파
		Reset_weight();															//가중치 초기화
	}
}

float Perceptron::Elu(float nett) {												//활성화 함수 ELU활용
	if (nett >= 0)
		return nett;
	else if (nett < 0)
		return exp(nett) - 1;
}

float Perceptron::Delu(float elu) {												//활성화 함수의 미분, 활성화 함수가 적용된 값으로 구할 수 있다.
	if (elu >= 0)
		return 1;
	else if (elu < 0)
		return elu + 1;
}

void Perceptron::File_Write() {													//최종 가중치 파일 생성
	//string out_line;
	ofstream out("result.txt");
	for (int i = 0; i < layer; i++) {
		for (int k = 0; k < node[i + 1]; k++) {
			out << "threshold" << i << k << " = " << w[i][node[i]][k] << endl;
		}
	}
	for (int i = 0; i < layer; i++) {											//x의 각각의 마지막 행은 threshold
		for (int j = 0; j < node[i]; j++) {
			for (int k = 0; k < node[i + 1]; k++) {
				out << "w" << i << j << k << " = " << w[i][j][k] << endl;
			}
		}
	}
	out.close();
}

int main() {
	Perceptron AND;
	AND.File_Write();

	Perceptron OR;
	OR.File_Write();

	Perceptron XOR;
	XOR.File_Write();

	Perceptron DONUT;
	DONUT.File_Write();

	return 0;
}