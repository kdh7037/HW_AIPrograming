#include <iostream>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <fstream>
#include <string>

/*
�ΰ����� #2 ����
2015920005 �����
���� ȯ��
�ü�� : Windows 10
���� ���� : Visual Studio 2019 (v142)
���� ��� : C++14
*/

using namespace std;

float c = 0.05;                //learning rate      
int loop = 0;                  //���ѷ��� Ȯ�ο�

class Perceptron {
private:
	int layer;						//�� ���ΰ�?
	int trainnum;					//test case�� ����
	int incorrect = 1;				//test case�� ���� ����
	int* node;						//������ layer�� node ����, node[0]���� input ������ ����, �� �ڷδ� layer�� ����, node[layer]�� 1�� ����.

	float* train_set_y;				//��ǥ��
	float** train_set_x;			//�Է°�
	float** x;                      //net�� �����ϴ� �迭
	float*** w;						//����ġ�� �����ϴ� �迭
	float** deltax;					//x��̺а�(��ȭ��)�� �����ϴ� �迭
	float*** deltaw;				//����ġ ��ȭ���� �����ϴ� �迭                                                                                               

public:
	void Reset_weight();					//����ġ ������
	void Calculate_net();                   //net ������
	void Weight_Initialize();				//����ġ ���� �ʱ�ȭ
	void Backpropagation(float t);			//���� ������
	void Training();						//�ݺ� �н�
	void File_Write();						//���� ����ġ ���� ����
	float Elu(float nett);					//Ȱ��ȭ �Լ�
	float Delu(float elu);					//Ȱ��ȭ �Լ��� �̺�

	Perceptron() {
		cout << "�� �� �ۼ�Ʈ���� Ȱ���մϱ�?(2 �̻��� �ڿ����� �Է�) : ";
		cin >> layer;
		node = new int[layer + 1];
		cout << "Input ������ ���Դϱ�?(�ڿ����� �Է�) : ";
		cin >> node[0];			
		cout << "tast case�� �� ���Դϱ�?(�ڿ����� �Է�) : ";
		cin >> trainnum;


		for (int i = 1; i < layer; i++) {
			cout << i << "�� ° layer���� �� ���� ������ �����մϱ�?(1���� ū �ڿ����� �Է�) : ";
			cin >> node[i];
			if (node[i] <= 1) {
				cout << "�߸� �Է��ϼ̽��ϴ�. �ٽ� �Է����ּ���." << endl;
				i--;
				continue;
			}
		}
		node[layer] = 1;

		train_set_x = new float* [trainnum];			//input�� �Է�
		cout << "input���� �Է����ּ���. : " << endl;
		for (int i = 0; i < trainnum; i++) {
			train_set_x[i] = new float[node[0]];
			for (int j = 0; j < node[0]; j++) {
				cin >> train_set_x[i][j];
			}
		}
		cout << endl;

		train_set_y = new float[trainnum];				//output�� �Է�
		cout << "output���� �Է����ּ���. : " << endl;
		for (int i = 0; i < trainnum; i++) {
			cin >> train_set_y[i];
		}

		w = new float** [layer];				//w, deltaw�� 3���� �迭�� ����, [layer�� ����][���� layer�� node ����][���� layer�� node ����]�� ����ȴ�.
		deltaw = new float** [layer];
		x = new float* [layer + 1];				//x, deltax�� 2���� �迭�� ����, [layer�� ���� + 1][���� layer�� node ����]�� ����ȴ�.
		deltax = new float* [layer + 1];

		for (int i = 0; i < layer; i++) {
			w[i] = new float* [node[i] + 1];	//threshold�� ���� ���� ���ڸ� �� �߰�
			deltaw[i] = new float* [node[i] + 1];	
			x[i] = new float[node[i] + 1];
			deltax[i] = new float[node[i] + 1];
			for (int j = 0; j < node[i] + 1; j++) {
				w[i][j] = new float[node[i + 1]];
				deltaw[i][j] = new float[node[i + 1]];
			}
		}
		x[layer] = new float[1];			//������ ����� ������ ��� 1���Դϴ�.
		deltax[layer] = new float[1];

		srand((unsigned int)time(NULL));
		rand();                           //�ʱ� rand�����Ͽ� �������� rand ����
		Weight_Initialize();              //����ġ�� ������ ������ �ʱ�ȭ
		while (incorrect != 0) {		//incorrect�� 0�� �ƴҽ�, train�� �ٽ� �����Ѵ�.

			Training();
			
			//cout << incorrect << "�� Ʋ�Ƚ��ϴ�. �ݺ��Ͽ� �����մϴ�." << endl;
			//cout << "-------------------------------------------------" << endl;
			loop++;
		}


		cout << loop << "�� �Ŀ� �н��� ����Ǿ����ϴ�. " << endl;
		loop = 0;
	}
};



