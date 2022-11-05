#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <random>
#include <iterator>
#include <experimental/algorithm>
#include "json.hpp"

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

// Ingest a data sample and store it into database.
//
// Sample input data:
// {"time_arrive":"19/07/2008 11:55:00","pass":-1,"data":"3030.93 2564 2187.7333 1411.1265 1.3602 100 97.6133 0.1242 1.5005 0.0162 -0.0034 0.9455 202.4396 0 7.9558 414.871 10.0433 0.968 192.3963 12.519 1.4026 -5419 2916.5 -4043.75 751 0.8955 1.773 3.049 64.2333 2.0222 0.1632 3.5191 83.3971 9.5126 50.617 64.2588 49.383 66.3141 86.9555 117.5132 61.29 4.515 70 352.7173 10.1841 130.3691 723.3092 1.3072 141.2282 1 624.3145 218.3174 0 4.592 4.841 2834 0.9317 0.9484 4.7057 -1.7264 350.9264 10.6231 108.6427 16.1445 21.7264 29.5367 693.7724 0.9226 148.6009 1 608.17 84.0793 NaN NaN 0 0.0126 -0.0206 0.0141 -0.0307 -0.0083 -0.0026 -0.0567 -0.0044 7.2163 0.132 NaN 2.3895 0.969 1747.6049 0.1841 8671.9301 -0.3274 -0.0055 -0.0001 0.0001 0.0003 -0.2786 0 0.3974 -0.0251 0.0002 0.0002 0.135 -0.0042 0.0003 0.0056 0 -0.2468 0.3196 NaN NaN NaN NaN 0.946 0 748.6115 0.9908 58.4306 0.6002 0.9804 6.3788 15.88 2.639 15.94 15.93 0.8656 3.353 0.4098 3.188 -0.0473 0.7243 0.996 2.2967 1000.7263 39.2373 123 111.3 75.2 46.2 350.671 0.3948 0 6.78 0.0034 0.0898 0.085 0.0358 0.0328 12.2566 0 4.271 10.284 0.4734 0.0167 11.8901 0.41 0.0506 NaN NaN 1017 967 1066 368 0.09 0.048 0.095 2 0.9 0.069 0.046 0.725 0.1139 0.3183 0.5888 0.3184 0.9499 0.3979 0.16 0 0 20.95 0.333 12.49 16.713 0.0803 5.72 0 11.19 65.363 0 0 0 0 0 0 0.292 5.38 20.1 0.296 10.62 10.3 5.38 4.04 16.23 0.2951 8.64 0 10.3 97.314 0 0.0772 0.0599 0.07 0.0547 0.0704 0.052 0.0301 0.1135 3.4789 0.001 NaN 0.0707 0.0211 175.2173 0.0315 1940.3994 0 0.0744 0.0546 0 0 0 0 0 0 0 0 0 0.0027 0.004 0 0 0 0 NaN NaN NaN NaN 0.0188 0 219.9453 0.0011 2.8374 0.0189 0.005 0.4269 0 0 0 0 0 0 0 0 0 0 0 0.0472 40.855 4.5152 30.9815 33.9606 22.9057 15.9525 110.2144 0.131 0 2.5883 0.001 0.0319 0.0197 0.012 0.0109 3.9321 0 1.5123 3.5811 0.1337 0.0055 3.8447 0.1077 0.0167 NaN NaN 418.1363 398.3185 496.1582 158.333 0.0373 0.0202 0.0462 0.6083 0.3032 0.02 0.0174 0.2827 0.0434 0.1342 0.2419 0.1343 0.367 0.1431 0.061 0 0 0 6.2698 0.1181 3.8208 5.3737 0.0254 1.6252 0 3.2461 18.0118 0 0 0 0 0 0 0.0752 1.5989 6.5893 0.0913 3.0911 8.4654 1.5989 1.2293 5.3406 0.0867 2.8551 0 2.9971 31.8843 NaN NaN 0 0.0215 0.0274 0.0315 0.0238 0.0206 0.0238 0.0144 0.0491 1.2708 0.0004 NaN 0.0229 0.0065 55.2039 0.0105 560.2658 0 0.017 0.0148 0.0124 0.0114 0 0 0 0 0 0 0 0.001 0.0013 0 0 0 0 NaN NaN NaN NaN 0.0055 0 61.5932 0.0003 0.9967 0.0082 0.0017 0.1437 0 0 0 0 0 0 0 0 0 0 0 0.0151 14.2396 1.4392 5.6188 3.6721 2.9329 2.1118 24.8504 29.0271 0 6.9458 2.738 5.9846 525.0965 0 3.4641 6.0544 0 53.684 2.4788 4.7141 1.7275 6.18 3.275 3.6084 18.7673 33.1562 26.3617 49.0013 10.0503 2.7073 3.1158 3.1136 44.5055 42.2737 1.3071 0.8693 1.1975 0.6288 0.9163 0.6448 1.4324 0.4576 0.1362 0 0 0 5.9396 3.2698 9.5805 2.3106 6.1463 4.0502 0 1.7924 29.9394 0 0 0 0 0 0 6.2052 311.6377 5.7277 2.7864 9.7752 63.7987 24.7625 13.6778 2.3394 31.9893 5.8142 0 1.6936 115.7408 0 613.3069 291.4842 494.6996 178.1759 843.1138 0 53.1098 0 48.2091 0.7578 NaN 2.957 2.1739 10.0261 17.1202 22.3756 0 0 0 0 0 0 0 0 0 0 0 0 64.6707 0 0 0 0 0 NaN NaN NaN NaN 1.9864 0 29.3804 0.1094 4.856 3.1406 0.5064 6.6926 0 0 0 0 0 0 0 0 0 0 0 2.057 4.0825 11.5074 0.1096 0.0078 0.0026 7.116 1.0616 395.57 75.752 0.4234 12.93 0.78 0.1827 5.7349 0.3363 39.8842 3.2687 1.0297 1.0344 0.4385 0.1039 42.3877 NaN NaN NaN NaN NaN NaN NaN NaN 533.85 2.1113 8.95 0.3157 3.0624 0.1026 1.6765 14.9509 NaN NaN NaN NaN 0.5005 0.0118 0.0035 2.363 NaN NaN NaN NaN"}

