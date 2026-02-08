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



/***************************************************************
 *
 * INPUT NECESSARY VARIABLES: nd, np, rp
 * INPUT SECONDARY VARIABLES FOR ALGORITHM CUSTOMIZATION: sensRate, sensExactPhases, sensApproxPhases, maxErrIter
 *
 * OUTPUT VARIABLES: (related to class object itself) all computed values and eventual relative error or warning
 *
 **************************************************************/



#ifndef ALWAYSLESSEFFORT_H
#define ALWAYSLESSEFFORT_H

#include <vector>
#include <numeric>
#include <string>

using namespace std;


class AlwaysLessEffort
{

public:
    // input variables Days, Phases and Increase percentage/Rate
    const float nd, np, rp;

private:
    // Desired sensibility for the rate of work, exact number of phases and approximated number of phases
    const float sensRate, sensExactPhases, sensApproxPhases;
    // Maximum number of times that the correction iterations for the error happens
    const int maxErrIter;

public:
    float rate_first_last, totApproxPhases, totExactPhases;
    vector<int> d;
    vector<float> pd, pdap;
    float approxDays;
    // to distinguish the two cases of nd as integer or not
    float dec_part_last_day = 10;
    bool wrong_rate = false;
    string ErrorOrWarning = " ";


    AlwaysLessEffort(float nDays_in, float nPhases_in, float desiredRate_in, float sensRate_in = 0.0001f , float sensExactPhases_in = 0.01f, float sensApproxPhases_in = 0.15f , int maxErrIter_in = 2)
        //be careful of declaration variables order
        : nd { nDays_in }
        , np { nPhases_in }
        //here the percentage is converted in actual rate for the computation
        , rp { (desiredRate_in + 100.0f)/100.0f }
        , sensRate { sensRate_in }
        , sensExactPhases { sensExactPhases_in }
        , sensApproxPhases { sensApproxPhases_in }
        , maxErrIter { maxErrIter_in }
    {
        if( (nd > 1) || (rp > 1.0f)  ){
            double check_q =  2*np/nd;
            double check_m = 2*np/(nd * (nd-1));
            if( ( abs(check_q) < 1e20 ) && ( abs(check_m) < 1e20 )  ){
                this->AlgorithmElaboration();
            }
        }else{
            ErrorOrWarning = "ERROR";
        }
    }



private:

    float m, q;

    // eventual number of parts of integer to compute the approximated number of phases
    //int approxDiv = 2;


    void AlgorithmElaboration() {
        // Manage non integer number of days
        const int int_part_nd = static_cast< float >(static_cast< int >( this->nd ));
        // In case of nd as non-integer number
        if (this->nd > int_part_nd) {
            int N;
            this->dec_part_last_day = static_cast< float >(this->nd - int_part_nd);
            // If the rational part is less or equal than 0.5 it will be considered as half day
            if (this->dec_part_last_day <= 0.5) {
                N = 2 * static_cast< int >(int_part_nd) + 1;
                this->d.reserve(N);
                this->pd.reserve(N);
                this->pdap.reserve(N);
                this->approxDays = static_cast< float >(N) / 2.0f;
                this->ParametersComputation(N);
            }
            // If the rational part is greater or equal than 0.5 one day more will be considered hence it will be elaborated as an entire day
            else {
                N = 2 * int_part_nd + 2;
                this->d.reserve(N / 2);
                this->pd.reserve(N / 2);
                this->pdap.reserve(N / 2);
                this->approxDays = N / 2.0f;
                this->ParametersComputation(N / 2);
            }
        }
        // In case nd as an integer number
        else {
            this->approxDays = this->nd;
            this->d.reserve(static_cast< int >(int_part_nd));
            this->pd.reserve(static_cast< int >(int_part_nd));
            this->pdap.reserve(static_cast< int >(int_part_nd));
            this->ParametersComputation(static_cast< int >(int_part_nd));
        }
    }


    // FUNCTIONS TO COMPUTE THE ALGORITHM RESULTS

