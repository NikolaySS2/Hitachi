#include "Hitachi.h"
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
//#include <curl/curl.h>

using namespace std;

const string Filename = "C:\\Users\\nikis\\source\\repos\\Hitachi\\Hitachi.csv";
vector<vector<string>> lineofwords;
vector<double> weather;

// Read data from the file and store it in vectors
void putInVectors() {
    for (int i = 0; i <= 6; i++) {
        ifstream inputFile(Filename);
        string line;

        // Skip lines before the desired line
        for (int k = 0; k < i; k++) {
            inputFile.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // Read the desired line
        getline(inputFile, line);
        istringstream iss(line);

        vector<string> words; // Declare words vector for each line

        bool firstWordSkipped = false;
        string word;
        while (getline(iss, word, ',')) {
            if (!firstWordSkipped) {
                firstWordSkipped = true; // Set the flag to true after skipping the first word
                continue; // Skip the first word
            }
            words.push_back(word);
        }
        lineofwords.push_back(words);
        inputFile.close();
    }
}

// Check if the weather conditions are appropriate for each day
int* appropriateDays(int lowtemps, int hightemps, int windspeed, int humiditypercent,int numberofdays) {
    ifstream inputFile(Filename);
    int* array = new int[numberofdays] { 0 };
    string value;
    getline(inputFile, value);
    int number;

    for (int i = 0; i <= 6; i++) {
        getline(inputFile, value);
        istringstream iss(value);
        bool firstWordSkipped = false;
        int daycounter = 0;

        while (getline(iss, value, ',')) {
            if (!firstWordSkipped) {
                firstWordSkipped = true; // Set the flag to true after skipping the first word
                continue; // Skip the first word
            }
            else {
                switch (i) {
                case 0: {
                    number = stoi(value);
                    if (number < lowtemps || number > hightemps) {
                        array[daycounter] = 1;
                    }
                    break;
                }
                case 1: {
                    number = stoi(value);
                    if (number > windspeed) {
                        array[daycounter] = 1;
                    }
                    break;
                }
                case 2: {
                    number = stoi(value);
                    if (number > humiditypercent) {
                        array[daycounter] = 1;
                    }
                    break;
                }
                case 3: {
                    number = stoi(value);
                    if (number != 0) {
                        array[daycounter] = 1;
                    }
                    break;
                }
                case 4: {
                    if (value != "No") {
                        array[daycounter] = 1;
                    }
                    break;
                }
                case 5: {
                    if (value == "Cumulus" || value == "Nimbus") {
                        array[daycounter] = 1;
                    }
                    break;
                }
                }
            }
            daycounter++;
        }
    }

    return array;
}

// Create the output file
void createFile() {
    ofstream outputFile("WeatherReport.csv");
    if (!outputFile) {
        cout << "Error creating output file: WeatherReport.csv" << endl;
        return;
    }
    outputFile << "Category,Average value,Min value,Max value,Median value" << endl;
    outputFile.close();
}

// Calculate average, minimum, maximum, and median values for each category and write them to the output file
void averageValues(int templow, int temphigh, int windspeed, int humidity) {
    createFile(); // Create the output file

    double averageValue = 0;
    double min = 10000;
    double max = 0;
    double medianValue = 0;

    ofstream outputFile("WeatherReport.csv", ios::app);
    if (!outputFile) {
        cout << "Error opening output file: WeatherReport.csv" << endl;
        return;
    }
    int counter = 0;
    // Calculate average, minimum, maximum, and median values for each category
    if (lineofwords.size() >= 6) {
        for (int i = 1; i < 5; i++) {
            vector<string>& line = lineofwords[i];
            sort(line.begin(), line.end());
            min = 1000;
            max = 0;
            averageValue = 0;
            counter = 0;
            for (const string& word : line) {
                double num = stoi(word);
                counter++;
                if (min > num) {
                    min = num;
                }
                if (max < num) {
                    max = num;
                }
                if (counter == 8) {
                    medianValue = num;
                }
                averageValue += num;
            }

            averageValue /= counter;
            switch (i) {
            case 1:
                outputFile << "Temperature (C),";
                break;
            case 2:
                outputFile << "Wind (m/s),";
                break;
            case 3:
                outputFile << "Humidity (%),";
                break;
            case 4:
                outputFile << "Precipitation (%),";
                break;
            }
            outputFile << averageValue << ",";
            outputFile << min << ",";
            outputFile << max << ",";
            outputFile << medianValue << endl;
        }
    }
    outputFile << endl << "Most appropriate days";
    int* arrayofdays = appropriateDays(templow, temphigh, windspeed, humidity,counter);
    for (int i = 0; i < counter; i++) {
        if (arrayofdays[i] == 0) {
            outputFile << "," << i + 1;
        }
    }
    outputFile << endl;
}

// Get custom or default parameters from the user
void customParameters() {
    int templow = 2;
    int temphigh = 31;
    int windspeed = 10;
    int humidity = 60;
    cout << "Do you want to set custom criteria parameters?" << endl << "Enter 1 for Yes and 2 to use default parameters" << endl;
    int answer;
    cin >> answer;
    while (answer != 1 && answer != 2) {
        cout << "Invalid input. Please try again: ";
        cin >> answer;
    }
        switch (answer) {
        case 1: {
            cout << "Select temperature borders" << endl;
            cin >> templow >> temphigh;
            cout << "Select wind border speed under which it is acceptable" << endl;
            cin >> windspeed;
            cout << "Select humidity border speed under which it is acceptable" << endl;
            cin >> humidity;
            if (templow > temphigh) {
                swap(templow, temphigh);
            }
            putInVectors();
            averageValues(templow, temphigh, windspeed, humidity);
            break;
        }
        case 2: {
            putInVectors();
            averageValues(templow, temphigh, windspeed, humidity);
            break;
        }
        
    }
}

// Print the contents of the file
void printFileContents() {
    ifstream inputFile("WeatherReport.csv");
    if (!inputFile) {
        cout << "Error opening file: " << Filename << endl;
        return;
    }
    string line;
    while (getline(inputFile, line)) {
        cout << line << endl;
    }
    inputFile.close();
}

//// Send an email with the WeatherReport.csv file as an attachment
//void sendEmailWithAttachment(const string& apiEndpoint, const string& senderEmail, const string& senderPassword,
//    const string& receiverEmail, const string& subject, const string& message,
//    const string& attachmentPath)
//{
//    CURL* curl = curl_easy_init();
//    if (!curl)
//    {
//        cout << "Failed to initialize libcurl." << endl;
//        return;
//    }
//
//    // Create the form data for the request
//    struct curl_httppost* formPost = nullptr;
//    struct curl_httppost* lastPost = nullptr;
//
//    // Add sender email
//    curl_formadd(&formPost, &lastPost,
//        CURLFORM_COPYNAME, "sender",
//        CURLFORM_COPYCONTENTS, senderEmail.c_str(),
//        CURLFORM_END);
//
//    // Add sender password
//    curl_formadd(&formPost, &lastPost,
//        CURLFORM_COPYNAME, "password",
//        CURLFORM_COPYCONTENTS, senderPassword.c_str(),
//        CURLFORM_END);
//
//    // Add receiver email
//    curl_formadd(&formPost, &lastPost,
//        CURLFORM_COPYNAME, "recipient",
//        CURLFORM_COPYCONTENTS, receiverEmail.c_str(),
//        CURLFORM_END);
//
//    // Add subject
//    curl_formadd(&formPost, &lastPost,
//        CURLFORM_COPYNAME, "subject",
//        CURLFORM_COPYCONTENTS, subject.c_str(),
//        CURLFORM_END);
//
//    // Add message
//    curl_formadd(&formPost, &lastPost,
//        CURLFORM_COPYNAME, "message",
//        CURLFORM_COPYCONTENTS, message.c_str(),
//        CURLFORM_END);
//
//    // Add attachment
//    if (!attachmentPath.empty())
//    {
//        curl_formadd(&formPost, &lastPost,
//            CURLFORM_COPYNAME, "attachment",
//            CURLFORM_FILE, attachmentPath.c_str(),
//            CURLFORM_END);
//    }
//
//    // Set the URL for the POST request
//    curl_easy_setopt(curl, CURLOPT_URL, apiEndpoint.c_str());
//
//    // Set the form data for the POST request
//    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formPost);
//
//    // Perform the request and get the response
//    CURLcode res = curl_easy_perform(curl);
//    if (res != CURLE_OK)
//    {
//        cout << "Failed to send email. Error: " << curl_easy_strerror(res) << endl;
//    }

//}

