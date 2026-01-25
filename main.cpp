/*
*
*  Copyright 2026 -  Samuel Bresciani - bresciani.app@gmail.com
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
*/


#include "AlwaysLessEffort.h"
#include "TextCreator.h"

#include <iostream>
#include <fstream>

using namespace std;

int main(){
    // computation parameters
    float nPhases = 267.5;
    float nDays = 12.7;
    float desiredRate = 45;
    // that means 45% more of work done on first day compared to the last day
    // that will be transformed inside AlwaysLessEffort to the rate of 1.45

    //output text language
    // it can accept only "en" or "it"
    string language = "en";

    // Compute all algorithm variables
    AlwaysLessEffort ALE {nDays,nPhases, desiredRate};
    // Obtain output variables: ALE.d; ALE.pd; ALE.pdap;...

    // In case of standard non error situation, the output file text can be created
    if (ALE.ErrorOrWarning != "ERROR") {

        TextCreator outText {ALE, language};
        string outputStreamText = outText.TextToWriteInFile;

        ofstream outputFile;
        string textRp = to_string(desiredRate).substr(0, 4).erase(to_string(desiredRate).find('.'),1);
        // in this way the file name has also decimal digits of input days, even if the considered days are approximated
        string textPhases = to_string(nPhases).substr(0, 6).erase(to_string(nPhases).find('.'),1);
        string textDays = to_string(nDays).substr(0, 4).erase(to_string(nDays).find('.'),1);
        outputFile.open("SCHEDULE_i"+ textRp + "_p"+ textPhases + "_d"+ textDays+ "_"+ language +".txt", ios::out | ios::trunc);
        if (outputFile.is_open()){
            outputFile << outputStreamText;
            outputFile.close();
        }else{
            cout << "Error in opening file : please check permissions.";
        }

        // rare case of error situation on total phases
    }else{
        cout<<" Please change the input values of the algorithm!\"";
    }

    return 0;
}