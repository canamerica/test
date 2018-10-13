#include <iostream>
#include <thread>
#include <algorithm>  
#include <vector>
#include <mutex>

using namespace std;

const int numRadar = 4;								  // ����4���״�

enum SimOrder {START, END, PROCESS };                 // �������̨SimCtrl��ָ�������������Radar�߳�������Ľ���


struct Radar_Data{									  // Radar�̵߳ļ��������ݣ������ݽ����ظ�ָ�س���
	int RadarID;
	double data1;
	double data2;
	SimOrder order;
};

class Radar {												// Radar��	
private:
	Radar_Data radar_data;									// Radar���ظ�ָ�ص�����
public:
	Radar() { }
	~Radar() { }
	void Radar_main(Radar_Data &data, mutex &mtx) {									// Radar�����̵߳��������������ﻹӦ����DDS���ֵĳ�������ʡ�ԣ��貹��
		while (true) {																// Radar�����߳���������һ�����޿�ѭ����ֱ�����յ��������ָ��
			if (data.order == END) break;
			if (data.order == START) {
				lock_guard<mutex> guard(mtx);										// ��ʱ����guard�Զ�����������
				data.order = PROCESS;												// ʵ�ʳ�����Ҫ��γ����������������
				data.data1 += data.RadarID;											// ʵ�ʳ�����Ҫ��γ����������������
				
				
				this->radar_data.RadarID = data.RadarID;
				this->radar_data.order = data.order;
				this->radar_data.data1 = data.data1;								// ʵ�ʳ�����Ҫ��γ����������������
				this->radar_data.data2 = data.data2;
				cout << "RadarID = " << this->radar_data.RadarID << " order = " << this->radar_data.order << " data1 = " << this->radar_data.data1 << " data2 = " << this->radar_data.data2 << endl;
//				this_thread::sleep_for(chrono::milliseconds(500));

			}
		}
	}									 
};

int main() {												// ָ��������
	Radar_Data Radar_Data[numRadar];						// ָ�س�������������Radar�̷߳��ص�����
	Radar RadarObject[numRadar];							// Radar����
	vector<thread> threads;									// Radar�߳�vector	
	mutex mtx;

	for (int i = 0; i < numRadar; i++) {					// Radar���ݳ�ʼ�����������������
		Radar_Data[i].RadarID = i;
		Radar_Data[i].data1 = 10 * i;
		Radar_Data[i].data2 = 20 * i;
		Radar_Data[i].order = START;
	}

	for (int i = 0; i < numRadar; i++) {					// ����numRadar��Radar�߳�,���ҽ�Radar���ݳ�ʼֵ�ֱ𴫵ݸ���Ӧ��Radar�߳�
		threads.push_back(thread(&Radar::Radar_main, &RadarObject[i], ref(Radar_Data[i]), ref(mtx)));		// Radar_Data[i]�������÷�ʽ���̴߳��ݳ�ֵ��������̵߳ļ�������Ҳ���Բ���ָ�뷽ʽ����
	}

	system("pause");
	
	for (int i = 0; i < numRadar; i++) {
		Radar_Data[i].order = END;							// �����������ָ��
	}

	for (int i = 0; i < numRadar; i++) {
		if(threads.at(i).joinable()) threads.at(i).join();								// ����numRadar��Radar�߳�
	}

//	system("pause");
 	for (int k = 0; k < numRadar; k++) {					// ����������
		cout << "Radar ID =  " << Radar_Data[k].RadarID  << " data1 = " << Radar_Data[k].data1 << endl;           
	}
	system("pause");

	return 0;
}