    // Computation m and q parameters function
    void ParametersComputation(int N) {

        // q and m parameters definition
        this->q = this->np * 2.0f * this->rp / ( static_cast<float>(N) *  (this->rp + 1.0f) );
        this->m = this->q * (this->rp - 1.0f) / (this->rp * static_cast<float>(N - 1) );
        // Algorithm results computation
        this->ResultsComputation(N);

        // self-correction phases in case of sensibility not satisfied
        int counter = 0;
        while( ( (ErrorOrWarning == "ERROR") || (this->wrong_rate ) ) && (counter < this->maxErrIter) ){
            // Rescale m to P and adjust q
            this->m = this->m * (this->np / this->totExactPhases);
            this->q = (this->np + this->m * (static_cast<float>(N * (N - 1)) / 2.0f)) / static_cast<float>(N);
            if (this->q != (this->m * static_cast<float>(N - 1))) {
                float R_check = this->q / (this->q - this->m * static_cast<float>(N - 1));
                // if the correction drifts the R value from desired one more than 2 * sensRate the correction is stopped
                if (((R_check <= (this->rp - 2 * this->sensRate)) ||
                     (R_check >= (this->rp + 2 * this->sensRate)) ||
                     (this->q < this->m * static_cast<float>(N - 1)))) {
                    counter = 10;
                } else {
                    this->d.clear();
                    this->pd.clear();
                    this->pdap.clear();
                    this->ErrorOrWarning = " ";
                    this->ResultsComputation(N);
                    counter++;
                }
            } else {
                counter = 10;
            }
        }

    }


    // Results computation function
    void ResultsComputation(int N) {
        // Only the first computation outside the for cycle, avoiding to check if i==0 every iteration
        float each_day_value = this->q;
        float c = static_cast<float>(static_cast< int >( each_day_value ));
        float r = each_day_value - 1.0f * c;
        r  = (r < 0.25) ? 0.0f : ((r >= 0.25) && (r < 0.75)) ? 0.5f : 1.0f ;
        this->d.push_back(1);
        this->pd.push_back( each_day_value );
        this->pdap.push_back( (c + r) );
        // other iterations
        for (int i = 1; i < N; i++) {
            each_day_value -= this->m;
            c = static_cast<float>(static_cast< int >( each_day_value ));
            r = each_day_value - 1.0f * c;
            r  = (r < 0.25) ? 0.0f : ((r >= 0.25) && (r < 0.75)) ? 0.5f : 1.0f ;
            this->d.push_back( i + 1 );
            this->pd.push_back( each_day_value );
            this->pdap.push_back( (c + r) );
        }

        // Final checks of computed values near enough to desired values
        // exact phases
        this->totExactPhases = accumulate(this->pd.begin(), this->pd.end(), 0.0f);
        if ((this->totExactPhases <= (this->np - this->sensExactPhases)) || (this->totExactPhases >= (this->np + this->sensExactPhases))) {
            this->ErrorOrWarning = "ERROR";
        } else {
            // approximated phases
            this->totApproxPhases = accumulate(this->pdap.begin(), this->pdap.end(), 0.0f);
            if ((this->totApproxPhases <= (this->np - this->sensApproxPhases)) || (this->totApproxPhases >= (this->np + this->sensApproxPhases))) {
                this->ErrorOrWarning = "WARNING";
            }
        }
        // rate between first and last day value
        this->rate_first_last = this->pd.at(0) /  this->pd.at(this->d.size() - 1);
        this->wrong_rate = ( (this->rate_first_last <= (this->rp - this->sensRate)) || (this->rate_first_last >= (this->rp + this->sensRate)) );
    }

    /*
    float approxPhasesComputation(float r){
        // generalization of approximation computation

        // a general solution to manage more cases, but computational heavier
        for( float j = 0.0f ; j <= 1.0f; j += (1.0f / this->approxDiv) ){
            if (j == 0.0f){
                if ( r < (j/2.0f) ){ r = 0.0f; }
            }
            else if (j == 1.0f){
                if ( r >= (1.0f - (j/2.0f)) ){ r = 1.0f; }
            }
            else{
                if ( (r >= (j/2.0f) ) && (r < (j + (j/2.0f)) )) { r = j; }
            }
        }

        return outR;
    }
    */

};

#endif