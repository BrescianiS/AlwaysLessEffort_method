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



/*******************************************************
 *
 * INPUT NECESSARY VARIABLE: ALE class object
 * INPUT SECONDARY VARIABLE: output text language string (default "en" as English; other possible value "it" as Italian) and output approximations values outRate, outExact, outApprox
 *
 * OUTPUT VARIABLE: (related to class object itself) string of content of output text file
 *
 *******************************************************/



#ifndef TEXTCREATION_H
#define TEXTCREATION_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

#include "AlwaysLessEffort.h"

using namespace std;



class TextCreator
{

private:

    const float nd, np, rp;
    const vector<int> d;
    vector<float> pd, pdap;
    const float approxDays;
    const bool onlyIntegerDays;
    float rate_first_last, totApproxPhases, totExactPhases;
    const string ErrorOrWarning;
    const string lang;
    // Output value approximation for the rate, the exact number of phases and approximated number of phases
    const float outRate, outExact, outSingleApprox, outTotApprox;


    // function to set output values approximation
    void outputApproximation(){

        for (int i = 0; i < static_cast<int>(d.size()) ; i++) {
            this->pd.at(i) = roundf( this->pd.at(i) / this->outExact ) * this->outExact ;
            this->pdap.at(i) = roundf( this->pdap.at(i) / this->outSingleApprox ) * this->outSingleApprox ;
        }
        this->totExactPhases = roundf( this->totExactPhases / this->outExact ) * this->outExact ;
        // it handles also the particular error case with totApproxPhases about 0
        this->totApproxPhases = ( this->totApproxPhases < this->outTotApprox ) ? this->outTotApprox : roundf(this->totApproxPhases/ this->outTotApprox) * this->outTotApprox ;
        this->rate_first_last = roundf( this->rate_first_last / this->outRate ) * this->outRate;
    }


    // header

    void english_header(stringstream& file) {
        stringstream outD;
        if ( this->approxDays == this->nd ) {
            outD << this->nd;
        }else{
            outD << setprecision(3) << this->approxDays << " (" << setprecision(4) << this->nd << ")";
        }
        file << "Entered values: \n  "
                "initial desired Increment: +"<< ((this->rp -1.0f) *100.0f) << "%;  "<<
                "number of Phases: " << this->np << ";  number of Days: " << outD.str() <<". \n\n\n\n";

        this->english_infofile(file);


        file << "\n\n SCHEDULE\n\n\n";

        // In case of integer nd
        if (this->onlyIntegerDays) {
            file << "Day\t Phases per day \t (approx)  \n\n";
        }
        // In case of non integer nd
        else {
            file << "Day\t\tPhases per day   (approx) \t Phases per half day   (approx) \n\n";
        }
    }

    void italian_header(stringstream& file) {
        stringstream outD;
        if ( this->approxDays == this->nd ) {
            outD << this->nd;
        }else{
            outD << setprecision(3) << this->approxDays << " (" << setprecision(4) << this->nd << ")";
        }
        file << "Valori inseriti: \n  "
                 "Incremento iniziale richiesto: +" << ((this->rp - 1.0f) * 100.0f) << "%;  "<<
                 "numero di Fasi: " << this->np << ";  numero di Giorni: " << outD.str() <<". \n\n\n\n";

        this->italian_infofile(file);

        file << "\n\n PIANIFICAZIONE\n\n\n";

        // In case of integer nd
        if (this->onlyIntegerDays) {
            file << "Giorno\t Fasi al giorno \t (appross)  \n\n";
        }
        // In case of non integer nd
        else {
            file << "Giorno\t\tFasi al giorno   (appross) \t Fasi ogni mezza giornata   (appross) \n\n";
        }
    }


    // results informations

    void english_infofile(stringstream& file) {
        if(this->ErrorOrWarning == "WARNING") {
            float tot_approx = accumulate(this->pdap.begin(), this->pdap.end(), 0.0f);
            file<<"Approximated values of phases per day were computed but be careful to consider: \n"<<
                " (sum of approximated values of phases = "<< this->totApproxPhases <<") ; \n"<<
                "it would be better to consider the phases in precise form: \n"<<
                " (sum of precise values of phases = "<< this->totExactPhases <<") .\n";
        }else{
            file<<"You can consider for convenience the values of phases per day either in approximated form or in precise form. \n"
                  "For both of them the total resulted coherent with the requested number of phases ("<<this->np<<") \n"
                  "Once chosen, it is suggested to always follow the values of the same column.\n";
        }

        file << "\n\nConfirmation of increment of first day compared to the last day: \n"
             << this->pd.at(0) << "/" << this->pd.at(this->d.size() - 1) <<
             " = " << this->rate_first_last << " --> +"
             << ((this->rate_first_last) - 1.0f) * 100.0f << "% . \n\n\n";
    }

