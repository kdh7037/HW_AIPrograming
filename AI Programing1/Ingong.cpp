#include <iostream>
#include <cstdlib>
#include <ctime>
#include <math.h>

/*
�ΰ����� #2 ����
2015920005 �����
���� ȯ��
�ü�� : Windows 10
���� ���� : Visual Studio 2019 (v142)
���� ��� : C++14
*/

using namespace std;

float c = 0.05;						//running mate		
int loop = 0;						//���ѷ��� Ȯ�ο�

class Perceptron {
private:
	int pow_two;						//2^num									
	int num;							//Input ����											
	float* net;							//�� ����Ŭ�� net�� �����ϴ� �迭
	float* w;							//����ġ ���� �迭
	float threshold;

public:
	void set_AND_Perceptron();			//AND-gate �н�
	void set_OR_Perceptron();			//OR-gate �н�
	void set_XOR_Perceptron();			//XOR-gate �н�
	void Reset_weight(int bit, int t, int o);	//����ġ ������
	void Calculate_net();						//net ������
	void Initialize();

	Perceptron() {
		cout << fixed;
		cout.precision(2);						//float ����Ʈ�� �Ҽ��� 2�ڸ�����
		cout << "Input ������ ���Դϱ�?(10 �̳��� �ڿ����� �Է�) : ";
		cin >> num;
		while (!(num > 0 && num <= 10)) {		//����ó��
			cout << "�ٸ��� �Է��ϼ̽��ϴ�. �ٽ� �Է��ϼ��� : ";
			cin >> num;
		}
		pow_two = pow(2, num);
		w = new float[num];
		net = new float[pow(2, num)];
		srand((unsigned int)time(NULL));
		rand();									//�ʱ� rand�����Ͽ� �������� rand ����
		Initialize();							//����ġ �� threshold�� ������ ������ �ʱ�ȭ
		Calculate_net();						//net ���
	}
};

void Perceptron::Initialize() {
	for (int i = 0; i < num; i++)
		w[i] = rand() / (float)RAND_MAX * 4.0f - 2.0f;		//����ġ -2 ~ 2�� ��������
	threshold = rand() / (float)RAND_MAX * 2.0f;			//threshold 0 ~ 2�� ��������
	cout << "���� ";
	for (int n = 0; n < num; n++) {            //Input ������ n�� 1-layer perceptron�� ����� �ڵ�
		cout << "w" << n + 1 << " = " << w[n] << ", ";
	}
	cout << "threshold = " << threshold << " �Դϴ�." << endl;
	printf("%.2f=%.2fx+%.2fy\n", threshold, w[0], w[1]);
	cout << "--------------------------------------------------------------------" << endl;
}

void Perceptron::Calculate_net() {				//������ net�� ����ϴ� �Լ�
	for (int i = 0; i < pow_two; i++) {
		net[i] = 0;
		for (int j = 0; j < num; j++) {
			net[i] = net[i] + w[j] * (float)((i & (pow_two >> (j + 1))) >> (num - j - 1));	//������ x ���� �������� ���
		}
	}
}

void Perceptron::Reset_weight(int bit, int t, int o) {	//����ġ �缳��
	int temp_bit;										//������ ����ġ�� ���� x�� ��
	cout << "w���� �缳���մϴ�." << endl;
	for (int i = 0; i < num; i++) {
		temp_bit = (bit & (pow_two >> (i + 1)));		//��Ʈ ������ ���Ͽ� x�� ������ �з���Ų��.
		temp_bit >>= (num - i - 1);
		cout << "w" << i << " = " << w[i] << " -> ";
		w[i] = w[i] + c * (t - o) * temp_bit;			//����ġ �缳��
		cout << "w" << i << " = " << w[i] << endl;
	}
	printf("%.2f=%.2fx+%.2fy\n", threshold, w[0], w[1]);
	Calculate_net();						//����ġ �缳���� ���� net �缳��
}

