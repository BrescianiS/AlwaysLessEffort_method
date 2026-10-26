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
 * INPUT NECESSARY VARIABLE: AlwaysLessEffort class object
 * INPUT SECONDARY VARIABLES: output text language (string with default "en" as English; other possible value is "it" as Italian) and output approximations values (TextRounding: outRate, outExact, outSingleApprox, outTotApprox)
 *
 * OUTPUT VARIABLE: (related to class object itself) string of content of output text file
 *
 *******************************************************/



#ifndef TEXTCREATION_H
#define TEXTCREATION_H

#include <cmath>
#include <cstddef>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <vector>

#include "AlwaysLessEffort.h"

// Output value approximation for the rate, the exact number of phases and approximated number of phases.
struct TextRounding
{
    float outRate = 0.001f;
    float outExact = 0.01f;
    float outSingleApprox = 0.01f;
    float outTotApprox = 0.1f;
};

namespace text_detail
{
    struct Labels
    {
        std::string_view enteredValues;
        std::string_view initialIncrement;
        std::string_view phasesNumber;
        std::string_view daysNumber;
        std::string_view warningIntro;
        std::string_view warningApproxSum;
        std::string_view warningPreferPrecise;
        std::string_view warningPreciseSum;
        std::string_view okIntro;
        std::string_view okOutro;
        std::string_view incrementCheck;
        std::string_view scheduleTitle;
        std::string_view tableIntegerHeader;
        std::string_view tableHalfDayHeader;
        std::string_view goodWork;
    };

    inline constexpr Labels english {
        "Entered values: \n  ",
        "initial desired Increment: +",
        "number of Phases: ",
        "number of Days: ",
        "Approximated values of phases per day were computed but be careful to consider: \n",
        " (sum of approximated values of phases = ",
        "it would be better to consider the phases in precise form: \n",
        " (sum of precise values of phases = ",
        "You can consider for convenience the values of phases per day either in approximated form or in precise form. \n"
        "For both of them the total resulted coherent with the requested number of phases (",
        ") \nOnce chosen, it is suggested to always follow the values of the same column.\n",
        "\n\nConfirmation of increment of first day compared to the last day: \n",
        "SCHEDULE",
        "Day\t Phases per day \t (approx)  \n\n",
        "Day\t\tPhases per day   (approx) \t Phases per half day   (approx) \n\n",
        "Have a good work! \n"
    };

    inline constexpr Labels italian {
        "Valori inseriti: \n  ",
        "Incremento iniziale richiesto: +",
        "numero di Fasi: ",
        "numero di Giorni: ",
        "Sono stati calcolati anche i valori approssimati delle fasi al giorno però è meglio fare attenzione nel considerarli: \n",
        " (somma dei valori approssimati delle fasi = ",
        "sarebbe meglio considerare quelle in forma precisa \n",
        " (somma dei valori precisi delle fasi = ",
        "Si può considerare sia i valori approssimati sia i valori precisi delle fasi per ogni giornata. \n"
        "Per entrambi il totale è risultato coerente con il numero di fasi richieste (",
        ").\nUna volta scelto si consiglia di seguire sempre i valori della stessa colonna.\n",
        "\n\nConferma dell'incremento del primo giorno rispetto all'ultimo: \n",
        "PIANIFICAZIONE",
        "Giorno\t Fasi al giorno \t (appross)  \n\n",
        "Giorno\t\tFasi al giorno   (appross) \t Fasi ogni mezza giornata   (appross) \n\n",
        "Buono lavoro! \n"
    };

}



class TextCreator
{

public:

    std::string TextToWriteInFile;

    explicit TextCreator( const AlwaysLessEffort& ale, std::string language = "en", TextRounding roundingIn = {} )
        // initialization order must follow the declaration order
        : nd { ale.nd }
        , np { ale.np }
        , rp { ale.rp }
        , d { ale.d }
        , pd { ale.pd }
        , pdap { ale.pdap }
        , approxDays { ale.approxDays }
        , onlyIntegerDays { ale.onlyIntegerDays }
        , rate_first_last { ale.rate_first_last }
        , totApproxPhases { ale.totApproxPhases }
        , totExactPhases { ale.totExactPhases }
        , status { ale.status }
        , lbl { labelsFor(language) }
        , rounding { roundingIn }
    {
        // the approximation steps are divisors
        if (!((rounding.outRate > 0.0f) && (rounding.outExact > 0.0f) && (rounding.outSingleApprox > 0.0f) && (rounding.outTotApprox > 0.0f))) {
            throw std::invalid_argument("TextCreator: the output approximation steps must be greater than zero");
        }
        if (d.empty()) {
            throw std::invalid_argument("TextCreator: the AlwaysLessEffort object has no computed results (check its status)");
        }
        outputApproximation();
        contentWriter();
    }


private:

    const float nd, np, rp;
    const std::vector<int> d;
    std::vector<float> pd, pdap;
    const float approxDays;
    const bool onlyIntegerDays;
    float rate_first_last, totApproxPhases, totExactPhases;
    const AlwaysLessEffort::Status status;
    const text_detail::Labels& lbl;
    const TextRounding rounding;


    [[nodiscard]] static const text_detail::Labels& labelsFor(std::string language) noexcept
    {
        return (language == "it") ? text_detail::italian : text_detail::english;
    }

    [[nodiscard]] static float roundTo(float value, float step) noexcept
    {
        return std::round(value / step) * step;
    }


    // function to set output values approximation
    void outputApproximation()
    {
        for (float& value : pd)   value = roundTo(value, rounding.outExact);
        for (float& value : pdap) value = roundTo(value, rounding.outSingleApprox);

        totExactPhases = roundTo(totExactPhases, rounding.outExact);
        // it handles also the particular error case with totApproxPhases about 0
        totApproxPhases = (totApproxPhases < rounding.outTotApprox) ? rounding.outTotApprox : roundTo(totApproxPhases, rounding.outTotApprox);
        rate_first_last = roundTo(rate_first_last, rounding.outRate);
    }


    // header

    void writeHeader(std::ostream& file) const
    {
        std::ostringstream outD;
        if (approxDays == nd) {
            outD << nd;
        } else {
            outD << std::setprecision(3) << approxDays << " (" << std::setprecision(4) << nd << ")";
        }
        file << lbl.enteredValues
             << lbl.initialIncrement << ((rp - 1.0f) * 100.0f) << "%;  "
             << lbl.phasesNumber << np << ";  "
             << lbl.daysNumber << outD.str() << ". \n\n\n\n";

        writeInfo(file);

        file << "\n\n " << lbl.scheduleTitle << "\n\n\n";

        // integer nd and non integer nd have different table columns
        file << (onlyIntegerDays ? lbl.tableIntegerHeader : lbl.tableHalfDayHeader);
    }


    // results informations

    void writeInfo(std::ostream& file) const
    {
        if (status == AlwaysLessEffort::Status::Warning) {
            file << lbl.warningIntro
                 << lbl.warningApproxSum << totApproxPhases << ") ; \n"
                 << lbl.warningPreferPrecise
                 << lbl.warningPreciseSum << totExactPhases << ") .\n";
        } else {
            file << lbl.okIntro << np << lbl.okOutro;
        }

        file << lbl.incrementCheck
             << pd.front() << "/" << pd.back()
             << " = " << rate_first_last << " --> +"
             << (rate_first_last - 1.0f) * 100.0f << "% . \n\n\n";
    }


    // schedule rows

    // In case of integer nd
    void writeIntegerRows(std::ostream& file) const
    {
        for (std::size_t i = 0; i < d.size(); ++i) {
            file << d[i] << "\t" << pd[i] << " \t\t ("
                 << pdap[i] << ") \n";
        }
    }

    // In case of non integer nd
    void writeHalfDayRows(std::ostream& file) const
    {
        const std::size_t last = d.size() - 1;
        for (std::size_t i = 0; i < d.size(); ++i) {
            if (i == last) {
                file << approxDays << "\t \t"
                     << pd[i] << "\t\t (" << pdap[i] << ")\t\t\t "
                     << pd[i] << "\t\t\t (" << pdap[i] << ")\n\n";
            } else if ((i % 2) == 0) {
                const float phs_d = pd[i] + pd[i + 1];
                const float phs_d_ap = pdap[i] + pdap[i + 1];
                file << (d[i] / 2) + 1 << " \t\t" << phs_d << "\t\t ("
                     << phs_d_ap << ")\t\t\t " << pd[i] << "\t\t\t ("
                     << pdap[i] << ")\n";
            } else {
                file << (d[i] / 2) << "\t\t\t\t\t\t\t\t\t" << pd[i] << "\t\t\t ("
                     << pdap[i] << ")\n\n\n";
            }
        }
    }


    // output file content definition function

    void contentWriter()
    {
        std::ostringstream file;

        writeHeader(file);

        if (onlyIntegerDays) {
            writeIntegerRows(file);
        } else {
            writeHalfDayRows(file);
        }

        file << "\n\n\n" << lbl.goodWork;

        file << "\n\n\n\n\n\nPowered by Always-Less-Effort method \n(https://github.com/BrescianiS/AlwaysLessEffort_method)";

        TextToWriteInFile = file.str();
    }

};
#endif