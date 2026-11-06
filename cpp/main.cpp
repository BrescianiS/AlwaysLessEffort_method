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

#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>


namespace
{
    // function to handle the number formatting for output files' name
    std::string numberForFileName(float value)
    {
        std::ostringstream out;
        out.imbue(std::locale::classic());   // always '.'
        out << value;
        return out.str();
    }
}


int main()
{
    // computation parameters
    const float nDays = 12.7f;
    const float nPhases = 267.5f;
    const float desiredRate = 45.0f;
    // that means 45% more of work done on first day compared to the last day
    // that will be transformed inside AlwaysLessEffort to the rate of 1.45
    
    //output text language
    // it can accept only "en" or "it"
    const std::string language = "en";

    // Compute all algorithm variables
    // (optional: the tolerance and convergence values can be customized, e.g.
    //    AleTolerances tolerances;  
    //    tolerances.maxErrIter = 3;
    //    const AlwaysLessEffort ale { nDays, nPhases, desiredRate, tolerances }; )
    const AlwaysLessEffort ale { nDays, nPhases, desiredRate };

    // Obtain output variables: ale.d; ale.pd; ale.pdap;...

    // Error situation (invalid or particular input values)
    if (ale.status == AlwaysLessEffort::Status::Error) {
        std::cerr << " Please change the input values of the algorithm!\n";
        return 1;
    }

    // In case of standard non error situation, the output file text can be created
    const TextCreator outText { ale, language };

    const std::string fileName = "SCHEDULE_i" + numberForFileName(desiredRate) + "_p" + numberForFileName(nPhases) + "_d" + numberForFileName(nDays) + "_" + language + ".txt";

    std::ofstream outputFile { fileName };
    if (!outputFile.is_open()) {
        std::cerr << "Error in opening file : please check permissions.\n";
        return 1;
    }
    outputFile << outText.TextToWriteInFile;
    // close explicitly to be sure that the whole text has been written
    outputFile.close();
    if (outputFile.fail()) {
        std::cerr << "Error in writing file : please check free space and permissions.\n";
        return 1;
    }

    return 0;
}