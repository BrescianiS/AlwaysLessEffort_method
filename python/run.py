#
#  Copyright 2026 -  Samuel Bresciani - bresciani.app@gmail.com
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#

from AlwaysLessEffort import AlwaysLessEffort, STATUS #, AleTolerances

if __name__ == "__main__":
    # computation parameters
    n_days = 12.7
    n_phases = 267.5
    desired_rate = 45.0
    # that means 45% more of work done on first day compared to the last day
    # that will be transformed inside AlwaysLessEffort to the rate of 1.45

    # Compute all algorithm variables
    # (optional: the tolerance and convergence values can be customized, e.g.
    #    tol = AleTolerances()
    #    tol.maxErrIter = 3
    #    ale = AlwaysLessEffort(n_days, n_phases, desired_rate, tol)
    ale = AlwaysLessEffort(n_days, n_phases, desired_rate)

    # Obtain output variables: ale.d; ale.pd; ale.pdap;...

    # Error situation (invalid or particular input values)
    if (ale.status == STATUS[2]):
        print(" Please change the input values of the algorithm!\n")
        exit #sys.exit()    

    # In case of standard non error situation, the outputs can be considered
    for i in range(len(ale.d)):
        day_str = f"day: {ale.d[i]}" if ale.only_integer_days else f" day: {int((ale.d[i] +1) //2)}; half-day: {int((ale.d[i] +1) %2) +1}"
        print(f"{day_str}; phases: {ale.pd[i]}; appr_phases: {ale.pdap[i]}. \n")