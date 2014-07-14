/*

Slimmed down, fast version of the boxplot tool

the original (slow) version using regex and a map<string,vector<double>>
data structure was added in commit 4f36f44 and might still be useful later.

*/

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <ctime>

using namespace std;

double mean(vector<double> samples)
{
    double sum = accumulate(samples.begin(), samples.end(), 0.0);
    return sum / samples.size();
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
    // Second Quartile = Median
    if (size % 2 == 0)
        answer.push_back((samples[size / 2 - 1] + samples[size / 2]) / 2);
    else
        answer.push_back(samples[size / 2]);
    // Third Quartile
    answer.push_back(samples[size*3/4]);
    return answer;
}

void print_stats(string dateString, vector<double> data)
{
    sort(data.begin(), data.end());
    vector<double> q = quartiles(data);
    cout << dateString << " ";
    cout << data.front() << " ";
    cout << q[0] << " ";
    cout << q[1] << " ";
    cout << q[2] << " ";
    cout << data.back() << " ";
    cout << mean(data) << " ";
    cout << data.size() << endl;
}

int main( int argc, char* argv[] )
{
    ifstream input_file( argv[1] );

    if( ! input_file ) return 2;

    string line_of_file;

    char dateoutput[100];
    struct tm timeDate;
    string dateString = "";
    double currValue;

    vector<double> data;

    while( !input_file.eof() ) {
        getline( input_file, line_of_file );
        if( input_file ) {
            string search = " temperature: ";
            std::size_t found = line_of_file.find(search);
            if (found != string::npos) {
                currValue = stod(line_of_file.substr(found + search.size()));
                strptime(line_of_file.substr(0,19).c_str(),"%Y-%m-%d_%H:%M:%S", &timeDate);
                if (!strftime(dateoutput, sizeof(dateoutput), "%F", &timeDate))
                    continue;
                if (data.size() == 0)
                {
                    cout << "# date whisker_low box_low median box_high whisker_high mean nsamples" << endl;
                    dateString = string(dateoutput);
                }
                if (dateString == string(dateoutput))
                    data.push_back(currValue);
                else
                {
                    print_stats(dateString, data);
                    dateString = string(dateoutput);
                    data = vector<double>({currValue});
                }
            }
        }
        else break;
    }

  return 0;
}

