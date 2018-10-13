#include <iostream>
#include <thread>
#include <algorithm>  
#include <vector>
#include <mutex>

using namespace std;

const int numRadar = 4;								  // 部署4个雷达

enum SimOrder {START, END, PROCESS };                 // 仿真控制台SimCtrl的指令，这里用来控制Radar线程主程序的结束


struct Radar_Data{									  // Radar线程的计算结果数据，该数据将返回给指控程序
	int RadarID;
	double data1;
	double data2;
	SimOrder order;
};

class Radar {												// Radar类	
private:
	Radar_Data radar_data;									// Radar返回给指控的数据
public:
	Radar() { }
	~Radar() { }
	void Radar_main(Radar_Data &data, mutex &mtx) {									// Radar仿真线程的主程序，主程序里还应包括DDS部分的程序，这里省略，需补充
		while (true) {																// Radar仿真线程主程序是一个无限空循环，直到接收到仿真结束指令
			if (data.order == END) break;
			if (data.order == START) {
				lock_guard<mutex> guard(mtx);										// 临时对象guard自动加锁与销毁
				data.order = PROCESS;												// 实际程序不需要这段程序，这里仅作测试用
				data.data1 += data.RadarID;											// 实际程序不需要这段程序，这里仅作测试用
				
				
				this->radar_data.RadarID = data.RadarID;
				this->radar_data.order = data.order;
				this->radar_data.data1 = data.data1;								// 实际程序不需要这段程序，这里仅作测试用
				this->radar_data.data2 = data.data2;
				cout << "RadarID = " << this->radar_data.RadarID << " order = " << this->radar_data.order << " data1 = " << this->radar_data.data1 << " data2 = " << this->radar_data.data2 << endl;
//				this_thread::sleep_for(chrono::milliseconds(500));

			}
		}
	}									 
};

int main() {												// 指控主程序
	Radar_Data Radar_Data[numRadar];						// 指控程序中用来保存Radar线程返回的数据
	Radar RadarObject[numRadar];							// Radar对象
	vector<thread> threads;									// Radar线程vector	
	mutex mtx;

	for (int i = 0; i < numRadar; i++) {					// Radar数据初始化，这里仅作测试用
		Radar_Data[i].RadarID = i;
		Radar_Data[i].data1 = 10 * i;
		Radar_Data[i].data2 = 20 * i;
		Radar_Data[i].order = START;
	}

	for (int i = 0; i < numRadar; i++) {					// 创建numRadar个Radar线程,并且将Radar数据初始值分别传递给相应的Radar线程
		threads.push_back(thread(&Radar::Radar_main, &RadarObject[i], ref(Radar_Data[i]), ref(mtx)));		// Radar_Data[i]采用引用方式向线程传递初值，并获得线程的计算结果，也可以采用指针方式传递
	}

	system("pause");
	
	for (int i = 0; i < numRadar; i++) {
		Radar_Data[i].order = END;							// 发出仿真结束指令
	}

	for (int i = 0; i < numRadar; i++) {
		if(threads.at(i).joinable()) threads.at(i).join();								// 结束numRadar个Radar线程
	}

//	system("pause");
 	for (int k = 0; k < numRadar; k++) {					// 仅作测试用
		cout << "Radar ID =  " << Radar_Data[k].RadarID  << " data1 = " << Radar_Data[k].data1 << endl;           
	}
	system("pause");

	return 0;
}
