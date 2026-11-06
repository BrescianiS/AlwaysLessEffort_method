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
 * INPUT SECONDARY VARIABLES FOR ALGORITHM CUSTOMIZATION: AleTolerances (sensRate, sensExactPhases, sensApproxSinglePhase, sensApproxTotPhases, maxErrIter)
 *
 * OUTPUT VARIABLES: (related to class object itself) all computed values and  eventual relative error or warning
 *
 **************************************************************/



#ifndef ALWAYSLESSEFFORT_H
#define ALWAYSLESSEFFORT_H

#include <cmath>
#include <cstddef>
#include <numeric>
#include <string_view>
#include <vector>


// Secondary parameters of the algorithm, with their default values.
// Defined at namespace scope (not nested) so it can be used as default argument
// of the AlwaysLessEffort constructor.
struct AleTolerances 
{
    // Desired sensibility for the rate of work
    float sensRate = 0.0001f;
    // Desired sensibility for the exact number of phases
    float sensExactPhases = 0.01f;
    // Rounding step of the approximated phases of a single day
    float sensApproxSinglePhase = 0.01f;
    // Desired sensibility for the approximated total number of phases
    float sensApproxTotPhases = 0.1f;
    // Maximum number of times that the correction iterations for the error happens
    int maxErrIter = 2;
};


class AlwaysLessEffort 
{

public:
    // input variables Days, Phases and Increase Percentage/Rate
    const float nd, np, rp;
    // generic ok-error-warning status     
    enum class Status { Ok, Warning, Error };
    Status status = Status::Ok;
    // to distinguish the two cases of nd as integer or not
    bool onlyIntegerDays = true;
    float approxDays = 0.0f;
    // other computation variables
    float rate_first_last = 0.0f;
    float totApproxPhases = 0.0f;
    float totExactPhases = 0.0f;
    std::vector<int> d;
    std::vector<float> pd, pdap;
    bool wrong_rate = false;


    AlwaysLessEffort(float nDays_in, float nPhases_in, float desiredRate_in, AleTolerances tolerances = {})
        // initialization order must follow the declaration order (nd, np, rp, tol)
        : nd { nDays_in }
        , np { nPhases_in }
        // here the percentage is converted in actual rate for the computation
        , rp { (desiredRate_in + 100.0f) / 100.0f }
        , tol { tolerances }
    {
        if( !( (nd >= 1.25f) && (nd <= maxDays) && (rp > 1.0f) && (rp < compLimit) && (np > 1.0f) && (np < compLimit) && (tol.sensApproxSinglePhase > 0.0f) ) ){
            status = Status::Error;
            return;
        }
        const double check_q = 2.0 * np / nd;
        const double check_m = 2.0 * np / (static_cast<double>(nd) * (static_cast<double>(nd) - 1.0));
        if ((check_q >= compLimit) || (check_m >= compLimit)) {
            status = Status::Error;
            return;
        }
        // all conditions of existance satisfied
        algorithmElaboration();
    }

    // Text version of the status: " " (ok), "WARNING" or "ERROR"
    [[nodiscard]] std::string_view statusText() const noexcept 
    {
        switch (status) {
            case Status::Warning: return "WARNING";
            case Status::Error:   return "ERROR";
            default:              return " ";
        }
    }


private:

    float m = 0.0f, q = 0.0f;
    // computational limit
    static constexpr float compLimit = 1e20f;
    // maximum number of days: N vectors elements are allocated and N is stored in an int
    static constexpr float maxDays = 1e6f;
    const AleTolerances tol;

    // true if value is outside the open interval (target - tolerance, target + tolerance)
    [[nodiscard]] static constexpr bool isOutsideBand(float value, float target, float tolerance) noexcept 
    {
        return (value <= (target - tolerance)) || (value >= (target + tolerance));
    }


    void algorithmElaboration() 
    {
        // nd is rounded to the nearest half day: halfDays is the number of half days
        // (nd >= 1.25 is guaranteed by the constructor, so halfDays >= 3)
        const int halfDays = static_cast<int>(std::lround(nd * 2.0f));
        approxDays = static_cast<float>(halfDays) / 2.0f;
        onlyIntegerDays = (halfDays % 2) == 0;
        const int N = onlyIntegerDays ? (halfDays / 2) : halfDays;

        const auto size = static_cast<std::size_t>(N);
        d.reserve(size);
        pd.reserve(size);
        pdap.reserve(size);
        parametersComputation(N);
    }


    // FUNCTIONS TO COMPUTE THE ALGORITHM RESULTS

    // Computation m and q parameters function
    void parametersComputation(int N) 
    {
        // q and m parameters definition
        q = np * 2.0f * rp / (static_cast<float>(N) * (rp + 1.0f));
        m = q * (rp - 1.0f) / (rp * static_cast<float>(N - 1));
        // Algorithm results computation
        resultsComputation(N);

        // self-correction phases in case of sensibility not satisfied
        for (int iter = 0; iter < tol.maxErrIter && ((status == Status::Error) || wrong_rate); ++iter) {
            // Rescale m to P and adjust q
            m = m * (np / totExactPhases);
            q = (np + m * (static_cast<float>(N) * static_cast<float>(N - 1) / 2.0f)) / static_cast<float>(N);

            const float mSpan = m * static_cast<float>(N - 1);
            if (q == mSpan) break;
            const float R_check = q / (q - mSpan);
            // if the correction drifts the R value from desired one, more than 2 * sensRate, the correction is stopped
            if (isOutsideBand(R_check, rp, 2.0f * tol.sensRate) || (q < mSpan)) break;

            resultsComputation(N);
        }
    }


    // Results computation function
    void resultsComputation(int N) 
    {
        d.assign(static_cast<std::size_t>(N), 0);
        std::iota(d.begin(), d.end(), 1);
        pd.clear();
        pdap.clear();
        status = Status::Ok;

        // iterations
        float each_day_value = 0;
        for (int i = 0; i < N; ++i) {
            each_day_value = q - static_cast<float>(i) * m;
            pd.push_back(each_day_value);
            pdap.push_back(std::round(each_day_value / tol.sensApproxSinglePhase) * tol.sensApproxSinglePhase);
        }

        // Final checks of computed values near enough to desired values
        // exact phases
        totExactPhases = std::accumulate(pd.begin(), pd.end(), 0.0f);
        if (isOutsideBand(totExactPhases, np, tol.sensExactPhases)) {
            status = Status::Error;
        } else {
            // approximated phases
            totApproxPhases = std::accumulate(pdap.begin(), pdap.end(), 0.0f);
            if (isOutsideBand(totApproxPhases, np, tol.sensApproxTotPhases)) {
                status = Status::Warning;
            }
        }
        // rate between first and last day value
        rate_first_last = pd.front() / pd.back();
        wrong_rate = isOutsideBand(rate_first_last, rp, tol.sensRate);
    }

};

#endif