    void italian_infofile(stringstream& file) {
        if(this->ErrorOrWarning == "WARNING") {
            file<<"Sono stati calcolati anche i valori approssimati delle fasi al giorno però è meglio fare attenzione nel considerarli: \n"
                  " (somma dei valori approssimati delle fasi = "<< this->totApproxPhases <<") ; \n"<<
                "sarebbe meglio considerare quelle in forma precisa \n"
                " (somma dei valori precisi delle fasi = "<< this->totExactPhases <<") .\n";
        }else{
            file<<"Si può considerare sia i valori approssimati sia i valori precisi delle fasi per ogni giornata. \n"
                  "Per entrambi il totale è risultato coerente con il numero di fasi richieste ("<<this->np<<").\n"
                  "Una volta scelto si consiglia di seguire sempre i valori della stessa colonna.\n";
        }

        file << "\n\nConferma dell'incremento del primo giorno rispetto all'ultimo: \n"
             << this->pd.at(0) << "/" << this->pd.at(this->d.size() - 1) <<
             " = " << this->rate_first_last << " --> +"
             << ((this->rate_first_last) - 1.0f) * 100.0f << "% . \n\n\n";
    }


    // output file content definition function

    void ContentWriter() {

        stringstream file;

        if (this->lang == "it") {
            this->italian_header(file);
        } else {
            this->english_header(file);
        }

        // In case of integer nd
        if (this->onlyIntegerDays) {
            for (int i = 0; i < d.size(); i++) {
                file << this->d.at(i) << "\t" << this->pd.at(i) << " \t\t ("
                     << this->pdap.at(i) << ") \n";
            }
        }
        // In case of non integer nd
        else {
            float phs_d, phs_d_ap;
            for (int i = 0; i < static_cast<int>(this->d.size()); i++) {
                if (i == (static_cast<int>(this->d.size()) - 1)) {
                    file << this->approxDays << "\t \t"
                         << this->pd.at(i) << "\t\t (" << this->pdap.at(i) << ")\t\t\t "
                         << this->pd.at(i) << "\t\t\t (" << this->pdap.at(i) << ")\n\n";
                } else {
                    if ((i % 2) == 0) {
                        phs_d = this->pd.at(i) + this->pd.at(i + 1);
                        phs_d_ap = this->pdap.at(i) + this->pdap.at(i + 1);
                        file << (this->d.at(i) / 2) + 1 << " \t\t" << phs_d << "\t\t ("
                             << phs_d_ap << ")\t\t\t " << this->pd.at(i) << "\t\t\t ("
                             << this->pdap.at(i) << ")\n";
                    } else {
                        file << (this->d.at(i) / 2)  <<"\t\t\t\t\t\t\t\t\t" << this->pd.at(i) << "\t\t\t ("
                             << this->pdap.at(i) << ")\n\n\n";
                    }
                }
            }
        }

        if (this->lang == "it") {
            file << "\n\n\nBuono lavoro! \n";
        } else {
            file << "\n\n\nHave a good work! \n";
        }

        file<<"\n\n\n\n\n\nPowered by Always-Less-Effort method \n(https://github.com/BrescianiS/AlwaysLessEffort_method)";

        this->TextToWriteInFile = file.str();
    }


public:

    string TextToWriteInFile;

    TextCreator( AlwaysLessEffort ALE, string language = "en", float outRate_in = 0.001f, float outExact_in = 0.01f, float outSingleApprox_in = 0.01f, float outTotApprox_in = 0.1f )
        //be careful of declaration variables order
        : nd { ALE.nd }
        , np { ALE.np }
        , rp { ALE.rp }
        , d { ALE.d }
        , pd { ALE.pd }
        , pdap { ALE.pdap }
        , approxDays { ALE.approxDays }
        , onlyIntegerDays { ALE.onlyIntegerDays }
        , rate_first_last { ALE.rate_first_last }
        , totApproxPhases { ALE.totApproxPhases }
        , totExactPhases { ALE.totExactPhases }        
        , ErrorOrWarning { ALE.ErrorOrWarning }
        , lang { std::move(language) }
        , outRate { outRate_in }
        , outExact { outExact_in }
        , outSingleApprox { outSingleApprox_in }
        , outTotApprox { outTotApprox_in }
    {
        this->outputApproximation();
        this->ContentWriter();
    }


};
#endif