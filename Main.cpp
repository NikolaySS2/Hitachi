#include <iostream>
#include "Hitachi.h"

int main() {
    customParameters();
    cout << "This is what WeatherReport.csv contains" << endl << endl;
    printFileContents();

    // string apiEndpoint = "https://www.abv.bg-api-endpoint.com/send";
    //string senderEmail = "sender@example.com";
    //string senderPassword = "password";
    //string receiverEmail = "receiver@example.com";
    //string subject = "Weather Report";
    //string message = "Please find the attached Weather Report.";

    //// Send the email with the WeatherReport.csv file as an attachment
    //sendEmailWithAttachment(apiEndpoint, senderEmail, senderPassword, receiverEmail, subject, message, "WeatherReport.csv");
    return 0;
}