#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

int stringToInt (string time) //turn string version of time to an int for proper comparison
{
    int colonPosition = 0;
    for (int i = 0; i < time.length(); i++) //goes through all characters of the string to find the colon
    {
        if (time[i]==':') colonPosition=i;
    }
    time = time.substr(0, colonPosition) + time.substr(colonPosition+1, time.length() - colonPosition - 1); //recreates without colon
    int output = stoi(time);
    return output;
}


int main()
{
    ifstream r;
    ifstream rTheft;
    string fileContent, fileContentThefts;
    string temp;

    string *date; //these hold parking meter data
    string *timeIn;
    string *timeOut;
    string *make;
    string *model;
    string *plate;

    string *dateTheft; //these hold theft data
    string *timeTheft;

    vector<string> makeTheft; //these will hold cars associated with thefts
    vector<string> modelTheft;
    vector<string> plateTheft;

    string userInput;
    cout << "Please enter name of file containing METER data: ";
    getline(cin,userInput);
    r.open(userInput);
    if(!r.is_open()) //this checks if the meter file opens
    {
        cout << "Cannot locate the file csv" << endl;
        return -1;
    }

    cout << "Please enter name of file containing THEFT data: ";
    getline(cin,userInput);
    rTheft.open(userInput);
    if(!rTheft.is_open()) //this checks if the theft file opens
    {
        cout << "Cannot locate the file thefts" << endl;
        return -1;
    }

    while(r.peek()!= EOF) //enter the content of the meter file into fileContent
    {
        r >> temp;
        fileContent += temp + "\n";
    }

    while(rTheft.peek()!= EOF) //enter the content of the theft file into fielContentThefts
    {
        rTheft >> temp;
        fileContentThefts += temp + "\n";
    }

    fileContent = fileContent.substr(54,fileContent.length() - 100) + '\n';
    int meterCount=0;
    for (int i=0; i < fileContent.length(); i++){ //counts number of entries in meter File
        if (fileContent[i]=='\n')
            meterCount++;
    }
    date = new string[meterCount]; //these can now be initalized since size is obtained
    timeIn = new string[meterCount];
    timeOut = new string[meterCount];
    make = new string[meterCount];
    model = new string[meterCount];
    plate = new string[meterCount];

    int theftCount=0;
    for (int i=0; i < fileContentThefts.length(); i++){ //counts number of entries in theft File
        if (fileContentThefts[i]=='\n')
            theftCount++;
    }
    dateTheft = new string[theftCount]; //these can now be initalized since size is obtained
    timeTheft = new string[theftCount];

    temp = "";
    int i=0, sepCount = 0; //sepCount tells us how many commas or next line characters we have come accross

    while (i<fileContent.length()) //this parses the meter file to enter data into the arrays
    {
        if (fileContent[i] == ',' || fileContent[i] == '\n') {
            if (sepCount % 6 == 0) //for example, if there have been six seperations then 6 % 6 = 0, so we enter data into date
                date[sepCount / 6] = temp;
            else if (sepCount % 6 == 1)
                timeIn[sepCount / 6] = temp;
            else if (sepCount % 6 == 2)
                timeOut[sepCount / 6] = temp;
            else if (sepCount % 6 == 3)
                make[sepCount / 6] = temp;
            else if (sepCount % 6 == 4)
                model[sepCount / 6] = temp;
            else
                plate[sepCount / 6] = temp;
            sepCount++;
            temp = "";
        }
        else { //if not a separation, continue adding onto temp
            temp += fileContent[i];
        }
        i++;
    }
    sepCount = 0;
    temp = ""; i =0;
    while (i<fileContentThefts.length()) //does the same thing as the last while loop but for theft
    {
        if (fileContentThefts[i] == ',' || fileContentThefts[i] == '\n') {
            if (sepCount % 2 == 0)
                dateTheft[sepCount / 2] = temp;
            else
                timeTheft[sepCount / 2] = temp;
            sepCount++;
            temp = "";
        }
        else {
            temp += fileContentThefts[i];
        }
        i++;
    }


    for (int i = 0; i < meterCount; i++) //goes through all meter data
    {
        for (int j = 0; j < theftCount; j++) //goes through all theft data
        {

            if (stringToInt(timeTheft[j]) > stringToInt(timeIn[i]) && stringToInt(timeTheft[j]) < stringToInt(timeOut[i]) && dateTheft[j][2] == date[i][2] && dateTheft[j][3] == date[i][3]){
               // if time theft is in between time in and time out, and the dates match...
                makeTheft.push_back(make[i]);
                modelTheft.push_back(model[i]);
                plateTheft.push_back(plate[i]);
            //record car information in the vectors
            }
        }
    }
    for (int i = 0; i < makeTheft.size(); i++) { //prints all information in the vectors
        cout << makeTheft[i] << " " << modelTheft[i] << " " << plateTheft[i] << endl;
    }

    r.clear();
    r.close();
    return 0;
}
