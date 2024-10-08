#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;

string convertTimeToString(time_t timeInSeconds) {
	tm timeInfo; // Создаем локальную переменную структуры tm
	localtime_s(&timeInfo, &timeInSeconds); // Безопасная версия функции localtime

	char buffer[25]; // Буфер для хранения форматированной строки
	strftime(buffer, sizeof(buffer), "%Y::%m::%d %H::%M::%S", &timeInfo); // Форматирование времени
	return buffer; // Возвращаем строку с отформатированным временем
}


long long int Define_Longer_Time(map<string, string> options_map) {

	long long int to_time;

	if (options_map["--to="] != "") {
		to_time = stoi(options_map["--to="]);
	}
	else if(options_map["-e"] != ""){
		to_time = 4147483647;
	}
	else {
		to_time = 4147483647;
	}

	return to_time;
}


long long int Determine_Shorter_Time(map<string, string> options_map) {

	long long int from_time;

	if (options_map["--from="] != "") {
		from_time = stoi(options_map["--from="]);
	}
	else if (options_map["-f"] != ""){
		from_time = 0;
	}
	else {
		from_time = 0;
	}

	return from_time;

}


time_t convertToUnixTimestamp(const std::string& date_str) {

	map<string, int> month_map = {
	{"Jan", 0}, {"Feb", 1}, {"Mar", 2}, {"Apr", 3},
	{"May", 4}, {"Jun", 5}, {"Jul", 6}, {"Aug", 7},
	{"Sep", 8}, {"Oct", 9}, {"Nov", 10}, {"Dec", 11}
	};

	int day, year, hour, minute, second, timezone_offset;
	string month_str;

	// Извлечение дня
	size_t pos = 0;
	size_t slash_pos = date_str.find('/');
	day = std::stoi(date_str.substr(pos, slash_pos - pos));

	// Извлечение месяца
	pos = slash_pos + 1;
	size_t next_slash_pos = date_str.find('/', pos);
	month_str = date_str.substr(pos, next_slash_pos - pos);

	// Извлечение года
	pos = next_slash_pos + 1;
	size_t colon_pos = date_str.find(':', pos);
	year = std::stoi(date_str.substr(pos, colon_pos - pos));

	// Извлечение часов, минут и секунд
	hour = stoi(date_str.substr(colon_pos + 1, 2));
	minute = stoi(date_str.substr(colon_pos + 4, 2));
	second = stoi(date_str.substr(colon_pos + 7, 2));

	// Извлечение смещения временной зоны
	pos = date_str.find(' ', colon_pos) + 1;
	timezone_offset = std::stoi(date_str.substr(pos, 5));



	

	// Преобразуем название месяца в числовое значение
	int month = month_map[month_str];

	// Создаем и заполняем структуру tm
	tm time_info = {};
	time_info.tm_mday = day;          // День месяца
	time_info.tm_mon = month;         // Месяц (0-11)
	time_info.tm_year = year - 1900;  // Год с 1900
	time_info.tm_hour = hour;         // Часы
	time_info.tm_min = minute;        // Минуты
	time_info.tm_sec = second;        // Секунды

	time_t timestamp = mktime(&time_info);
	// После получения timestamp
	
	int tz_hours = timezone_offset / 100;    // Извлекаем часы из смещения
	int tz_minutes = timezone_offset % 100;  // Извлекаем минуты из смещения
	int total_offset_seconds = tz_hours * 3600 + tz_minutes * 60; // Общий сдвиг в секундах

	// Корректируем временную метку на смещение временной зоны
	timestamp -= total_offset_seconds;

	return timestamp;
}

string func_path(map<string, string> options_map) {

	string path;

	if (options_map["-o"] != "") {
		path = options_map["-o"];
	}
	else if (options_map["--output="] != "") {
		path = options_map["--output="];
	}
	else {
		path = "";
	}

	return path;
}

int window(map<string, string>options_map) {

	long long int t;

	if (options_map["-w"] != "0") {
		t = stoi(options_map["-w"]);
	}
	else if (options_map["--window="] != "0") {
		t = stoi(options_map["--window="]);
	}
	else { t = 0; }

	return t;
}

int stat(map<string, string> options_map) {

	int n;

	if (options_map["--stats="] != "") {

		n = stoi(options_map["--stats="]);
	}
	else if (options_map["-s"] != "") {

		n = stoi(options_map["-s"]);
	}
	else {
		n = 10;
	}
	return n;
}

