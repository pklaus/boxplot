#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include <algorithm>
#include <vector>

#include <cstdio>
#include <cstdlib>

#include <ctime>

using namespace std;

double mean(vector<double> samples)
{
    double sum = accumulate(samples.begin(), samples.end(), 0.0);
    return sum / samples.size();
    //// standard deviation
    //double m =  sum / v.size();
    //double accum = 0.0;
    //for_each (samples.begin(), samples.end(), [&](const double d) {
    //    accum += (d - m) * (d - m);
    //});
    //stdev = sqrt(accum / (v.size()-1));
}

vector<double> quartiles(vector<double> samples)
{
    // return as vector containing {first quartile, median, third quartile}
    vector<double> answer;
    size_t size = samples.size();
    // faster with http://www.cplusplus.com/reference/algorithm/nth_element/ ?
    sort(samples.begin(), samples.end());
    // First Quartile
    answer.push_back(samples[size/4]);
    // Second Quartile
    if (size % 2 == 0)
        answer.push_back((samples[size / 2 - 1] + samples[size / 2]) / 2);
    else
        answer.push_back(samples[size / 2]);
    // Third Quartile
    answer.push_back(samples[size*3/4]);
    return answer;
}


int main( int argc, char* argv[] )
{
    ifstream input_file( argv[1] );

    if( ! input_file ) return 2;

    string line_of_file;
    smatch m;
    regex e ("temperature: ([+-]?[0-9]+\\.[0-9]*)");   // matches words beginning by "sub"

    char dateoutput[100];
    struct tm timeDate;
    string dateString;

    map<string,vector<double>> data;

    while( !input_file.eof() ) {
        getline( input_file, line_of_file );
        if( input_file ) {
            if (regex_search (line_of_file,m,e)) {
                strptime(line_of_file.substr(0,19).c_str(),"%Y-%m-%d_%H:%M:%S", &timeDate);
                if (!strftime(dateoutput, sizeof(dateoutput), "%F", &timeDate)) {
                    continue;
                }
                dateString = string(dateoutput);
                if (!data.count(dateString)>0)
                    data[dateString] = vector<double>({stod(m[1])});
                else
                    data[dateString].push_back(stod(m[1]));
            }
        }
        else break;
    }

    for(auto iter=data.begin(); iter!=data.end(); ++iter) {
        sort(iter->second.begin(), iter->second.end());
        vector<double> q = quartiles(iter->second);
        cout << iter->first << " ";
        cout << iter->second.front() << " ";
        cout << q[0] << " ";
        cout << q[1] << " ";
        //cout << median(iter->second) << " ";
        cout << q[2] << " ";
        cout << iter->second.back() << " ";
        cout << mean(iter->second) << " ";
        cout << iter->second.size() << endl;
    }

  return 0;
}