void Perceptron::Weight_Initialize() {			//����ġ ���� �ʱ�ȭ
	for (int i = 0; i < layer; i++) {
		for (int j = 0; j < node[i] + 1; j++) {
			for (int k = 0; k < node[i + 1]; k++) {
				w[i][j][k] = rand() / (float)RAND_MAX * 2.0f - 1.0f;      //����ġ -0.01 ~ 0.01�� ��������
			}
		}
	}

	//cout << "���� " << endl;					//����ġ ���
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


		//cout << "�Դϴ�. ����" << endl;
	//cout << "--------------------------------------------------------------------" << endl;
}

void Perceptron::Calculate_net() {            //������ net�� ����ϴ� �Լ�
	for (int i = 1; i < layer + 1; i++) {
		for (int j = 0; j < node[i]; j++) {
			x[i][j] = 0;
		}
	}
	for (int i = 1; i < layer + 1; i++) {	//net ���
		for (int k = 0; k < node[i]; k++) {
			for (int j = 0; j < node[i - 1] + 1; j++) {
				x[i][k] = x[i][k] + w[i - 1][j][k] * x[i - 1][j];   //������ x ���� ��������
			}
			x[i][k] = Elu(x[i][k]);			//Ȱ��ȭ �Լ� ó��
		}
	}
	for (int i = 0; i < layer + 1; i++) {
		for (int j = 0; j < node[i]; j++) {
			//cout << "x" << i << j << "�� ���� " << x[i][j] << endl;
		}
	}
}


void Perceptron::Backpropagation(float t) {
	for (int i = 0; i < layer; i++) {										//deltax 0���� �ʱ�ȭ
		for (int j = 0; j < node[i] + 1; j++) {
			deltax[i][j] = 0;
		}
	}
	deltax[layer][0] = -(t - x[layer][0]) * Delu(x[layer][0]);				//�� ������ ��Ÿ
	for (int i = layer - 1; i > 0; i--) {
		for (int j = 0; j < node[i] + 1; j++) {
			for (int k = 0; k < node[i + 1]; k++) {
				deltax[i][j] = deltax[i][j] + deltax[i + 1][k] * w[i][j][k];	//��Ÿ�� �� �����۰��̼�
				deltaw[i][j][k] = deltax[i + 1][k] * x[i][j];					//����ġ ��ȭ�� �� �����۰��̼�
			}
			deltax[i][j] = Delu(x[i][j]) * deltax[i][j];						//Ȱ��ȭ �Լ� �̺а� ��
		}
	}
	for (int j = 0; j < node[0] + 1; j++) {											//����ġ ��ȭ�� ���� ���
		for (int k = 0; k < node[1]; k++) {
			deltaw[0][j][k] = deltax[1][k] * x[0][j];
		}
	}
}

void Perceptron::Reset_weight() {												//����ġ �缳��
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

void Perceptron::Training() {													//�н� �Լ�
	//cout << loop << "�� ° �õ�" << endl;

	incorrect = 0;																

	for (int i = 0; i < trainnum; i++) {										//x[0]�� �Է°� ����
		for (int j = 0; j < node[0]; j++) {
			x[0][j] = train_set_x[i][j];
		}

		for (int j = 0; j < layer; j++) {										//x�� ������ �࿡�� -1�� �־� threshold���� �ο��Ѵ�.
			x[j][node[j]] = -1;
		}

		Calculate_net();														//net ���

		float error = pow(train_set_y[i] - x[layer][0], 2) * 0.5;				//���� ����
		if (error <= 0.05) {													//���� ���� ���� ���̸� ���
			continue;
		}	
		incorrect++;															//���� ���� ���̸� incorrect�� ����
		Backpropagation(train_set_y[i]);										//���� ������
		Reset_weight();															//����ġ �ʱ�ȭ
	}
}

float Perceptron::Elu(float nett) {												//Ȱ��ȭ �Լ� ELUȰ��
	if (nett >= 0)
		return nett;
	else if (nett < 0)
		return exp(nett) - 1;
}

float Perceptron::Delu(float elu) {												//Ȱ��ȭ �Լ��� �̺�, Ȱ��ȭ �Լ��� ����� ������ ���� �� �ִ�.
	if (elu >= 0)
		return 1;
	else if (elu < 0)
		return elu + 1;
}

void Perceptron::File_Write() {													//���� ����ġ ���� ����
	//string out_line;
	ofstream out("result.txt");
	for (int i = 0; i < layer; i++) {
		for (int k = 0; k < node[i + 1]; k++) {
			out << "threshold" << i << k << " = " << w[i][node[i]][k] << endl;
		}
	}
	for (int i = 0; i < layer; i++) {											//x�� ������ ������ ���� threshold
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