void Perceptron::set_AND_Perceptron() {		//AND-gate �н�
	int incorrect = 0;
	cout << "AND gate" << endl;
	cout << fixed;
	cout.precision(2);
	for (int i = 0; i < pow_two - 1; i++) {		//0�� �������� �н�
		if (net[i] <= threshold) {
			cout << "net" << i << " <= threshold, output 0 ...... ��ġ" << endl;
		}
		else {
			cout << "net" << i << " > threshold, output 1 .... ����ġ" << endl;
			Reset_weight(i, 0, 1);
			incorrect++;
		}
	}
	if (net[pow_two - 1] <= threshold) {		//�������� 1�� ���;� ��
		cout << "net" << pow_two - 1 << " <= threshold, output 0 .... ����ġ" << endl;
		Reset_weight(pow_two - 1, 1, 0);
		incorrect++;
	}
	else {
		cout << "net" << pow_two - 1 << " > threshold, output 1 ...... ��ġ" << endl;
	}
	if (incorrect == 0) {
		cout << "��� ������� AND gate�� ��ġ�մϴ�." << endl;
		cout << "--------------------------------------------------------------------" << endl;
		cout << "--------------------------------------------------------------------" << endl;
		return;
	}
	else {
		cout << "output�� " << incorrect << "ȸ Ʋ�Ƚ��ϴ�. �ݺ� �н��Ͽ� �����մϴ�." << endl;
		cout << "--------------------------------------------------------------------" << endl;
		loop++;
		if (loop == 100) {
			cout << "100���� ���࿡�� �н��� �Ϸ����� ���߽��ϴ�." << endl;
			loop = 0;
			return;
		}
		set_AND_Perceptron();				//����� �ݺ�
	}
}
void Perceptron::set_OR_Perceptron() {		//OR-gate �н�
	int incorrect = 0;
	cout << "OR gate" << endl;
	cout << fixed;
	cout.precision(2);
	if (net[0] <= threshold) {				//ó������ 0�� ���;� ��
		cout << "net0 <= threshold, output 0 ...... ��ġ" << endl;
	}
	else {
		cout << "net0 > threshold, output 1 .... ����ġ" << endl;
		Reset_weight(0, 0, 1);
		incorrect++;
	}
	for (int i = 1; i < pow_two; i++) {		//�� �ڿ��� 1�� �������� �н�
		if (net[i] <= threshold) {
			cout << "net" << i << " <= threshold, output 0 .... ����ġ" << endl;
			Reset_weight(i, 1, 0);
			incorrect++;
		}
		else {
			cout << "net" << i << " > threshold, output 1 ...... ��ġ" << endl;
		}
	}
	if (incorrect == 0) {
		cout << "��� ������� OR gate�� ��ġ�մϴ�." << endl;
		cout << "--------------------------------------------------------------------" << endl;
		cout << "--------------------------------------------------------------------" << endl;
		return;
	}
	else {
		cout << "output�� " << incorrect << "ȸ Ʋ�Ƚ��ϴ�. �ݺ� �н��Ͽ� �����մϴ�." << endl;
		cout << "--------------------------------------------------------------------" << endl;
		loop++;
		if (loop == 100) {
			cout << "100���� ���࿡�� �н��� �Ϸ����� ���߽��ϴ�." << endl;
			loop = 0;
			return;
		}
		set_OR_Perceptron();			//��������� �ݺ�
	}
}

void Perceptron::set_XOR_Perceptron() {	//XOR-gate �н�
	int incorrect = 0;
	cout << "XOR gate" << endl;
	cout << fixed;
	cout.precision(2);
	for (int i = 0; i < pow_two; i++) {
		int totalbits = 0;				//x�� xor���� �ϱ����Ͽ�, x�� 1�� ������ �����ϴ� ����
		unsigned int bits = i;
		for (int j = 0; j < num; j++) {	//totalbits ���
			totalbits += bits & 1;
			bits >>= 1;
		}
		if (totalbits % 2 == 0) {		//x�� 1�� ������ ¦���� ���, output 0
			if (net[i] <= threshold)
				cout << "net" << i << " <= threshold, output 0 ...... ��ġ" << endl;
			else {
				cout << "net" << i << " > threshold, output 1 .... ����ġ" << endl;
				Reset_weight(i, 0, 1);
				incorrect++;
			}
		}
		else {							//x�� 1�� ������ Ȧ���� ���, output 1
			if (net[i] <= threshold) {
				cout << "net" << i << " <= threshold, output 0 .... ����ġ" << endl;
				Reset_weight(i, 1, 0);
				incorrect++;
			}
			else
				cout << "net" << i << " > threshold, output 1 ...... ��ġ" << endl;
		}
	}
	if (incorrect == 0) {
		cout << "��� ������� XOR gate�� ��ġ�մϴ�." << endl;
		cout << "--------------------------------------------------------------------" << endl;
		cout << "--------------------------------------------------------------------" << endl;
		return;
	}
	else {
		cout << "output�� " << incorrect << "ȸ Ʋ�Ƚ��ϴ�. �ݺ� �н��Ͽ� �����մϴ�." << endl;
		cout << "--------------------------------------------------------------------" << endl;
		loop++;
		if (loop == 100) {
			cout << "100���� ���࿡�� �н��� �Ϸ����� ���߽��ϴ�." << endl;
			loop = 0;
			return;
		}
		set_XOR_Perceptron();		//��������� �ݺ�
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