// split string by delimiter
size_t split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

    return strs.size();
}

string date_convert(string date_str){
    int year, month, day, hour, minute, second;
    char buf[21];
    sscanf(date_str.c_str(), "%02d/%02d/%d %02d:%02d:%02d", &day, &month, &year, &hour, &minute, &second);
    sprintf(buf, "%d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second);
    string format_date = buf;
    return format_date;
}

void simulate_compute(vector<float> data, float sum, int count){
    // Average, max, min
    float avg = sum / count, max_ele = *max_element(data.begin(), data.end()), min_ele = *min_element(data.begin(), data.end());
    // Normalized
    vector<float> normalized(count);
    for(int i = 0; i < count; i++){
        normalized[i] = data[i] / sum;
    }

    // scale random elements, scale
    int num = rand() % count;
    vector<float> sam;
    std::experimental::sample(data.begin(), data.end(), std::back_inserter(sam), num, std::mt19937{std::random_device{}()});
    for(int i = 0; i < num; i++){
        sam[i] *= 1.5;
    }

    // simulate conversion
    int choose = rand() % count;
    float t = ((data[choose] - 600) / 1000 + 23.5) * 1.1125;
    choose = rand() % count;
    t = (data[choose] / 5.535 + 12.696) * 3.333;
}

string gen_sql_str(int pass, string format_date, vector<string> col2add, vector<float> data){
    // Required std::stringstream object
    stringstream ss;

    // Convert all but the last element to avoid a trailing ","
    copy(col2add.begin(), col2add.end() - 1,ostream_iterator<std::string>(ss, ", "));
    // Now add the last element with no delimiter
    ss << col2add.back();

    string query = "INSERT INTO sensor_data(pass, arrived_at, " + ss.str() +  ") VALUES(" + to_string(pass) + ", \"" + format_date + "\", ";
    ss.clear();
    ss.str("");
    copy(data.begin(), data.end() - 1, ostream_iterator<float>(ss, ", "));
    // Now add the last element with no delimiter
    ss << data.back();
    query += ss.str() + ");";

    return query;
}

string handle(string body) {
    auto data_json = nlohmann::json::parse(body);

    // Format arrive time
    int pass = data_json["pass"].get<int>();
    string format_date = date_convert(data_json["time_arrive"].get<string>());

    float sum = 0; int count = 0;
    vector<float> data;
    vector<string> col2add;
    vector<string> v;
    string s = "s";
    string data_str = data_json["data"].get<string>();
    split(data_str, v, ' ');
    for(size_t i = 0; i < v.size(); ++i) {
        if(v[i] != "NaN"){
            col2add.push_back(s + to_string(i));
            float temp = stof(v[i]);
            data.push_back(temp);
            sum += temp;
            count += 1;
        }
    }

    // Calculation Simulation
    simulate_compute(data, sum, count);

    // Concat SQL string
    string query = gen_sql_str(pass, format_date, col2add, data);

    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;

        string addr (std::getenv("MYSQL_ADDRESS"));
        string username (std::getenv("MYSQL_USERNAME"));
        string password (std::getenv("MYSQL_PASSWORD"));

        /* Create a connection */
        driver = get_driver_instance();
        con = driver->connect(addr, username, password);
        /* Connect to the MySQL test database */
        con->setSchema("db");

        stmt = con->createStatement();
        stmt->execute(query);

        // delete res;
        delete stmt;
        delete con;
        return "ok";
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        return "error";
    }
}