int main(int argc, char* argv[])
{
	string arg;
	map<string, string> options_map = {
	{"-o", ""}, {"--output=", ""}, {"-p", ""}, {"--print", ""},
	{"-s", ""}, {"--stats=", ""}, {"-w", "0"}, {"--window=", "0"},
	{"-f", ""}, {"--from=", ""}, {"-e", ""}, {"--to=", ""}, {"file_name", ""}
	};

	for (int i = 1; i < argc; i++) {

		string tmp = argv[i];
		string arg;
		string value_arg;

		if (tmp.rfind("--", 0) == 0) {  

			if (tmp.find('=') != string::npos) {
				size_t pivot = tmp.find('=');                     
				arg = tmp.substr(0, pivot + 1);                    
				value_arg = tmp.substr(pivot + 1);                 
				options_map[arg] = value_arg;

				cout << arg << " = " << options_map[arg] << endl;

			}
			else {
				arg = tmp;                                         
				options_map[arg] = arg;                            
				cout << arg << " = " << options_map[arg] << endl;
			}

		}
		else if (tmp.find('-') == 0) {  

			arg = tmp;                                            

			
			if (i + 1 < argc - 1 && argv[i + 1][0] != '-') {
				value_arg = argv[i + 1];
				options_map[arg] = value_arg;
				i++;  
			}
			else {
				options_map[arg] = arg;  
			}

			cout << arg << " = " << options_map[arg] << endl;

		}
		else {
			
			if (i == argc - 1) {
				
				options_map["file_name"] = tmp;
				cout << "file_name = " << options_map["file_name"] << endl;
			}
			else {
				
				cout << "Error: Non-option argument '" << tmp << "' encountered before the end." << endl;
				return 1;
			}
		}
	}

	if (options_map["file_name"] == "") {

		cout << "Error, the access.log file is not specified" << endl;
		return 0;

	}

	ifstream file_log;
	file_log.open(options_map["file_name"]);

	if (!file_log.is_open()) {

		cout << "Error, the file did not open" << endl;
		return 0;

	}

	
	string str;
	string remote_addr;
	string local_time;
	string request;
	string status;
	string bytes_send;

	string path = func_path(options_map);
	string print;
	long long int from_time = Determine_Shorter_Time(options_map);
	long long int to_time   = Define_Longer_Time(options_map);
	long long int window_time = 0;
	long long int t = window(options_map);
	int n = stat(options_map);
	
	int cnt = 0;
	map<string, int> statistic;
	int currentTime;
	ofstream outFile;

	vector<long long int> local_times;
	size_t window_start_index = 0;
	int maxRequests = 0;
	long long int maxWindowStart = 0; 

	if (path != "") {
		outFile.open(path);
	}

	while (getline(file_log, str)) {

		if (str.find(' ') != 0) {

			remote_addr = str.substr(0, str.find(' '));
			
		}
		else { continue; }
		
		if (str.find('[') != string::npos) {

			local_time = str.substr(str.find('[') + 1, str.find(']') - str.find('[') - 1);
			
		}
		else { continue; }

		if (str.find('"') != string::npos) {

			request = str.substr(str.find('"') + 1, str.rfind('"') - str.find('"') - 1);
			
		}
		else { continue; }

		if (str.find("\" ") != string::npos) {

			status = str.substr(str.rfind("\" ") + 2, str.rfind(' ') - str.rfind("\" ") - 2);
			
		}
		else { continue;  }

		if (str.rfind(' ') != string::npos) {

			bytes_send = str.substr(str.rfind(' ') + 1);
		}
		else { continue; }
		


		long long int local_time_unix = convertToUnixTimestamp(local_time);

		if ((local_time_unix >= from_time) and (local_time_unix <= to_time)) {

			// Операции для window
			if (t != 0) {
				/// Добавляем текущую временную метку в вектор
				local_times.push_back(local_time_unix);
				

				// Удаляем из окна временные метки, которые выходят за пределы текущего окна
				while (window_start_index < local_times.size() && local_time_unix - local_times[window_start_index] >= t) {
					window_start_index++;
				}

				// Текущий размер окна
				int currentWindowSize = local_times.size() - window_start_index;

				// Если размер текущего окна больше, обновляем максимальные значения
				if (currentWindowSize > maxRequests) {
					maxRequests = currentWindowSize;
					maxWindowStart = local_times[window_start_index];
				}
			}

			if (path != "") { // Записываем если указан error файл 

				int status_copy = stoi(status);

				if (status_copy >= 500 and status_copy < 600) {
					cout << str << endl;
					outFile << str << endl;

					if (n != 0) {
						statistic[request]++;
					}
				}

			}
		}
	}

	cout << "\n";

	file_log.close();

	if (path != "") { outFile.close(); }

	if (t != 0 and maxRequests > 0) {
		string formattedTimeStart = convertTimeToString(maxWindowStart);
		string formattedTimeEnd = convertTimeToString(maxWindowStart + t);

		cout << "Max Requests time: [" << formattedTimeEnd << "] - " << "[" << formattedTimeEnd << "] " << "Counter: " << maxRequests << "\n" << endl;
	}

	// Сортируем вектор по убыванию частоты
	if (n != 0 and (options_map["-o"] != "" or options_map["--output="] != "")) {

		// Переносим данные из map в вектор для сортировки
		vector<pair<string, int>> errorFrequency(statistic.begin(), statistic.end());
		sort(errorFrequency.begin(), errorFrequency.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
			return a.second > b.second; // Сортируем по убыванию количества встреч
			});

		// Выводим n самых частых запросов с ошибками 5XX
		cout << "TOP: " << n << " the most frequent requests with errors are 5XX:" << endl;
		for (int i = 0; i < n && i < errorFrequency.size(); ++i) {
			cout << "Request: " << errorFrequency[i].first << ", met: " << errorFrequency[i].second << " times" << endl;
		}
		cout << "\n";
	}
	if (options_map["-p"] != "" or options_map["--print"] != "") {

		file_log.open(path);

		if (file_log.is_open()) {
			while (getline(file_log, str)) {

				cout << str << endl;

			}
		}
		else {
			cout << "Error The file could not be opened" << endl;
		}
	}
}
